#!/bin/bash
# LART Dashboard Docker Demo Script
# This script demonstrates the basic workflow for running the ROS 2 containers

set -e

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  LART Dashboard ROS 2 Docker Setup - Demo Script           ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if docker is installed
if ! command -v docker &> /dev/null; then
    echo -e "${YELLOW}ERROR: Docker is not installed or not in PATH${NC}"
    echo "Install Docker from: https://docs.docker.com/get-docker/"
    exit 1
fi

# Check if docker-compose is installed
if ! command -v docker-compose &> /dev/null; then
    echo -e "${YELLOW}WARNING: docker-compose not found, will use 'docker compose'${NC}"
fi

# Menu function
show_menu() {
    echo ""
    echo -e "${BLUE}Select an action:${NC}"
    echo "1) Build Docker image"
    echo "2) Start containers (docker-compose)"
    echo "3) View logs"
    echo "4) Open shell in publisher container"
    echo "5) Check ROS 2 topics"
    echo "6) Stop containers"
    echo "7) Clean everything"
    echo "8) Exit"
    echo ""
}

# Action functions
build_image() {
    echo -e "${GREEN}Building Docker image...${NC}"
    make build
    echo -e "${GREEN}✓ Build complete${NC}"
}

start_containers() {
    echo -e "${GREEN}Starting containers with docker-compose...${NC}"
    make compose-up
    sleep 2
    echo ""
    echo -e "${GREEN}✓ Containers are running!${NC}"
    echo ""
    echo -e "${BLUE}Container Status:${NC}"
    make ps
    echo ""
    echo "Next step: View logs with option 3"
}

view_logs() {
    echo -e "${GREEN}Viewing container logs (Press Ctrl+C to exit)...${NC}"
    echo ""
    make compose-logs 2>/dev/null || {
        echo -e "${YELLOW}Containers may not be running. Start them first with option 2${NC}"
    }
}

open_shell() {
    echo -e "${GREEN}Opening shell in publisher container...${NC}"
    echo "Try these commands:"
    echo "  ros2 topic list         # See available ROS 2 topics"
    echo "  ros2 topic echo /vehicle/rpm  # View speed data"
    echo "  exit                    # Exit the container"
    echo ""
    make shell 2>/dev/null || {
        echo -e "${YELLOW}Publisher container is not running. Start it first with option 2${NC}"
    }
}

check_topics() {
    echo -e "${GREEN}Checking ROS 2 topics...${NC}"
    echo ""
    make shell_cmd="docker exec -it lart-dashboard-publisher /root/entrypoint.sh ros2 topic list -t" 2>/dev/null || {
        docker exec -it lart-dashboard-publisher bash -c "source /root/.bashrc && ros2 topic list -t"
    } 2>/dev/null || {
        echo -e "${YELLOW}Publisher container is not running${NC}"
        echo "Run 'make compose-up' first"
    }
}

stop_containers() {
    echo -e "${GREEN}Stopping containers...${NC}"
    make compose-down
    echo -e "${GREEN}✓ Containers stopped${NC}"
}

clean_everything() {
    echo -e "${YELLOW}This will remove containers, images, and networks.${NC}"
    read -p "Continue? (y/N) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${GREEN}Cleaning up...${NC}"
        make clean
        echo -e "${GREEN}✓ Cleanup complete${NC}"
    else
        echo "Cancelled"
    fi
}

# Main loop
while true; do
    show_menu
    read -p "Enter your choice [1-8]: " choice
    
    case $choice in
        1)
            build_image
            ;;
        2)
            start_containers
            ;;
        3)
            view_logs
            ;;
        4)
            open_shell
            ;;
        5)
            check_topics
            ;;
        6)
            stop_containers
            ;;
        7)
            clean_everything
            ;;
        8)
            echo -e "${GREEN}Goodbye!${NC}"
            exit 0
            ;;
        *)
            echo -e "${YELLOW}Invalid option. Please select 1-8.${NC}"
            ;;
    esac
done
