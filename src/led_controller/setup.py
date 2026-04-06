import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'led_controller'

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
    description='Addressable LED strip controller for RPM gauge',
    license='MIT',
    entry_points={
        'console_scripts': [
            'led_controller = led_controller.led_node:main',
        ],
    },
)
