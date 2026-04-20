#!/usr/bin/env python3
"""Speed listener node for ROS 2 dashboard.

This node subscribes to speed/RPM topics and listens for updates.
Useful for inter-container communication testing and dashboard data monitoring.
"""

from __future__ import annotations

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from lart_msgs.msg import DashboardState


class SpeedListener(Node):
    def __init__(self):
        super().__init__('speed_listener')

        # Declare parameters
        self.declare_parameter('topic_prefix', '/vehicle')
        topic_prefix = self.get_parameter('topic_prefix').value

        # Subscribe to speed topics
        self.rpm_sub = self.create_subscription(
            Float32,
            f'{topic_prefix}/rpm',
            self.rpm_callback,
            10
        )

        self.state_sub = self.create_subscription(
            DashboardState,
            f'{topic_prefix}/dashboard_state',
            self.state_callback,
            10
        )

        self.get_logger().info(
            f'Speed listener started, subscribing to {topic_prefix}/*'
        )

    def rpm_callback(self, msg: Float32) -> None:
        """Handle RPM messages."""
        self.get_logger().info(f'RPM: {msg.data:.2f}')

    def state_callback(self, msg: DashboardState) -> None:
        """Handle DashboardState messages."""
        self.get_logger().info(
            f'Speed: {msg.speed_kph:.2f} km/h | '
            f'RPM: {msg.rpm:.0f} | '
            f'Throttle: {msg.throttle_percent:.1f}%'
        )


def main(args=None):
    rclpy.init(args=args)
    node = SpeedListener()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
