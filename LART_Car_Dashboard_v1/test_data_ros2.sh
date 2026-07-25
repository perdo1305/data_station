#!/bin/bash
# Test Data Generator for LART Dashboard ROS 2 (Local / Pure ROS 2 Version)
# Publishes sample speed data directly using local ROS 2 environment

set -e

export ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-42}"

TOPIC_PREFIX="/vehicle"
SCREEN_TOPIC="/dashboard/set_screen"
SPEED_VALUES=(10 20 30 40 50 60 70 80 90 100)
SEND_DELAY_NORMAL=0
SEND_DELAY_FAST=0

BAG_DIR="${BAG_DIR:-$HOME/bags}"
BAG_RECORD_REGEX="${BAG_RECORD_REGEX:-/can/dbc/.*}"
CAN_SIM_PID=""
BAG_RECORD_PID=""

# Stop any background CAN simulator / bag record process we started
cleanup_background() {
    if [ -n "$CAN_SIM_PID" ] && kill -0 "$CAN_SIM_PID" 2>/dev/null; then
        echo "Stopping CAN simulator (PID $CAN_SIM_PID)..."
        kill "$CAN_SIM_PID" 2>/dev/null
    fi
    if [ -n "$BAG_RECORD_PID" ] && kill -0 "$BAG_RECORD_PID" 2>/dev/null; then
        echo "Stopping bag record (PID $BAG_RECORD_PID)..."
        kill -INT "$BAG_RECORD_PID" 2>/dev/null
    fi
}
trap cleanup_background EXIT

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  LART Dashboard - ROS 2 Test Data Generator (Local)        ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Source ROS 2 if not already available
if ! command -v ros2 &> /dev/null; then
    echo "ROS 2 command not found, trying to source setup files..."
    if [ -f "/opt/ros/jazzy/setup.bash" ]; then
        echo "Sourcing /opt/ros/jazzy/setup.bash"
        source /opt/ros/jazzy/setup.bash
    elif [ -f "$HOME/ros2_jazzy/install/local_setup.bash" ]; then
        echo "Sourcing $HOME/ros2_jazzy/install/local_setup.bash"
        source "$HOME/ros2_jazzy/install/local_setup.bash"
    fi
fi

# Source workspace setup if not already sourced
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
if [ -f "$SCRIPT_DIR/install/setup.bash" ]; then
    echo "Sourcing local workspace: $SCRIPT_DIR/install/setup.bash"
    source "$SCRIPT_DIR/install/setup.bash"
elif [ -f "$SCRIPT_DIR/../install/setup.bash" ]; then
    echo "Sourcing local workspace: $SCRIPT_DIR/../install/setup.bash"
    source "$SCRIPT_DIR/../install/setup.bash"
fi

if ! command -v ros2 &> /dev/null; then
    echo "✗ ROS 2 is not found! Please source ROS 2 first."
    exit 1
fi

echo "✓ ROS 2 environment is ready."
echo ""

rand_int() {
    local min=$1
    local max=$2
    echo $((RANDOM % (max - min + 1) + min))
}

rand_float() {
    local min=$1
    local max=$2
    awk -v min="$min" -v max="$max" -v seed="$RANDOM" 'BEGIN { srand(seed); printf "%.2f", min + rand() * (max - min) }'
}

chance_true() {
    local percent_true=$1
    if [ $((RANDOM % 100)) -lt "$percent_true" ]; then
        echo "true"
    else
        echo "false"
    fi
}

# Function to publish test data
publish_speed_batch() {
    local delay=$1
    shift

    if [ "$#" -eq 0 ]; then
        return 0
    fi

    local delay_sec="$delay"
    for speed_kph in "$@"; do
        if [ "$speed_kph" -lt 0 ]; then
            speed_kph=0
        fi
        if [ "$speed_kph" -gt 120 ]; then
            speed_kph=120
        fi

        echo "Publishing: speed_kph=$speed_kph km/h"
        if ! ros2 topic pub -1 /vehicle/speed_kph std_msgs/msg/Float32 "{data: $speed_kph}"; then
            echo "✗ Failed to publish test data to $TOPIC_PREFIX/speed_kph"
            return 1
        fi

        if [ "$delay_sec" != "0" ]; then
            sleep "$delay_sec"
        fi
    done
}

