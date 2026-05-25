"""dbc_sim.launch.py — Full DBC-driven simulation stack on virtual CAN.

Starts:
  1. can_simulator  (sim)          — creates vcan0 and injects DBC-encoded frames
  2. can_bridge     (lart_bringup) — reads vcan0, decodes with cantools, publishes
                                     /can/dbc/<msg>/<signal> topics

Optionally override the DBC file:
  ros2 launch lart_bringup dbc_sim.launch.py dbc_file:=powertrain_t26.dbc

The DBC files are expected in <workspace>/dbc_signals/.
The absolute path is resolved at launch time via FindPackageShare.
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('lart_bringup'), 'config', 'rpi_config.yaml'
    )

    # Resolve the workspace root.
    # In Docker (docker-compose sets DATA_STATION_WS=/root/data_station_ws) the
    # source volume is mounted separately from the colcon build directory, so we
    # prefer DATA_STATION_WS when available.  For local/non-Docker builds we fall
    # back to walking 4 levels up from the installed share directory.
    pkg_share = get_package_share_directory('lart_bringup')
    ws_root = os.environ.get(
        'DATA_STATION_WS',
        os.path.normpath(os.path.join(pkg_share, '..', '..', '..', '..')),
    )
    dbc_signals_dir = os.path.join(ws_root, 'dbc_signals')

    # Default DBC — autonomous_t26.dbc.
    # To use a different file, change the argument default or set the env var
    # DBC_FILE before launching:  DBC_FILE=powertrain_t26.dbc ros2 launch ...
    dbc_filename = os.environ.get('DBC_FILE', 'autonomous_t26.dbc')
    resolved_dbc_path = os.path.join(dbc_signals_dir, dbc_filename)

    if not os.path.isfile(resolved_dbc_path):
        import warnings
        warnings.warn(
            f'DBC file not found: {resolved_dbc_path}\n'
            f'Set DBC_FILE env var or place the file in {dbc_signals_dir}',
            RuntimeWarning,
        )

    dbc_params = {
        'can_interface': 'vcan0',
        'dbc_path': resolved_dbc_path,
        'publish_hz': 10.0,
    }

    return LaunchDescription([
        # ── Simulator: creates vcan0 and pumps CAN frames from the DBC ──────
        Node(
            package='sim',
            executable='can_simulator',
            name='can_simulator',
            parameters=[config, dbc_params],
            output='screen',
            emulate_tty=True,
        ),

        # ── Bridge: reads vcan0, decodes DBC, publishes /can/dbc/* topics ───
        Node(
            package='lart_bringup',
            executable='can_bridge',
            name='can_bridge',
            parameters=[config, dbc_params],
            output='screen',
            emulate_tty=True,
        ),
    ])
