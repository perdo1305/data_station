# LART Dashboard - Start Guide

## Project Overview

The **LART Dashboard** is a ROS 2-based vehicle dashboard system designed for Formula Student cars. It features:

- **Real-time Vehicle Data Display**: Shows speed, RPM, and other vehicle metrics
- **Docker-based Architecture**: Containerized ROS 2 environment for easy deployment
- **LVGL UI Framework**: Modern, responsive dashboard interface built with LVGL (Light and Versatile Graphics Library)
- **Modular Components**:
  - `publisher`: ROS 2 node for publishing vehicle data
  - `display`: Dashboard UI rendering engine

### Technology Stack

- **ROS 2 Jazzy**: Robot Operating System middleware
- **Docker & Docker Compose**: Containerization
- **LVGL**: Embedded graphics library
- **CMake**: Build system for the UI
- **Python**: Test and demo scripts

---

## Prerequisites

### System Requirements

- **Docker & Docker Compose**: Latest versions
- **X11 Server** (Linux with display): For UI rendering
- **4GB+ RAM**: Recommended for smooth operation
- **Modern CPU**: For comfortable performance

### Installation

1. **Install Docker**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install docker.io docker-compose -y
   sudo usermod -aG docker $USER
   ```

2. **Verify Installation**:
   ```bash
   docker --version
   docker compose version
   ```

3. **Clone/Setup Project**:
   ```bash
   cd /home/sintra/dev/data_station/LART_Car_Dashboard_v1
   ```

---

## Quick Start

### Option 1: Automated Demo (Recommended)

Run the demo script for an interactive walkthrough:

```bash
bash demo.sh
```

This launches an interactive menu that guides you through:
1. Building the Docker image
2. Starting containers
3. Viewing logs
4. Opening a shell
5. Checking ROS 2 topics
6. Stopping and cleaning up

### Option 2: Manual Setup

```bash
# Start all containers
make compose-up

# In another terminal, view logs
make compose-logs

# Open shell in publisher container
make shell
```

### Option 3: Quick Build & Run (One-liner)

```bash
cd /home/sintra/dev/data_station/LART_Car_Dashboard_v1 && \
docker compose build --no-cache display && \
docker compose up display
```

---

## Using demo.sh

The **demo.sh** script provides an interactive menu-driven interface to manage the Docker containers.

### Features

✓ **Docker verification** - Checks if Docker is installed  
✓ **Interactive menu** - Easy navigation through common tasks  
✓ **Build management** - Build or rebuild Docker images  
✓ **Container control** - Start, stop, and manage containers  
✓ **Log viewing** - Real-time container logs  
✓ **ROS 2 integration** - Check topics and interact with ROS 2  
✓ **Cleanup options** - Remove containers and images  

### Running the Script

```bash
bash demo.sh
```

### Menu Options

```
1) Build Docker image
   - Builds the Docker image from the Dockerfile
   - Use when: First time setup or after code changes

2) Start containers (docker-compose)
   - Launches publisher and display containers
   - Creates the ROS 2 network bridge
   - Use when: Ready to run the dashboard

3) View logs
   - Shows real-time logs from all containers
   - Press Ctrl+C to exit
   - Useful for: Debugging and monitoring

4) Open shell in publisher container
   - Provides shell access to the publisher container
   - Useful for: Running ROS 2 commands, testing topics

5) Check ROS 2 topics
   - Lists available ROS 2 topics with their types
   - Example output: /vehicle/speed_kph [std_msgs/msg/Float32]

6) Stop containers
   - Gracefully stops all running containers
   - Data is preserved for next run

7) Clean everything
   - Removes all containers and images
   - Use when: You need a fresh start

8) Exit
   - Closes the demo script
```

### Workflow Example

```bash
# 1. Run demo script
bash demo.sh

# 2. Select option 1 - Build Docker image (first time only)
# 3. Select option 2 - Start containers
# 4. Select option 3 - View logs (watch startup process)
# 5. In another terminal: Select option 5 - Check ROS 2 topics
# 6. Select option 6 - Stop containers when done
```

---

## Using test_data.sh

The **test_data.sh** script publishes test speed data to the ROS 2 network for testing the dashboard without live vehicle data.

### Purpose

- **Test dashboard responsiveness** without real vehicle data
- **Simulate speed changes** to verify UI updates
- **Validate ROS 2 communication** between containers
- **Develop and debug** without hardware dependencies

### Prerequisites

Before running `test_data.sh`, ensure:

```bash
# 1. Containers are running
make compose-up

# 2. Publisher container is active
docker ps | grep lart-dashboard-publisher

# 3. ROS 2 network is established
docker exec lart-dashboard-publisher ros2 topic list
```

### Running the Script

```bash
bash test_data.sh
```

### Data Publishing

The script publishes speed data to the ROS 2 topic:

```
Topic: /vehicle/speed_kph
Message Type: std_msgs/msg/Float32
Value Range: 0-120 km/h
```

### Test Scenarios

#### Scenario 1: Slow Acceleration (Watch UI updates)

```bash
# Terminal 1: View logs
make compose-logs

