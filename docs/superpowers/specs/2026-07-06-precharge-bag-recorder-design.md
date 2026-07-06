# Precharge-Triggered Rosbag Recorder — Design

**Date:** 2026-07-06
**Status:** Approved

## Purpose

Automatically record decoded CAN topics to rosbag2 bags while the vehicle is
active. Recording is gated by the precharge request signal so bags only cover
periods when the tractive system is (or is about to be) energized. Raw CAN
frames (`/can/frames`) are deliberately excluded — only DBC-decoded topics are
saved.

## Trigger Signal

- Topic: `/can/dbc/start_precharge` (`lart_msgs/StartPrecharge`)
- Field: `precharge_request` (float32; DBC signal `Precharge_Request`,
  CAN ID 131 `Start_PreCharge`)
- Published by `can_bridge` with BEST_EFFORT QoS.
- A value ≥ 0.5 is treated as "1" (active), below 0.5 as "0".

## Behavior (state machine)

```
IDLE ──req=1──▶ RECORDING ──req=0──▶ GRACE(30 s) ──req=1──▶ RECORDING (same bag)
                                          │
                                     timer expires
                                          ▼
                                    finalize → IDLE
```

- **IDLE → RECORDING:** on `precharge_request` rising to 1, spawn a recording
  session (new bag folder named `precharge_YYYYMMDD_HHMMSS`).
- **RECORDING → GRACE:** on `precharge_request` falling to 0, arm a one-shot
  30 s timer. The recorder subprocess stays alive, so data during the grace
  window is still captured.
- **GRACE → RECORDING:** if `precharge_request` returns to 1 before the timer
  expires, cancel the timer and continue the *same* bag session.
- **GRACE → IDLE:** when the timer expires, finalize the bag (SIGINT to the
  recorder subprocess, wait for clean exit). The next rising edge starts a new
  session folder.
- **Node shutdown:** SIGINT any live recorder subprocess so bags are never
  left corrupt.
- **Subprocess dies unexpectedly:** log an error, reset to IDLE; the next
  rising edge starts a fresh session.

## Recording

Implementation: the node spawns a subprocess

```
ros2 bag record --regex '/can/dbc/.*' -d 60 -o <bag_dir>/precharge_YYYYMMDD_HHMMSS
```

- `--regex '/can/dbc/.*'` records every decoded CAN topic and nothing else;
  `/can/frames` (raw) and UI/dashboard topics are never matched.
- `-d 60` uses rosbag2's native splitting: one folder per session with a new
  chunk file every 60 s.
- Storage format: rosbag2 default for Jazzy (mcap).
- Output root: `~/bags/` (created if missing).

## Node & Integration

- New node `bag_recorder` in package `lart_bringup`
  (`lart_bringup/bag_recorder.py`).
- Console-script entry point in `setup.py`.
- Added to `launch/car.launch.py` with the shared `rpi_config.yaml`.

### Parameters (`rpi_config.yaml` → `bag_recorder`)

| Parameter          | Default                    | Meaning                              |
| ------------------ | -------------------------- | ------------------------------------ |
| `trigger_topic`    | `/can/dbc/start_precharge` | Topic carrying the precharge request |
| `record_regex`     | `/can/dbc/.*`              | Topics to record                     |
| `bag_dir`          | `~/bags`                   | Root folder for bag sessions         |
| `stop_grace_s`     | `30.0`                     | Delay after req=0 before finalizing  |
| `split_duration_s` | `60`                       | Bag split interval                   |

## Error Handling

- Trigger topic silent: node idles; no recording, no crash.
- `bag_dir` not writable / disk full: spawn fails → error logged, state IDLE.
- Rapid toggling of `precharge_request`: a rising edge during GRACE cancels
  the timer and returns to RECORDING (no new session). A later falling edge
  arms a fresh 30 s timer, so every falling edge gets the full grace period.
  Repeated identical values (0,0,0… or 1,1,1…) are ignored — only edges
  change state.

## Testing

- Unit-style test of the state machine with a fake clock/timer (edge cases:
  resume within grace, timer expiry, rapid toggles, subprocess death).
- Manual/sim validation with `dbc_sim.launch.py` + publishing
  `/can/dbc/start_precharge` by hand, verifying bag folders, 60 s chunking,
  and clean finalize.
