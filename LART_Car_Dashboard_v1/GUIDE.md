# Complete LART Dashboard Docker & ROS 2 Guide

## 📋 Overview

This setup provides a complete ROS 2 (Jazzy) Docker environment for the LART Car Dashboard with inter-container communication for speed/RPM data.

### What You Get

✅ **Docker Environment**
- osrf/ros:jazzy-desktop base image  
- Fully configured ROS 2 Jazzy
- Auto-built workspace

✅ **Multi-Container Setup**
- Publisher container (publishes speed data)
- Listener container (subscribes to speed)
- Automatic network communication

✅ **Easy Management**
- Makefile with simple commands
- docker-compose orchestration
- Interactive demo script

---

## 🚀 Quick Start (5 minutes)

### Command 1: Start Everything
```bash
cd "LART Car Dashboard v1"
make compose-up
```

**What happens:**
- Docker image is built (first time only, ~5-10 min)
- Publisher container starts
- Listener container starts
- Both discover each other via ROS 2

### Command 2: Watch the Data Flow
```bash
make compose-logs
```

**What you'll see:**
```
publisher    | Starting Speed Publisher...
publisher    | Publishing RPM updates...
listener     | Speed listener started
listener     | RPM: 2500.00
listener     | RPM: 2750.50
```

### Command 3: Stop When Done
```bash
make compose-down
```

---

## 🏗️ Architecture Explained

### Container Layout
```
┌───────────────────────────────────────────────────────────┐
│                   Docker Host                             │
│                                                           │
│  ┌─────────────────────────────────────────────────────┐ │
│  │         Docker Bridge Network                       │ │
│  │       (lart-ros2-network)                           │ │
│  │                                                     │ │
│  │  ┌──────────────────┐      ┌──────────────────┐   │ │
│  │  │  Publisher       │      │  Listener        │   │ │
│  │  │  Container       │      │  Container       │   │ │
│  │  │                  │◄────►│                  │   │ │
│  │  │ • ROS 2 Node     │ ROS2 │ • ROS 2 Node     │   │ │
│  │  │ • Publishes      │ DDS  │ • Subscribes     │   │ │
│  │  │   speed data     │      │ • Logs data      │   │ │
│  │  │ • Port: internal │      │ • Port: internal │   │ │
│  │  └──────────────────┘      └──────────────────┘   │ │
│  │                                                     │ │
│  └─────────────────────────────────────────────────────┘ │
│                                                           │
│  ROS 2 Topics (via DDS middleware):                      │
│  ├── /vehicle/rpm (Float32)                              │
│  └── /vehicle/dashboard_state (DashboardState)           │
│                                                           │
└───────────────────────────────────────────────────────────┘
```

### Data Flow
```
Publisher Container        Listener Container
    │                             ▲
    │  Publishes                  │
    │  /vehicle/rpm               │
    │  (RPM value)                │
    ├────────────────────────────►│
    │                             │
    │  Publishes                  │
    │  /vehicle/dashboard_state   │
    │  (speed, throttle, etc.)    │
    ├────────────────────────────►│
    │                             │
    │                             │ Logs received data
    │                             │ Echo to console
    │                             │
    │◄────────────  ROS 2 Discovery packet ────────────┤
    │         (topic discovery via DDS)                │
```

---

## 📁 Files Created

### Essential Files
| File | Purpose |
|------|---------|
| `Dockerfile` | Container definition with ROS 2 |
| `docker-compose.yml` | Multi-container orchestration |
| `Makefile` | Helper commands |
| `.dockerignore` | Exclude files from build |

### Documentation
| File | Purpose |
|------|---------|
| `README_DOCKER.md` | Complete documentation |
| `DOCKER_QUICKSTART.md` | Quick reference |
| `SETUP_SUMMARY.md` | What was created and why |
| `GUIDE.md` | This file |

### Scripts
| File | Purpose |
|------|---------|
| `demo.sh` | Interactive menu interface |
| `test_data.sh` | Generate test speed data |

---

## 💻 Commands Reference

### Start & Stop

```bash
# Start with docker-compose (recommended)
make compose-up

# Start manually (step by step)
make build          # Build image once
make up             # Start both containers

# Stop everything
make compose-down   # OR: make stop

# Full cleanup (removes everything)
make clean
```

### Monitoring

```bash
# View logs from all containers
make compose-logs   # Follow in real-time

# View specific container logs
make logs-pub       # Publisher only
make logs-listen    # Listener only

# See container status
make ps
```

### Interactive Access

