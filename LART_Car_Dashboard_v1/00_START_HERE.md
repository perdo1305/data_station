## 🚀 Quick Start (Copy & Paste)

### Start Everything
```bash
cd "LART Car Dashboard v1"
make compose-up
```

### Watch the Data Flow
```bash
make compose-logs
```

### Stop When Done
```bash
make compose-down
```

---

## 📊 Architecture Created

### Two Containers Running in a Shared Network:

```
┌──────────────────────────────────────┐
│   Docker Bridge Network              │
│   (lart-ros2-network)                │
│                                      │
│  Publisher         ◄──DDS──►  Listener
│  Container                    Container
│  • Topic pub       ROS 2       • Topic sub
│  • Speed publisher Middleware  • Logs data
│                                │
└──────────────────────────────────────┘

Topics:
  /vehicle/rpm              (Float32)
  /vehicle/dashboard_state  (DashboardState)
```

---

## 💻 Available Commands

### Start & Stop
```bash
make compose-up          # Start all containers (recommended)
make compose-down        # Stop all containers
make clean               # Remove everything
```

### Monitoring
```bash
make compose-logs        # View real-time logs
make ps                  # Show running containers
make logs-pub            # Publisher logs only
make logs-listen         # Listener logs only
```

### Interactive
```bash
make shell               # Open bash in container
make help                # Show all available commands
```

### Demo & Testing
```bash
./demo.sh                # Interactive guided tour
./test_data.sh           # Test data generator
```

---

## 📚 Documentation Guide

| Document | Best For | Read Time |
|----------|----------|-----------|
| **CHEATSHEET.md** | Quick command lookup | 2 min |
| **README_DOCKER.md** | Complete reference | 10 min |
| **DOCKER_QUICKSTART.md** | Getting started | 5 min |
| **GUIDE.md** | Learning everything | 15 min |
| **SETUP_SUMMARY.md** | Understanding what was created | 5 min |

---

## 🎯 What Happens When You Run Commands

### `make compose-up`
1. Builds Docker image (osrf/ros:jazzy-desktop base)
2. Creates bridge network for inter-container communication
3. Starts publisher container
4. Starts listener container
5. Both discover each other via ROS 2 DDS
6. Topics become available for communication

### `make compose-logs`
Shows real-time output from both containers:
- Publisher: Broadcasting speed/RPM data
- Listener: Receiving and logging data

### `make shell`
Opens an interactive bash session inside the publisher container:
- Full ROS 2 environment available
- Can run `ros2 topic`, `ros2 node`, etc.
- Type `exit` to leave

---

## 🔍 Inside the Container (After `make shell`)

Once inside, you can:

```bash
# See all ROS 2 topics
ros2 topic list -t

# Watch speed data live
ros2 topic echo /vehicle/rpm

# Check node status
ros2 node list
ros2 node info /speed_listener

# Verify ROS 2 setup
ros2 doctor
```

---

## 📈 Integration Points

### ROS 2 Topics Published
- **`/vehicle/rpm`** - RPM value (Float32)
- **`/vehicle/dashboard_state`** - Dashboard state (DashboardState)

### Node Names
- **Publisher:** `dashboard_ui` (from speed publisher)
- **Listener:** `speed_listener` (the listener node)

### Environment Variables
- `ROS_DOMAIN_ID=0` - ROS 2 domain for discovery
- `ROS_LOCALHOST_ONLY=0` - Allow network communication
- Both automatically configured

---

## ⚡ Performance Specs

| Aspect | Value |
|--------|-------|
| Base Image | osrf/ros:jazzy-desktop (~2GB) |
| Network | Docker bridge (fast) |
| Communication | ROS 2 DDS (Fast-DDS) |
| First Build | ~5-10 minutes |
| Subsequent Builds | ~1-2 minutes |
| Container Startup | ~2-3 seconds |

---

## 🔧 Troubleshooting Quick Links

**Problem: Containers won't start?**
```bash
make compose-logs  # Check error messages
make clean && make compose-up  # Clean rebuild
```

**Problem: ROS 2 topics not visible?**
```bash
make shell
ros2 topic list
```

**Problem: Want to rebuild after code changes?**
```bash
make compose-rebuild
```

---

## 📝 Next Steps

### Option 1: Quick Demo (5 min)
```bash
./demo.sh
# Select: 2 (Start), 3 (View logs), exit
```

### Option 2: Manual Exploration (10 min)
```bash
make compose-up
make compose-logs        # Terminal 2
make shell               # Terminal 3 - run: ros2 topic list
make compose-down
```

### Option 3: Test Data (10 min)
```bash
make compose-up
make compose-logs        # Terminal 2
./test_data.sh          # Terminal 3 - Select: 4 (Acceleration)
# Watch the data flow in Terminal 2
```

---

## 📞 Help & Support

### View Help
```bash
make help                # All commands
./demo.sh                # Interactive guide
cat CHEATSHEET.md        # Quick reference
```

### Check Documentation
- **Quick start**: `DOCKER_QUICKSTART.md` (2 min read)
- **Full guide**: `GUIDE.md` (comprehensive)
- **Technical details**: `README_DOCKER.md` (complete reference)

### Verify Installation
```bash
cd "LART Car Dashboard v1"
make ps                  # Show containers
docker images | grep lart  # Show images
docker network ls | grep lart  # Show networks
```

---

## ✨ Key Features

✅ **Zero Configuration** - Just run `make compose-up`  
✅ **ROS 2 Jazzy** - Latest LTS version pre-installed  
✅ **Inter-Container Communication** - Automatic via DDS  
✅ **Easy Commands** - Simple Makefile interface  
✅ **Complete Documentation** - 5 guide files  
✅ **Helper Scripts** - Interactive demo and test data generator  
✅ **Production Ready** - Proper error handling and logging  

---

## 🎓 Learning Path

1. **Start here**: `CHEATSHEET.md` (2 min) - Learn basic commands
2. **Then**: Run `make compose-up` (1 min) - Start containers
3. **Explore**: `make shell` (5 min) - Try ROS 2 commands
4. **Deep dive**: Read `GUIDE.md` (15 min) - Understand architecture
5. **Practice**: Use `./test_data.sh` (10 min) - Generate test data

---

## 🚀 You're All Set!

Everything is configured and ready to use. The fastest way to start:

```bash
cd "LART Car Dashboard v1"
make compose-up
```

Then in another terminal:

```bash
cd "LART Car Dashboard v1"
make compose-logs
```

**That's it! Your ROS 2 Docker environment is running.** 🎉

---

## 📋 File Checklist

- [x] Dockerfile (container definition)
- [x] docker-compose.yml (2 containers)
- [x] Makefile (15+ commands)
- [x] .dockerignore (build optimization)
- [x] README_DOCKER.md (full docs)
- [x] DOCKER_QUICKSTART.md (quick ref)
- [x] GUIDE.md (comprehensive guide)
- [x] SETUP_SUMMARY.md (setup info)
- [x] CHEATSHEET.md (command reference)
- [x] demo.sh (interactive demo)
- [x] test_data.sh (test scenarios)
- [x] speed_listener.py (listener node)
- [x] setup.py (updated entry point)

---

**Status: ✅ Setup Complete - Ready to Use**

Start with: `make compose-up` 🚀
