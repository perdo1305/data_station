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
  ROS 2 topics (`/can/frames`, `/can/*`).
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

    Bridge -- "/can/frames\n/can/*" --> DDS(("ROS 2 / DDS"))
    ACU -- "ROS 2 topics\n(direct, no CAN)" --> DDS

    DDS --> UI["ui_runner\n(LVGL dashboard)"]
    DDS --> Logger["Datalogger (rosbag)"]
    DDS --> Input["input_handler / led_controller"]
```

## Interface preview

![Data Station driver interface](imgs/Interface.jpeg)

The driver view keeps speed, readiness, temperatures, voltage, SOC, lap data and
pedal status visible with high contrast. Engineering-only signals remain on the
debug pages and in the recorded telemetry.

## Project structure

```
.
├── src/
│   ├── lart_bringup/     launch files + shared config (car.launch.py, sim.launch.py, config/rpi_config.yaml)
│   ├── lart_msgs/        custom ROS 2 messages (CanFrame, ButtonEvent, EncoderDelta, DashboardState)
│   ├── sim/              mock_can.py — simulated vehicle values for home testing
│   ├── input_handler/    GPIO buttons + encoders (sim_mode skips hardware)
│   └── led_controller/   WS2812 relative-current bar (safe no-op without NeoPixel libs)
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
- `led_controller`: WS2812 relative-current bar (safe no-op on machines without NeoPixel libs).

## Data flow

- CAN CH1/CH2 -> `can_bridge` (car) or `mock_can` (home).
- Topics published: `/can/frames`, `/vehicle/rpm`, `/vehicle/dashboard_state` and dynamic `/can/*` topics.
- Jetson/ACU autonomous stack publishes its own ROS 2 topics directly (no CAN involved); `ui_runner` subscribes to them.
- `ui_runner` (LVGL dashboard) consumes the CAN topics, autonomous topics, and state topics to render the cockpit UI.
- `led_controller` consumes inverter relative-current requests and drives the LED strip.
- `input_handler` publishes `/input/buttons` and `/input/encoders`.

## Build

Compiled output (the ROS2 workspace and the `ui_runner` dashboard binary) is
built by GitHub Actions (`.github/workflows/build-arm64.yml`) on every push to
`master`. Every successful build is retained as a GitHub Release tagged
`arm64-<full-commit-sha>`, while `latest-arm64` continues to point to the most
recent build. The Pi pulls the latest release instead of compiling locally:

CI caches the `lart_msgs` install (keyed by the submodule content and toolchain)
and compiler output through ccache, but always builds and packages the complete
`install/` and `ui-build/` trees. `scripts/check_arm64_bundle.sh` verifies the
bundle before it is published, and `pull_arm64_build.sh` runs the same check on
a staging copy before replacing the Pi's current build, so an incomplete
download leaves the old build in place.

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

