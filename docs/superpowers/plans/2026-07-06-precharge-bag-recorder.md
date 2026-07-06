# Precharge-Triggered Bag Recorder Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A `bag_recorder` ROS 2 node that records all `/can/dbc/*` decoded topics to rosbag2 (mcap) sessions, gated by the precharge request signal, with a 30 s stop-grace window and 60 s bag splitting.

**Architecture:** A pure-Python state machine (`trigger_state.py`, no ROS imports, fully unit-tested) drives four injected callbacks. A thin ROS node (`bag_recorder.py`) wires those callbacks to a `ros2 bag record` subprocess, an rclpy one-shot grace timer, and the `/can/dbc/start_precharge` subscription.

**Tech Stack:** ROS 2 Jazzy, rclpy, `ros2 bag record` CLI (rosbag2, mcap default), pytest, ament_python package `lart_bringup`.

## Global Constraints

- Spec: `docs/superpowers/specs/2026-07-06-precharge-bag-recorder-design.md`
- Trigger topic: `/can/dbc/start_precharge` (`lart_msgs/StartPrecharge`, field `precharge_request`), BEST_EFFORT QoS.
- Threshold: `precharge_request >= 0.5` is active.
- Record regex: `/can/dbc/.*` — raw `/can/frames` must never be recorded.
- Grace: 30.0 s default; split: 60 s default; output root: `~/bags` default. All parameterized.
- Session folder naming: `precharge_YYYYMMDD_HHMMSS`.
- Jazzy `ros2 bag record` flags (verified): `-e REGEX`, `-d MAX_BAG_DURATION`, `-o OUT`, `--disable-keyboard-controls`. Storage defaults to mcap.
- Single-threaded default executor: subscription + timers never race; no locks needed.

---

### Task 1: Pure trigger state machine (TDD)

**Files:**
- Create: `src/lart_bringup/lart_bringup/trigger_state.py`
- Test: `src/lart_bringup/test/test_trigger_state.py`

**Interfaces:**
- Consumes: nothing (stdlib only — no ROS imports allowed in this file).
- Produces: `State` enum (`IDLE`, `RECORDING`, `GRACE`) and class
  `TriggerStateMachine(start_recording, stop_recording, arm_timer, cancel_timer)`
  with methods `on_trigger(active: bool)`, `on_grace_expired()`,
  `on_recorder_died()`, `shutdown()`, attribute `state`. Task 2 imports these.

- [ ] **Step 1: Write the failing tests**

Create `src/lart_bringup/test/test_trigger_state.py`:

