"""car.launch.py — real hardware bringup.

Starts all four nodes with the shared rpi_config.yaml parameter file.
Run on the car (CAN HAT connected, LEDs wired, encoders/buttons attached):

  ros2 launch lart_bringup car.launch.py
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
        Node(
            package='lart_bringup',
            executable='can_bridge',
            name='can_bridge',
            parameters=[config],
            output='screen',
            emulate_tty=True,
        ),
        Node(
            package='input_handler',
            executable='input_handler',
            name='input_handler',
            parameters=[config],
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
            output='screen',
            emulate_tty=True,
        ),
    ])
