#!/usr/bin/env python3
"""Publish a speed value into ROS 2 for dashboard testing.

This script publishes both:
  - /vehicle/dashboard_state (lart_msgs/DashboardState)
  - /vehicle/rpm            (std_msgs/Float32)

The dashboard UI uses `speed_kph` from DashboardState when present, while
other parts of the stack still listen to RPM.
"""

from __future__ import annotations

import argparse
import math
import sys
import time

try:
    import rclpy
    from rclpy.node import Node
    from std_msgs.msg import Float32
    from lart_msgs.msg import DashboardState
except ModuleNotFoundError as exc:
    raise SystemExit(
        'Missing ROS 2 Python module: '
        f'{exc.name}.\n'
        'This script must run in a sourced ROS 2 workspace.\n\n'
        'Example:\n'
        '  source /opt/ros/jazzy/setup.bash\n'
        '  source install/setup.bash\n'
        '  python3 LART_Car_Dashboard_v1/speed.py 40\n'
    ) from exc


_SPEED_TO_RPM = 1.0 / 0.02454


class SpeedPublisher(Node):
    def __init__(self, speed_kph: float, rate_hz: float, topic_prefix: str):
        super().__init__('speed_publisher')

        self._speed_kph = max(0.0, float(speed_kph))
        self._rpm = self._speed_kph * _SPEED_TO_RPM
        self._period = 1.0 / max(rate_hz, 0.1)
        self._topic_prefix = topic_prefix.rstrip('/') or '/vehicle'

        self._rpm_pub = self.create_publisher(Float32, f'{self._topic_prefix}/rpm', 10)
        self._state_pub = self.create_publisher(
            DashboardState, f'{self._topic_prefix}/dashboard_state', 10
        )

    def publish_once(self) -> None:
        now = self.get_clock().now().to_msg()

        rpm_msg = Float32()
        rpm_msg.data = float(self._rpm)
        self._rpm_pub.publish(rpm_msg)

        state_msg = DashboardState()
        state_msg.stamp = now
        state_msg.rpm = float(self._rpm)
        state_msg.speed_kph = float(self._speed_kph)
        state_msg.inv_temp_c = 0.0
        state_msg.motor_temp_c = 0.0
        state_msg.bps = 0.0
        state_msg.kw_inst = 0.0
        state_msg.kw_limit = 0.0
        state_msg.soc_lv = 0.0
        state_msg.soc_hv = 0.0
        state_msg.lv_voltage = 0.0
        state_msg.r2d_ready = True
        state_msg.can_active = True
        self._state_pub.publish(state_msg)

    @property
    def period(self) -> float:
        return self._period


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description='Publish a speed value to ROS 2 dashboard topics.'
    )
    parser.add_argument(
        'speed_kph',
        type=float,
        help='Speed in km/h to publish',
    )
    parser.add_argument(
        '--rate-hz',
        type=float,
        default=10.0,
        help='Publish rate in Hz when repeating (default: 10)',
    )
    parser.add_argument(
        '--count',
        type=int,
        default=0,
        help='How many messages to send before exiting; 0 keeps publishing until Ctrl-C',
    )
    parser.add_argument(
        '--topic-prefix',
        default='/vehicle',
        help='Topic prefix for rpm and dashboard_state topics',
    )
    return parser.parse_args()


def main() -> None:
    args = _parse_args()

    if not math.isfinite(args.speed_kph):
        raise SystemExit('speed_kph must be a finite number')

    rclpy.init()
    node = SpeedPublisher(args.speed_kph, args.rate_hz, args.topic_prefix)
    try:
        if args.count == 0:
            while rclpy.ok():
                node.publish_once()
                time.sleep(node.period)
        else:
            for _ in range(max(args.count, 1)):
                node.publish_once()
                time.sleep(node.period)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
