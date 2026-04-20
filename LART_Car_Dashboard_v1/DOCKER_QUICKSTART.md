# LART Dashboard Docker - Quick Reference

## Start Publishing & Listening

```bash
# Fastest way - uses docker-compose
make compose-up

# Or manually
make build
make up
```

## Monitor What's Happening

```bash
# See all logs from both containers
make compose-logs

# Just publisher
make logs-pub

# Just listener
make logs-listen

# See container status
make ps
```

## Open a Terminal in the Container

```bash
# Interactive shell in publisher
make shell

# Inside the container, you can run:
ros2 topic list          # See all ROS 2 topics
ros2 topic echo /vehicle/rpm  # Watch speed data
exit                     # Leave the container
```

## Stop Everything

```bash
# Stop but keep images
make compose-down

# Full cleanup (removes everything)
make clean
```

## ROS 2 Topics Published

The containers communicate via these topics:

```
/vehicle/rpm                    (Float32)
/vehicle/dashboard_state        (DashboardState)
```

## Docker Architecture

```
┌─ Publisher Container ─┐
│  Publishes speed/RPM  │
└──────────┬────────────┘
           │
      Docker Network
           │
┌──────────┴───────────┐
│ Listener Container   │
│ Subscribes & Logs    │
└──────────────────────┘
```

## Troubleshooting

**Containers not starting?**
```bash
make compose-logs    # Check what went wrong
```

**Rebuild everything?**
```bash
make clean
make compose-up
```

**Want to debug inside?**
```bash
make shell
ros2 doctor          # Check ROS 2 setup
```

## File Reference

- **Dockerfile** - Container definition (osrf/ros:jazzy-desktop base)
- **docker-compose.yml** - Multi-container orchestration
- **Makefile** - All helper commands
- **README_DOCKER.md** - Full documentation
- **speed_listener.py** - Node that subscribes to speed topics

## Environment Setup

Both containers have:
- ROS 2 Jazzy pre-installed
- Python 3 with necessary packages
- Automatic ROS 2 environment setup

## Key Commands Summary

| Task | Command |
|------|---------|
| Start everything | `make compose-up` |
| View logs | `make compose-logs` |
| Stop everything | `make compose-down` |
| Build image | `make build` |
| Shell access | `make shell` |
| Clean all | `make clean` |
| Interactive demo | `./demo.sh` |
