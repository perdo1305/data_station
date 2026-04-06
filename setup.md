# Archived Setup Notes

This file keeps information that was removed from `README.md` during cleanup.

## Legacy System Credentials

- Hostname: `lart2026-desktop`
- Username: `lart2026`
- Password: `lart2026`
- SSH: `ssh lart2026@lart2026-desktop.local`

## Legacy CAN HAT Overlay Setup

Edit `/boot/firmware/config.txt` and add:

```ini
dtparam=spi=on
dtoverlay=i2c0
dtoverlay=spi1-3cs
dtoverlay=mcp2515,spi1-1,oscillator=16000000,interrupt=22
dtoverlay=mcp2515,spi1-2,oscillator=16000000,interrupt=13
```

Reboot after changes.

## Legacy CAN Bringup / Loopback Commands

Bring interfaces up:

```bash
sudo ip link set can0 up type can bitrate 1000000
sudo ip link set can1 up type can bitrate 1000000
sudo ifconfig can0 txqueuelen 65536
sudo ifconfig can1 txqueuelen 65536
```

Loopback test:

```bash
candump can0
cansend can1 000#11.22.33.44
```

## Legacy Dev Utility

Run OpenCode server on RPi:

```bash
opencode serve --hostname 0.0.0.0 --port 4096
```
