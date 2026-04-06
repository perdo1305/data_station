"""sim.launch.py — home / bench testing without any hardware.

Replaces can_bridge with mock_can and disables GPIO in input_handler.
Everything downstream (led_controller, dashboard_ui) is identical to car.launch.py.

Run at home to test the full UI and LED logic with synthetic RPM data:

  ros2 launch lart_bringup sim.launch.py
"""

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('lart_bringup'), 'config', 'rpi_config.yaml'
    )

    return LaunchDescription([
        # Simulated CAN source — replaces can_bridge on the car
        Node(
            package='sim',
            executable='mock_can',
            name='mock_can',
            parameters=[config],
            output='screen',
            emulate_tty=True,
        ),
        # input_handler with sim_mode=true — no GPIO, no events (keyboard/mouse
        # events from the pygame window can be added to ui_app.py for testing)
        Node(
            package='input_handler',
            executable='input_handler',
            name='input_handler',
            parameters=[config, {'sim_mode': True}],
            output='screen',
            emulate_tty=True,
        ),
        Node(
            package='led_controller',
            executable='led_controller',
            name='led_controller',
            parameters=[config],
            output='screen',
            emulate_tty=True,
        ),
        Node(
            package='dashboard_ui',
            executable='dashboard_ui',
            name='dashboard_ui',
            parameters=[config],
            additional_env={
                'PYGAME_HIDE_SUPPORT_PROMPT': '1',
                'SDL_RENDER_DRIVER': 'software',
                'LIBGL_ALWAYS_SOFTWARE': '1',
                'MESA_LOADER_DRIVER_OVERRIDE': 'llvmpipe',
            },
            output='screen',
            emulate_tty=True,
        ),
    ])
