"""Addressable LED strip controller — solid green status light.

Requires adafruit-circuitpython-neopixel + adafruit-blinka:
  pip install adafruit-circuitpython-neopixel

LED behaviour:
  - All LEDs remain green while the node is running
  - All LEDs turn off when the node shuts down
"""

import rclpy
from rclpy.node import Node

try:
    import board
    import neopixel
    _HAS_NEOPIXEL = True
except (ImportError, NotImplementedError):
    _HAS_NEOPIXEL = False

_COLOR_GREEN = (0, 200, 0)
_COLOR_OFF = (0, 0, 0)


class LedControllerNode(Node):
    def __init__(self):
        super().__init__('led_controller')

        self.declare_parameter('led_count', 8)
        self.declare_parameter('led_pin', 18)

        self._count = self.get_parameter('led_count').value

        if _HAS_NEOPIXEL:
            pin_num = self.get_parameter('led_pin').value
            gpio_pin = getattr(board, f'D{pin_num}', board.D18)
            self._pixels = neopixel.NeoPixel(
                gpio_pin, self._count, brightness=0.8, auto_write=False
            )
            self._pixels.fill(_COLOR_GREEN)
            self._pixels.show()
        else:
            self._pixels = None
            self.get_logger().warn(
                'NeoPixel library not found — LED output disabled. '
                'Install: pip install adafruit-circuitpython-neopixel'
            )

        self.get_logger().info(
            f'LED controller ready — {self._count} LEDs solid green'
        )

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
