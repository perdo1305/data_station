"""ROS topic naming for messages decoded from each DBC."""

from pathlib import Path


_DBC_TOPIC_PREFIXES = {
    "data_t26": "/data/dbc",
    "powertrain_t26": "/pwt/dbc",
    "autonomous_t26": "/can/dbc",
}


def dbc_topic_prefix(dbc_path: str) -> str:
    """Return the ROS namespace assigned to a DBC file."""
    return _DBC_TOPIC_PREFIXES.get(Path(dbc_path).stem, "/can/dbc")
