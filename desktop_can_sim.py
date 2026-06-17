#!/usr/bin/env python3
"""
desktop_can_sim.py — Virtual CAN traffic generator for DBC-based testing.
Runs standalone on the desktop, sending realistic CAN traffic via a hardware CAN interface
(e.g., can0 using usb2can) to be consumed by the Raspberry Pi data station.
"""

import argparse
import glob
import math
import os
import random
import sys
import time

try:
    import can
    import cantools
except ImportError:
    print("Error: Missing required packages.")
    print("Please install them using: pip install python-can cantools")
    sys.exit(1)


def _clamp(value: float, minimum: float, maximum: float) -> float:
    return max(minimum, min(maximum, value))


def _encodable_range(signal: cantools.db.Signal) -> tuple[float, float]:
    """Return the (min, max) physical values that can be encoded without overflow."""
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

    if signal.minimum is not None:
        enc_min = max(enc_min, signal.minimum)
    if signal.maximum is not None:
        enc_max = min(enc_max, signal.maximum)

    if enc_max <= enc_min:
        if signal.maximum is not None and signal.minimum is not None and signal.maximum == signal.minimum:
            pass
        else:
            enc_max = enc_min + abs(scale) if scale else enc_min + 1.0

    return enc_min, enc_max


def _make_signal_value(signal: cantools.db.Signal, t: float) -> float:
    """Generate a realistic, time-varying value for a DBC signal."""
    enc_min, enc_max = _encodable_range(signal)
    name_lower = signal.name.lower()

    if name_lower == 'ivt_result_u3':
        soc_lv = max(20.0, 78.0 - 0.06 * t)
        val = 20000.0 + (soc_lv / 100.0) * (28800.0 - 20000.0)
        return _clamp(val, enc_min, enc_max)

    if getattr(signal, 'choices', None) is not None and len(signal.choices) > 0:
        keys = sorted(list(signal.choices.keys()))
        idx = int(t * 0.2) % len(keys)
        return float(keys[idx])

    unit = (signal.unit or "").strip()
    boolean_keywords = ['ign', 'r2d', 'button', 'emergency', 'switch', 'bots', 'enable', 'ok', 'fail', 'error', 'active', 'state', 'status']
    is_boolean = signal.length == 1 or any(x in name_lower for x in boolean_keywords) or ('res' in name_lower and 'result' not in name_lower)
    if is_boolean:
        if enc_max - enc_min == 1:
            return 1.0 if (int(t * 0.25) % 2 == 0) else 0.0
        elif enc_max - enc_min <= 10:
            num_states = int(enc_max - enc_min) + 1
            return float(int(t * 0.5) % num_states + int(enc_min))

    cycle_t = t % 40.0
    
    if cycle_t < 22.0:
        accel_pct = min(100.0, cycle_t * 8.0)
        brake_pct = 0.0
        speed_factor = min(1.0, 0.15 + (cycle_t / 22.0) * 0.85)
        steer_factor = 0.0
    elif cycle_t < 32.0:
        brake_time = cycle_t - 22.0
        accel_pct = 0.0
        if brake_time < 2.0:
            brake_pct = brake_time * 40.0
        else:
            brake_pct = max(10.0, 80.0 - (brake_time - 2.0) * 8.0)
        speed_factor = max(0.2, 1.0 - (brake_time / 10.0) * 0.75)
        steer_factor = math.sin((brake_time / 10.0) * math.pi)
    else:
        exit_time = cycle_t - 32.0
        accel_pct = 15.0 + exit_time * 10.0
        brake_pct = 0.0
        speed_factor = 0.25 + (exit_time / 8.0) * 0.4
        steer_factor = max(0.0, 0.5 * (1.0 - exit_time / 8.0))

    is_percentage = (unit == '%' or any(x in name_lower for x in ['apps', 'pedal', 'brake_hydr', 'torque', 'moment', 'percent', 'pct', 'duty', 'throttle']))
    is_speed = any(x in name_lower for x in ['speed', 'spd', 'vel', 'kph', 'rpm', 'erpm'])
    is_pressure = any(x in name_lower for x in ['press', 'pressure', 'bar'])
    is_temp = any(x in name_lower for x in ['temp', 'temperature', 'ntc'])
    is_current = any(x in name_lower for x in ['current', 'curr', 'amp'])
    is_volt = any(x in name_lower for x in ['voltage', 'volt', 'v_'])
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
        val += random.uniform(-0.01, 0.01) * (max_val - min_val)
    elif is_pressure:
        if 'brake' in name_lower or 'brk' in name_lower:
            min_val = max(0.0, enc_min)
            max_val = enc_max
            val = min_val + (max_val - min_val) * (brake_pct / 100.0)
        else:
            min_val = max(0.0, enc_min)
            max_val = enc_max
            val = max_val - (max_val - min_val) * 0.15 * (0.5 + 0.5 * math.sin(t * 0.05))
    elif is_temp:
        min_val = max(20.0, enc_min)
        max_val = min(110.0, enc_max)
        if max_val <= min_val:
            min_val, max_val = enc_min, enc_max
        val = min_val + (max_val - min_val) * (0.35 + 0.45 * speed_factor + 0.1 * math.sin(t * 0.02))
    elif is_current:
        min_val = max(0.0, enc_min)
        max_val = enc_max
        val = min_val + (max_val - min_val) * (accel_pct / 100.0)
        val += random.uniform(-0.02, 0.02) * (max_val - min_val)
    elif is_volt:
        min_val = max(0.0, enc_min)
        max_val = enc_max
        val = max_val - (max_val - min_val) * (0.1 + 0.1 * (accel_pct / 100.0))
        val += random.uniform(-0.005, 0.005) * (max_val - min_val)
    elif is_steering:
        val = enc_min + (enc_max - enc_min) * (0.5 + 0.45 * steer_factor)
    else:
        mid = (enc_min + enc_max) / 2.0
        half = (enc_max - enc_min) / 2.0
        phase = hash(signal.name) % 100 / 100.0 * 2 * math.pi
        val = mid + half * 0.8 * math.sin(t * 0.5 + phase)
        val += random.uniform(-half * 0.05, half * 0.05)

    return _clamp(val, enc_min, enc_max)


