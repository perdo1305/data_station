import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'lart_bringup'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='LART',
    maintainer_email='lart@example.com',
    description='CAN-to-ROS 2 bridge node and system launch files',
    license='MIT',
    entry_points={
        'console_scripts': [
            'can_bridge = lart_bringup.can_bridge:main',
        ],
    },
)
