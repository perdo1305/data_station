"""QoS regression tests for CAN-derived dashboard subscriptions."""

from rclpy.qos import QoSReliabilityPolicy

from lart_bringup.dashboard_state_bridge import can_subscription_qos


def test_can_subscriptions_accept_best_effort_bridge_publishers():
    assert can_subscription_qos().reliability == QoSReliabilityPolicy.BEST_EFFORT
