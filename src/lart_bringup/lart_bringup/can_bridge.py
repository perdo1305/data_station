"""CAN-to-ROS 2 bridge.

Reads from one CAN bus via python-can (SocketCAN) and publishes:
  /can/frames    (lart_msgs/CanFrame)   — every raw frame received
  /vehicle/rpm   (std_msgs/Float32)     — decoded RPM for a configured CAN ID

RPM signal config (rpi_config.yaml → can_bridge):
  rpm_can_id       — arbitration ID of the ECU RPM message  (decimal)
  rpm_start_byte   — byte offset inside the 8-byte payload
  rpm_length_bytes — 1 or 2 bytes, decoded big-endian unsigned
  rpm_scale        — RPM = raw * scale + offset
  rpm_offset
"""

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


class CanBridgeNode(Node):
    def __init__(self):
        super().__init__('can_bridge')

        self.declare_parameter('can_interface', 'can0')
        self.declare_parameter('rpm_can_id', 291)       # 0x123
        self.declare_parameter('rpm_start_byte', 0)
        self.declare_parameter('rpm_length_bytes', 2)
        self.declare_parameter('rpm_scale', 1.0)
        self.declare_parameter('rpm_offset', 0.0)

        iface = self.get_parameter('can_interface').value
        self._rpm_id = self.get_parameter('rpm_can_id').value
        self._rpm_start = self.get_parameter('rpm_start_byte').value
        self._rpm_len = self.get_parameter('rpm_length_bytes').value
        self._rpm_scale = self.get_parameter('rpm_scale').value
        self._rpm_offset = self.get_parameter('rpm_offset').value

        self._frame_pub = self.create_publisher(CanFrame, '/can/frames', _BEST_EFFORT)
        self._rpm_pub = self.create_publisher(Float32, '/vehicle/rpm', _BEST_EFFORT)

        try:
            self._bus = can.interface.Bus(channel=iface, interface='socketcan')
        except Exception as exc:
            self.get_logger().error(f'Cannot open CAN interface "{iface}": {exc}')
            raise

        self._notifier = can.Notifier(self._bus, [self._on_message])
        self.get_logger().info(f'CAN bridge running on {iface} (RPM ID=0x{self._rpm_id:03X})')

    # ------------------------------------------------------------------

    def _on_message(self, msg: can.Message) -> None:
        stamp = self.get_clock().now().to_msg()

        # Publish raw frame
        frame = CanFrame()
        frame.stamp = stamp
        frame.can_id = msg.arbitration_id
        frame.dlc = msg.dlc
        data = list(msg.data[:8])
        data += [0] * (8 - len(data))
        frame.data = data
        self._frame_pub.publish(frame)

        # Decode and publish RPM when this is the right message
        if msg.arbitration_id == self._rpm_id:
            end = self._rpm_start + self._rpm_len
            if end <= msg.dlc:
                raw = int.from_bytes(
                    msg.data[self._rpm_start:end], byteorder='big', signed=False
                )
                rpm_msg = Float32()
                rpm_msg.data = float(raw * self._rpm_scale + self._rpm_offset)
                self._rpm_pub.publish(rpm_msg)

    # ------------------------------------------------------------------

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