```bash
# Open shell in publisher
make shell

# Inside the container:
ros2 topic list                 # See ROS 2 topics
ros2 topic echo /vehicle/rpm    # Watch speed data
ros2 node list                  # See ROS 2 nodes
ros2 doctor                     # Check ROS 2 setup
exit                            # Leave container
```

### Testing

```bash
# Interactive test data generator
./test_data.sh

# Scenarios available:
# 1. Idle (0 km/h)
# 2. City driving (30 km/h)
# 3. Highway (100 km/h)
# 4. Acceleration sequence
# 5. Deceleration sequence
# 6. Random values
# 7. Custom speed
```

### Demo Mode

```bash
# Interactive guided tour
./demo.sh

# Menu options:
# 1. Build image
# 2. Start containers
# 3. View logs
# 4. Open shell
# 5. Check topics
# 6. Stop containers
# 7. Clean everything
# 8. Exit
```

---

## 📊 ROS 2 Topics

### Published Topics

#### `/vehicle/rpm` (Float32)
- **Published by:** Publisher container
- **Subscribed by:** Listener container
- **Content:** RPM value as floating-point number
- **Example:** `31414.0`

#### `/vehicle/dashboard_state` (DashboardState)
- **Published by:** Publisher container  
- **Subscribed by:** Listener container
- **Content:**
  ```
  speed_kph: float          # Speed in km/h
  rpm: float                # RPM value
  throttle_percent: float   # Throttle 0-100%
  timestamp: ROS time       # Message timestamp
  ```

### Monitoring Topics

Inside the container:

```bash
# List all available topics
ros2 topic list -t

# Echo topic data
ros2 topic echo /vehicle/rpm
ros2 topic echo /vehicle/dashboard_state

# Topic information
ros2 topic info /vehicle/rpm

# Subscribe in Python
import rclpy
subscriber = node.create_subscription(Float32, '/vehicle/rpm', callback, 10)
```

---

## 🔧 Troubleshooting

### Problem: "Containers won't start"
```bash
# Check what went wrong
make compose-logs

# Rebuild everything
make clean
make compose-up
```

### Problem: "Topics not appearing"
```bash
# Inside container
make shell
source /root/.bashrc
ros2 doctor
ros2 topic list

# Check network communication
ping listener  # From publisher
ping publisher # From listener
```

### Problem: "Build takes too long"
```bash
# Use cached layers (don't use --no-cache)
make compose-up

# Or view build progress
docker-compose build --progress=plain
```

### Problem: "Permission denied"
```bash
# Add user to docker group
sudo usermod -aG docker $USER
newgrp docker

# Or use sudo
sudo make compose-up
```

### Problem: "Port already in use"
```bash
# Find and stop other containers
docker ps
docker stop container_id

# Or use different port in docker-compose.yml
```

---

## 🔐 Environment Variables

Both containers have these ROS 2 environment variables:

```bash
ROS_DOMAIN_ID=0              # ROS 2 domain (0-232)
ROS_LOCALHOST_ONLY=0         # Allow network communication
PYTHONUNBUFFERED=1           # Real-time output
```

To override:

```bash
docker-compose.yml:
  environment:
    ROS_DOMAIN_ID: 5
    ROS_LOCALHOST_ONLY: 1
```

---

## 📈 Common Workflows

### Workflow 1: Quick Test
```bash
# Terminal 1
make compose-up

# Terminal 2
make compose-logs

# Terminal 3 (after a few seconds)
./test_data.sh
# Select option: 4 (acceleration sequence)

# View results in Terminal 2
# Stop with Ctrl+C
```

### Workflow 2: Development
```bash
# Start containers
make compose-up

# Edit code in src/
vim src/dashboard_ui/dashboard_ui/speed_listener.py

# Rebuild
make compose-rebuild

# Test changes
make logs-listen
```

### Workflow 3: Debugging
```bash
# Start containers
make compose-up

# Enter container
make shell

# Inside container, debug
ros2 doctor
ros2 node list
ros2 topic list -t
ros2 topic echo /vehicle/rpm

# Try running node with debug info
python3 -m pdb /root/lart_ws/src/dashboard_ui/dashboard_ui/speed_listener.py
```

---

## 🎯 What Happens When You Run Commands

