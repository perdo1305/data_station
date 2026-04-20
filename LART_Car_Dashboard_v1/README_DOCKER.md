# LART Dashboard ROS 2 Docker Setup

This directory contains Docker configuration for running the LART Dashboard with ROS 2 (Jazzy) in containerized environments.

## Quick Start

### Using Docker Compose (Recommended)

```bash
# Start both publisher and listener containers
make compose-up

# View logs
make compose-logs

# Stop containers
make compose-down
```

### Using Manual Docker Commands

```bash
# Build the image
make build

# Start publisher and listener
make up

# View logs
make logs

# Stop everything
make stop
```

## Architecture

The setup uses two containers communicating via ROS 2 topics:

```
┌─────────────────────────────────────────┐
│      Docker Bridge Network              │
│   (lart-ros2-network)                   │
│                                         │
│  ┌──────────────────┐  ┌─────────────┐ │
│  │  Publisher       │  │   Listener  │ │
│  │  Container       │  │  Container  │ │
│  │                  │  │             │ │
│  │ • Speed/RPM      │  │ • Subscribes│ │
│  │   Publisher      │  │   to topics │ │
│  │ • Broadcasts     │  │ • Logs data │ │
│  │   /vehicle/rpm   │  │ • Stores    │ │
│  │ • Broadcasts     │  │   metrics   │ │
│  │   /vehicle/      │  │             │ │
│  │   dashboard_state│  │             │ │
│  └──────────────────┘  └─────────────┘ │
│          │                    ▲         │
│          └────────────────────┘         │
│         ROS 2 DDS Middleware            │
└─────────────────────────────────────────┘
```

## Available Makefile Targets

### Docker Compose Commands

- **`make compose-up`** - Start all containers using docker-compose
- **`make compose-down`** - Stop and remove all containers
- **`make compose-logs`** - View logs with automatic following
- **`make compose-rebuild`** - Rebuild images and restart containers

### Manual Docker Commands

- **`make build`** - Build the Docker image
- **`make up`** - Start both publisher and listener containers
- **`make publisher`** - Start only the publisher container
- **`make listener`** - Start only the listener container
- **`make stop`** - Stop all containers
- **`make down`** - Stop and remove all containers
- **`make clean`** - Remove image, containers, and network
- **`make shell`** - Open bash shell in publisher container
- **`make logs`** - Show current logs from all containers
- **`make logs-pub`** - Follow publisher container logs
- **`make logs-listen`** - Follow listener container logs
- **`make ps`** - List running LART Dashboard containers

## ROS 2 Topics

The setup communicates through the following ROS 2 topics:

### Publishing Topics

The **publisher** container publishes:

- **`/vehicle/rpm`** (`std_msgs/Float32`)
  - Current RPM value
  - Example: `31414.0`

- **`/vehicle/dashboard_state`** (`lart_msgs/DashboardState`)
  - Complete dashboard state including:
    - `speed_kph`: Speed in km/h
    - `rpm`: RPM value
    - `throttle_percent`: Throttle percentage
    - `timestamp`: Message timestamp

### Subscribing Topics

The **listener** container subscribes to:
- `/vehicle/rpm` - Logs RPM updates
- `/vehicle/dashboard_state` - Logs dashboard state updates

## Environment Variables

Both containers use these ROS 2 environment variables:

```bash
ROS_DOMAIN_ID=0              # ROS 2 domain ID (0-232)
ROS_LOCALHOST_ONLY=0         # Enable network communication
PYTHONUNBUFFERED=1           # Real-time Python output
```

## Building the Image

### Prerequisites

- Docker Engine 20.10+
- Docker Compose 2.0+ (for compose-based commands)
- ~4GB disk space for image

### Build Process

The image is automatically built when you run:
- `make build`
- `make up`
- `make compose-up`

The build process:
1. Uses `osrf/ros:jazzy-desktop` as base image
2. Installs system dependencies (python3, can-utils, etc.)
3. Installs Python dependencies from requirements.txt
4. Copies the entire ROS 2 workspace
5. Builds the workspace with colcon
6. Sets up ROS 2 environment in the entrypoint

## Container Networking

### Docker Network

A bridge network (`lart-ros2-network`) is created for inter-container communication:

```bash
# View the network
docker network inspect lart-ros2-network

# The containers communicate via:
# - Container names: `publisher`, `listener`
# - ROS 2 DDS middleware (Fast-DDS)
```

### Accessing from Host

To communicate with containers from the host:

1. **Using localhost** (if containers expose ports):
   ```bash
   ros2 topic list -t
   ros2 topic echo /vehicle/rpm
   ```

2. **Entering a container**:
   ```bash
   make shell
   ```

## Troubleshooting

### Containers Won't Start

```bash
# Check logs
make compose-logs

# Or check individual container logs
docker logs lart-dashboard-publisher
docker logs lart-dashboard-listener
```

### ROS 2 Topics Not Visible

```bash
# Inside container, check ROS 2 setup
source /root/.bashrc
ros2 topic list

# Verify DDS middleware
ros2 doctor
```

### Build Fails

```bash
# Clean and rebuild
make clean
make compose-rebuild

# Check Dockerfile logs during build
docker-compose up --build 2>&1 | head -100
```

### Permission Issues

```bash
# Ensure Docker daemon is running
sudo systemctl start docker

# Add user to docker group (optional)
sudo usermod -aG docker $USER
newgrp docker
```

## Performance Tips

- Use `make compose-up` for faster startup
- Ensure 2+ CPU cores available for smooth ROS 2 operation
- Monitor logs with `make compose-logs` for debugging
- Keep ROS_LOCALHOST_ONLY=0 for docker network communication

## Testing the Setup

### Basic Connectivity Test

```bash
# Terminal 1: Start containers
make compose-up

# Terminal 2: Check topics
make shell
# Inside container:
ros2 topic list -t
ros2 topic echo /vehicle/rpm

# Terminal 3: Check listener logs
make logs-listen
```

### Generate Test Data

Inside the publisher container:

```bash
# Publish test speed
ros2 run lart_bringup can_bridge &

# Or use the included speed publisher
python3 /root/lart_ws/src/lart_bringup/speed.py --speed 50 --rate 10
```

## File Structure

```
LART Car Dashboard v1/
├── Dockerfile              # Multi-stage build for ROS 2
├── docker-compose.yml      # Docker Compose orchestration
├── Makefile                # Helper commands
├── .dockerignore            # Exclude files from build context
└── README_DOCKER.md        # This file
```

## References

- [ROS 2 Jazzy Documentation](https://docs.ros.org/en/jazzy/)
- [OSRF ROS 2 Docker Images](https://hub.docker.com/r/osrf/ros)
- [Docker Compose Documentation](https://docs.docker.com/compose/)
- [DDS Middleware Information](https://docs.ros.org/en/jazzy/Concepts/About-Middleware-Implementations.html)

## Support

For issues or questions:
1. Check logs: `make compose-logs`
2. Review Docker build output: `docker-compose build --no-cache`
3. Verify network: `docker network inspect lart-ros2-network`
4. Check ROS 2 setup: `make shell` then `ros2 doctor`