To use an older build, open the repository's GitHub Releases page, select the
`arm64-<full-commit-sha>` release for the desired commit, and download its
`lart-dashboard-arm64.tar.gz` asset. Fast builds are retained in the same way
under `faster-arm64-<full-commit-sha>` tags.

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
sudo ip link set can1 up type can bitrate 500000
```

2) Launch:

```bash
source ~/ros2_jazzy/install/local_setup.bash
source ~/GIT/lart_dashboard_ws/install/setup.bash
ros2 launch lart_bringup car.launch.py
```

For DBC changes, follow the [DBC update guide](docs/DBC-Update-Guide.md) to validate, regenerate, rebuild, and update consumers.

## Python CAN admin panel

Once the workspace is built, start from any directory with:

```bash
python3 /home/sintra/dev/data_station/start_admin_panel.py
```

The launcher sources ROS Jazzy and this workspace automatically, preserving
`ROS_DOMAIN_ID` or defaulting to `42`. It checks the virtual CAN interfaces first
and runs the existing setup script if they are missing or down. Run it in a
terminal so you can enter your sudo password when prompted. Setup failure stops
the launcher before any ROS nodes start. Interfaces may need setup again after
a reboot.

Build once after updating the workspace, then open the desktop panel:

```bash
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install --packages-select sim lart_bringup
source install/setup.bash
export ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-42}"
ros2 run sim admin_panel
```

Tkinter is required (`sudo apt install python3-tk` if missing). The panel uses
existing virtual CAN interfaces; set them up once with
`sudo ./scripts/setup_vcan_interfaces.sh vcan_data vcan_pwt vcan_auto`.
Start the dashboard separately with the same ROS domain.
`LART_Car_Dashboard_v1/start_local.sh` opens it minimized in a normal window
and leaves simulation startup to the Python panel. Opening the panel starts or
attaches to the simulation stack automatically.

- **Start / attach** starts `dbc_sim.launch.py` or connects to its existing nodes.
  **Pause/Resume** controls one bus or all buses. **Stop owned stack** only stops
  a launch started by this panel.
- Select a signal, choose **Auto**, **Fixed**, **Sweep**, or **Random**, and apply.
  Values and min/max use DBC physical units. Sweep duration is a complete
  minimum → maximum → minimum cycle. Discrete choices and multiplexer branches
  support fixed/random valid values. Inactive branch values appear when selected
  by the multiplexer. Blank message timing restores the bus default.
- The scenario controls reproduce the shell test menu's speed sequences, with
  an editable step interval (500 ms by default). Speed is converted into inverter
  ERPM using the dashboard's existing gearing and tire dimensions; it does not
  publish the unused `/vehicle/speed_kph` topic.
- **Error tests** provides 15 temporary presets: motor/inverter/battery over-
  and undertemperature, either inverter's drivetrain fault, thermal derating,
  low LV voltage, low SOC, low cell voltage, overcurrent, steering actuator
  fault, and emergency/shutdown. Ctrl/Shift-click to combine conditions, review
  their exact DBC values, then **Apply selected errors**. The transport selector
  above chooses CAN simulation or Direct ROS. Conflicting presets and missing
  target buses are rejected before updates. **Clear errors / restore previous**
  restores prior signal controls and bus pause states; closing also restores
  them. Fault tests never save to the cfg file.
  Motor/controller cold tests inject −20 °C telemetry; Driver Gauge currently
  has no cold advisory. Battery cold uses 0 °C and BMS undertemperature code 3
  because the battery temperature fields are unsigned. Low LV/SOC presets
  inject 22 V/10%, below the gauge's 24.5 V/15% advisory thresholds. Inverter
  presets inject nonzero code 1; the preview describes the condition without
  assuming an undocumented DBC fault-code label.
- **CAN simulation** tests the full simulator → CAN bridge → current ROS topics
  path. **Direct ROS** edits current aggregated `/data/*`, `/pwt/*`, `/can/*`
  messages, preserving other fields. An initial message is required. Affected
  simulator buses pause during direct tests; **Cancel / end test** restores
  their prior state. CAN scenarios restore prior controls on completion/cancel.
- Mission selectors and **HV ON sequence** use the existing simulator parameters.
  Screen buttons select the three production screens. **Record / stop bag** uses
  `BAG_DIR` (default `~/bags`) and `BAG_RECORD_REGEX` from the shell test menu.
  Closing the panel ends temporary tests and finalizes recordings it started.

Live simulator parameters are `enabled`, `publish_hz`, `signal_controls`, and
`message_intervals_ms`. The latter two are JSON strings, for example:

```json
{"INV1_ERPM_DUTY_VOLTAGE": {"INV1_Actual_ERPM": {"mode": "fixed", "value": 20000}}}
```

```json
{"INV1_ERPM_DUTY_VOLTAGE": 50}
```

Applied CAN signal controls (including ranges, modes, and sweep periods) and
message timings are saved automatically in `config/can_simulator.cfg`, a JSON
file. The panel restores them when it discovers the same simulator and DBC on
the next run. **Save cfg** also captures current controls; **Load cfg** reapplies
them. Temporary scenarios, direct ROS tests, pause state, and precharge shortcuts
are not saved. Saves replace the file atomically.

The file's `ranges` section sets normal Auto telemetry bands: speed 0–80 km/h,
pack voltage 500–600 V, cell voltage 3.5–4.2 V, LV voltage 24–28 V, battery
temperature 25–40 °C, inverter temperature 35–55 °C, motor temperature 40–70 °C,
current −40–180 A, brake pressure 0–60 bar, and SOC 70–95%.
These are editable dashboard test defaults. DBC encoding limits still apply.
`drive_cycle` points are `[seconds, fraction_of_maximum_speed]`: the default
80-second cycle stops, accelerates to 40 km/h, cruises, accelerates to 80 km/h,
cruises, brakes to a stop, and waits before repeating. Speed and inverter ERPM
share the dashboard's conversion; pedals/current follow the driving phase and
normal fault codes remain zero. IVT voltage/current values use their DBC mV/mA
units. Other unclassified signals keep their existing generators.

Restart the simulators after editing normal ranges or the drive cycle. Explicit
Fixed/Sweep/Random controls take precedence over Auto defaults; reset a signal
to Auto to use the configured normal sequence. `LART_SIM_CONFIG` selects an
alternative configuration file. Existing simulators must
be restarted after rebuilding to expose the new parameters. If a single DBC
launch is used, set up its `vcan0` interface instead.