### `make compose-up`
```
1. Build Docker image (if not exists)
   └─ Downloads osrf/ros:jazzy-desktop (2GB)
   └─ Installs ROS 2 Jazzy
   └─ Installs dependencies
   └─ Builds ROS 2 workspace

2. Create Docker network
   └─ Creates bridge network: lart-ros2-network

3. Start publisher container
   └─ Container name: lart-dashboard-publisher
   └─ Hostname: publisher
   └─ Network: lart-ros2-network
   └─ Command: ros2 run dashboard_ui dashboard_ui

4. Start listener container
   └─ Container name: lart-dashboard-listener
   └─ Hostname: listener
   └─ Network: lart-ros2-network
   └─ Command: ros2 run dashboard_ui speed_listener_node

5. Both containers start ROS 2 nodes
   └─ DDS discovers each other via UDP
   └─ Topics become available
   └─ Communication established
```

### `make shell`
```
1. Executes bash in publisher container
2. Sources ROS 2 environment
3. Opens interactive shell
4. You can run ros2 commands
5. Exit with: exit
```

---

## 🌐 Network Details

### Docker Bridge Network
```
lart-ros2-network (bridge)
├── Publisher Container
│   ├── IP: 172.xxx.xxx.xxx (auto-assigned)
│   ├── Hostname: publisher
│   └── Accessible as: tcp://publisher:5600 (DDS)
│
└── Listener Container
    ├── IP: 172.xxx.xxx.xxx (auto-assigned)
    ├── Hostname: listener
    └── Accessible as: tcp://listener:5600 (DDS)
```

### DDS Communication
```
Fast-DDS (ROS 2 default):
├── Discovery:    UDP multicast on 239.255.0.1:7400
├── Topics:       TCP port 7400-7413
└── Reliable:     Automatic with DDS protocol
```

---

## 📚 Learning Resources

### Inside Container
```bash
# ROS 2 help
ros2 help
ros2 topic --help
ros2 node --help

# ROS 2 Tutorial
ros2 tutorial

# Check system
uname -a
python3 --version
```

### External Resources
- ROS 2 Jazzy: https://docs.ros.org/en/jazzy/
- OSRF Docker Images: https://hub.docker.com/r/osrf/ros
- Docker Compose: https://docs.docker.com/compose/
- DDS Middleware: https://docs.ros.org/en/jazzy/Concepts/About-Middleware-Implementations.html

---

## ✅ Checklist

Before deploying to hardware:

- [ ] Docker setup works locally (`make compose-up` succeeds)
- [ ] Containers communicate (`make compose-logs` shows data)
- [ ] Test data works (`./test_data.sh` completes)
- [ ] Custom node runs (`make shell` then `ros2 run ...`)
- [ ] Topics are correct (`ros2 topic list -t`)
- [ ] Rebuild succeeds (`make compose-rebuild`)
- [ ] Cleanup works (`make clean` removes all)

---

## 🎓 Next Steps

### Customize
1. Edit nodes in `src/dashboard_ui/`
2. Add new ROS 2 packages in `src/`
3. Rebuild with `make compose-rebuild`

### Deploy
1. Use `docker buildx` for ARM64 (Raspberry Pi)
2. Push to Docker registry
3. Pull on target device
4. Run with same Makefile

### Scale
1. Add more containers to `docker-compose.yml`
2. Add services to communicate
3. Use same bridge network for all

---

## 📞 Support

If something doesn't work:

1. **Check logs first:**
   ```bash
   make compose-logs | head -100
   ```

2. **Verify setup:**
   ```bash
   docker ps
   docker network ls
   docker volume ls
   ```

3. **Test manually:**
   ```bash
   make shell
   ros2 doctor
   ```

4. **Clean and rebuild:**
   ```bash
   make clean
   make compose-up
   ```

---

## 📄 Files Summary

```
LART Car Dashboard v1/
│
├── Dockerfile              ← Container definition
├── docker-compose.yml      ← Orchestration
├── Makefile                ← Commands
├── .dockerignore            ← Build filter
│
├── README_DOCKER.md        ← Full docs
├── DOCKER_QUICKSTART.md    ← Quick ref
├── SETUP_SUMMARY.md        ← Setup details
├── GUIDE.md                ← This file
│
├── demo.sh                 ← Interactive demo
├── test_data.sh            ← Test scenarios
│
└── src/                    ← ROS 2 workspace
    ├── dashboard_ui/
    │   └── dashboard_ui/
    │       └── speed_listener.py  ← Listener node
    ├── input_handler/
    ├── lart_bringup/
    ├── lart_msgs/
    ├── led_controller/
    └── sim/
```

---

## 🎉 You're Ready!

Everything is set up and ready to go. Start with:

```bash
cd "LART Car Dashboard v1"
make compose-up
```

Then open another terminal and run:

```bash
make compose-logs
```

Happy coding! 🚀