```python
"""Unit tests for the precharge-gated recording state machine."""

from unittest.mock import Mock

from lart_bringup.trigger_state import State, TriggerStateMachine


def make_sm(start_ok: bool = True):
    cbs = Mock()
    cbs.start.return_value = start_ok
    sm = TriggerStateMachine(
        start_recording=cbs.start,
        stop_recording=cbs.stop,
        arm_timer=cbs.arm,
        cancel_timer=cbs.cancel,
    )
    return sm, cbs


def test_starts_idle():
    sm, _ = make_sm()
    assert sm.state is State.IDLE


def test_inactive_while_idle_does_nothing():
    sm, cbs = make_sm()
    sm.on_trigger(False)
    assert sm.state is State.IDLE
    cbs.start.assert_not_called()


def test_rising_edge_starts_recording():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    assert sm.state is State.RECORDING
    cbs.start.assert_called_once()


def test_repeated_active_starts_only_once():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(True)
    sm.on_trigger(True)
    cbs.start.assert_called_once()


def test_failed_start_stays_idle():
    sm, cbs = make_sm(start_ok=False)
    sm.on_trigger(True)
    assert sm.state is State.IDLE
    # next rising sample retries
    sm.on_trigger(True)
    assert cbs.start.call_count == 2


def test_falling_edge_arms_grace_timer_keeps_recording():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    assert sm.state is State.GRACE
    cbs.arm.assert_called_once()
    cbs.stop.assert_not_called()


def test_active_during_grace_resumes_same_session():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_trigger(True)
    assert sm.state is State.RECORDING
    cbs.cancel.assert_called_once()
    cbs.start.assert_called_once()  # no new session
    cbs.stop.assert_not_called()


def test_each_falling_edge_rearms_full_grace():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_trigger(True)
    sm.on_trigger(False)
    assert sm.state is State.GRACE
    assert cbs.arm.call_count == 2


def test_grace_expiry_finalizes():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_grace_expired()
    assert sm.state is State.IDLE
    cbs.stop.assert_called_once()


def test_stale_grace_expiry_ignored_after_resume():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_trigger(True)          # resumed — timer cancelled
    sm.on_grace_expired()        # stale callback fires anyway
    assert sm.state is State.RECORDING
    cbs.stop.assert_not_called()


def test_new_session_after_finalize():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_grace_expired()
    sm.on_trigger(True)
    assert sm.state is State.RECORDING
    assert cbs.start.call_count == 2


def test_recorder_death_while_recording_resets_to_idle():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_recorder_died()
    assert sm.state is State.IDLE
    cbs.stop.assert_not_called()  # process already dead — nothing to stop
    sm.on_trigger(True)           # next rising sample recovers
    assert cbs.start.call_count == 2


def test_recorder_death_during_grace_cancels_timer():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.on_recorder_died()
    assert sm.state is State.IDLE
    cbs.cancel.assert_called_once()


def test_shutdown_while_recording_stops():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.shutdown()
    assert sm.state is State.IDLE
    cbs.stop.assert_called_once()


def test_shutdown_during_grace_cancels_and_stops():
    sm, cbs = make_sm()
    sm.on_trigger(True)
    sm.on_trigger(False)
    sm.shutdown()
    assert sm.state is State.IDLE
    cbs.cancel.assert_called_once()
    cbs.stop.assert_called_once()


def test_shutdown_while_idle_is_noop():
    sm, cbs = make_sm()
    sm.shutdown()
    cbs.stop.assert_not_called()
    cbs.cancel.assert_not_called()
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd /home/sintra/dev/data_station/src/lart_bringup && python3 -m pytest test/test_trigger_state.py -v`
Expected: collection error — `ModuleNotFoundError: No module named 'lart_bringup.trigger_state'`

- [ ] **Step 3: Write minimal implementation**

Create `src/lart_bringup/lart_bringup/trigger_state.py`:

