# LART DataStation — Project Context

> **For LLMs, collaborators, and new developers.**  
> This document provides a single-stop reference for understanding the architecture, data flows, and development conventions of this codebase.

---

## Project Overview

**LART DataStation** is a Formula Student vehicle dashboard system built for a **Raspberry Pi 5** (RPi5) target with a dual-channel CAN bus HAT (Waveshare 2-CH CAN HAT+). It provides the driver and pit crew with real-time telemetry from every major subsystem of the car — powertrain, battery management system (AMS/BMS), inverter, autonomous control unit (ACU), wheel sensors, pedals, pressures, and more.

| Dimension | Details |
|-----------|---------|
| **Target audience** | Formula Student team — drivers, engineers, and software developers |
| **Core problem** | Bridge raw automotive CAN frames to an ergonomic, real-time visual dashboard |
| **Hardware target** | Raspberry Pi 5 + Waveshare 2-CH CAN HAT+ (`can0`/`can1` at 1 Mbit/s) |
| **UI target** | 800 × 480 px embedded display (HDMI or DSI) |
| **Developer target** | Any Linux x86-64 workstation via Docker + virtual CAN (`vcan0`) |

The system supports two operational modes:
1. **Car mode** — reads live CAN frames via SocketCAN, decodes using DBC files, and drives the full dashboard + LED RPM bar.
2. **Simulation mode** — uses a virtual CAN interface and mock nodes so the entire stack can be developed and tested without hardware.

---

## Architecture & Tech Stack

### High-Level Layers

```
┌─────────────────────────────────────────────────────────┐
│                   LVGL Dashboard UI                      │
│  (C/C++ · LVGL v8/v9 · EEZ Studio generated · SDL2)     │
├─────────────────────────────────────────────────────────┤
│              ROS 2 Middleware (Jazzy)                    │
│  DDS pub/sub over a shared network (real or Docker)     │
├──────────────┬──────────────────────────┬───────────────┤
│  CAN Bridge  │  Dashboard State Bridge  │  Input Handler│
│  (C++ node)  │       (Python node)      │  (Python node)│
├──────────────┴──────────────────────────┴───────────────┤
│              Hardware / Virtual CAN                      │
│  SocketCAN: can0 / can1  OR  vcan0 (simulation)         │
└─────────────────────────────────────────────────────────┘
```

### Frontend (Dashboard UI)

