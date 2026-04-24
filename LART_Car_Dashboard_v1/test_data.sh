#!/bin/bash
# Test Data Generator for LART Dashboard ROS 2
# Publishes sample speed data to test the topic communication

set -e

CONTAINER_NAME="lart-dashboard-publisher"
TOPIC_PREFIX="/vehicle"
SPEED_VALUES=(10 20 30 40 50 60 70 80 90 100)
SEND_DELAY_NORMAL=0.2
SEND_DELAY_FAST=0.1

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  LART Dashboard - ROS 2 Test Data Generator                ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Check if container is running
if ! docker ps -q --filter "name=$CONTAINER_NAME" | grep -q .; then
    echo "✗ Publisher container is not running!"
    echo "  Start it with: make compose-up"
    exit 1
fi

echo "✓ Publisher container is running: $CONTAINER_NAME"
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
publish_test_data() {
    local speed=$1
    local speed_noise=$((RANDOM % 7 - 3))
    local speed_kph=$((speed + speed_noise))
    local rpm
    local inv_temp_c
    local motor_temp_c
    local bps
    local kw_inst
    local kw_limit
    local soc_lv
    local soc_hv
    local lv_voltage
    local r2d_ready
    local can_active

    if [ "$speed_kph" -lt 0 ]; then
        speed_kph=0
    fi
    if [ "$speed_kph" -gt 120 ]; then
        speed_kph=120
    fi

    rpm=$((speed_kph * $(rand_int 90 150)))
    inv_temp_c=$(rand_float 25 85)
    motor_temp_c=$(rand_float 30 110)
    bps=$(rand_float 0 100)
    kw_inst=$(rand_float 0 120)
    kw_limit=$(rand_float 80 180)
    soc_lv=$(rand_float 20 100)
    soc_hv=$(rand_float 10 100)
    lv_voltage=$(rand_float 11.50 14.80)
    r2d_ready=$(chance_true 90)
    can_active=$(chance_true 95)
    
    echo "Publishing: speed_kph=$speed_kph km/h, rpm=$rpm, inv=${inv_temp_c}C, motor=${motor_temp_c}C"
    
    # Use explicit ROS setup files because .bashrc may skip setup in non-interactive shells.
    if ! docker exec "$CONTAINER_NAME" bash -lc "
        source /opt/ros/jazzy/setup.bash &&
        source /root/lart_ws/install/setup.bash &&
        ros2 topic pub -1 $TOPIC_PREFIX/dashboard_state lart_msgs/msg/DashboardState '{
                    rpm: $rpm,
                    speed_kph: $speed_kph,
                    inv_temp_c: $inv_temp_c,
                    motor_temp_c: $motor_temp_c,
                    bps: $bps,
                    kw_inst: $kw_inst,
                    kw_limit: $kw_limit,
                    soc_lv: $soc_lv,
                    soc_hv: $soc_hv,
                    lv_voltage: $lv_voltage,
                    r2d_ready: $r2d_ready,
                    can_active: $can_active
        }'
    "; then
        echo "✗ Failed to publish test data to $TOPIC_PREFIX/dashboard_state"
        echo "  Check container logs with: make logs-pub"
        return 1
    fi
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
    echo "8) Help"
    echo "9) Exit"
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
    for speed in 10 15 20 25 30 25 20 15 10; do
        publish_test_data $speed
        sleep "$SEND_DELAY_NORMAL"
    done
    echo "✓ City driving sequence completed"
}

test_highway() {
    echo "Publishing highway speeds..."
    for i in {1..5}; do
        for speed in 70 80 90 100 90 80 70; do
            publish_test_data $speed
            sleep "$SEND_DELAY_NORMAL"
        done
    done
    echo "✓ Highway sequence completed"
}

test_acceleration() {
    echo "Publishing acceleration sequence..."
    for speed in {0..100..10}; do
        publish_test_data $speed
        sleep "$SEND_DELAY_FAST"
    done
    echo "✓ Acceleration sequence completed"
}

test_deceleration() {
    echo "Publishing deceleration sequence..."
    for speed in {100..0..10}; do
        publish_test_data $speed
        sleep "$SEND_DELAY_FAST"
    done
    echo "✓ Deceleration sequence completed"
}

test_random() {
    echo "Publishing random speeds in fast mode..."
    for i in {1..40}; do
        speed=$((RANDOM % 101))
        publish_test_data $speed
        sleep "$SEND_DELAY_NORMAL"
    done
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

show_help() {
    cat << EOF
╔════════════════════════════════════════════════════════════╗
║  Test Data Generator - Help                                ║
╚════════════════════════════════════════════════════════════╝

This script helps test the ROS 2 topic communication by publishing
sample speed data to dashboard topics.

Publishing to:
    - $TOPIC_PREFIX/dashboard_state (lart_msgs/DashboardState)

Test Scenarios:
  1. Idle      - Tests zero speed (0 km/h)
  2. City      - Simulates city driving (10-30 km/h)
  3. Highway   - High speed run (70-100 km/h)
  4. Accel     - Acceleration from 0 to 100 km/h
  5. Decel     - Deceleration from 100 to 0 km/h
  6. Random    - Random speeds for 10 seconds
  7. Custom    - Publish a single custom speed

Prerequisites:
  - Docker containers must be running: make compose-up
  - Containers need network connectivity

Monitoring:
  - View publisher logs:    make logs-pub
  - View listener logs:     make logs-listen
  - Check all topics:       make shell
                            ros2 topic echo /vehicle/dashboard_state

Example workflow:
  1. make compose-up        # Start containers
  2. ./test_data.sh         # Run this script
  3. make compose-logs      # View results
  4. make compose-down      # Stop when done

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
    read -p "Select option [1-9]: " choice
    
    case $choice in
        1) test_idle ;;
        2) test_city ;;
        3) test_highway ;;
        4) test_acceleration ;;
        5) test_deceleration ;;
        6) test_random ;;
        7) test_custom ;;
        8) show_help ;;
        9) 
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "✗ Invalid option. Please select 1-9."
            ;;
    esac
    
    echo ""
    read -p "Press Enter to continue..."
done