publish_test_data() {
    local speed=$1
    local speed_kph=$speed
    
    if [ "$speed_kph" -lt 0 ]; then
        speed_kph=0
    fi
    if [ "$speed_kph" -gt 120 ]; then
        speed_kph=120
    fi

    publish_speed_batch 0 "$speed_kph"
}

# Menu
show_menu() {
    echo "Select test scenario:"
    echo "1) Idle (0 km/h)"
    echo "2) City driving (30 km/h)"
    echo "3) Highway (100 km/h)"
    echo "4) Acceleration sequence"
    echo "5) Deceleration sequence"
    echo "6) Random values"
    echo "7) Custom speed"
    echo "8) Change screen (ROS topic)"
    echo "9) Help"
    echo "a) Start/Stop CAN simulator (dbc_sim.launch.py)"
    echo "b) Start/Stop bag record"
    echo "c) Set Mission_select (autonomous mission select)"
    echo "d) Set AS_MISSION (mission fallback signal)"
    echo "0) Exit"
    echo ""
}

# Test scenarios
test_idle() {
    echo "Publishing idle speed..."
    publish_test_data 0
    echo "✓ Idle data published"
}

test_city() {
    echo "Publishing city driving speeds..."
    publish_speed_batch "$SEND_DELAY_NORMAL" 10 15 20 25 30 25 20 15 10
    echo "✓ City driving sequence completed"
}

test_highway() {
    echo "Publishing highway speeds..."
    publish_speed_batch "$SEND_DELAY_NORMAL" \
        70 80 90 100 90 80 70 \
        70 80 90 100 90 80 70 \
        70 80 90 100 90 80 70 \
        70 80 90 100 90 80 70 \
        70 80 90 100 90 80 70
    echo "✓ Highway sequence completed"
}

test_acceleration() {
    echo "Publishing acceleration sequence..."
    publish_speed_batch "$SEND_DELAY_FAST" 0 10 20 30 40 50 60 70 80 90 100
    echo "✓ Acceleration sequence completed"
}

test_deceleration() {
    echo "Publishing deceleration sequence..."
    publish_speed_batch "$SEND_DELAY_FAST" 100 90 80 70 60 50 40 30 20 10 0
    echo "✓ Deceleration sequence completed"
}

test_random() {
    echo "Publishing random speeds in fast mode..."
    local random_speeds=()
    for i in {1..40}; do
        random_speeds+=("$((RANDOM % 101))")
    done
    publish_speed_batch "$SEND_DELAY_NORMAL" "${random_speeds[@]}"
    echo "✓ Random speed sequence completed"
}

test_custom() {
    read -p "Enter speed in km/h (0-120): " speed
    
    if ! [[ "$speed" =~ ^[0-9]+$ ]] || [ "$speed" -lt 0 ] || [ "$speed" -gt 120 ]; then
        echo "✗ Invalid speed. Please enter a value between 0 and 120."
        return 1
    fi
    
    publish_test_data "$speed"
    echo "✓ Custom speed published"
}

toggle_can_sim() {
    if [ -n "$CAN_SIM_PID" ] && kill -0 "$CAN_SIM_PID" 2>/dev/null; then
        echo "Stopping CAN simulator (PID $CAN_SIM_PID)..."
        kill "$CAN_SIM_PID" 2>/dev/null
        wait "$CAN_SIM_PID" 2>/dev/null
        CAN_SIM_PID=""
        echo "✓ CAN simulator stopped"
        return
    fi

    echo "Starting CAN simulator (dbc_sim.launch.py)..."
    ros2 launch lart_bringup dbc_sim.launch.py &
    CAN_SIM_PID=$!
    echo "✓ CAN simulator started (PID $CAN_SIM_PID)"
}

