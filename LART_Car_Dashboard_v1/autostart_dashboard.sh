#!/bin/bash
# LART Dashboard Autostart Sequence

PROJECT_DIR="/home/lart2026/GIT/data_station/LART_Car_Dashboard_v1"

# Log output for debugging with timestamps
LOG_FILE="/home/lart2026/dashboard_boot.log"
if [ ! -t 1 ]; then
    # Non-interactive: redirect everything to log file with timestamps
    exec > >(while IFS= read -r line; do echo "$(date '+[%Y-%m-%d %H:%M:%S]') $line"; done >> "$LOG_FILE") 2>&1
else
    # Interactive: show in terminal and write to log file, both with timestamps
    exec 3>&1
    exec > >(while IFS= read -r line; do
        formatted="$(date '+[%Y-%m-%d %H:%M:%S]') $line"
        echo "$formatted" >&3
        echo "$formatted" >> "$LOG_FILE"
    done) 2>&1
fi

echo "Starting LART Dashboard at $(date)"

# 1. Source ROS 2 Environment
# This fixes the 'ament_package' and shared library errors
source /opt/ros/jazzy/setup.bash 2>/dev/null || true
source /home/lart2026/ros2_jazzy/install/setup.bash 2>/dev/null || true
source /home/lart2026/GIT/data_station/install/setup.bash 2>/dev/null || true

# 2. Export necessary environment variables
export ROS_DOMAIN_ID=42
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lart2026/ros2_jazzy/install/lib
export DISPLAY=:0
_UID=$(id -u)
export XDG_RUNTIME_DIR=/run/user/$_UID

# 3. Navigate to Project
cd "$PROJECT_DIR" || exit 1

# 4. Wait for Desktop Environment to settle
# Check dynamically up to 10 seconds (50 * 0.2s) instead of a hard sleep 10
#X_READY=0
#for i in {1..50}; do
#    if xset -q >/dev/null 2>&1 || xhost >/dev/null 2>&1; then
#        echo "Desktop Environment/X server is ready."
#        X_READY=1
#        break
#    fi
#    sleep 0.2
#done

# If it is ready, add a tiny extra delay (e.g. 0.5s) to let the window manager finish layout.
# If not ready, continue anyway.
#if [ $X_READY -eq 1 ]; then
#    sleep 0.5
#else
#    echo "WARNING: X server not responsive or commands not available. Proceeding..."
#fi

# 5. Launch Dashboard and Kill Splash Screen
# Only build if the binary does not exist yet (first run / after clean).
# On subsequent boots the pre-built binary is launched directly, skipping
# the long cmake+compile step entirely.
UI_BIN="$PROJECT_DIR/build/ui-build/ui_runner"
CAN_BRIDGE_BIN="$PROJECT_DIR/build/ui-build/can_bridge"

if [ ! -x "$UI_BIN" ]; then
    echo "ui_runner not found – building for the first time (this will take a while)..."
    # Quit plymouth since building will take a while and we don't want the splash screen hanging
    plymouth quit 2>/dev/null || true
    make display-local
else
    echo "ui_runner already built – launching directly."

    # 5a. Start the CAN bridges in the background (SocketCAN → ROS2 topics)
    if [ -x "$CAN_BRIDGE_BIN" ]; then
        echo "Starting can_bridge_can0 on can0 (domain $ROS_DOMAIN_ID)..."
        "$CAN_BRIDGE_BIN" --ros-args -r __node:=can_bridge_can0 -p can_interface:=can0 &
        CAN_BRIDGE_0_PID=$!
        echo "can_bridge_can0 PID: $CAN_BRIDGE_0_PID"

        echo "Starting can_bridge_can1 on can1 (domain $ROS_DOMAIN_ID)..."
        "$CAN_BRIDGE_BIN" --ros-args -r __node:=can_bridge_can1 -p can_interface:=can1 &
        CAN_BRIDGE_1_PID=$!
        echo "can_bridge_can1 PID: $CAN_BRIDGE_1_PID"
    else
        echo "WARNING: can_bridge binary not found at $CAN_BRIDGE_BIN – CAN data will not be bridged to ROS2."
    fi

    # Quit plymouth in the background after 1 second so the UI has time to initialize and draw
    (sleep 1 && plymouth quit 2>/dev/null || true) &

    # 5b. Launch the UI (replaces this process)
    exec "$UI_BIN"
fi
