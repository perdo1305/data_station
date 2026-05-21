#!/bin/bash
# LART Dashboard Autostart Sequence

PROJECT_DIR="/home/lart2026/GIT/data_station/LART_Car_Dashboard_v1"

# Log output for debugging
exec > /home/lart2026/dashboard_boot.log 2>&1

echo "Starting LART Dashboard at $(date)"

# 1. Source ROS 2 Environment
# This fixes the 'ament_package' and shared library errors
source /opt/ros/jazzy/setup.bash
source /home/lart2026/ros2_jazzy/install/setup.bash

# 2. Export necessary environment variables
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lart2026/ros2_jazzy/install/lib
export DISPLAY=:0
export XDG_RUNTIME_DIR=/run/user/$(id -u)

# 3. Navigate to Project
cd "$PROJECT_DIR" || exit 1

# 4. Wait for Desktop Environment to settle
sleep 10

# 5. Launch Dashboard and Kill Splash Screen
# We run make in the background so we can signal Plymouth to quit
make display-local

# Give the app a moment to claim the window, then hide loading screen
sleep 10
plymouth quit
