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
