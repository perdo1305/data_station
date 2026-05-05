.PHONY: help build up down logs clean shell compose-up compose-down compose-logs \
		build-image rebuild network publisher display ps stop

# Docker image and container names
IMAGE_NAME := lart-dashboard-ros2
CONTAINER_PUBLISHER := lart-dashboard-publisher
NETWORK_NAME := lart-ros2-network
PROJECT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CONTAINER_UI_DIR := /root/lart_ws/LART_Car_Dashboard_v1/src/ui
CONTAINER_UI_BUILD_DIR := /tmp/lart-ui-build

help:
	@echo "LART Dashboard ROS2 Docker Management"
	@echo "====================================="
	@echo ""
	@echo "Docker Compose (Recommended):"
	@echo "  compose-up    - Start all containers using docker compose (recommended)"
	@echo "  compose-down  - Stop all containers"
	@echo "  compose-logs  - View logs from all running containers"
	@echo "  compose-rebuild - Rebuild images and start containers"
	@echo ""
	@echo "Manual Docker Management:"
	@echo "  build         - Build the Docker image"
	@echo "  up            - Start publisher and display containers"
	@echo "  publisher     - Run only the publisher container"
	@echo "  display       - Run only the display container"
	@echo "  shell         - Open shell in publisher container"
	@echo "  logs          - Show logs from all containers"
	@echo "  logs-pub      - Follow publisher container logs"
	@echo "  ps            - Show running containers"
	@echo "  stop          - Stop all containers"
	@echo "  down          - Stop and remove all containers"
	@echo "  clean         - Remove image and all containers"

# ============================================================================
# Docker Compose Targets (Recommended for easy multi-container management)
# ============================================================================

compose-up:
	@echo "Starting containers with docker compose..."
	docker compose up -d
	@echo "✓ Containers are running"
	@echo ""
	@echo "View logs:"
	@echo "  make compose-logs"
	@echo ""
	@echo "Stop containers:"
	@echo "  make compose-down"

compose-down:
	@echo "Stopping containers..."
	docker compose down
	@echo "✓ Containers stopped"

compose-logs:
	docker compose logs -f

compose-rebuild:
	@echo "Rebuilding images and starting containers..."
	docker compose up -d --build
	@echo "✓ Rebuild complete"

# ============================================================================
# Manual Docker Targets
# ============================================================================

build:
	@echo "Building Docker image: $(IMAGE_NAME)"
	docker build -t $(IMAGE_NAME) $(PROJECT_DIR)

rebuild: clean build

network:
	@echo "Creating Docker network: $(NETWORK_NAME)"
	docker network create $(NETWORK_NAME) 2>/dev/null || echo "Network already exists"

up: build network publisher display
	@echo "✓ Publisher and display are running"
	@echo ""
	@echo "View logs: make logs"
	@echo "Stop:      make stop"

publisher: build network
	@echo "Starting publisher container..."
	docker run -d \
		--name $(CONTAINER_PUBLISHER) \
		--network $(NETWORK_NAME) \
		--hostname publisher \
		-e ROS_DOMAIN_ID=0 \
		-e ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET \
		-e PYTHONUNBUFFERED=1 \
		-e DISPLAY=$$DISPLAY \
		-e QT_X11_NO_MITSHM=1 \
		-e XAUTHORITY=/root/.Xauthority \
		-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
		-v $$HOME/.Xauthority:/root/.Xauthority:ro \
		-v $(PROJECT_DIR):/root/lart_ws/LART_Car_Dashboard_v1 \
		$(IMAGE_NAME) \
		bash -c "source /root/.bashrc && echo 'Publisher running...' && ros2 topic pub -r 10 /vehicle/speed_kph std_msgs/msg/Float32 '{data: 40.0}'"
	@echo "✓ Publisher started: $(CONTAINER_PUBLISHER)"

display: build network
	@echo "Starting display container..."
	docker run -d \
		--name lart-dashboard-display \
		--network $(NETWORK_NAME) \
		--hostname display \
		-e ROS_DOMAIN_ID=0 \
		-e ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET \
		-e PYTHONUNBUFFERED=1 \
		-e DISPLAY=$$DISPLAY \
		-e QT_X11_NO_MITSHM=1 \
		-e XAUTHORITY=/root/.Xauthority \
		-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
		-v $$HOME/.Xauthority:/root/.Xauthority:ro \
		-v $(PROJECT_DIR):/root/lart_ws/LART_Car_Dashboard_v1 \
		--ipc=host \
		$(IMAGE_NAME) \
		bash -c "source /root/.bashrc && rm -rf $(CONTAINER_UI_BUILD_DIR) && cmake -S $(CONTAINER_UI_DIR) -B $(CONTAINER_UI_BUILD_DIR) && cmake --build $(CONTAINER_UI_BUILD_DIR) --parallel $$(nproc) && $(CONTAINER_UI_BUILD_DIR)/ui_runner"
	@echo "✓ Display started: lart-dashboard-display"

listener: build network
	@echo "Starting listener container..."
	docker run -d \
		--name $(CONTAINER_LISTENER) \
		--network $(NETWORK_NAME) \
		--hostname listener \
		-e ROS_DOMAIN_ID=0 \
		-e ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET \
		-e PYTHONUNBUFFERED=1 \
		-e DISPLAY=$$DISPLAY \
		-e QT_X11_NO_MITSHM=1 \
		-e XAUTHORITY=/root/.Xauthority \
		-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
		-v $$HOME/.Xauthority:/root/.Xauthority:ro \
		$(IMAGE_NAME) \
		bash -c "source /root/.bashrc && echo 'Listener running...' && ros2 run dashboard_ui speed_listener_node"
	@echo "✓ Listener started: $(CONTAINER_LISTENER)"

shell:
	@echo "Opening shell in publisher container..."
	docker exec -it $(CONTAINER_PUBLISHER) /bin/bash

ps:
	@docker ps -a --filter "name=lart-dashboard" --format "table {{.Names}}\t{{.Status}}\t{{.Image}}"

stop:
	@echo "Stopping all containers..."
	-docker stop $(CONTAINER_PUBLISHER) lart-dashboard-display 2>/dev/null || true
	@echo "✓ Containers stopped"

down: stop
	@echo "Removing all containers..."
	-docker rm $(CONTAINER_PUBLISHER) lart-dashboard-display 2>/dev/null || true
	@echo "✓ Containers removed"

clean: down
	@echo "Removing Docker image and network..."
	-docker rmi $(IMAGE_NAME) 2>/dev/null || true
	-docker network rm $(NETWORK_NAME) 2>/dev/null || true
	@echo "✓ Cleanup complete"

logs:
	@echo "=== Publisher Logs ==="
	@docker logs $(CONTAINER_PUBLISHER) 2>/dev/null || echo "Publisher not running"
	@echo ""
	@echo "=== Display Logs ==="
	@docker logs lart-dashboard-display 2>/dev/null || echo "Display not running"

logs-pub:
	@docker logs -f $(CONTAINER_PUBLISHER)
