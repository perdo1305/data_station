"""Addressable LED strip controller — RPM gauge.

Requires adafruit-circuitpython-neopixel + adafruit-blinka:
  pip install adafruit-circuitpython-neopixel

When NeoPixel is not available (e.g. on a dev laptop), the node still starts
and logs RPM updates — useful for checking topic data flow.

Subscribed topics:
  /vehicle/rpm  (std_msgs/Float32)

LED behaviour:
  - Fill from left proportional to RPM / rpm_max
  - Green below rpm_shift, red at or above rpm_shift
  - All LEDs off at RPM = 0
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32

try:
    import board
    import neopixel
    _HAS_NEOPIXEL = True
except (ImportError, NotImplementedError):
    _HAS_NEOPIXEL = False

_COLOR_GREEN = (0, 200, 0)
_COLOR_RED = (220, 0, 0)
_COLOR_OFF = (0, 0, 0)


class LedControllerNode(Node):
    def __init__(self):
        super().__init__('led_controller')

        self.declare_parameter('led_count', 30)
        self.declare_parameter('led_pin', 18)
        self.declare_parameter('rpm_shift', 11500)
        self.declare_parameter('rpm_max', 12000)

        self._count = self.get_parameter('led_count').value
        self._rpm_shift = float(self.get_parameter('rpm_shift').value)
        self._rpm_max = float(self.get_parameter('rpm_max').value)

        if _HAS_NEOPIXEL:
            pin_num = self.get_parameter('led_pin').value
            gpio_pin = getattr(board, f'D{pin_num}', board.D18)
            self._pixels = neopixel.NeoPixel(
                gpio_pin, self._count, brightness=0.8, auto_write=False
            )
            self._pixels.fill(_COLOR_OFF)
            self._pixels.show()
        else:
            self._pixels = None
            self.get_logger().warn(
                'NeoPixel library not found — LED output disabled. '
                'Install: pip install adafruit-circuitpython-neopixel'
            )

        self.create_subscription(Float32, '/vehicle/rpm', self._on_rpm, 10)
        self.get_logger().info(
            f'LED controller ready — {self._count} LEDs, '
            f'shift at {int(self._rpm_shift)} RPM'
        )

    # ------------------------------------------------------------------

    def _on_rpm(self, msg: Float32) -> None:
        rpm = max(0.0, min(float(msg.data), self._rpm_max))
        lit = round(rpm / self._rpm_max * self._count)
        color = _COLOR_RED if rpm >= self._rpm_shift else _COLOR_GREEN

        if self._pixels is None:
            return

        for i in range(self._count):
            self._pixels[i] = color if i < lit else _COLOR_OFF
        self._pixels.show()

    # ------------------------------------------------------------------

    def destroy_node(self):
        if self._pixels is not None:
            self._pixels.fill(_COLOR_OFF)
            self._pixels.show()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = LedControllerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