toggle_bag_record() {
    if [ -n "$BAG_RECORD_PID" ] && kill -0 "$BAG_RECORD_PID" 2>/dev/null; then
        echo "Stopping bag record (PID $BAG_RECORD_PID)..."
        kill -INT "$BAG_RECORD_PID" 2>/dev/null
        wait "$BAG_RECORD_PID" 2>/dev/null
        BAG_RECORD_PID=""
        echo "✓ Bag record stopped"
        return
    fi

    mkdir -p "$BAG_DIR"
    local stamp
    stamp="$(date '+%Y-%m-%dT%H-%M-%S')"
    local out="$BAG_DIR/datastation_$stamp"
    echo "Starting bag record → $out (topics matching: $BAG_RECORD_REGEX)..."
    ros2 bag record -o "$out" --regex "$BAG_RECORD_REGEX" &
    BAG_RECORD_PID=$!
    echo "✓ Bag record started (PID $BAG_RECORD_PID)"
}

set_mission_select() {
    read -p "Select Mission_select raw value [0-7]: " mission

    if ! [[ "$mission" =~ ^[0-7]$ ]]; then
        echo "✗ Invalid value. Please enter a number between 0 and 7."
        return 1
    fi

    if ! ros2 param set /can_simulator mission_select_value "$mission.0"; then
        echo "✗ Failed to set mission_select_value (is the CAN simulator running? option 'a')"
        return 1
    fi
    echo "✓ Mission_select set to $mission"
}

set_as_mission() {
    # Dashboard's mission display falls back to AS_MISSION when
    # Mission_select is 0, so this needs setting too.
    read -p "Select AS_MISSION raw value [0-7]: " mission

    if ! [[ "$mission" =~ ^[0-7]$ ]]; then
        echo "✗ Invalid value. Please enter a number between 0 and 7."
        return 1
    fi

    if ! ros2 param set /can_simulator as_mission_value "$mission.0"; then
        echo "✗ Failed to set as_mission_value (is the CAN simulator running? option 'a')"
        return 1
    fi
    echo "✓ AS_MISSION set to $mission"
}

publish_screen() {
    local screen_id=$1
    echo "Publishing screen change: id=$screen_id to $SCREEN_TOPIC..."

    if ! ros2 topic pub -1 $SCREEN_TOPIC std_msgs/msg/Int32 "{data: ${screen_id}}"; then
        echo "✗ Failed to publish screen change to $SCREEN_TOPIC"
        return 1
    fi
}

show_screen_menu() {
    while true; do
        echo ""
        echo "╔════════════════════════════════════════════╗"
        echo "║  Change Screen (ROS /dashboard/set_screen) ║"
        echo "╚════════════════════════════════════════════╝"
        echo "  0) Driver View"
        echo "  1) Autonomous"
        echo "  2) Debug 1"
        echo "  3) Debug Inverter 2"
        echo "  4) Debug 3"
        echo "  5) Debug Wheels 4"
        echo "  6) Debug 5"
        echo "  7) Debug Autonomous 1"
        echo "  8) Debug Autonomous 2"
        echo "  9) Debug Autonomous 3"
        echo "  10) Debug Autonomous 4"
        echo "  11) Debug Autonomous 5"
        echo "  b) Back"
        echo ""
        read -p "Select screen [0-11/b]: " scr
        case $scr in
            0) publish_screen 0 && echo "✓ Switched to Driver View" ;;
            1) publish_screen 1 && echo "✓ Switched to Autonomous" ;;
            2) publish_screen 2 && echo "✓ Switched to Debug 1" ;;
            3) publish_screen 3 && echo "✓ Switched to Debug Inverter 2" ;;
            4) publish_screen 4 && echo "✓ Switched to Debug 3" ;;
            5) publish_screen 5 && echo "✓ Switched to Debug Wheels 4" ;;
            6) publish_screen 6 && echo "✓ Switched to Debug 5" ;;
            7) publish_screen 7 && echo "✓ Switched to Debug Autonomous 1" ;;
            8) publish_screen 8 && echo "✓ Switched to Debug Autonomous 2" ;;
            9) publish_screen 9 && echo "✓ Switched to Debug Autonomous 3" ;;
            10) publish_screen 10 && echo "✓ Switched to Debug Autonomous 4" ;;
            11) publish_screen 11 && echo "✓ Switched to Debug Autonomous 5" ;;
            b|B) return ;;
            *) echo "✗ Invalid option." ;;
        esac
    done
}

