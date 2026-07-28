<p align="center">
  <img src="imgs/LART_LogoBranco.png" alt="LART logo" width="600">
</p>

# LART DataStation w/ ROS2

Formula Student dashboard workspace for Raspberry Pi 5 + dual CAN (Waveshare 2-CH CAN HAT+).

## Architecture

The DataStation sits between the car's CAN bus and the autonomous stack, and
exposes everything to the driver through the LVGL dashboard:

- **CAN in** — `can0`/`can1` carry `data_t26.dbc` and `powertrain_t26.dbc`
  frames (speed, temps, pressures, HV/LV, inverter/motor telemetry). The
  `can_bridge` decodes these directly off the wire and republishes them as
  ROS 2 topics (`/can/frames`, `/can/dbc/*`).
- **Autonomous in** — the Jetson/ACU stack does **not** go through the CAN
  bridge. It publishes its own ROS 2 topics over DDS (mission state, ACU
  state, AS state, emergency cause, SLAM/lap info), and the DataStation
  simply **subscribes** to them.
- **Dashboard out** — `ui_runner` subscribes to both the CAN-derived topics
  and the autonomous topics and renders everything on the cockpit UI
  (Driver View, Autonomous, and the paged Debug/Debug-Autonomous screens).

```mermaid
flowchart LR
    subgraph CAN["Vehicle CAN bus"]
        CH1["can0"]
        CH2["can1"]
    end

    subgraph Jetson["Autonomous stack (Jetson / ACU)"]
        ACU["ACU / mission / AS state"]
    end

    CH1 --> Bridge["can_bridge\n(data_t26.dbc, powertrain_t26.dbc)"]
    CH2 --> Bridge

    Bridge -- "/can/frames\n/can/dbc/*" --> DDS(("ROS 2 / DDS"))
    ACU -- "ROS 2 topics\n(direct, no CAN)" --> DDS

    DDS --> UI["ui_runner\n(LVGL dashboard)"]
    DDS --> Logger["Datalogger (rosbag)"]
    DDS --> Input["input_handler / led_controller"]
```

![DataStation interface](imgs/Interface.jpeg)

## Project structure

```
.
├── src/
│   ├── lart_bringup/     launch files + shared config (car.launch.py, sim.launch.py, config/rpi_config.yaml)
│   ├── lart_msgs/        custom ROS 2 messages (CanFrame, ButtonEvent, EncoderDelta, DashboardState)
│   ├── sim/              mock_can.py — simulated vehicle values for home testing
│   ├── input_handler/    GPIO buttons + encoders (sim_mode skips hardware)
│   └── led_controller/   WS2812 RPM bar (safe no-op without NeoPixel libs)
├── LART_Car_Dashboard_v1/
│   └── src/ui/           LVGL C++ dashboard (ui_runner, can_bridge, generated DBC API)
├── dbc_signals/          DBC source files (data_t26.dbc, powertrain_t26.dbc, autonomous_t26.dbc)
├── scripts/              pull_arm64_build.sh — pulls the CI-built arm64 release onto the Pi
├── .github/workflows/    build-arm64.yml — CI build + release of the ROS2 workspace + ui_runner
├── imgs/                 README assets (logo, interface screenshot)
├── context.md            full architecture/dev-conventions reference for collaborators & LLMs
└── setup.md              archived legacy setup notes (credentials, old HAT overlay, loopback tests)
```

## What does what

- `lart_bringup`: launch + shared config (`car.launch.py`, `sim.launch.py`, `config/rpi_config.yaml`).
- `lart_msgs`: custom ROS 2 messages (`CanFrame`, `ButtonEvent`, `EncoderDelta`, `DashboardState`).
- `lart_bringup/can_bridge.py`: real car CAN reader (`can0/can1`) and ROS publisher.
- `sim/mock_can.py`: simulated vehicle values for home testing.
- `LART_Car_Dashboard_v1/src/ui`: LVGL C++ dashboard interface (production `ui_runner` application).
- `input_handler`: GPIO buttons + encoders (`sim_mode` skips hardware).
- `led_controller`: WS2812 RPM bar (safe no-op on machines without NeoPixel libs).

## Data flow

- CAN CH1/CH2 -> `can_bridge` (car) or `mock_can` (home).
- Topics published: `/can/frames`, `/vehicle/rpm`, `/vehicle/dashboard_state` and dynamic `/can/dbc/*` topics.
- Jetson/ACU autonomous stack publishes its own ROS 2 topics directly (no CAN involved); `ui_runner` subscribes to them.
- `ui_runner` (LVGL dashboard) consumes the CAN topics, autonomous topics, and state topics to render the cockpit UI.
- `led_controller` consumes RPM and drives LED strip.
- `input_handler` publishes `/input/buttons` and `/input/encoders`.

## Build

Compiled output (the ROS2 workspace and the `ui_runner` dashboard binary) is
built by GitHub Actions (`.github/workflows/build-arm64.yml`) on every push to
`master` and published to the `latest-arm64` release — the Pi pulls it
instead of compiling locally:

**One-time prerequisite** (run once on a fresh/reimaged Pi):
```bash
sudo apt install libsdl2-2.0-0
```
This installs the SDL2 runtime library needed by the `ui_runner` binary.

Then pull and run the build:
```bash
cd ~/GIT/lart_dashboard_ws
git pull
pip install -r requirements.txt --break-system-packages
./scripts/pull_arm64_build.sh
source install/setup.bash
```

To build locally instead (e.g. while developing on a non-arm64 machine, or if
CI is unavailable), use the original workflow:

```bash
cd ~/GIT/lart_dashboard_ws
source ~/ros2_jazzy/install/local_setup.bash
pip install -r requirements.txt --break-system-packages
colcon build --symlink-install
source install/setup.bash
```

## Run simulation

```bash
source ~/ros2_jazzy/install/local_setup.bash
source ~/GIT/lart_dashboard_ws/install/setup.bash
ros2 launch lart_bringup sim.launch.py
```

## Run on the car (real CAN + GPIO + LEDs)

1) Bring CAN interfaces up:

```bash
sudo ip link set can0 up type can bitrate 1000000
sudo ip link set can1 up type can bitrate 1000000
```

2) Launch:

```bash
source ~/ros2_jazzy/install/local_setup.bash
source ~/GIT/lart_dashboard_ws/install/setup.bash
ros2 launch lart_bringup car.launch.py
```

