"""CAN-to-ROS 2 bridge — with optional DBC-based dynamic decoding.

Reads from one CAN bus via python-can (SocketCAN) and publishes:
  /data/dbc/<msg_name>  for data_t26.dbc
  /pwt/dbc/<msg_name>   for powertrain_t26.dbc
  /can/dbc/<msg_name>   for autonomous_t26.dbc and legacy/unknown DBCs

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

from .topic_names import dbc_topic_prefix

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
    slug = _UNSAFE.sub('_', raw).strip('_').lower()
    if slug and slug[0].isdigit():
        slug = 'val_' + slug
    return slug


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

        # Restrict RX to IDs we actually care about (RPM ID + every DBC message
        # ID) so the kernel/hardware drops everything else instead of us
        # decoding-and-discarding it in Python at bus rate.
        filter_ids = set(self._dbc_pubs.keys())
        filter_ids.add(self._rpm_id)
        try:
            self._bus.set_filters([
                {'can_id': fid, 'can_mask': 0x7FF, 'extended': False} for fid in filter_ids
            ])
        except Exception as exc:
            self.get_logger().warn(f'Failed to set CAN filters, receiving all IDs: {exc}')

        self._notifier = can.Notifier(self._bus, [self._on_message])
        self.get_logger().info(
            f'CAN bridge running on {iface}  '
            f'(RPM ID=0x{self._rpm_id:03X}, '
            f'DBC messages={len(self._dbc_pubs)})'
        )

    # ──────────────────────────────────────────────────────────────────────

    def _load_dbc(self, dbc_path: str) -> None:
        """Load DBC file or directory of DBC files and pre-create one Float32 publisher per signal."""
        try:
            import cantools  # imported here so missing lib is a soft error
        except ImportError:
            self.get_logger().error(
                'cantools is not installed — DBC decoding disabled. '
                'Run: pip install cantools --break-system-packages'
            )
            return

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

        total_signals = 0
        import importlib
        try:
            msg_module = importlib.import_module('lart_msgs.msg')
        except ImportError:
            self.get_logger().error("lart_msgs not found! CAN bridge will not work.")
            return

        topic_prefix = dbc_topic_prefix(dbc_path)

        for msg in self._db.messages:
            if not msg.signals:
                continue

            if msg.frame_id in self._dbc_pubs:
                prior_name = self._dbc_pubs[msg.frame_id]['name']
                self.get_logger().error(
                    f'Arbitration ID 0x{msg.frame_id:03X} is used by both '
                    f'"{prior_name}" and "{msg.name}" in the loaded DBC file(s) — '
                    f'keeping "{prior_name}", skipping "{msg.name}".'
                )
                continue

            msg_slug = _ros_name(msg.name)
            msg_class_name = ''.join(word.capitalize() for word in msg_slug.split('_') if word)
            try:
                msg_class = getattr(msg_module, msg_class_name)
            except AttributeError:
                self.get_logger().error(f"Message class {msg_class_name} not found in lart_msgs.msg!")
                continue

            topic = f'{topic_prefix}/{msg_slug}'
            try:
                pub = self.create_publisher(msg_class, topic, _BEST_EFFORT)
            except Exception as exc:
                self.get_logger().error(f'Failed to create publisher for "{msg.name}" on "{topic}": {exc}')
                continue

            self._dbc_pubs[msg.frame_id] = {
                'name': msg.name,
                'class': msg_class,
                'pub': pub,
                'signals': {sig.name: _ros_name(sig.name) for sig in msg.signals},
                'instance': msg_class(),
            }
            total_signals += len(msg.signals)

        self.get_logger().info(
            f'DBC loaded from: {dbc_path}\n'
            f'  → {len(self._dbc_pubs)} messages, {total_signals} signals\n'
            f'  → Publishing on {topic_prefix}/<msg>'
        )

    # ──────────────────────────────────────────────────────────────────────

    def _on_message(self, msg: can.Message) -> None:
        # ── Legacy RPM decode ──────────────────────────────────────────────
        if msg.arbitration_id == self._rpm_id:
            end = self._rpm_start + self._rpm_len
            if end <= len(msg.data):
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

        pub_info = self._dbc_pubs[msg.arbitration_id]
        out = pub_info['class']()
        for sig_name, value in decoded.items():
            sig_slug = pub_info['signals'].get(sig_name)
            if sig_slug is not None:
                field_type = out.get_fields_and_field_types()[sig_slug]
                converted = float(value) if field_type in ('float', 'double', 'float32', 'float64') else int(value)
                setattr(out, sig_slug, converted)
        pub_info['pub'].publish(out)

    # ──────────────────────────────────────────────────────────────────────

    def destroy_node(self):
        if hasattr(self, '_notifier'):
            self._notifier.stop()
        if hasattr(self, '_bus'):
            self._bus.shutdown()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = None
    try:
        node = CanBridgeNode()
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        if node is not None:
            node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
