"""Bridge decoded CAN signals into DashboardState for the UI.

Subscribes to configured Float32 topics (typically /can/dbc/* or /vehicle/rpm)
and publishes /vehicle/dashboard_state for dashboard_ui.
"""

from __future__ import annotations

import math
import time

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from lart_msgs.msg import DashboardState


def _nan() -> float:
    return float('nan')


def _is_finite(value: float) -> bool:
    return math.isfinite(value)


class DashboardStateBridge(Node):
    def __init__(self) -> None:
        super().__init__('dashboard_state_bridge')

        self.declare_parameter('publish_hz', 20.0)
        self.declare_parameter('can_active_timeout_s', 1.0)
        self.declare_parameter('speed_scale', 1.0)
        self.declare_parameter('speed_from_rpm_scale', 0.02454)

        self.declare_parameter('rpm_topic', '/vehicle/rpm')
        self.declare_parameter('speed_topic', '')
        self.declare_parameter('inv_temp_topic', '')
        self.declare_parameter('motor_temp_topic', '')
        self.declare_parameter('bps_topic', '')
        self.declare_parameter('kw_inst_topic', '')
        self.declare_parameter('kw_limit_topic', '')
        self.declare_parameter('soc_lv_topic', '')
        self.declare_parameter('soc_hv_topic', '')
        self.declare_parameter('lv_voltage_topic', '')
        self.declare_parameter('r2d_ready_topic', '')

        self._publish_hz = float(self.get_parameter('publish_hz').value)
        self._can_active_timeout_s = float(
            self.get_parameter('can_active_timeout_s').value
        )
        self._speed_scale = float(self.get_parameter('speed_scale').value)
        self._speed_from_rpm_scale = float(
            self.get_parameter('speed_from_rpm_scale').value
        )

        self._values = {
            'rpm': _nan(),
            'speed_kph': _nan(),
            'inv_temp_c': _nan(),
            'motor_temp_c': _nan(),
            'bps': _nan(),
            'kw_inst': _nan(),
            'kw_limit': _nan(),
            'soc_lv': _nan(),
            'soc_hv': _nan(),
            'lv_voltage': _nan(),
        }
        self._r2d_ready = False
        self._last_rx: float | None = None

        self._state_pub = self.create_publisher(
            DashboardState, '/vehicle/dashboard_state', 10
        )
        self._speed_pub = self.create_publisher(
            Float32, '/vehicle/speed_kph', 10
        )

        self._maybe_subscribe('rpm', self.get_parameter('rpm_topic').value)
        self._maybe_subscribe(
            'speed_kph',
            self.get_parameter('speed_topic').value,
            scale=self._speed_scale,
        )
        self._maybe_subscribe('inv_temp_c', self.get_parameter('inv_temp_topic').value)
        self._maybe_subscribe(
            'motor_temp_c', self.get_parameter('motor_temp_topic').value
        )
        self._maybe_subscribe('bps', self.get_parameter('bps_topic').value)
        self._maybe_subscribe('kw_inst', self.get_parameter('kw_inst_topic').value)
        self._maybe_subscribe('kw_limit', self.get_parameter('kw_limit_topic').value)
        self._maybe_subscribe('soc_lv', self.get_parameter('soc_lv_topic').value)
        self._maybe_subscribe('soc_hv', self.get_parameter('soc_hv_topic').value)
        self._maybe_subscribe(
            'lv_voltage', self.get_parameter('lv_voltage_topic').value
        )

        r2d_topic = self.get_parameter('r2d_ready_topic').value
        if r2d_topic:
            self.create_subscription(Float32, r2d_topic, self._on_r2d, 10)

        period_s = 1.0 / max(self._publish_hz, 1.0)
        self.create_timer(period_s, self._publish)

        self.get_logger().info('Dashboard state bridge running')

    def _maybe_subscribe(self, key: str, topic: str, scale: float = 1.0) -> None:
        if not topic:
            return

        def _cb(msg: Float32, *, field: str = key, factor: float = scale) -> None:
            value = float(msg.data) * factor
            self._values[field] = value
            self._last_rx = time.time()

        self.create_subscription(Float32, topic, _cb, 10)

    def _on_r2d(self, msg: Float32) -> None:
        self._r2d_ready = bool(msg.data > 0.5)
        self._last_rx = time.time()

    def _publish(self) -> None:
        speed = self._values['speed_kph']
        if not _is_finite(speed):
            rpm = self._values['rpm']
            if _is_finite(rpm):
                speed = rpm * self._speed_from_rpm_scale

        msg = DashboardState()
        msg.stamp = self.get_clock().now().to_msg()
        msg.rpm = float(self._values['rpm'])
        msg.speed_kph = float(speed) if _is_finite(speed) else _nan()
        msg.inv_temp_c = float(self._values['inv_temp_c'])
        msg.motor_temp_c = float(self._values['motor_temp_c'])
        msg.bps = float(self._values['bps'])
        msg.kw_inst = float(self._values['kw_inst'])
        msg.kw_limit = float(self._values['kw_limit'])
        msg.soc_lv = float(self._values['soc_lv'])
        msg.soc_hv = float(self._values['soc_hv'])
        msg.lv_voltage = float(self._values['lv_voltage'])
        msg.r2d_ready = bool(self._r2d_ready)

        now = time.time()
        msg.can_active = (
            self._last_rx is not None
            and (now - self._last_rx) < self._can_active_timeout_s
        )

        self._state_pub.publish(msg)

        if _is_finite(speed):
            speed_msg = Float32()
            speed_msg.data = float(speed)
            self._speed_pub.publish(speed_msg)


def main(args=None) -> None:
    rclpy.init(args=args)
    node = DashboardStateBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
