# LART Dashboard Docker - Cheat Sheet

## 🚀 The Essential Commands

```bash
# START
make compose-up

# WATCH
make compose-logs

# CODE
make shell

# STOP
make compose-down
```

## 📋 Full Command Reference

### Basic Operations
```bash
make help              # Show all available commands
make compose-up        # Start everything
make compose-down      # Stop everything
make clean             # Remove everything (including images)
```

### Monitoring
```bash
make compose-logs      # Real-time logs from all containers
make logs-pub          # Just publisher logs
make logs-listen       # Just listener logs  
make ps                # Show running containers
```

### Development
```bash
make shell             # Open bash in container
make build             # Build Docker image
make compose-rebuild   # Rebuild and restart
```

### Testing
```bash
./demo.sh              # Interactive guided demo
./test_data.sh         # Generate test speed data
```

## 🔗 Inside the Container

Once you run `make shell`:

```bash
# View topics
ros2 topic list -t
ros2 topic info /vehicle/rpm

# Echo (watch) a topic
ros2 topic echo /vehicle/rpm
ros2 topic echo /vehicle/dashboard_state

# View nodes
ros2 node list
ros2 node info /speed_listener

# System check
ros2 doctor

# Exit
exit
```

## 📊 Topics Available

```
/vehicle/rpm                 → Float32 (RPM value)
/vehicle/dashboard_state     → DashboardState (speed, rpm, throttle)
```

## ⚡ Quick Troubleshooting

### Containers won't start?
```bash
make compose-logs
# Look for errors, then:
make clean && make compose-up
```

### Topics not visible?
```bash
make shell
ros2 topic list
```

### Want to rebuild after code changes?
```bash
make compose-rebuild
```

### Need to see what's running?
```bash
make ps
```

## 📁 File Locations

In the `LART Car Dashboard v1/` directory:

| File | What It Does |
|------|--------------|
| `Dockerfile` | Defines the container |
| `docker-compose.yml` | Runs 2 containers |
| `Makefile` | Helper commands |
| `speed_listener.py` | Subscribes to topics |
| `README_DOCKER.md` | Full documentation |
| `GUIDE.md` | Complete guide |

## 🎯 Typical Workflow

```bash
# 1. Start in one terminal
make compose-up

# 2. Monitor in another terminal
make compose-logs

# 3. Debug if needed in third terminal
make shell
ros2 topic echo /vehicle/rpm

# 4. Stop when done
make compose-down
```

