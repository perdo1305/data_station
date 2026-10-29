"""Regression tests for DBC-specific ROS topic namespaces."""

from lart_bringup.topic_names import dbc_sim_routes, dbc_topic_prefix


def test_data_dbc_uses_data_namespace():
    assert dbc_topic_prefix("/opt/dbc/data_t26.dbc") == "/data"


def test_powertrain_dbc_uses_pwt_namespace():
    assert dbc_topic_prefix("/opt/dbc/powertrain_t26.dbc") == "/pwt"


def test_autonomous_dbc_keeps_can_namespace():
    assert dbc_topic_prefix("/opt/dbc/autonomous_t26.dbc") == "/can"


def test_all_dbc_simulation_uses_an_isolated_bus_per_namespace(tmp_path):
    for name in ("data_t26.dbc", "powertrain_t26.dbc", "autonomous_t26.dbc"):
        (tmp_path / name).touch()

    assert dbc_sim_routes(str(tmp_path), "all") == [
        ("data", str(tmp_path / "data_t26.dbc"), "vcan_data", "/data"),
        ("powertrain", str(tmp_path / "powertrain_t26.dbc"), "vcan_pwt", "/pwt"),
        ("autonomous", str(tmp_path / "autonomous_t26.dbc"), "vcan_auto", "/can"),
    ]


def test_single_dbc_simulation_keeps_legacy_vcan0_route(tmp_path):
    dbc = tmp_path / "powertrain_t26.dbc"
    dbc.touch()

    assert dbc_sim_routes(str(tmp_path), dbc.name) == [
        ("powertrain", str(dbc), "vcan0", "/pwt"),
    ]


def test_bus_specific_message_types_fall_back_to_shared_type():
    from lart_bringup.topic_names import lart_msg_type_names

    assert lart_msg_type_names("/data", "aqt1") == ["DataAqt1", "Aqt1"]
    assert lart_msg_type_names("/pwt", "aqt4") == ["PowertrainAqt4", "Aqt4"]
    assert lart_msg_type_names("/can", "aqt1") == ["Aqt1"]