def main():
    parser = argparse.ArgumentParser(description="Standalone Desktop CAN Simulator for LART Dashboard")
    parser.add_argument("--interface", type=str, default="can0", help="CAN interface to use (default: can0)")
    parser.add_argument("--dbc-dir", type=str, default="./dbc_signals", help="Directory containing DBC files (default: ./dbc_signals)")
    parser.add_argument("--hz", type=float, default=10.0, help="Publish frequency in Hz (default: 10.0)")
    args = parser.parse_args()

    print(f"--- Desktop CAN Simulator ---")
    print(f"Interface : {args.interface}")
    print(f"DBC Dir   : {args.dbc_dir}")
    print(f"Frequency : {args.hz} Hz")

    if not os.path.isdir(args.dbc_dir):
        print(f"Error: Directory '{args.dbc_dir}' not found.")
        sys.exit(1)

    dbc_files = sorted(glob.glob(os.path.join(args.dbc_dir, '*.dbc')))
    if not dbc_files:
        print(f"Error: No .dbc files found in '{args.dbc_dir}'.")
        sys.exit(1)

    db = cantools.database.Database()
    for df in dbc_files:
        try:
            db.add_dbc_file(df)
            print(f"Loaded DBC: {os.path.basename(df)}")
        except Exception as e:
            print(f"Warning: Failed to load {df} - {e}")

    messages = [m for m in db.messages if m.frame_id <= 0x1FFFFFFF]
    if not messages:
        print("Error: No simulatable messages found.")
        sys.exit(1)

    print(f"Simulating {len(messages)} messages.")

    msg_mux_valid_ids = {}
    for m in messages:
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
            msg_mux_valid_ids[m.name] = mux_valid_ids

    try:
        bus = can.interface.Bus(channel=args.interface, interface='socketcan')
    except Exception as e:
        print(f"Error: Cannot open SocketCAN bus '{args.interface}': {e}")
        print("Have you brought up the interface? (e.g. 'sudo ip link set can0 up type can bitrate 1000000')")
        sys.exit(1)

    print("Started publishing. Press Ctrl+C to stop.")

    dt = 1.0 / args.hz
    t = 0.0

    try:
        while True:
            start_time = time.time()
            t += dt

            for dbc_msg in messages:
                signals = {}
                mux_valid_ids = msg_mux_valid_ids.get(dbc_msg.name, {})

                for sig in dbc_msg.signals:
                    if sig.name in mux_valid_ids:
                        valid_list = mux_valid_ids[sig.name]
                        idx = int(t * 0.5) % len(valid_list)
                        signals[sig.name] = float(valid_list[idx])
                    else:
                        signals[sig.name] = _make_signal_value(sig, t)

                try:
                    data = dbc_msg.encode(signals, scaling=True, padding=True)
                except Exception as e:
                    continue

                is_extended = dbc_msg.frame_id > 0x7FF
                frame = can.Message(
                    arbitration_id=dbc_msg.frame_id,
                    data=data,
                    is_extended_id=is_extended,
                    timestamp=time.time()
                )

                try:
                    bus.send(frame)
                except Exception as e:
                    pass

            elapsed = time.time() - start_time
            sleep_time = dt - elapsed
            if sleep_time > 0:
                time.sleep(sleep_time)
            
    except KeyboardInterrupt:
        print("\nStopping CAN simulator.")
    finally:
        bus.shutdown()


if __name__ == "__main__":
    main()
