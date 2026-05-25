"""CAN-to-ROS 2 bridge — with optional DBC-based dynamic decoding.

Reads from one CAN bus via python-can (SocketCAN) and publishes:
  /can/frames                        (lart_msgs/CanFrame)  — every raw frame
  /vehicle/rpm                       (std_msgs/Float32)    — legacy RPM signal
  /can/dbc/<msg_name>/<signal_name>  (std_msgs/Float32)    — all DBC signals
                                                             (only when dbc_path is set)

Legacy RPM config (rpi_config.yaml → can_bridge):
  rpm_can_id       — arbitration ID of the ECU RPM message (decimal)
  rpm_start_byte   — byte offset inside the 8-byte payload
  rpm_length_bytes — 1 or 2 bytes, decoded big-endian unsigned
  rpm_scale        — RPM = raw * scale + offset
  rpm_offset

DBC decode config:
  dbc_path         — absolute path to a .dbc file; empty string = disabled
"""

import re

import can
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy
from std_msgs.msg import Float32
from lart_msgs.msg import CanFrame

_BEST_EFFORT = QoSProfile(
    reliability=QoSReliabilityPolicy.BEST_EFFORT,
    history=QoSHistoryPolicy.KEEP_LAST,
    depth=10,
)

# Regex to sanitise DBC message/signal names for use in ROS topic names.
# Keeps letters, digits, and underscores; collapses anything else to '_'.
_UNSAFE = re.compile(r'[^a-zA-Z0-9_]')


def _ros_name(raw: str) -> str:
    """Convert a DBC identifier to a valid, lowercase ROS topic segment."""
    return _UNSAFE.sub('_', raw).strip('_').lower()


class CanBridgeNode(Node):
    def __init__(self):
        super().__init__('can_bridge')

        # ── Legacy RPM parameters (unchanged) ──────────────────────────────
        self.declare_parameter('can_interface', 'can0')
        self.declare_parameter('rpm_can_id', 291)       # 0x123
        self.declare_parameter('rpm_start_byte', 0)
        self.declare_parameter('rpm_length_bytes', 2)
        self.declare_parameter('rpm_scale', 1.0)
        self.declare_parameter('rpm_offset', 0.0)

        # ── DBC decode parameters ──────────────────────────────────────────
        self.declare_parameter('dbc_path', '')

        iface = self.get_parameter('can_interface').value
        self._rpm_id    = self.get_parameter('rpm_can_id').value
        self._rpm_start = self.get_parameter('rpm_start_byte').value
        self._rpm_len   = self.get_parameter('rpm_length_bytes').value
        self._rpm_scale = self.get_parameter('rpm_scale').value
        self._rpm_offset = self.get_parameter('rpm_offset').value
        dbc_path        = self.get_parameter('dbc_path').value

        # ── Core publishers ────────────────────────────────────────────────
        self._frame_pub = self.create_publisher(CanFrame, '/can/frames', _BEST_EFFORT)
        self._rpm_pub   = self.create_publisher(Float32, '/vehicle/rpm', _BEST_EFFORT)

        # ── DBC setup ─────────────────────────────────────────────────────
        # _dbc_pubs: {arbitration_id: {signal_name: Publisher}}
        self._dbc_pubs: dict[int, dict[str, rclpy.publisher.Publisher]] = {}
        self._db = None

        if dbc_path:
            self._load_dbc(dbc_path)
        else:
            self.get_logger().info('dbc_path not set — running in legacy RPM-only mode.')

        # ── CAN interface ──────────────────────────────────────────────────
        try:
            self._bus = can.interface.Bus(channel=iface, interface='socketcan')
        except Exception as exc:
            self.get_logger().error(f'Cannot open CAN interface "{iface}": {exc}')
            raise

        self._notifier = can.Notifier(self._bus, [self._on_message])
        self.get_logger().info(
            f'CAN bridge running on {iface}  '
            f'(RPM ID=0x{self._rpm_id:03X}, '
            f'DBC messages={len(self._dbc_pubs)})'
        )

    # ──────────────────────────────────────────────────────────────────────

    def _load_dbc(self, dbc_path: str) -> None:
        """Load DBC file and pre-create one Float32 publisher per signal."""
        try:
            import cantools  # imported here so missing lib is a soft error
        except ImportError:
            self.get_logger().error(
                'cantools is not installed — DBC decoding disabled. '
                'Run: pip install cantools --break-system-packages'
            )
            return

        try:
            self._db = cantools.database.load_file(dbc_path)
        except Exception as exc:
            self.get_logger().error(f'Failed to load DBC file "{dbc_path}": {exc}')
            return

        total_signals = 0
        for msg in self._db.messages:
            if not msg.signals:
                continue
            msg_slug = _ros_name(msg.name)
            self._dbc_pubs[msg.frame_id] = {}
            for sig in msg.signals:
                sig_slug = _ros_name(sig.name)
                topic = f'/can/dbc/{msg_slug}/{sig_slug}'
                pub = self.create_publisher(Float32, topic, _BEST_EFFORT)
                self._dbc_pubs[msg.frame_id][sig.name] = pub
                total_signals += 1

        self.get_logger().info(
            f'DBC loaded: {dbc_path}\n'
            f'  → {len(self._dbc_pubs)} messages, {total_signals} signals\n'
            f'  → Publishing on /can/dbc/<msg>/<signal>'
        )

    # ──────────────────────────────────────────────────────────────────────

    def _on_message(self, msg: can.Message) -> None:
        stamp = self.get_clock().now().to_msg()

        # ── Publish raw frame ──────────────────────────────────────────────
        frame = CanFrame()
        frame.stamp = stamp
        frame.can_id = msg.arbitration_id
        frame.dlc = msg.dlc
        data = list(msg.data[:8])
        data += [0] * (8 - len(data))
        frame.data = data
        self._frame_pub.publish(frame)

        # ── Legacy RPM decode ──────────────────────────────────────────────
        if msg.arbitration_id == self._rpm_id:
            end = self._rpm_start + self._rpm_len
            if end <= msg.dlc:
                raw = int.from_bytes(
                    msg.data[self._rpm_start:end], byteorder='big', signed=False
                )
                rpm_msg = Float32()
                rpm_msg.data = float(raw * self._rpm_scale + self._rpm_offset)
                self._rpm_pub.publish(rpm_msg)

        # ── DBC dynamic decode ─────────────────────────────────────────────
        if self._db is None or msg.arbitration_id not in self._dbc_pubs:
            return

        try:
            decoded: dict = self._db.decode_message(
                msg.arbitration_id,
                msg.data,
                decode_choices=False,   # return raw numeric, not string label
            )
        except Exception as exc:
            self.get_logger().warn(
                f'DBC decode error for ID 0x{msg.arbitration_id:03X}: {exc}',
                throttle_duration_sec=5.0,
            )
            return

        pubs = self._dbc_pubs[msg.arbitration_id]
        for sig_name, value in decoded.items():
            pub = pubs.get(sig_name)
            if pub is None:
                continue
            out = Float32()
            out.data = float(value)
            pub.publish(out)

    # ──────────────────────────────────────────────────────────────────────

    def destroy_node(self):
        self._notifier.stop()
        self._bus.shutdown()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = CanBridgeNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
