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
