"""ROS topic naming for messages decoded from each DBC."""

import os
from pathlib import Path


_DBC_TOPIC_PREFIXES = {
    "data_t26": "/data",
    "powertrain_t26": "/pwt",
    "autonomous_t26": "/can",
}

_DBC_SIM_BUSES = {
    "data_t26": ("data", "vcan_data"),
    "powertrain_t26": ("powertrain", "vcan_pwt"),
    "autonomous_t26": ("autonomous", "vcan_auto"),
}


def dbc_topic_prefix(dbc_path: str) -> str:
    """Return the ROS namespace assigned to a DBC file."""
    return _DBC_TOPIC_PREFIXES.get(Path(dbc_path).stem, "/can")


def dbc_sim_routes(dbc_dir: str, dbc_filename: str):
    """Return isolated CAN routes for the requested simulation DBCs."""
    if dbc_filename.lower() != "all":
        dbc_path = os.path.join(dbc_dir, dbc_filename)
        stem = Path(dbc_path).stem
        label = _DBC_SIM_BUSES.get(stem, (stem, "vcan0"))[0]
        return [(label, dbc_path, "vcan0", dbc_topic_prefix(dbc_path))]

    routes = []
    for stem, (label, interface) in _DBC_SIM_BUSES.items():
        dbc_path = os.path.join(dbc_dir, f"{stem}.dbc")
        routes.append((label, dbc_path, interface, dbc_topic_prefix(dbc_path)))
    return routes
