"""Input handler node — buttons and rotary encoders via GPIO.

Requires python-gpiod >= 2.0  (apt install python3-gpiod on RPi OS Bookworm).
On RPi 5 the GPIO chip is /dev/gpiochip4; falls back to /dev/gpiochip0.

When sim_mode=true (or gpiod is not installed), the node starts successfully
but publishes no events — useful for home testing without hardware.

Published topics:
  /input/buttons   (lart_msgs/ButtonEvent)
  /input/encoders  (lart_msgs/EncoderDelta)

Encoder direction convention (rising CLK edge):
  DT is LOW  → clockwise   → delta = +1
  DT is HIGH → counter-CW  → delta = -1
"""

import datetime
import threading

import rclpy
from rclpy.node import Node
from lart_msgs.msg import ButtonEvent, EncoderDelta

try:
    import gpiod
    from gpiod.line import Direction, Edge, Value as LineValue
    _HAS_GPIOD = True
except ImportError:
    _HAS_GPIOD = False


class InputHandlerNode(Node):
    def __init__(self):
        super().__init__('input_handler')

        self.declare_parameter('button_a', 17)
        self.declare_parameter('button_b', 27)
        self.declare_parameter('encoder_a_clk', 22)
        self.declare_parameter('encoder_a_dt', 23)
        self.declare_parameter('encoder_b_clk', 24)
        self.declare_parameter('encoder_b_dt', 25)
        self.declare_parameter('debounce_ms', 50)
        self.declare_parameter('sim_mode', False)

        self._btn_pub = self.create_publisher(ButtonEvent, '/input/buttons', 10)
        self._enc_pub = self.create_publisher(EncoderDelta, '/input/encoders', 10)

        sim_mode = self.get_parameter('sim_mode').value
        if sim_mode or not _HAS_GPIOD:
            if not _HAS_GPIOD:
                self.get_logger().warn(
                    'python-gpiod not found — input_handler in no-op mode. '
                    'Install: apt install python3-gpiod'
                )
            else:
                self.get_logger().info('input_handler in sim_mode — no GPIO events')
            return

        debounce_ms = self.get_parameter('debounce_ms').value
        debounce = datetime.timedelta(milliseconds=debounce_ms)

        btn_pins = {
            0: self.get_parameter('button_a').value,
            1: self.get_parameter('button_b').value,
        }
        enc_clk_pins = {
            0: self.get_parameter('encoder_a_clk').value,
            1: self.get_parameter('encoder_b_clk').value,
        }
        enc_dt_pins = {
            0: self.get_parameter('encoder_a_dt').value,
            1: self.get_parameter('encoder_b_dt').value,
        }

        chip_path = self._detect_chip()

        # Buttons: edge detection with hardware debounce
        btn_cfg = {
            pin: gpiod.LineSettings(
                direction=Direction.INPUT,
                edge_detection=Edge.BOTH,
                debounce_period=debounce,
            )
            for pin in btn_pins.values()
        }
        self._btn_req = gpiod.request_lines(chip_path, consumer='lart_btn', config=btn_cfg)
        self._btn_pin_to_id = {v: k for k, v in btn_pins.items()}

        # Encoder CLK: edge detection (no hardware debounce — mechanical encoders pulse fast)
        enc_clk_cfg = {
            pin: gpiod.LineSettings(direction=Direction.INPUT, edge_detection=Edge.BOTH)
            for pin in enc_clk_pins.values()
        }
        self._enc_clk_req = gpiod.request_lines(
            chip_path, consumer='lart_enc_clk', config=enc_clk_cfg
        )

        # Encoder DT: level read only
        enc_dt_cfg = {
            pin: gpiod.LineSettings(direction=Direction.INPUT)
            for pin in enc_dt_pins.values()
        }
        self._enc_dt_req = gpiod.request_lines(
            chip_path, consumer='lart_enc_dt', config=enc_dt_cfg
        )

        self._enc_clk_pin_to_id = {v: k for k, v in enc_clk_pins.items()}
        self._enc_dt_pins = enc_dt_pins  # enc_id → dt_pin

        self._running = True
        self._thread = threading.Thread(target=self._poll_loop, daemon=True)
        self._thread.start()
        self.get_logger().info(
            f'input_handler started on chip {chip_path} | '
            f'btns={list(btn_pins.values())} enc_clk={list(enc_clk_pins.values())}'
        )

    # ------------------------------------------------------------------

    def _detect_chip(self) -> str:
        import os
        for path in ('/dev/gpiochip4', '/dev/gpiochip0'):
            if os.path.exists(path):
                return path
        raise RuntimeError('No gpiochip device found')

    def _poll_loop(self):
        timeout = datetime.timedelta(milliseconds=50)
        while self._running:
            if self._btn_req.wait_edge_events(timeout):
                for ev in self._btn_req.read_edge_events():
                    self._handle_button(ev)
            if self._enc_clk_req.wait_edge_events(timeout):
                for ev in self._enc_clk_req.read_edge_events():
                    self._handle_encoder_clk(ev)

    def _handle_button(self, ev) -> None:
        btn_id = self._btn_pin_to_id[ev.line_offset]
        pressed = ev.event_type == gpiod.EdgeEvent.Type.RISING_EDGE
        msg = ButtonEvent()
        msg.stamp = self.get_clock().now().to_msg()
        msg.button_id = btn_id
        msg.pressed = pressed
        self._btn_pub.publish(msg)

    def _handle_encoder_clk(self, ev) -> None:
        # Only act on rising edge of CLK
        if ev.event_type != gpiod.EdgeEvent.Type.RISING_EDGE:
            return
        enc_id = self._enc_clk_pin_to_id[ev.line_offset]
        dt_pin = self._enc_dt_pins[enc_id]
        dt_val = self._enc_dt_req.get_value(dt_pin)
        # DT LOW → CW (+1), DT HIGH → CCW (−1)
        delta = -1 if dt_val == LineValue.ACTIVE else 1
        msg = EncoderDelta()
        msg.stamp = self.get_clock().now().to_msg()
        msg.encoder_id = enc_id
        msg.delta = delta
        self._enc_pub.publish(msg)

    # ------------------------------------------------------------------

    def destroy_node(self):
        self._running = False
        if hasattr(self, '_thread'):
            self._thread.join(timeout=1.0)
        for attr in ('_btn_req', '_enc_clk_req', '_enc_dt_req'):
            req = getattr(self, attr, None)
            if req is not None:
                req.release()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = InputHandlerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
