"""Check topic selection without starting DDS or writing a bag."""

import re
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock, patch

from lart_bringup.bag_recorder import BagRecorderNode


def test_records_all_topics_except_zed_including_hidden_topics(tmp_path):
    recorder = SimpleNamespace(
        _bag_dir=tmp_path / 'bags',
        _record_regex='/data/.*|/pwt/.*|/can/(?!frames$).*|/imu/.*',
        _split_s=0,
        _as_active=False,
        get_logger=Mock(),
    )
    with patch('lart_bringup.bag_recorder.subprocess.Popen') as popen:
        assert BagRecorderNode._start_recording(recorder)

    command = popen.call_args.args[0]
    assert command[:3] == ['ros2', 'bag', 'record']
    assert '--all-topics' in command
    assert '--include-hidden-topics' in command
    assert '-e' not in command
    assert '--exclude-topics' not in command
    excluded = command[command.index('--exclude-regex') + 1]
    assert re.search(excluded, '/zed/zed_node/rgb/image_rect_color')
    assert re.search(excluded, '/zed/point_cloud')
    assert not re.search(excluded, '/can/frames')
    assert not re.search(excluded, '/data/speed')
    assert not re.search(excluded, '/zed_other/image')
    assert not re.search(excluded, '/other/zed/image')
    assert '--no-discovery' not in command
    assert recorder._proc is popen.return_value


def _trigger_recorder():
    return SimpleNamespace(_precharge_active=False, _as_active=False, _sm=Mock())


def test_as_state_ready_triggers_recording():
    recorder = _trigger_recorder()
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=2))
    recorder._sm.on_trigger.assert_called_with(True)
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=3))
    recorder._sm.on_trigger.assert_called_with(False)


def test_either_trigger_keeps_recording_active():
    recorder = _trigger_recorder()
    BagRecorderNode._on_trigger(recorder, SimpleNamespace(precharge_request=1.0))
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=1))
    recorder._sm.on_trigger.assert_called_with(True)
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=2))
    BagRecorderNode._on_trigger(recorder, SimpleNamespace(precharge_request=0.0))
    recorder._sm.on_trigger.assert_called_with(True)


def test_autonomous_session_folder_prefix(tmp_path):
    recorder = SimpleNamespace(
        _bag_dir=tmp_path / 'bags', _split_s=0, _as_active=True, get_logger=Mock(),
    )
    with patch('lart_bringup.bag_recorder.subprocess.Popen') as popen:
        assert BagRecorderNode._start_recording(recorder)
    command = popen.call_args.args[0]
    out = Path(command[command.index('-o') + 1])
    assert out.name.startswith('autonomous_')


def _session(tmp_path, name, ready_seen):
    out = tmp_path / name
    out.mkdir()
    return SimpleNamespace(_out=out, _ready_seen=ready_seen, get_logger=Mock())


def test_precharge_session_that_saw_ready_becomes_autonomous(tmp_path):
    recorder = _session(tmp_path, 'precharge_2026-10-31T18-16-47', ready_seen=True)
    BagRecorderNode._finish_session(recorder)
    assert (tmp_path / 'autonomous_2026-10-31T18-16-47').is_dir()
    assert not (tmp_path / 'precharge_2026-10-31T18-16-47').exists()
    assert recorder._out is None


def test_precharge_session_without_ready_keeps_its_name(tmp_path):
    recorder = _session(tmp_path, 'precharge_x', ready_seen=False)
    BagRecorderNode._finish_session(recorder)
    assert (tmp_path / 'precharge_x').is_dir()


def test_rename_does_not_overwrite_an_existing_folder(tmp_path):
    (tmp_path / 'autonomous_x').mkdir()
    recorder = _session(tmp_path, 'precharge_x', ready_seen=True)
    BagRecorderNode._finish_session(recorder)
    assert (tmp_path / 'autonomous_x_1').is_dir() and (tmp_path / 'autonomous_x').is_dir()


def test_ready_during_session_is_remembered_and_start_resets_it(tmp_path):
    recorder = _trigger_recorder()
    recorder._ready_seen = False
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=2))
    assert recorder._ready_seen
    BagRecorderNode._on_as_state(recorder, SimpleNamespace(as_state=1))
    assert recorder._ready_seen  # stays set after READY ends
    recorder._bag_dir, recorder._split_s, recorder._as_active = tmp_path / 'b', 0, False
    recorder.get_logger = Mock()
    with patch('lart_bringup.bag_recorder.subprocess.Popen'):
        assert BagRecorderNode._start_recording(recorder)
    assert not recorder._ready_seen  # a new precharge session starts clean