```python
"""Pure trigger state machine for the precharge-gated bag recorder.

No ROS imports — unit-testable standalone. Timing and process management
are injected as callbacks:

  start_recording() -> bool   spawn the recorder; False = failed, stay IDLE
  stop_recording()            finalize the recorder
  arm_timer()                 (re)start the one-shot grace timer
  cancel_timer()              cancel the grace timer

State transitions (spec: 2026-07-06-precharge-bag-recorder-design.md):

  IDLE ──req=1──▶ RECORDING ──req=0──▶ GRACE ──req=1──▶ RECORDING (same bag)
                                          │
                                     timer expires
                                          ▼
                                    finalize → IDLE
"""

from enum import Enum, auto


class State(Enum):
    IDLE = auto()
    RECORDING = auto()
    GRACE = auto()


class TriggerStateMachine:
    def __init__(self, start_recording, stop_recording, arm_timer, cancel_timer):
        self._start = start_recording
        self._stop = stop_recording
        self._arm = arm_timer
        self._cancel = cancel_timer
        self.state = State.IDLE

    def on_trigger(self, active: bool) -> None:
        """Feed one sample of the trigger signal (already thresholded)."""
        if self.state is State.IDLE and active:
            if self._start():
                self.state = State.RECORDING
        elif self.state is State.RECORDING and not active:
            self._arm()
            self.state = State.GRACE
        elif self.state is State.GRACE and active:
            self._cancel()
            self.state = State.RECORDING

    def on_grace_expired(self) -> None:
        """Grace timer fired. Stale firings (after resume) are ignored."""
        if self.state is State.GRACE:
            self._stop()
            self.state = State.IDLE

    def on_recorder_died(self) -> None:
        """Recorder process exited on its own — nothing left to stop."""
        if self.state is State.GRACE:
            self._cancel()
        self.state = State.IDLE

    def shutdown(self) -> None:
        """Node is going down — finalize cleanly whatever is running."""
        if self.state is State.GRACE:
            self._cancel()
        if self.state in (State.RECORDING, State.GRACE):
            self._stop()
        self.state = State.IDLE
```

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd /home/sintra/dev/data_station/src/lart_bringup && python3 -m pytest test/test_trigger_state.py -v`
Expected: 16 passed

- [ ] **Step 5: Commit**

```bash
cd /home/sintra/dev/data_station
git add src/lart_bringup/lart_bringup/trigger_state.py src/lart_bringup/test/test_trigger_state.py
git commit -m "feat: add trigger state machine for precharge-gated bag recording"
```

---

### Task 2: bag_recorder ROS node

**Files:**
- Create: `src/lart_bringup/lart_bringup/bag_recorder.py`
- Modify: `src/lart_bringup/setup.py` (add console script)

**Interfaces:**
- Consumes: `TriggerStateMachine`, `State` from `lart_bringup.trigger_state`
  (Task 1); `lart_msgs/StartPrecharge` (existing, field `precharge_request: float32`).
- Produces: executable `bag_recorder` (entry point `lart_bringup.bag_recorder:main`)
  with parameters `trigger_topic`, `record_regex`, `bag_dir`, `stop_grace_s`,
  `split_duration_s`. Task 3 references executable and parameter names.

- [ ] **Step 1: Write the node**

Create `src/lart_bringup/lart_bringup/bag_recorder.py`:

```python
"""Precharge-triggered rosbag2 recorder.

Watches the precharge request signal and records all DBC-decoded CAN topics
(/can/dbc/*) into timestamped rosbag2 sessions:

  precharge_request >= 0.5       → start recording (new session folder)
  precharge_request < 0.5        → keep recording for stop_grace_s, then finalize
  request back to 1 within grace → same session continues

Bags split every split_duration_s (rosbag2 -d). Raw frames (/can/frames) are
never recorded — the regex only matches /can/dbc/*.

Config (rpi_config.yaml → bag_recorder):
  trigger_topic, record_regex, bag_dir, stop_grace_s, split_duration_s
"""

import signal
import subprocess
from datetime import datetime
from pathlib import Path

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

from lart_msgs.msg import StartPrecharge
from lart_bringup.trigger_state import State, TriggerStateMachine

_BEST_EFFORT = QoSProfile(
    reliability=QoSReliabilityPolicy.BEST_EFFORT,
    history=QoSHistoryPolicy.KEEP_LAST,
    depth=10,
)


