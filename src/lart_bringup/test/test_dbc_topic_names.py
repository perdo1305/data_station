"""Regression tests for DBC-specific ROS topic namespaces."""

from lart_bringup.topic_names import dbc_topic_prefix


def test_data_dbc_uses_data_namespace():
    assert dbc_topic_prefix("/opt/dbc/data_t26.dbc") == "/data/dbc"


def test_powertrain_dbc_uses_pwt_namespace():
    assert dbc_topic_prefix("/opt/dbc/powertrain_t26.dbc") == "/pwt/dbc"


def test_autonomous_dbc_keeps_can_namespace():
    assert dbc_topic_prefix("/opt/dbc/autonomous_t26.dbc") == "/can/dbc"