show_help() {
    cat << EOF
╔════════════════════════════════════════════════════════════╗
║  Test Data Generator (Local ROS 2) - Help                  ║
╚════════════════════════════════════════════════════════════╝

This script helps test the ROS 2 topic communication by publishing
sample speed data to dashboard topics locally.

Publishing to:
    - $TOPIC_PREFIX/speed_kph (std_msgs/Float32)
    - $SCREEN_TOPIC (std_msgs/Int32)

Test Scenarios:
  1. Idle      - Tests zero speed (0 km/h)
  2. City      - Simulates city driving (10-30 km/h)
  3. Highway   - High speed run (70-100 km/h)
  4. Accel     - Acceleration from 0 to 100 km/h
  5. Decel     - Deceleration from 100 to 0 km/h
  6. Random    - Random speeds for 10 seconds
  7. Custom    - Publish a single custom speed
  8. Screen    - Change dashboard screen via ROS topic
                 0=Driver View, 1=Autonomous, 2-6=Debug 1-5, 7-11=Debug Autonomous 1-5
  c. Mission_select - Set the CAN simulator's Mission_select value (0-7) via
                  ros2 param set /can_simulator mission_select_value
  d. AS_MISSION - Set the CAN simulator's AS_MISSION value (0-7) via
                  ros2 param set /can_simulator as_mission_value
                  (dashboard falls back to this when Mission_select is 0)

Prerequisites:
  - ROS 2 Jazzy must be installed and sourced.
  - Local workspace should be built and sourced.

Monitoring:
  - Check all topics:       ros2 topic list
                            ros2 topic echo /vehicle/speed_kph
                            ros2 topic echo /dashboard/set_screen

Example workflow:
  1. source /opt/ros/jazzy/setup.bash
  2. source install/setup.bash
  3. ./test_data_ros2.sh

Speed Values:
  - 0 km/h    : Idle
  - 10-50 km/h: City driving
  - 50-100 km/h: Highway
  - 120 km/h  : Maximum (for testing)

EOF
}

# Main loop
while true; do
    echo ""
    show_menu
    read -p "Select option [0-9/a/b/c/d]: " choice

    case $choice in
        1) test_idle; echo ""; read -p "Press Enter to continue..." ;;
        2) test_city; echo ""; read -p "Press Enter to continue..." ;;
        3) test_highway; echo ""; read -p "Press Enter to continue..." ;;
        4) test_acceleration; echo ""; read -p "Press Enter to continue..." ;;
        5) test_deceleration; echo ""; read -p "Press Enter to continue..." ;;
        6) test_random; echo ""; read -p "Press Enter to continue..." ;;
        7) test_custom; echo ""; read -p "Press Enter to continue..." ;;
        8) show_screen_menu ;;
        9) show_help; echo ""; read -p "Press Enter to continue..." ;;
        a|A) toggle_can_sim; echo ""; read -p "Press Enter to continue..." ;;
        b|B) toggle_bag_record; echo ""; read -p "Press Enter to continue..." ;;
        c|C) set_mission_select; echo ""; read -p "Press Enter to continue..." ;;
        d|D) set_as_mission; echo ""; read -p "Press Enter to continue..." ;;
        0)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "✗ Invalid option. Please select 0-9, a, b, c, or d."
            echo ""
            read -p "Press Enter to continue..."
            ;;
    esac
done