class BagRecorderNode(Node):
    def __init__(self):
        super().__init__('bag_recorder')

        self.declare_parameter('trigger_topic', '/can/dbc/start_precharge')
        self.declare_parameter('record_regex', '/can/dbc/.*')
        self.declare_parameter('bag_dir', '~/bags')
        self.declare_parameter('stop_grace_s', 30.0)
        self.declare_parameter('split_duration_s', 60)

        self._trigger_topic = self.get_parameter('trigger_topic').value
        self._record_regex = self.get_parameter('record_regex').value
        self._bag_dir = Path(self.get_parameter('bag_dir').value).expanduser()
        self._stop_grace_s = float(self.get_parameter('stop_grace_s').value)
        self._split_s = int(self.get_parameter('split_duration_s').value)

        self._proc: subprocess.Popen | None = None
        self._grace_timer = None

        self._sm = TriggerStateMachine(
            start_recording=self._start_recording,
            stop_recording=self._stop_recording,
            arm_timer=self._arm_grace_timer,
            cancel_timer=self._cancel_grace_timer,
        )

        self.create_subscription(
            StartPrecharge, self._trigger_topic, self._on_trigger, _BEST_EFFORT
        )
        self.create_timer(2.0, self._check_recorder_alive)

        self.get_logger().info(
            f'bag_recorder ready — trigger={self._trigger_topic}, '
            f'regex={self._record_regex}, out={self._bag_dir}, '
            f'grace={self._stop_grace_s:.0f}s, split={self._split_s}s'
        )

    # ── state machine callbacks ────────────────────────────────────────────

    def _start_recording(self) -> bool:
        stamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        out = self._bag_dir / f'precharge_{stamp}'
        n = 1
        while out.exists():
            out = self._bag_dir / f'precharge_{stamp}_{n}'
            n += 1
        cmd = [
            'ros2', 'bag', 'record',
            '-e', self._record_regex,
            '-d', str(self._split_s),
            '-o', str(out),
            '--disable-keyboard-controls',
        ]
        try:
            self._bag_dir.mkdir(parents=True, exist_ok=True)
            self._proc = subprocess.Popen(cmd)
        except OSError as exc:
            self.get_logger().error(f'Cannot start bag recorder: {exc}')
            self._proc = None
            return False
        self.get_logger().info(f'Recording started → {out}')
        return True

    def _stop_recording(self) -> None:
        proc, self._proc = self._proc, None
        if proc is None or proc.poll() is not None:
            return
        proc.send_signal(signal.SIGINT)
        try:
            proc.wait(timeout=10.0)
            self.get_logger().info('Recording finalized.')
        except subprocess.TimeoutExpired:
            self.get_logger().error('Recorder did not exit on SIGINT — killing.')
            proc.kill()
            proc.wait()

    def _arm_grace_timer(self) -> None:
        self._cancel_grace_timer()
        self._grace_timer = self.create_timer(
            self._stop_grace_s, self._on_grace_expired
        )
        self.get_logger().info(
            f'Precharge off — finalizing in {self._stop_grace_s:.0f}s '
            f'unless it returns.'
        )

    def _cancel_grace_timer(self) -> None:
        if self._grace_timer is not None:
            self._grace_timer.cancel()
            self.destroy_timer(self._grace_timer)
            self._grace_timer = None

    # ── event sources ──────────────────────────────────────────────────────

    def _on_trigger(self, msg: StartPrecharge) -> None:
        self._sm.on_trigger(msg.precharge_request >= 0.5)

    def _on_grace_expired(self) -> None:
        self._cancel_grace_timer()  # rclpy timers repeat — make it one-shot
        self._sm.on_grace_expired()

    def _check_recorder_alive(self) -> None:
        if self._sm.state is State.IDLE or self._proc is None:
            return
        if self._proc.poll() is not None:
            self.get_logger().error(
                f'Bag recorder died unexpectedly (exit {self._proc.returncode}).'
            )
            self._proc = None
            self._sm.on_recorder_died()

    def destroy_node(self):
        self._sm.shutdown()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = BagRecorderNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
```

- [ ] **Step 2: Add console script**

Modify `src/lart_bringup/setup.py` entry_points block:

```python
    entry_points={
        'console_scripts': [
            'can_bridge = lart_bringup.can_bridge:main',
            'dashboard_state_bridge = lart_bringup.dashboard_state_bridge:main',
            'bag_recorder = lart_bringup.bag_recorder:main',
        ],
    },
