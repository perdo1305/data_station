"""can_simulator.py — Virtual CAN traffic generator for DBC-based testing.

Creates a vcan interface (vcan0 by default), loads a DBC file with cantools,
and periodically sends realistic CAN frames so that can_bridge can exercise
the full decode pipeline without any real hardware.

Parameters (rpi_config.yaml → can_simulator):
  can_interface   — vcan interface name (default: vcan0)
  dbc_path        — absolute path to DBC file (required)
  publish_hz      — frame injection rate in Hz (default: 10.0)
  message_ids     — list of CAN IDs to simulate; empty = all messages in DBC

Setup performed automatically at startup:
  sudo modprobe vcan
  sudo ip link add <iface> type vcan   (idempotent — ignored if already exists)
  sudo ip link set <iface> up
"""

import math
import random
import subprocess
import time

import can
import cantools
import rclpy
from rclpy.node import Node


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _run(cmd: list[str], check: bool = False) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, capture_output=True, text=True, check=check)


def setup_vcan(iface: str, logger=None) -> bool:
    """Create and bring up a virtual CAN interface.

    Returns True on success, False if anything fails.
    Errors are non-fatal so the node can still start and report the problem.
    """
    def _log(msg: str) -> None:
        if logger:
            logger.info(msg)
        else:
            print(msg)

    def _err(msg: str) -> None:
        if logger:
            logger.error(msg)
        else:
            print(f"ERROR: {msg}")

    import os
    iface_exists = os.path.exists(f'/sys/class/net/{iface}')
    if iface_exists:
        _log(f'Virtual CAN interface {iface} already exists.')
        try:
            with open(f'/sys/class/net/{iface}/operstate', 'r') as f:
                state = f.read().strip()
            if state in ('up', 'unknown'):
                _log(f'Virtual CAN interface {iface} is already UP.')
                return True
        except Exception:
            pass
    else:
        # Try to load the vcan kernel module.
        # Inside Docker the module files are absent from the container's
        # /lib/modules, but the host may have already loaded vcan — in that case
        # modprobe will fail yet `ip link add … type vcan` will still succeed.
        # We therefore treat a modprobe failure as a warning, not a hard error.
        r = _run(['modprobe', 'vcan'])
        if r.returncode != 0:
            _log(f'modprobe vcan skipped (will try ip link anyway): {r.stderr.strip()}')
        else:
            _log('vcan kernel module loaded.')

        # Create the interface (idempotent — ignore "File exists" / "already exists")
        r = _run(['ip', 'link', 'add', iface, 'type', 'vcan'])
        if r.returncode != 0 and not any(kw in r.stderr for kw in ('File exists', 'already exists', 'RTNETLINK answers: File exists')):
            _err(f'Failed to create {iface}: {r.stderr.strip()}')
            return False

    # Bring the interface up
    r = _run(['ip', 'link', 'set', iface, 'up'])
    if r.returncode != 0:
        if iface_exists:
            _log(f'Warning: Could not set {iface} up, but it already exists. Proceeding anyway.')
            return True
        _err(f'Failed to bring up {iface}: {r.stderr.strip()}')
        return False

    _log(f'Virtual CAN interface {iface} is UP.')
    return True


def _clamp(value: float, minimum: float, maximum: float) -> float:
    return max(minimum, min(maximum, value))


def _raw_to_physical(raw: float, signal: cantools.db.Signal) -> float:
    """Apply scale and offset to convert a raw integer to physical value."""
    scale = signal.scale if signal.scale is not None else 1.0
    offset = signal.offset if signal.offset is not None else 0.0
    return raw * scale + offset


def _encodable_range(signal: cantools.db.Signal) -> tuple[float, float]:
    """Return the (min, max) physical values that can be encoded without overflow.

    Derives the range from the signal's bit length, sign, scale, and offset so
    that the value is always representable as a raw integer inside the frame —
    regardless of whether the DBC author populated the min/max fields.
    """
    if signal.is_signed:
        raw_min = -(2 ** (signal.length - 1))
        raw_max = 2 ** (signal.length - 1) - 1
    else:
        raw_min = 0
        raw_max = 2 ** signal.length - 1

    scale = signal.scale if signal.scale is not None else 1.0
    offset = signal.offset if signal.offset is not None else 0.0

    phys_a = raw_min * scale + offset
    phys_b = raw_max * scale + offset
    enc_min = min(phys_a, phys_b)
    enc_max = max(phys_a, phys_b)

    # Also honour the DBC-defined physical limits when present
    if signal.minimum is not None:
        enc_min = max(enc_min, signal.minimum)
    if signal.maximum is not None:
        enc_max = min(enc_max, signal.maximum)

    # Guard against degenerate ranges
    if enc_max <= enc_min:
        if signal.maximum is not None and signal.minimum is not None and signal.maximum == signal.minimum:
            pass
        else:
            enc_max = enc_min + abs(scale) if scale else enc_min + 1.0

    return enc_min, enc_max