# Terminal 2: Publish test data
bash test_data.sh
# This will gradually increase speed values
```

#### Scenario 2: Manual Speed Testing

```bash
# In publisher container shell
docker exec -it lart-dashboard-publisher bash

# Then publish a specific speed value
source /opt/ros/jazzy/setup.bash
source /root/lart_ws/install/setup.bash
ros2 topic pub -1 /vehicle/speed_kph std_msgs/msg/Float32 "{data: 85.5}"
```

#### Scenario 3: Continuous Data Stream

```bash
# Publish speed at regular intervals
docker exec -it lart-dashboard-publisher bash -c "
  source /opt/ros/jazzy/setup.bash
  source /root/lart_ws/install/setup.bash
  for i in {10..100..10}; do
    ros2 topic pub -1 /vehicle/speed_kph std_msgs/msg/Float32 \"{data: \$i}\"
    sleep 1
  done
"
```

---

## Common Workflows

### Workflow 1: First-Time Setup

```bash
# 1. Navigate to project
cd /home/sintra/dev/data_station/LART_Car_Dashboard_v1

# 2. Run demo script
bash demo.sh

# 3. Select "1" to build Docker image (takes 2-5 minutes)
# 4. Select "2" to start containers
# 5. Watch for startup messages in logs
# 6. Check containers are running
docker ps
```

### Workflow 2: Daily Development

```bash
# Terminal 1: Start containers and view logs
make compose-up
make compose-logs

# Terminal 2: Run test data
bash test_data.sh

# Terminal 3: Interact with ROS 2
docker exec -it lart-dashboard-publisher bash
ros2 topic echo /vehicle/speed_kph
```

### Workflow 3: Debugging

```bash
# 1. View container logs
make compose-logs

# 2. Open shell in publisher
make shell

# 3. Check ROS 2 topics
ros2 topic list -t

# 4. Echo a specific topic
ros2 topic echo /vehicle/speed_kph

# 5. Check node status
ros2 node list
```

### Workflow 4: Code Changes & Rebuild

```bash
# 1. Stop containers
make compose-down

# 2. Make code changes

# 3. Rebuild everything
make compose-rebuild

# Or use demo.sh option 7 (Clean everything) then option 1 (Build)
```

---

## Docker Compose Commands Reference

| Command | Purpose |
|---------|---------|
| `make compose-up` | Start all containers |
| `make compose-down` | Stop all containers |
| `make compose-logs` | View real-time logs |
| `make compose-rebuild` | Clean rebuild everything |
| `make ps` | Show running containers |
| `make shell` | Open shell in publisher |
| `make clean` | Remove all images/containers |

---

## Troubleshooting

### Issue: "Docker is not installed"

**Solution**: Install Docker
```bash
sudo apt-get install docker.io docker-compose -y
sudo usermod -aG docker $USER
# Log out and log back in
```

### Issue: "Container fails to start"

**Solution**: Check logs and rebuild
```bash
make compose-logs
make compose-down
make compose-rebuild
```

### Issue: "X11 display not found" or "Authorization required"

**Solution**: Set up X11 forwarding and permissions (Linux)
If you see errors like `SDL_Init failed: x11 not available` or `Authorization required, but no authorization protocol specified`:
```bash
# Allow local root access to X11 (required for docker container)
xhost +local:root

# Ensure DISPLAY variable is set
export DISPLAY=:0

# Restart containers
make compose-up
```

### Issue: "ROS 2 topics not showing"

**Solution**: Verify network connection
```bash
# Check if publisher is running
docker ps | grep publisher

# List topics
docker exec lart-dashboard-publisher ros2 topic list

# If empty, restart containers
make compose-down
make compose-up
```

### Issue: "Permission denied for docker"

**Solution**: Add user to docker group
```bash
sudo usermod -aG docker $USER
# Log out and log back in
```

---

## File Structure

```
LART_Car_Dashboard_v1/
├── demo.sh                 # Interactive demo script
├── test_data.sh           # Test data publisher script
├── docker-compose.yml     # Container configuration
├── Dockerfile             # Docker image definition
├── Makefile              # Build automation
├── guide.md              # This file
└── src/
    └── ui/
        └── (LVGL UI source code)
```

---

## Next Steps

1. **Run the demo**: `bash demo.sh`
2. **Explore ROS 2 topics**: Check available data streams
3. **Test with data**: Use `bash test_data.sh`
4. **Customize**: Modify `docker-compose.yml` or LVGL UI as needed
5. **Deploy**: Run on actual hardware or different environments

---

## Additional Resources

- **ROS 2 Documentation**: https://docs.ros.org/en/jazzy/
- **LVGL Guide**: https://docs.lvgl.io/
- **Docker Documentation**: https://docs.docker.com/
- **Project Repository**: Check source code for more details

---

**Last Updated**: May 2026  
**Version**: 1.0

sudo apt install libsdl2-dev pkg-config
sudo apt install docker-buildx