```

- [ ] **Step 3: Build and verify node starts**

```bash
cd /home/sintra/dev/data_station
colcon build --packages-select lart_bringup
source install/setup.bash
timeout 5 ros2 run lart_bringup bag_recorder --ros-args -p bag_dir:=/tmp/test_bags
```

Expected log line: `bag_recorder ready — trigger=/can/dbc/start_precharge, regex=/can/dbc/.*, out=/tmp/test_bags, grace=30s, split=60s`, then clean exit on timeout's SIGTERM (exit code 124 or clean shutdown — no traceback).

- [ ] **Step 4: Re-run unit tests (still green)**

Run: `cd /home/sintra/dev/data_station/src/lart_bringup && python3 -m pytest test/test_trigger_state.py -v`
Expected: 16 passed

- [ ] **Step 5: Commit**

```bash
cd /home/sintra/dev/data_station
git add src/lart_bringup/lart_bringup/bag_recorder.py src/lart_bringup/setup.py
git commit -m "feat: add precharge-triggered bag_recorder node"
```

---

### Task 3: Config, launch integration, end-to-end validation

**Files:**
- Modify: `src/lart_bringup/config/rpi_config.yaml` (append section)
- Modify: `src/lart_bringup/launch/car.launch.py` (add node)

**Interfaces:**
- Consumes: executable `bag_recorder` + parameter names from Task 2.
- Produces: nothing downstream — final integration.

- [ ] **Step 1: Add parameters to rpi_config.yaml**

Append to `src/lart_bringup/config/rpi_config.yaml`:

```yaml

bag_recorder:
  ros__parameters:
    trigger_topic: /can/dbc/start_precharge
    record_regex: /can/dbc/.*
    bag_dir: "~/bags"            # session folders precharge_YYYYMMDD_HHMMSS
    stop_grace_s: 30.0           # keep recording this long after request=0
    split_duration_s: 60         # new bag chunk every 60 s
```

- [ ] **Step 2: Add node to car.launch.py**

In `src/lart_bringup/launch/car.launch.py`, append to the LaunchDescription list (after the `dashboard_ui` Node entry):

```python
        Node(
            package='lart_bringup',
            executable='bag_recorder',
            name='bag_recorder',
            parameters=[config],
            output='screen',
            emulate_tty=True,
        ),
```

- [ ] **Step 3: Rebuild and end-to-end test with short grace**

```bash
cd /home/sintra/dev/data_station
colcon build --packages-select lart_bringup
source install/setup.bash
rm -rf /tmp/test_bags
ros2 run lart_bringup bag_recorder --ros-args -p bag_dir:=/tmp/test_bags -p stop_grace_s:=5.0 &
sleep 3
# rising edge → recording starts
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 1.0}"
sleep 3
ls /tmp/test_bags/            # expect: one precharge_* folder
# falling edge → grace; back to 1 inside grace → same session
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 0.0}"
sleep 2
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 1.0}"
sleep 2
ls /tmp/test_bags/            # expect: STILL one folder (session resumed)
# falling edge → grace expires → finalize
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 0.0}"
sleep 8
cat /tmp/test_bags/precharge_*/metadata.yaml | head -5   # expect: metadata written = finalized
# new rising edge → NEW session folder
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 1.0}"
sleep 3
ls /tmp/test_bags/            # expect: two precharge_* folders
ros2 topic pub --once /can/dbc/start_precharge lart_msgs/msg/StartPrecharge "{precharge_request: 0.0}"
sleep 8
kill %1
```

Expected: folder count 1 → 1 → 2 as annotated; `metadata.yaml` exists after grace expiry; recorded topic list in metadata includes `/can/dbc/start_precharge`; no `/can/frames` anywhere.

- [ ] **Step 4: Verify split works (optional, longer)**

With recorder session running > 60 s and messages flowing, expect multiple `precharge_*_N.mcap` chunk files inside the session folder. Quick check with `-p split_duration_s:=10` and 25 s of `ros2 topic pub -r 5` publishing: expect ≥ 2 chunk files.

- [ ] **Step 5: Commit**

```bash
cd /home/sintra/dev/data_station
git add src/lart_bringup/config/rpi_config.yaml src/lart_bringup/launch/car.launch.py
git commit -m "feat: wire bag_recorder into car launch and config"
```
