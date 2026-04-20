import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'dashboard_ui'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='LART',
    maintainer_email='lart@example.com',
    description='Dashboard UI ROS 2 adapter and display application',
    license='MIT',
    entry_points={
        'console_scripts': [
            'dashboard_ui = dashboard_ui.ui_node:main',
            'speed_listener_node = dashboard_ui.speed_listener:main',
        ],
    },
)