def _make_signal_value(signal: cantools.db.Signal, t: float) -> float:
    """Generate a realistic, time-varying value for a DBC signal.

    Values are guaranteed to lie within the encodable physical range so that
    cantools.encode_message never raises an out-of-range error.
    """
    enc_min, enc_max = _encodable_range(signal)
    name_lower = signal.name.lower()

    if name_lower == 'ivt_result_u3':
        # Simulate 20V to 28.8V LV battery in mV (20000 to 28800 mV)
        soc_lv = max(20.0, 78.0 - 0.06 * t)
        val = 20000.0 + (soc_lv / 100.0) * (28800.0 - 20000.0)
        return _clamp(val, enc_min, enc_max)

    # 1. Discrete Choice / State Signals
    if getattr(signal, 'choices', None) is not None and len(signal.choices) > 0:
        keys = sorted(list(signal.choices.keys()))
        # Pick one choice based on time
        idx = int(t * 0.2) % len(keys)
        return float(keys[idx])

    # 2. Boolean/Discrete Flags
    unit = (signal.unit or "").strip()
    
    boolean_keywords = ['ign', 'r2d', 'button', 'emergency', 'switch', 'bots', 'enable', 'ok', 'fail', 'error', 'active', 'state', 'status']
    is_boolean = signal.length == 1 or any(x in name_lower for x in boolean_keywords) or ('res' in name_lower and 'result' not in name_lower)
    if is_boolean:
        if enc_max - enc_min == 1:
            return 1.0 if (int(t * 0.25) % 2 == 0) else 0.0
        elif enc_max - enc_min <= 10:
            # Cycle through integer states
            num_states = int(enc_max - enc_min) + 1
            return float(int(t * 0.5) % num_states + int(enc_min))

    # 3. Synchronized Driving Simulation (Lap Profile)
    # A single lap is simulated as 40 seconds long
    cycle_t = t % 40.0
    
    if cycle_t < 22.0:
        # Straight line: Acceleration phase
        # Accelerator goes up to 100%, brakes are 0%, speed & RPM rise
        accel_pct = min(100.0, cycle_t * 8.0)
        brake_pct = 0.0
        speed_factor = min(1.0, 0.15 + (cycle_t / 22.0) * 0.85)
        steer_factor = 0.0
    elif cycle_t < 32.0:
        # Heavy braking and cornering entry
        # Accelerator drops to 0%, brakes spike then taper, speed drops
        brake_time = cycle_t - 22.0
        accel_pct = 0.0
        if brake_time < 2.0:
            brake_pct = brake_time * 40.0 # Spikes to 80%
        else:
            brake_pct = max(10.0, 80.0 - (brake_time - 2.0) * 8.0)
        speed_factor = max(0.2, 1.0 - (brake_time / 10.0) * 0.75)
        # Turn steering wheel
        steer_factor = math.sin((brake_time / 10.0) * math.pi)
    else:
        # Exiting corner: partial throttle, zero brake, recovery
        exit_time = cycle_t - 32.0
        accel_pct = 15.0 + exit_time * 10.0 # rises to 95%
        brake_pct = 0.0
        speed_factor = 0.25 + (exit_time / 8.0) * 0.4
        # Straighten wheel
        steer_factor = max(0.0, 0.5 * (1.0 - exit_time / 8.0))

    # Identify the quantity category and scale to its physical bounds
    
    # Category A: Percentage / Pedals / Torque / Duty cycles
    is_percentage = (unit == '%' or 
                     any(x in name_lower for x in ['apps', 'pedal', 'brake_hydr', 'torque', 'moment', 'percent', 'pct', 'duty', 'throttle']))
    
    # Category B: Speed / RPM / Velocity
    is_speed = any(x in name_lower for x in ['speed', 'spd', 'vel', 'kph', 'rpm', 'erpm'])
    
    # Category C: Pressure
    is_pressure = any(x in name_lower for x in ['press', 'pressure', 'bar'])
    
    # Category D: Temperature
    is_temp = any(x in name_lower for x in ['temp', 'temperature', 'ntc'])
    
    # Category E: Current
    is_current = any(x in name_lower for x in ['current', 'curr', 'amp'])
    
    # Category F: Voltage
    is_volt = any(x in name_lower for x in ['voltage', 'volt', 'v_'])
    
    # Category G: Steering angle
    is_steering = any(x in name_lower for x in ['steering', 'steer', 'wheel_angle', 'st_angle'])

    if is_percentage:
        min_val = max(0.0, enc_min)
        max_val = min(100.0, enc_max)
        if 'brake' in name_lower or 'brk' in name_lower:
            val = min_val + (max_val - min_val) * (brake_pct / 100.0)
        else:
            val = min_val + (max_val - min_val) * (accel_pct / 100.0)
            
    elif is_speed:
        min_val = max(0.0, enc_min)
        max_val = enc_max
        val = min_val + (max_val - min_val) * speed_factor
        # Add slight engine speed vibration
        val += random.uniform(-0.01, 0.01) * (max_val - min_val)
        
    elif is_pressure:
        if 'brake' in name_lower or 'brk' in name_lower:
            # Brake pressure correlates with brake pedal
            min_val = max(0.0, enc_min)
            max_val = enc_max
            val = min_val + (max_val - min_val) * (brake_pct / 100.0)
        else:
            # Tank / accumulator pressure is high and stable
            min_val = max(0.0, enc_min)
            max_val = enc_max
            val = max_val - (max_val - min_val) * 0.15 * (0.5 + 0.5 * math.sin(t * 0.05))
            
    elif is_temp:
        # Temperature rises slowly with speed/RPM
        min_val = max(20.0, enc_min)
        max_val = min(110.0, enc_max)
        if max_val <= min_val:
            min_val, max_val = enc_min, enc_max
        val = min_val + (max_val - min_val) * (0.35 + 0.45 * speed_factor + 0.1 * math.sin(t * 0.02))
        
    elif is_current:
        # Current is positive and correlates with accelerator pedal
        min_val = max(0.0, enc_min)
        max_val = enc_max
        val = min_val + (max_val - min_val) * (accel_pct / 100.0)
        val += random.uniform(-0.02, 0.02) * (max_val - min_val)
        
    elif is_volt:
        # Voltage sags slightly under high acceleration
        min_val = max(0.0, enc_min)
        max_val = enc_max
        val = max_val - (max_val - min_val) * (0.1 + 0.1 * (accel_pct / 100.0))
        val += random.uniform(-0.005, 0.005) * (max_val - min_val)
        
    elif is_steering:
        # Steering is centered at 0 or mid-point, and turns positive/negative
        val = enc_min + (enc_max - enc_min) * (0.5 + 0.45 * steer_factor)
        
    else:
        # Default fallback: slow sine wave matching the signal's range
        mid = (enc_min + enc_max) / 2.0
        half = (enc_max - enc_min) / 2.0
        phase = hash(signal.name) % 100 / 100.0 * 2 * math.pi
        val = mid + half * 0.8 * math.sin(t * 0.5 + phase)
        val += random.uniform(-half * 0.05, half * 0.05)

    return _clamp(val, enc_min, enc_max)


