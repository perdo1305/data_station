# Docker Setup Summary

## Files Created

This Docker setup for LART Dashboard ROS 2 consists of the following files:

### Core Docker Files

#### 1. **Dockerfile**
- Base image: `osrf/ros:jazzy-desktop`
- Installs ROS 2 Jazzy, Python dependencies, and system tools
- Builds the ROS 2 workspace with colcon
- Provides an entrypoint script that sources ROS 2 environment

**Key features:**
- Automatic ROS 2 environment setup on container start
- Workspace built and ready to run
- Supports inter-container communication via ROS 2 DDS

#### 2. **docker-compose.yml**
- Orchestrates two services: `publisher` and `listener`
- Creates a bridge network for container communication
- Environment variables configured for ROS 2 multi-container setup
- Auto-restart on failure (unless-stopped)

**Services:**
- **publisher**: Runs dashboard_ui (speed publisher)
- **listener**: Runs speed_listener_node (ROS 2 topic subscriber)

### Helper Files

#### 3. **Makefile**
Provides convenient commands for container management:

**Compose commands (recommended):**
- `make compose-up` - Start all containers
- `make compose-down` - Stop all containers
- `make compose-logs` - View logs
- `make compose-rebuild` - Rebuild and restart

**Manual commands:**
- `make build` - Build image
- `make up` - Start publisher and listener
- `make shell` - Enter container shell
- `make ps` - Show running containers
- `make logs` - View logs
- `make clean` - Remove everything

#### 4. **.dockerignore**
- Excludes unnecessary files from Docker build context
- Reduces build time and image size
- Ignores: .git, __pycache__, .vscode, build artifacts, etc.

### Documentation Files

#### 5. **README_DOCKER.md**
Comprehensive documentation including:
- Architecture overview (visual diagram)
- Quick start instructions
- Complete Makefile reference
- ROS 2 topics documentation
- Troubleshooting guide
- Performance tips
- Testing procedures

#### 6. **DOCKER_QUICKSTART.md**
Quick reference guide with:
- Essential commands
- ROS 2 topics list
- Command summary table
- Troubleshooting tips

#### 7. **demo.sh**
Interactive demo script that provides a menu for:
- Building the Docker image
- Starting containers
- Viewing logs
- Opening shells
- Checking topics
- Stopping/cleaning up

Make it executable: `chmod +x demo.sh`
Run with: `./demo.sh`

### Application Files (Added to main workspace)

#### 8. **src/dashboard_ui/dashboard_ui/speed_listener.py**
ROS 2 node that subscribes to speed topics:
- Listens to `/vehicle/rpm` (Float32)
- Listens to `/vehicle/dashboard_state` (DashboardState)
- Logs received data to console
- Node name: `speed_listener`

#### 9. **src/dashboard_ui/setup.py** (Modified)
Updated entry points to include:
```python
'speed_listener_node = dashboard_ui.speed_listener:main'
```

This allows running: `ros2 run dashboard_ui speed_listener_node`

## Architecture

```
Directory: LART Car Dashboard v1/
├── Dockerfile           - Container definition
├── docker-compose.yml   - Multi-container orchestration
├── Makefile            - Helper commands
├── .dockerignore        - Build context filter
├── README_DOCKER.md    - Full documentation
├── DOCKER_QUICKSTART.md - Quick reference
├── demo.sh             - Interactive demo
└── src/                - ROS 2 workspace
    └── dashboard_ui/
        └── dashboard_ui/
            └── speed_listener.py  - Topic listener node
```

## Inter-Container Communication

Containers communicate via:

1. **Docker Bridge Network** (`lart-ros2-network`)
   - Containers can reach each other by hostname
   - publisher → listener, listener → publisher

2. **ROS 2 DDS Middleware**
   - Topics published by publisher container
   - Listener container subscribes to same topics
   - Both use ROS_DOMAIN_ID=0

3. **Topics**
   - `/vehicle/rpm` - Speed in RPM (Float32)
   - `/vehicle/dashboard_state` - Full dashboard state

## Quick Start

### Option 1: Docker Compose (Recommended)
```bash
cd "LART Car Dashboard v1"
make compose-up
```

### Option 2: Manual Docker
```bash
cd "LART Car Dashboard v1"
make build && make up
```

### Option 3: Interactive Demo
```bash
cd "LART Car Dashboard v1"
./demo.sh
```

## Workflow Example

1. **Start containers:**
   ```bash
   make compose-up
   ```

2. **View logs in real-time:**
   ```bash
   make compose-logs
   ```

3. **Enter publisher container:**
   ```bash
   make shell
   ```

4. **Inside container, check topics:**
   ```bash
   ros2 topic list
   ros2 topic echo /vehicle/rpm
   ```

5. **Exit and stop containers:**
   ```bash
   exit              # Leave shell
   make compose-down # Stop containers
   ```

## Environment Variables

All containers set:
- `ROS_DOMAIN_ID=0` - ROS 2 domain for discovery
- `ROS_LOCALHOST_ONLY=0` - Allow network communication
- `PYTHONUNBUFFERED=1` - Real-time output

## Performance Considerations

- Image size: ~4GB (osrf/ros:jazzy-desktop base)
- Memory per container: ~256MB base + ROS 2 usage
- Network: Docker bridge (fast, local)
- DDS: Uses Fast-DDS by default (efficient)

## Troubleshooting

**Containers fail to start:**
```bash
make compose-logs  # Check error output
```

**ROS 2 not found inside container:**
```bash
make shell
source /root/.bashrc  # Re-source environment
```

**Rebuild from scratch:**
```bash
make clean
make compose-up
```

## Next Steps

1. **Customize the nodes:**
   - Edit `src/dashboard_ui/dashboard_ui/speed_listener.py`
   - Modify docker-compose.yml for different services

2. **Add more containers:**
   - Add more services to docker-compose.yml
   - Update Makefile with new targets

3. **Deploy to Raspberry Pi:**
   - Use `docker buildx` for ARM64 builds
   - Push to private registry
   - Pull on Raspberry Pi

4. **Production setup:**
   - Add logging volume: `-v $(pwd)/logs:/root/logs`
   - Add resource limits in docker-compose.yml
   - Consider Kubernetes for orchestration
