.PHONY: help build build-arm build-amd up down logs clean shell compose-up compose-down compose-logs \
		build-image rebuild network publisher display ps stop

# Docker image and container names
IMAGE_NAME := lart-dashboard-ros2
CONTAINER_PUBLISHER := lart-dashboard-publisher
NETWORK_NAME := lart-ros2-network
PROJECT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CONTAINER_UI_DIR := /root/lart_ws/LART_Car_Dashboard_v1/src/ui
CONTAINER_UI_BUILD_DIR := /tmp/lart-ui-build
BUILD_PLAT ?= linux/amd64
BUILD_PLAT_ARM ?= linux/arm64
BUILD_PLAT_AMD ?= linux/amd64

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
	@echo "  build-arm     - Build the Docker image for ARM64"
	@echo "  build-amd     - Build the Docker image for AMD64"
	@echo "  up            - Start publisher and display containers"
	@echo "  publisher     - Run only the publisher container"
	@echo "  display       - Run only the display container"
	@echo "  shell         - Open shell in publisher container"
	@echo "  logs          - Show logs from all containers"
	@echo ""
	@echo "Local (no Docker):"
	@echo "  publish-local  - Run a simple local ROS2 publisher (no docker)"
	@echo "  listener-local - Run the Python listener locally"
	@echo "  display-local  - Build and run the UI locally (requires deps)"
	@echo "  run-all-local   - Start publisher+listener in background and run UI"
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
	@xhost +local:root >/dev/null 2>&1 || true
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

# Ensure buildx builder exists and is bootstrapped
.PHONY: ensure-buildx
ensure-buildx:
	@docker buildx inspect lart-builder >/dev/null 2>&1 || docker buildx create --name lart-builder --use
	@docker buildx inspect --bootstrap >/dev/null 2>&1 || true

# ============================================================================
# Manual Docker Targets
# ============================================================================

build:
	@echo "Building Docker image: $(IMAGE_NAME)"
	docker build -t $(IMAGE_NAME) $(PROJECT_DIR)

build-arm:
	@$(MAKE) build BUILD_PLAT=$(BUILD_PLAT_ARM)

build-amd:
	@$(MAKE) build BUILD_PLAT=$(BUILD_PLAT_AMD)

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
	@xhost +local:root >/dev/null 2>&1 || true
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

# ---------------------------------------------------------------------------
# Local (no Docker) run targets
# ---------------------------------------------------------------------------

check-ui-deps:
	@bash -lc 'set -e; if ! pkg-config --exists sdl2; then echo "Missing SDL2 development files for the desktop UI."; echo "Install on Raspberry Pi OS: sudo apt install libsdl2-dev pkg-config"; exit 2; fi; if [[ ! -d "$(PROJECT_DIR)/src/ui/third_party/lvgl/src" ]]; then echo "LVGL sources are missing; initializing the bundled submodule..."; git -C "$(PROJECT_DIR)" submodule update --init --recursive -- src/ui/third_party/lvgl; fi; if [[ ! -d "$(PROJECT_DIR)/src/ui/third_party/lvgl/src" ]]; then echo "Missing LVGL sources at $(PROJECT_DIR)/src/ui/third_party/lvgl/src."; echo "Run: git -C $(PROJECT_DIR) submodule update --init --recursive -- src/ui/third_party/lvgl"; exit 2; fi'

publish-local:
	@echo "Running local numeric publisher at 20Hz. Ctrl-C to stop"
	@bash -lc 'source /opt/ros/jazzy/setup.bash >/dev/null 2>&1 || true; source $(PROJECT_DIR)/install/setup.bash >/dev/null 2>&1 || true; while true; do for value in 0 10 20 30 40 50 60 70 80 90 100; do echo "speed_kph=$$value"; ros2 topic pub --once /vehicle/speed_kph std_msgs/msg/Float32 "{data: $$value}" >/dev/null; done; done'

listener-local:
	@echo "Running local listener (dashboard_ui.speed_listener)"
	@bash -lc 'source /opt/ros/jazzy/setup.bash >/dev/null 2>&1 || true; export PYTHONPATH=$(PROJECT_DIR)/src; python3 -m dashboard_ui.speed_listener'

display-local: check-ui-deps
	@echo "Building and running UI locally (requires SDL2; ROS2 is optional)"
	@bash -lc 'set -e; cmake -S $(PROJECT_DIR)/src/ui -B $(PROJECT_DIR)/build/ui-build || true; cmake --build $(PROJECT_DIR)/build/ui-build --parallel $$(nproc); UI_BIN="$(PROJECT_DIR)/build/ui-build/ui_runner"; if [[ ! -x "$$UI_BIN" ]]; then echo "ui_runner was not generated. Install libsdl2-dev and ensure LVGL sources exist at $(PROJECT_DIR)/src/ui/third_party/lvgl/src."; exit 2; fi; exec "$$UI_BIN"'

run-all-local: check-ui-deps
	@echo "Starting publisher and listener in background; launching display in foreground"
	@bash -lc 'set -e; source /opt/ros/jazzy/setup.bash >/dev/null 2>&1 || true; source $(PROJECT_DIR)/install/setup.bash >/dev/null 2>&1 || true; export PYTHONPATH=$(PROJECT_DIR)/src; ros2 topic pub -r 10 /vehicle/speed_kph std_msgs/msg/Float32 "{data: 40.0}" & PUB_PID=$$!; python3 -m dashboard_ui.speed_listener & LIST_PID=$$!; trap "kill $$PUB_PID $$LIST_PID 2>/dev/null || true" EXIT INT TERM; echo "Started publisher $$PUB_PID and listener $$LIST_PID"; cmake -S $(PROJECT_DIR)/src/ui -B $(PROJECT_DIR)/build/ui-build || true; cmake --build $(PROJECT_DIR)/build/ui-build --parallel $$(nproc); UI_BIN="$(PROJECT_DIR)/build/ui-build/ui_runner"; if [[ ! -x "$$UI_BIN" ]]; then echo "ui_runner was not generated. Install libsdl2-dev and ensure LVGL sources exist at $(PROJECT_DIR)/src/ui/third_party/lvgl/src."; exit 2; fi; exec "$$UI_BIN"'