# ---------------------------------------------------------------------------
# ROS 2 Node
# ---------------------------------------------------------------------------

class CanSimulatorNode(Node):
    def __init__(self):
        super().__init__('can_simulator')

        self.declare_parameter('can_interface', 'vcan0')
        self.declare_parameter('dbc_path', '')
        self.declare_parameter('publish_hz', 10.0)
        self.declare_parameter('message_ids', rclpy.parameter.Parameter.Type.INTEGER_ARRAY)

        iface = self.get_parameter('can_interface').value
        dbc_path = self.get_parameter('dbc_path').value
        hz = self.get_parameter('publish_hz').value

        try:
            ids_param = self.get_parameter('message_ids').value
            _filter_ids = set(ids_param) if ids_param else set()
        except Exception:
            _filter_ids = set()

        # --- Setup vcan ---
        if not setup_vcan(iface, logger=self.get_logger()):
            self.get_logger().fatal(
                f'Cannot set up virtual CAN interface "{iface}". '
                'Make sure you have sudo rights and the vcan module is available.'
            )
            raise RuntimeError(f'vcan setup failed for {iface}')

        # --- Load DBC ---
        if not dbc_path:
            self.get_logger().fatal('Parameter "dbc_path" is empty. Cannot simulate without a DBC file.')
            raise RuntimeError('dbc_path parameter is required for can_simulator')

        import glob
        import os
        self._db = None
        if os.path.isdir(dbc_path):
            self.get_logger().info(f'Loading all DBC files from directory: {dbc_path}')
            dbc_files = sorted(glob.glob(os.path.join(dbc_path, '*.dbc')))
        else:
            self.get_logger().info(f'Loading single DBC file: {dbc_path}')
            dbc_files = [dbc_path]

        for df in dbc_files:
            try:
                if self._db is None:
                    self._db = cantools.database.load_file(df)
                else:
                    self._db.add_dbc_file(df)
                self.get_logger().info(f'  Loaded DBC: {os.path.basename(df)}')
            except Exception as exc:
                self.get_logger().error(f'Failed to load DBC file "{df}": {exc}')

        if self._db is None:
            self._db = cantools.database.Database()

        # Filter messages: skip extended frames >0x7FF if they overflow SocketCAN
        # (cantools may include 29-bit IDs > 0x7FF which are valid extended frames)
        self._messages = [
            m for m in self._db.messages
            if (not _filter_ids or m.frame_id in _filter_ids)
            and m.frame_id <= 0x1FFFFFFF  # valid CAN extended ID upper bound
        ]

        if not self._messages:
            self.get_logger().warn('No simulatable messages found in DBC after filtering.')

        self.get_logger().info(
            f'Simulating {len(self._messages)} DBC messages on {iface} at {hz:.1f} Hz'
        )
        for m in self._messages:
            self.get_logger().info(
                f'  0x{m.frame_id:03X} ({m.frame_id:>5d})  {m.name}  '
                f'[{len(m.signals)} signal(s)]'
            )

        # --- Open CAN bus ---
        try:
            self._bus = can.interface.Bus(channel=iface, interface='socketcan')
        except Exception as exc:
            self.get_logger().fatal(f'Cannot open SocketCAN bus "{iface}": {exc}')
            raise

        # --- Pre-compute multiplexer info for each message ---
        self._msg_mux_valid_ids = {}
        for m in self._messages:
            mux_valid_ids = {}
            for sig in m.signals:
                if getattr(sig, 'is_multiplexer', False):
                    valid_ids = set()
                    for other_sig in m.signals:
                        if getattr(other_sig, 'multiplexer_signal', None) == sig.name and other_sig.multiplexer_ids:
                            valid_ids.update(other_sig.multiplexer_ids)
                    if valid_ids:
                        mux_valid_ids[sig.name] = sorted(list(valid_ids))
            if mux_valid_ids:
                self._msg_mux_valid_ids[m.name] = mux_valid_ids

        self._t = 0.0
        self._dt = 1.0 / hz

        self.create_timer(self._dt, self._tick)
        self.get_logger().info('CAN simulator running — sending frames to ' + iface)

    # -----------------------------------------------------------------------

    def _tick(self) -> None:
        self._t += self._dt
        if not self._messages:
            return

        for dbc_msg in self._messages:
            # Build signal values dict
            signals: dict[str, float] = {}

            # Get pre-computed multiplexer IDs for this message
            mux_valid_ids = self._msg_mux_valid_ids.get(dbc_msg.name, {})

            for sig in dbc_msg.signals:
                if sig.name in mux_valid_ids:
                    # Cycle through the valid multiplexer IDs based on t
                    valid_list = mux_valid_ids[sig.name]
                    idx = int(self._t * 0.5) % len(valid_list)
                    signals[sig.name] = float(valid_list[idx])
                else:
                    signals[sig.name] = _make_signal_value(sig, self._t)

            # Encode to bytes using cantools.
            # scaling=True  → values are physical (scale+offset applied automatically).
            # padding=True  → zero-pad frames whose signals don't fill all bytes.
            try:
                data = dbc_msg.encode(
                    signals, scaling=True, padding=True
                )
            except Exception as exc:
                self.get_logger().warn(
                    f'Encode error for 0x{dbc_msg.frame_id:03X} ({dbc_msg.name}): {exc}'
                )
                continue

            # Determine if this is an extended frame (ID > 0x7FF)
            is_extended = dbc_msg.frame_id > 0x7FF

            frame = can.Message(
                arbitration_id=dbc_msg.frame_id,
                data=data,
                is_extended_id=is_extended,
                timestamp=time.time(),
            )

            # try:
            #     self._bus.send(frame)
            # except Exception as exc:
            #     self.get_logger().warn(f'CAN send error: {exc}')

    # -----------------------------------------------------------------------

    def destroy_node(self):
        self._bus.shutdown()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = CanSimulatorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
