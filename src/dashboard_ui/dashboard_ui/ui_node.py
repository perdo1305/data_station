"""Dashboard UI ROS 2 adapter node.

Bridges ROS 2 topic data into the DashboardApp display thread.
All ROS callbacks are non-blocking; data flows via threading.Lock / queue.

Subscribed topics:
  /vehicle/dashboard_state (lart_msgs/DashboardState)
  /vehicle/rpm             (std_msgs/Float32)
  /can/frames              (lart_msgs/CanFrame)
  /input/buttons           (lart_msgs/ButtonEvent)
  /input/encoders          (lart_msgs/EncoderDelta)
"""

import threading

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from lart_msgs.msg import CanFrame, ButtonEvent, DashboardState, EncoderDelta

from .ui_app import DashboardApp


class DashboardUiNode(Node):
    def __init__(self):
        super().__init__('dashboard_ui')

        self.declare_parameter('rpm_shift', 11500)
        self.declare_parameter('rpm_max', 12000)

        rpm_shift = self.get_parameter('rpm_shift').value
        rpm_max = self.get_parameter('rpm_max').value

        self._app = DashboardApp(rpm_shift=rpm_shift, rpm_max=rpm_max)

        self.create_subscription(
            DashboardState, '/vehicle/dashboard_state', self._on_dashboard_state, 10
        )
        self.create_subscription(Float32, '/vehicle/rpm', self._on_rpm, 10)
        self.create_subscription(CanFrame, '/can/frames', self._on_can_frame, 10)
        self.create_subscription(ButtonEvent, '/input/buttons', self._on_button, 10)
        self.create_subscription(EncoderDelta, '/input/encoders', self._on_encoder, 10)

        self._ui_thread = threading.Thread(
            target=self._app.run, name='ui_thread', daemon=True
        )
        self._ui_thread.start()
        self.get_logger().info('Dashboard UI started')

    # ------------------------------------------------------------------

    def _on_dashboard_state(self, msg: DashboardState) -> None:
        self._app.update_dashboard_state(
            {
                'rpm': msg.rpm,
                'speed_kph': msg.speed_kph,
                'inv_temp_c': msg.inv_temp_c,
                'motor_temp_c': msg.motor_temp_c,
                'bps': msg.bps,
                'kw_inst': msg.kw_inst,
                'kw_limit': msg.kw_limit,
                'soc_lv': msg.soc_lv,
                'soc_hv': msg.soc_hv,
                'lv_voltage': msg.lv_voltage,
                'r2d_ready': msg.r2d_ready,
                'can_active': msg.can_active,
            }
        )

    def _on_rpm(self, msg: Float32) -> None:
        self._app.update_rpm(msg.data)

    def _on_can_frame(self, msg: CanFrame) -> None:
        self._app.update_can_frame(msg.can_id, bytes(msg.data[: msg.dlc]))

    def _on_button(self, msg: ButtonEvent) -> None:
        self._app.handle_button(msg.button_id, msg.pressed)

    def _on_encoder(self, msg: EncoderDelta) -> None:
        self._app.handle_encoder(msg.encoder_id, msg.delta)


def main(args=None):
    rclpy.init(args=args)
    node = DashboardUiNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
