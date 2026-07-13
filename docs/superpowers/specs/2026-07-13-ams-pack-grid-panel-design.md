# AMS Pack Grid — Foxglove Extension Panel Design

## Purpose

A custom Foxglove Studio panel purpose-built for debugging the accumulator
(AMS) pack, for both live testing and recorded-bag analysis. Generic
Foxglove panels (Raw Messages, Plot) can show any one topic, but reading
the health of a 12-module × 12-cell pack across ~60 scattered topics is
impractical with generic panels — this panel renders the whole pack as one
glanceable grid.

## Architecture

A Foxglove Studio extension (TypeScript + React) built with the
`@foxglove/extension` SDK, packaged as a local `.foxe` file and installed
into the existing local Foxglove Studio install. The panel subscribes to
topics through Foxglove's own panel context API, so it works unmodified
against:

- A recorded `.mcap` bag (e.g. `precharge_20260713_164407`) — works today,
  no extra setup, since rosbag2/mcap embeds the message schemas.
- A live ROS 2 session — requires `ros-jazzy-foxglove-bridge` running on
  the machine publishing the topics (not currently installed).

The panel does not talk to ROS directly (no rclpy/rosbridge client code) —
Foxglove itself owns the connection/playback and hands the panel decoded
messages per subscribed topic via `renderState.currentFrame`.

## Data model

Per AMS slave module `NN` (01–12):

- Cell voltages (12 per module, `V`, scale 0.001): topics
  `/can/dbc/slave_NN_voltage_id_1` (`cell_voltage_1..4`),
  `_id_2` (`cell_voltage_5..8`), `_id_3` (`cell_voltage_9..12`).
- Temperatures (6 sensors + max + delta per module, `degC`, scale 0.01):
  topics `/can/dbc/slave_NN_temperature_id_1`
  (`temperature_value_1..4`), `_id_2` (`temperature_value_5..6`,
  `temperature_maximum`, `temperature_delta`).

Total: 36 voltage topics + 24 temperature topics = 60 subscriptions.

Message types are the generated `lart_msgs` ROS 2 messages (e.g.
`Slave01VoltageId1`, `Slave01TemperatureId1`); Foxglove decodes these via
the schema embedded in the bag (or advertised live by `foxglove_bridge`),
so the panel receives them as plain JS objects with matching field names —
no code generation or IDL step needed in the extension itself.

## UI layout

- A 12×12 grid: rows = modules (01–12), columns = cells (1–12). Each cell
  shows the latest voltage value, color-coded (see thresholds below).
- Below/beside each module row, a compact temperature strip showing the 6
  sensor readings plus max and delta, also color-coded.
- A cell with no message received yet renders gray with a "no data" label
  rather than 0 or blank, so a genuinely silent topic (e.g. one lost to the
  known DBC arbitration-ID collision bug) is visually distinct from a
  real 0V/0°C reading.

## Thresholds (hardcoded constants in the panel source)

Matches the realistic ranges established in the CAN simulator fix
(`src/sim/sim/can_simulator.py`), not configurable via UI in this version:

- Voltage: green 3.4–4.15 V · amber 3.2–3.4 V or 4.15–4.2 V · red <3.2 V or
  >4.2 V
- Temperature: green <45 °C · amber 45–60 °C · red >60 °C

## Data flow

1. Foxglove decodes each incoming message (bag or live) using its schema.
2. On each render tick, `renderState.currentFrame` gives the panel the
   latest message(s) per subscribed topic since the last tick.
3. The panel keeps an in-memory "latest value per cell/sensor" map, updated
   from each frame, and re-renders the grid from that map.
4. No history/trend is retained — this is a live-snapshot view, not a
   plotting panel. (A trend view is out of scope for v1, see below.)

## Setup dependencies (one-time, outside the extension code)

- Node.js/npm — not currently installed on this machine; required to build
  the extension via the Foxglove extension SDK/CLI.
- `ros-jazzy-foxglove-bridge` (apt package) — only required for **live**
  ROS topics inside Foxglove. Bag playback already works without it.

## Testing plan

- Primary: load the existing recorded bag `precharge_20260713_164407`
  (contains real `slave_01_voltage_id_*`/`temperature_id_*` data from the
  fixed CAN simulator) in Foxglove Studio with the panel installed, and
  confirm the grid renders all 12 modules with correct values and
  color-coding.
- Secondary (only if `foxglove_bridge` is installed): connect to a live
  `dbc_sim.launch.py` session and confirm the grid updates in real time.

## Out of scope for v1

Noted for potential future panels, not built now:

- Powertrain/inverter overview panel (RPM, current, voltage, temps, fault
  flags for INV1/INV2).
- Precharge/VCU state-machine visualization.
- Configurable thresholds in the panel UI (hardcoded only, for now).
- Historical trend/plotting within the panel (Foxglove's own Plot panel
  already covers this for individual topics).
