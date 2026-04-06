# OpenCode Agent Instructions for lart_dashboard_ws

## Repository Overview
ROS 2 workspace for Formula Student car dashboard with:
- Raspberry Pi 5 + 2-CH CAN HAT+ (MCP2515 controllers)
- Dual CAN bus interfaces (can0, can1) at 1 Mbps
- ROS 2 Jazzy middleware (installed at ~/ros2_jazzy/)
- Hardware interface: 2 buttons, 2 encoders, LED strip (WS2812B), HDMI display
- Core functionality: CAN-to-ROS2 bridge for RPM and other vehicle data

## Essential Commands

### Initial Setup (One-time)
```bash
# System packages (Raspberry Pi)
sudo apt update
sudo apt install python3-can can-utils -y

# Python dependencies (both Pi and dev machine)
pip install -r requirements.txt --break-system-packages
# Dev-only alternative: pip install pygame --break-system-packages
```

### Workspace Build & Source
```bash
cd ~/lart_dashboard_ws
source ~/ros2_jazzy/install/local_setup.bash
colcon build --symlink-install
source install/setup.bash
```

### Launch Commands
```bash
# Simulation (home testing - no hardware needed)
ros2 launch lart_bringup sim.launch.py

# Hardware deployment (on car with CAN HAT)
ros2 launch lart_bringup car.launch.py
```

## Hardware Configuration

### CAN Interface Setup (in /boot/firmware/config.txt)
```ini
dtparam=spi=on
dtoverlay=i2c0
dtoverlay=spi1-3cs
dtoverlay=mcp2515,spi1-1,oscillator=16000000,interrupt=22
dtoverlay=mcp2515,spi1-2,oscillator=16000000,interrupt=13
```

### CAN Interface Activation
```bash
sudo ip link set can0 up type can bitrate 1000000
sudo ip link set can1 up type can bitrate 1000000
# Optional: increase queue length for heavy traffic
sudo ifconfig can0 txqueuelen 65536
sudo ifconfig can1 txqueuelen 65536
```

### Critical Configuration File
Edit `src/lart_bringup/config/rpi_config.yaml`:
- `can_bridge.ros__parameters.rpm_can_id`: Decimal CAN ID for RPM message (e.g., 291)
- `can_bridge.ros__parameters.rpm_start_byte`: Byte offset (default 0)
- `can_bridge.ros__parameters.rpm_length_bytes`: Bytes for RPM value (default 2)
- `can_bridge.ros__parameters.rpm_scale`: Raw value multiplier (default 1.0)
- `can_bridge.ros__parameters.rpm_offset`: Raw value offset (default 0.0)
- `can_bridge.ros__parameters.can_interface`: CAN interface to use (can0 or can1)

## Testing & Verification

### Hardware Loopback Test
1. Physically connect: CAN0_H ↔ CAN1_H and CAN0_L ↔ CAN1_L
2. Terminal 1: `candump can0`
3. Terminal 2: `cansend can1 000#11.22.33.44`
4. Verify Terminal 1 shows: `can0  000   [4]  11 22 33 44`

## Package Structure
- `src/lart_bringup`: Launch files, config, bridge node (`lart_bringup/can_bridge.py`)
- `src/lart_msgs`: Custom ROS2 message definitions
- `src/dashboard_ui`: PyGame-based user interface
- `src/input_handler`: GPIO button/encoder processing
- `src/led_controller`: WS2812B LED strip control (RPM gauge)
- `src/sim`: Simulation components for testing

## Key Dependencies (from requirements.txt)
- `python-can>=4.0`: CAN bus interface
- `gpiod>=2.0`: GPIO access (buttons/encoders)
- `adafruit-circuitpython-neopixel>=6.0`: LED strip control
- `adafruit-blinka>=8.0`: CircuitPython HAL for Raspberry Pi
- `pygame>=2.5`: Dashboard display

## Important Notes
- ROS 2 Jazzy must be pre-installed at `~/ros2_jazzy/`
- Workspace expects to be at `~/lart_dashboard_ws/`
- Hardware testing requires physical CAN bus loopback
- Dev laptop testing: simulation mode skips GPIO initialization
- GPIO pin assignments in rpi_config.yaml:
  - Buttons: 17 (A), 27 (B)
  - Encoder A: CLK=22, DT=23
  - Encoder B: CLK=24, DT=25
  - LED strip: GPIO 18 (PWM0 required for WS2812B)