"""ROS topic naming for messages decoded from each DBC."""

from pathlib import Path


_DBC_TOPIC_PREFIXES = {
    "data_t26": "/data",
    "powertrain_t26": "/pwt",
    "autonomous_t26": "/can",
}


def dbc_topic_prefix(dbc_path: str) -> str:
    """Return the ROS namespace assigned to a DBC file."""
    return _DBC_TOPIC_PREFIXES.get(Path(dbc_path).stem, "/can")
