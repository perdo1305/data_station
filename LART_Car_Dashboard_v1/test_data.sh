#!/bin/bash
# Test Data Generator for LART Dashboard ROS 2
# Publishes sample speed data to test the topic communication

set -e

CONTAINER_NAME="lart-dashboard-publisher"
TOPIC_PREFIX="/vehicle"
SPEED_VALUES=(10 20 30 40 50 60 70 80 90 100)

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

# Function to publish test data
publish_test_data() {
    local speed=$1
    local rpm=$((speed * 100))
    
    echo "Publishing: Speed=$speed km/h, RPM=$rpm"
    
    # Execute in container
    docker exec $CONTAINER_NAME bash -c "
        source /root/.bashrc
        
        # Publish to RPM topic
        ros2 topic pub -1 $TOPIC_PREFIX/rpm std_msgs/Float32 'data: $rpm'
    " 2>/dev/null || true
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
        sleep 1
    done
    echo "✓ City driving sequence completed"
}

test_highway() {
    echo "Publishing highway speeds..."
    for i in {1..5}; do
        for speed in 70 80 90 100 90 80 70; do
            publish_test_data $speed
            sleep 1
        done
    done
    echo "✓ Highway sequence completed"
}

test_acceleration() {
    echo "Publishing acceleration sequence..."
    for speed in {0..100..10}; do
        publish_test_data $speed
        sleep 0.5
    done
    echo "✓ Acceleration sequence completed"
}

test_deceleration() {
    echo "Publishing deceleration sequence..."
    for speed in {100..0..10}; do
        publish_test_data $speed
        sleep 0.5
    done
    echo "✓ Deceleration sequence completed"
}

test_random() {
    echo "Publishing random speeds for 10 seconds..."
    for i in {1..10}; do
        speed=$((RANDOM % 101))
        publish_test_data $speed
        sleep 1
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
sample speed data to the /vehicle/rpm topic.

Publishing to: $TOPIC_PREFIX/rpm (std_msgs/Float32)

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
                            ros2 topic echo /vehicle/rpm

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