| Component | Details |
|-----------|---------|
| **Framework** | [LVGL](https://docs.lvgl.io/) v8 / v9 (Light and Versatile Graphics Library) |
| **UI designer** | [EEZ Studio](https://www.envox.eu/eez-studio/) — generates `screens.c`, `ui.c`, `eez-flow.cpp` |
| **Rendering backend** | SDL2 (software renderer via X11 forwarding in Docker; native on RPi) |
| **Language** | C (LVGL widgets, screen logic) + C++ (OS integration, ROS2 subscriber) |
| **Resolution** | 800 × 480 px, 32-bit colour depth |
| **Fonts** | Orbitron family (15 / bold-15 / bold-20 / 25 pt) — motorsport aesthetic |
| **Entry point** | `ui_runner.cpp` → `main()` initialises SDL2, LVGL, ROS2 subscriber, then runs the tick loop |

**Screens (IDs):**

| ID | Name | Purpose |
|----|------|---------|
| 1 | Driver View | Primary race display — speed, RPM, temps, SOC, pedal traces |
| 2 | Autonomous | AV mission status, ACU/Jetson states, SLAM cones/laps |
| 3 | Debug | Raw telemetry dump for all subsystems |
| 4 | Debug Autonomous | Extended autonomous debug view |

### Backend / ROS 2 Nodes

| Node | Package | Language | Role |
|------|---------|----------|------|
| `can_bridge` | `lart_bringup` | C++ | SocketCAN reader → DBC decoder → ROS 2 publisher |
| `dashboard_state_bridge` | `lart_bringup` | Python | Aggregates raw topics into `/vehicle/dashboard_state` |
| `input_handler` | `input_handler` | Python | GPIO buttons + encoders → `/input/buttons`, `/input/encoders` |
| `led_controller` | `led_controller` | Python | WS2812B RPM LED strip driven by `/vehicle/rpm` |
| `dashboard_ui` | `dashboard_ui` | Python | Legacy pygame-based UI (MVP; superseded by LVGL UI) |
| `mock_can` | `sim` | Python | Simulates CAN frames over `vcan0` for home testing |
| `can_simulator` | `sim` | Python | DBC-driven simulator; publishes all messages from a DBC file |

### CAN / DBC

| File | Description |
|------|-------------|
| `dbc_signals/powertrain_t26.dbc` | Inverter (DTI HV500), VCU, AMS, IVT, pedals, wheel sensors |
| `dbc_signals/autonomous_t26.dbc` | ACU, Jetson, DV dynamics, EBS, steering, SLAM |
| `dbc_signals/aquisition_boards.dbc` | Acquisition boards (AQT1–AQT7), pressures, NTC sensors |
| `dbc_signals/data_t26.dbc` | Miscellaneous data messages |

DBC files are parsed at build time by `generate_dbc_api.py` (uses `cantools`) to produce `dbc_api.cpp` / `dbc_api.h`, a zero-dependency C++ dispatcher that routes raw CAN frame bytes directly to a typed `TelemetryData` struct.

### Infrastructure

| Tool | Purpose |
|------|---------|
| **Docker / Docker Compose** | Reproducible ROS 2 environment; used for desktop dev and CI |
| **CMake** | Builds the LVGL UI binary (`ui_runner`) inside the container |
| **colcon** | Builds the ROS 2 workspace packages (`src/`) |
| **Makefile** | Thin wrapper around `docker compose` and `colcon` for convenience |
| **X11 socket forwarding** | Renders the SDL2 window from inside a Docker container on the host display |

---

## Core Features & Workflows

### Implemented Features

- **Real-time telemetry display** — speed, RPM, inverter & motor temps, HV voltage, SOC, lap count, brake/throttle pressure bars
- **Multi-screen dashboard** — Driver View, Autonomous, Debug, Debug Autonomous; switchable via ROS 2 topic `/dashboard/set_screen`
- **DBC-driven CAN decoding** — code-generated C++ dispatcher maps raw CAN frame bytes to named signals at compile time (no runtime DBC parsing)
- **Error/fault notification overlay** — `ui_add_notification()` / `ui_clear_notification()` API surfaces fault signals from any DBC message on top of the active screen
- **Autonomous vehicle status screen** — ACU state machine, Jetson CPU/GPU/temp, SLAM cone & lap count, mission selector
- **LED RPM bar** — WS2812B strip driven by `/vehicle/rpm`; shifts colour above configurable RPM threshold
- **Hardware input** — GPIO buttons and rotary encoders via `input_handler`; screen switching and future in-car menus
- **Simulation stack** — full `sim.launch.py` + `dbc_sim.launch.py` pipelines for development without hardware
- **Docker-based dev environment** — single `make compose-up` to start all containers on any Linux machine with X11

### Critical Workflow 1: Live CAN → Screen Update

```
CAN HAT (can0/can1)
  │  SocketCAN frame (8 bytes)
  ▼
can_bridge (C++ ROS 2 node)
  │  Calls CanBridgeImpl::dispatch(frame_id, data)
  │  → generated dbc_api.cpp unpacks bytes → publishes on:
  │    /can/frames          (lart_msgs/CanFrame)
  │    /vehicle/rpm         (std_msgs/Float32)
  │    /can/dbc/<msg>/<sig> (std_msgs/Float32, one topic per signal)
  ▼
dashboard_state_bridge (Python node)
  │  Subscribes to the DBC signal topics configured in rpi_config.yaml
  │  Aggregates into DashboardState, publishes at 20 Hz on:
  │    /vehicle/dashboard_state
  ▼
ros2subscriber.cpp (C++ ROS 2 subscriber, lives in the UI process)
  │  Runs in a background spin thread
  │  Writes decoded values atomically into g_telemetry (TelemetryData)
  │  and g_latest_speed_kph / g_latest_hv atomic floats
  ▼
ui_runner.cpp tick loop (every 5 ms)
  │  Calls ros2subscriber_spin_some() → drains the executor
  │  Calls screen update functions in screens.c
  │  screens.c reads TelemetryData and sets LVGL widget values
  ▼
LVGL renders dirty regions → SDL2 texture → X11 / display
```

### Critical Workflow 2: DBC Code Generation

```
dbc_signals/*.dbc                    (source of truth for signal definitions)
  │
  ▼
generate_dbc_api.py (Python, cantools)
  │  Parses all .dbc files
  │  Emits dbc_api.h             — typed C++ dispatch function declarations
  │  Emits dbc_api.cpp           — inline unpack logic per CAN ID
  │  Emits generated/can_bridge_impl.hpp — publisher wiring for can_bridge node
  │  Identifies error/fault signals → emits notification hooks
  ▼
CMake / colcon build
  │  Compiles dbc_api.cpp into the ui_runner binary
  ▼
Runtime: can_bridge calls CanBridgeImpl::dispatch(id, data)
         → populates TelemetryData → ROS 2 topic per signal
```

> **Rule:** Never hand-edit `dbc_api.cpp`, `dbc_api.h`, or `generated/can_bridge_impl.hpp`. They are fully generated. Edit the `.dbc` files and re-run `generate_dbc_api.py`.

---

## Directory Structure

```
data_station/                         ← workspace root
│
├── context.md                        ← THIS FILE
├── README.md                         ← Quick-start reference
├── setup.md                          ← Archived legacy setup notes
├── requirements.txt                  ← Python deps for the ROS2 workspace
├── dbc2msg.py                        ← Utility: .dbc → lart_msgs field scaffold
├── desktop_can_sim.py                ← Standalone desktop CAN simulator
├── run_dbc_sim.sh                    ← Shell launcher for DBC simulation
├── test_slcan.py                     ← SLCAN adapter test utility
│
├── dbc_signals/                      ← CAN database files (source of truth)
│   ├── powertrain_t26.dbc            ← Inverter, VCU, AMS, IVT, pedals, wheels
│   ├── autonomous_t26.dbc            ← ACU, Jetson, DV, EBS, SLAM, steering
│   ├── aquisition_boards.dbc         ← AQT boards, pressures, NTC temps
│   └── data_t26.dbc                  ← Miscellaneous data messages
│
├── src/                              ← ROS 2 colcon workspace packages
│   ├── lart_msgs/                    ← Custom ROS 2 message definitions
│   │   └── msg/
│   │       ├── CanFrame.msg          ← Raw CAN frame (id, data[8])
│   │       ├── DashboardState.msg    ← Aggregated vehicle state for the UI
│   │       ├── ButtonEvent.msg       ← Button press events from input_handler
│   │       └── EncoderDelta.msg      ← Encoder step events from input_handler
│   │
│   ├── lart_bringup/                 ← Launch files + shared config + bridge nodes
│   │   ├── launch/
│   │   │   ├── car.launch.py         ← Real hardware: all 5 nodes + rpi_config
│   │   │   ├── sim.launch.py         ← Simulation: mock_can instead of can_bridge
│   │   │   └── dbc_sim.launch.py     ← DBC-driven vcan simulation stack
│   │   └── config/
│   │       └── rpi_config.yaml       ← All node parameters (CAN IDs, GPIO pins, …)
│   │
│   ├── dashboard_ui/                 ← Legacy pygame dashboard (MVP)
│   ├── input_handler/                ← GPIO buttons + rotary encoders
│   ├── led_controller/               ← WS2812B RPM LED strip
│   └── sim/                          ← mock_can + can_simulator nodes
│
└── LART_Car_Dashboard_v1/            ← LVGL dashboard UI (primary UI component)
    ├── Dockerfile                    ← ROS 2 Jazzy + SDL2 + LVGL build image
    ├── docker-compose.yml            ← Services: display, publisher, can_bridge, screen_changer
    ├── Makefile                      ← compose-up/down/logs/rebuild + local build helpers
    ├── demo.sh                       ← Interactive guided demo for first-time setup
    ├── test_data.sh                  ← Publishes test speed data to ROS 2
    ├── test_data_ros2.sh             ← ROS 2 native variant of test_data.sh
    ├── autostart_dashboard.sh        ← Systemd/autostart wrapper for RPi deployment
    ├── LART_car_dashboard_v1.eez-project  ← EEZ Studio project (UI designer source)
    ├── fonts/                        ← Orbitron TTF font files
    └── src/ui/                       ← All C/C++ UI source code
        ├── ui_runner.cpp             ← Main: SDL2 init, LVGL init, ROS2 spin loop
        ├── screens.c                 ← Screen update functions, widget bindings, notification overlay
        ├── screens.h
        ├── ui.c / ui.h               ← EEZ-generated widget tree (do not hand-edit)
        ├── eez-flow.cpp / .h         ← EEZ runtime (do not hand-edit)
        ├── ros2subscriber.cpp        ← C++ ROS 2 node: subscribes to all telemetry topics
        ├── ros2subscriber.h          ← TelemetryData struct + C API (called from screens.c)
        ├── can_bridge.cpp            ← Standalone SocketCAN → ROS 2 bridge node
        ├── dbc_api.cpp / dbc_api.h   ← ⚠ GENERATED — do not hand-edit
        ├── generate_dbc_api.py       ← DBC → C++ code generator (run to regenerate)
        ├── generated/
        │   └── can_bridge_impl.hpp   ← ⚠ GENERATED — do not hand-edit
        ├── structs.h                 ← EEZ flow value types (SubscriptionValue, MQTTMessage)
        ├── vars.h                    ← LVGL global variable enum (FlowGlobalVariables)
        ├── styles.c / styles.h       ← Shared LVGL style definitions
        ├── fonts.h                   ← Font declarations
        ├── images.c / images.h       ← Image asset declarations
        ├── actions.h                 ← EEZ action stubs
        ├── lv_conf.h                 ← LVGL compile-time configuration
        ├── CMakeLists.txt            ← CMake build script for the UI binary
        ├── package.xml               ← ROS 2 package manifest (for colcon)
        ├── fonts/                    ← Compiled LVGL font C files
        ├── images/                   ← Image assets (PNG → C arrays)
        ├── third_party/              ← Vendored LVGL library source
        └── build/                    ← CMake build output (git-ignored)
```

---

## Development & Constraints

### Prerequisites

- **Docker & Docker Compose** (recommended path — no native deps needed)
- **X11 server** on the host (`xhost +local:root` required for container rendering)
- **Python 3.10+** with `cantools`, `python-can`, `rclpy` (only needed for native / non-Docker workflow)
- **ROS 2 Jazzy** (only needed for native builds on RPi or if not using Docker)

### Setup & Run (Docker — Recommended)

```bash
# 1. Navigate to the UI project directory
cd /home/sintra/dev/data_station/LART_Car_Dashboard_v1

# 2. Build the Docker image (first time or after Dockerfile changes)
docker compose build display   # or: make build

# 3. Allow X11 connections from Docker containers
xhost +local:root

# 4. Start all containers (dashboard UI + publisher + CAN bridge)
make compose-up                # or: docker compose up -d

# 5. View logs
make compose-logs

# 6. Stop containers
make compose-down
```

### Interactive Demo

```bash
bash demo.sh    # guided menu: build → start → logs → inspect → stop
```

### Publish Test Data (Without Real CAN)

```bash
bash test_data.sh                 # publishes test speed data to /vehicle/speed_kph

# Or manually inside the publisher container:
docker exec -it lart-dashboard-publisher bash
source /opt/ros/jazzy/setup.bash
ros2 topic pub -1 /vehicle/speed_kph std_msgs/msg/Float32 "{data: 85.0}"

# Change screen (IDs: 1=Driver  2=Autonomous  3=Debug  4=Debug Auto):
ros2 topic pub -1 /dashboard/set_screen std_msgs/msg/Int32 "{data: 2}"
```

### Native ROS 2 Build (RPi / Bare Metal)

```bash
cd ~/GIT/lart_dashboard_ws
source ~/ros2_jazzy/install/local_setup.bash
pip install -r requirements.txt --break-system-packages
colcon build --symlink-install
source install/setup.bash

# Simulation:
ros2 launch lart_bringup sim.launch.py

# Real car (bring CAN interfaces up first):
sudo ip link set can0 up type can bitrate 1000000
sudo ip link set can1 up type can bitrate 1000000
ros2 launch lart_bringup car.launch.py
```

### Regenerate DBC API (after changing .dbc files)

```bash
# Run from the ui/ directory (or inside the Docker container)
pip install cantools
python3 LART_Car_Dashboard_v1/src/ui/generate_dbc_api.py
# Outputs: dbc_api.h, dbc_api.cpp, generated/can_bridge_impl.hpp
```

### Makefile Reference

| Command | Purpose |
|---------|---------|
| `make compose-up` | Start all containers |
| `make compose-down` | Stop all containers |
| `make compose-logs` | Follow real-time container logs |
| `make compose-rebuild` | Clean rebuild + restart |
| `make ps` | Show running containers |
| `make shell` | Open shell in publisher container |
| `make clean` | Remove all images and containers |
| `make build` | Build Docker image (amd64) |
| `make build-arm` | Build Docker image (arm64 for RPi) |

### ROS 2 Topics Reference

| Topic | Type | Publisher | Subscriber(s) |
|-------|------|-----------|---------------|
| `/can/frames` | `lart_msgs/CanFrame` | `can_bridge` | debug nodes |
| `/vehicle/rpm` | `std_msgs/Float32` | `can_bridge` | `dashboard_state_bridge`, `led_controller` |
| `/can/dbc/<msg>/<sig>` | `std_msgs/Float32` | `can_bridge` | `dashboard_state_bridge`, `ros2subscriber` |
| `/vehicle/dashboard_state` | `lart_msgs/DashboardState` | `dashboard_state_bridge` | `ros2subscriber` (UI) |
| `/input/buttons` | `lart_msgs/ButtonEvent` | `input_handler` | UI / future |
| `/input/encoders` | `lart_msgs/EncoderDelta` | `input_handler` | UI / future |
| `/dashboard/set_screen` | `std_msgs/Int32` | external / scripts | `ros2subscriber` (UI) |

### Key Configuration File

**`src/lart_bringup/config/rpi_config.yaml`** — single file that parameterises all nodes:

- `can_bridge.can_interface` — `can0` or `can1`
- `can_bridge.rpm_can_id` — **update to match your ECU's CAN ID**
- `can_bridge.dbc_path` — path to a `.dbc` file to enable `/can/dbc/*` topics
- `dashboard_state_bridge.*_topic` — map DBC signal topics to dashboard state fields
- `led_controller.rpm_shift` / `rpm_max` — LED colour threshold RPMs
- `input_handler.button_a/b`, `encoder_a/b_*` — GPIO pin assignments

### Architectural Rules & Constraints

| # | Rule |
|---|------|
| 1 | **Never hand-edit generated files**: `dbc_api.cpp`, `dbc_api.h`, `generated/can_bridge_impl.hpp`, `ui.c`, `eez-flow.cpp`. Run the generator or EEZ Studio instead. |
| 2 | **`TelemetryData` is the contract** between ROS 2 and the UI. Add new signals to the struct in `ros2subscriber.h` and add a corresponding subscriber in `ros2subscriber.cpp`. |
| 3 | **Thread safety**: `g_telemetry` is protected by `g_telemetry_mutex`. Atomic types (`std::atomic<float>`) are used for hot-path values (speed, HV). Respect this pattern when adding new topics. |
| 4 | **LVGL updates must occur on the LVGL thread** (the `ui_runner` tick loop). `screens.c` functions are called from this loop; never call LVGL APIs from the ROS 2 spin thread. |
| 5 | **DBC files are the source of truth** for all CAN signal definitions. `rpi_config.yaml` maps DBC topics to dashboard state fields; keep both in sync. |
| 6 | **Notification API** (`ui_add_notification` / `ui_clear_notification`) must be called from the LVGL thread or through a thread-safe queue — same constraint as rule 4. |
| 7 | **Docker build is canonical**: always verify that `docker compose build display` succeeds before merging UI changes. The CMake build inside Docker is the single authoritative build. |
| 8 | **GPIO is skipped in sim mode**: `input_handler` reads `sim_mode` from config. Set `sim_mode: true` in `sim.launch.py` to avoid hardware errors on dev machines. |
| 9 | **CAN bitrate is fixed at 1 Mbit/s** for both `can0` and `can1`. Do not change without updating hardware and all team tooling. |
| 10 | **EEZ Studio is the UI layout tool**. Layout changes must be made in the `.eez-project` file via EEZ Studio, then the generated C files committed. Direct edits to `ui.c` will be overwritten. |

---

*Last updated: June 2026 — generated from live codebase exploration.*
