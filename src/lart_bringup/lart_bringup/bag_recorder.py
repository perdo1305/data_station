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
    # SIGTERM (systemd/launch escalation) must finalize the bag like SIGINT
    signal.signal(signal.SIGTERM, signal.default_int_handler)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
