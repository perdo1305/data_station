"""Precharge- and autonomous-triggered rosbag2 recorder.

Watches the precharge request signal and the Jetson AS state, and records all discoverable ROS topics,
including hidden topics and raw CAN frames (/can/frames), into timestamped
rosbag2 sessions. Topics under /zed/ are excluded. Topic discovery stays
enabled during recording:

  precharge_request >= 0.5 or as_state == 2 (READY) → start recording (new session folder)
  both inactive                  → keep recording for stop_grace_s, then finalize
  either back within grace       → same session continues

Each session records into a single continuous mcap file (no splitting)
unless split_duration_s is set > 0. Splitting is avoided by default: a
session killed non-gracefully (power loss, hard kill) only loses its last
few messages, instead of corrupting a whole chunk that metadata.yaml (which
rosbag2 only writes once, at clean shutdown) then points to — which is what
breaks timestamp continuity when multiple bags are imported into Foxglove
together.

On startup, any existing session folder under bag_dir that is missing
metadata.yaml (left behind by an unclean shutdown of a previous run) is
repaired in place via `ros2 bag reindex` so it stays readable.

Config (rpi_config.yaml → bag_recorder):
  trigger_topic, as_state_topic, bag_dir, stop_grace_s, split_duration_s
"""

import signal
import subprocess
from datetime import datetime
from pathlib import Path

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

from lart_msgs.msg import Jetson, StartPrecharge
from lart_bringup.trigger_state import State, TriggerStateMachine

AS_STATE_READY = 2  # autonomous_t26.dbc VAL_ AS_STATE 2 "READY"

_BEST_EFFORT = QoSProfile(
    reliability=QoSReliabilityPolicy.BEST_EFFORT,
    history=QoSHistoryPolicy.KEEP_LAST,
    depth=10,
)


class BagRecorderNode(Node):
    def __init__(self):
        super().__init__('bag_recorder')

        self.declare_parameter('trigger_topic', '/pwt/start_precharge')
        self.declare_parameter('as_state_topic', '/can/jetson')
        self.declare_parameter('bag_dir', '~/bags')
        self.declare_parameter('stop_grace_s', 30.0)
        self.declare_parameter('split_duration_s', 0)

        self._trigger_topic = self.get_parameter('trigger_topic').value
        self._as_state_topic = self.get_parameter('as_state_topic').value
        self._bag_dir = Path(self.get_parameter('bag_dir').value).expanduser()
        self._stop_grace_s = float(self.get_parameter('stop_grace_s').value)
        self._split_s = int(self.get_parameter('split_duration_s').value)

        self._proc: subprocess.Popen | None = None
        self._precharge_active = False
        self._as_active = False
        self._grace_timer = None

        self._repair_incomplete_bags()

        self._sm = TriggerStateMachine(
            start_recording=self._start_recording,
            stop_recording=self._stop_recording,
            arm_timer=self._arm_grace_timer,
            cancel_timer=self._cancel_grace_timer,
        )

        self.create_subscription(
            StartPrecharge, self._trigger_topic, self._on_trigger, _BEST_EFFORT
        )
        self.create_subscription(
            Jetson, self._as_state_topic, self._on_as_state, _BEST_EFFORT
        )
        self.create_timer(2.0, self._check_recorder_alive)

        split_desc = f'{self._split_s}s' if self._split_s > 0 else 'off (single file)'
        self.get_logger().info(
            f'bag_recorder ready — trigger={self._trigger_topic} or '
            f'{self._as_state_topic} as_state=={AS_STATE_READY}, '
            f'topics=all (including hidden, excluding /zed/*), out={self._bag_dir}, '
            f'grace={self._stop_grace_s:.0f}s, split={split_desc}'
        )

    def _repair_incomplete_bags(self) -> None:
        """Reindex any past session left without metadata.yaml by an unclean shutdown."""
        if not self._bag_dir.is_dir():
            return
        for session in sorted(self._bag_dir.iterdir()):
            if not session.is_dir() or (session / 'metadata.yaml').exists():
                continue
            has_data = any(session.glob('*.mcap')) or any(session.glob('*.db3'))
            if not has_data:
                continue
            self.get_logger().warn(
                f'{session.name} is missing metadata.yaml (unclean shutdown) — reindexing.'
            )
            try:
                subprocess.run(
                    ['ros2', 'bag', 'reindex', str(session)],
                    check=True, capture_output=True, text=True,
                )
                self.get_logger().info(f'Repaired {session.name}.')
            except (subprocess.CalledProcessError, OSError) as exc:
                detail = exc.stderr if isinstance(exc, subprocess.CalledProcessError) else exc
                self.get_logger().error(f'Failed to reindex {session.name}: {detail}')

    # ── state machine callbacks ────────────────────────────────────────────

    def _start_recording(self) -> bool:
        stamp = datetime.now().strftime('%Y-%m-%dT%H-%M-%S')
        prefix = 'autonomous' if self._as_active else 'precharge'
        out = self._bag_dir / f'{prefix}_{stamp}'
        n = 1
        while out.exists():
            out = self._bag_dir / f'{prefix}_{stamp}_{n}'
            n += 1
        cmd = [
            'ros2', 'bag', 'record',
            '--all-topics', '--include-hidden-topics',
            '--exclude-regex', '^/zed/.*',
            '-o', str(out),
            '--disable-keyboard-controls',
        ]
        if self._split_s > 0:
            cmd += ['-d', str(self._split_s)]
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
            f'Precharge and AS READY off — finalizing in {self._stop_grace_s:.0f}s '
            f'unless it returns.'
        )

    def _cancel_grace_timer(self) -> None:
        if self._grace_timer is not None:
            self._grace_timer.cancel()
            self.destroy_timer(self._grace_timer)
            self._grace_timer = None

    # ── event sources ──────────────────────────────────────────────────────

    def _on_trigger(self, msg: StartPrecharge) -> None:
        self._precharge_active = msg.precharge_request >= 0.5
        self._sm.on_trigger(self._precharge_active or self._as_active)

    def _on_as_state(self, msg: Jetson) -> None:
        self._as_active = msg.as_state == AS_STATE_READY
        self._sm.on_trigger(self._precharge_active or self._as_active)

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
    # SIGTERM (systemd/launch escalation) must finalize the bag like SIGINT
    signal.signal(signal.SIGTERM, signal.default_int_handler)
    try:
        rclpy.spin(node)
    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
