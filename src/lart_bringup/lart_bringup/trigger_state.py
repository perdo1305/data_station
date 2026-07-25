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
