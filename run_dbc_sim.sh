#!/bin/bash
# run_dbc_sim.sh — Helper script to run DBC simulation stack

set -e

# Project root setup
PROJECT_DIR="/home/lart2026/GIT/data_station"
cd "$PROJECT_DIR"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  LART Dashboard - DBC Simulator Launcher                   ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# 1. Source ROS 2 Environment
if ! command -v ros2 &> /dev/null; then
    echo "ROS 2 command not found. Trying to source setup files..."
    if [ -f "/opt/ros/jazzy/setup.bash" ]; then
        echo "✓ Sourcing /opt/ros/jazzy/setup.bash"
        source /opt/ros/jazzy/setup.bash
    elif [ -f "$HOME/ros2_jazzy/install/local_setup.bash" ]; then
        echo "✓ Sourcing $HOME/ros2_jazzy/install/local_setup.bash"
        source "$HOME/ros2_jazzy/install/local_setup.bash"
    else
        echo "✗ Error: ROS 2 setup file not found in /opt/ros/jazzy/ or ~/ros2_jazzy/."
        exit 1
    fi
fi

# 2. Source Workspace setup
echo "🛠 Rebuilding simulation packages to ensure files are registered..."
colcon build --symlink-install --packages-select lart_bringup sim

if [ -f "$PROJECT_DIR/install/setup.bash" ]; then
    echo "✓ Sourcing local workspace setup.bash"
    source "$PROJECT_DIR/install/setup.bash"
else
    echo "✗ Error: Failed to find or source install/setup.bash."
    exit 1
fi

# 3. DBC File Selection
DBC_DIR="$PROJECT_DIR/dbc_signals"
echo ""
echo "Select which DBC file to simulate:"
echo "1) All DBC files (Simulation of entire bus)"
echo "2) powertrain_t26.dbc"
echo "3) autonomous_t26.dbc"
echo "4) data_t26.dbc"
echo "5) aquisition_boards.dbc"
read -p "Select option [1-5, default 1]: " opt

case "$opt" in
    2) export DBC_FILE="powertrain_t26.dbc" ;;
    3) export DBC_FILE="autonomous_t26.dbc" ;;
    4) export DBC_FILE="data_t26.dbc" ;;
    5) export DBC_FILE="aquisition_boards.dbc" ;;
    *) export DBC_FILE="all" ;;
esac

# 4. Create the isolated virtual buses required by the selected mode.
if [ "$DBC_FILE" = "all" ]; then
    "$PROJECT_DIR/scripts/setup_vcan_interfaces.sh" vcan_data vcan_pwt vcan_auto
else
    "$PROJECT_DIR/scripts/setup_vcan_interfaces.sh" vcan0
fi

echo ""
if [ "$DBC_FILE" = "all" ]; then
    echo "🚀 Launching simulation stack for ALL DBC files on isolated virtual buses..."
else
    echo "🚀 Launching simulation stack for $DBC_FILE on vcan0..."
fi
echo "Press Ctrl+C to stop."
echo ""

ros2 launch lart_bringup dbc_sim.launch.py
