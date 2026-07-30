"""Unit tests for deploy/foxglove_upload.py."""

import socket
from pathlib import Path

import pytest

import foxglove_upload as fu


def test_load_api_key_missing_file(tmp_path):
    assert fu.load_api_key(tmp_path / "nope.env") is None


def test_load_api_key_present(tmp_path):
    env_path = tmp_path / ".env"
    env_path.write_text("FOXGLOVE_API_KEY=abc123\n")
    assert fu.load_api_key(env_path) == "abc123"


def test_load_api_key_strips_quotes_and_whitespace(tmp_path):
    env_path = tmp_path / ".env"
    env_path.write_text('FOXGLOVE_API_KEY = "abc123"  \n')
    assert fu.load_api_key(env_path) == "abc123"


def test_load_api_key_ignores_comments_and_blank_lines(tmp_path):
    env_path = tmp_path / ".env"
    env_path.write_text("# comment\n\nFOXGLOVE_API_KEY=abc123\n")
    assert fu.load_api_key(env_path) == "abc123"


def test_load_api_key_empty_value_is_none(tmp_path):
    env_path = tmp_path / ".env"
    env_path.write_text("FOXGLOVE_API_KEY=\n")
    assert fu.load_api_key(env_path) is None


def test_check_connectivity_success(monkeypatch):
    class FakeConn:
        def __enter__(self):
            return self

        def __exit__(self, *a):
            return False

    monkeypatch.setattr(fu.socket, "create_connection", lambda addr, timeout: FakeConn())
    assert fu.check_connectivity("api.foxglove.dev", 443, 1.0) is True


def test_check_connectivity_failure(monkeypatch):
    def raise_os_error(addr, timeout):
        raise OSError("unreachable")

    monkeypatch.setattr(fu.socket, "create_connection", raise_os_error)
    assert fu.check_connectivity("api.foxglove.dev", 443, 1.0) is False


def test_read_bag_dir(tmp_path):
    config_path = tmp_path / "rpi_config.yaml"
    config_path.write_text(
        "bag_recorder:\n"
        "  ros__parameters:\n"
        "    bag_dir: ~/bags\n"
    )
    assert fu.read_bag_dir(config_path) == Path("~/bags").expanduser()


def test_find_mcap_file_single(tmp_path):
    (tmp_path / "session_0.mcap").write_bytes(b"data")
    assert fu.find_mcap_file(tmp_path) == tmp_path / "session_0.mcap"


def test_find_mcap_file_none(tmp_path):
    assert fu.find_mcap_file(tmp_path) is None


def test_find_mcap_file_multiple(tmp_path):
    (tmp_path / "a.mcap").write_bytes(b"data")
    (tmp_path / "b.mcap").write_bytes(b"data")
    assert fu.find_mcap_file(tmp_path) is None


def _make_session(bag_dir, name, finalized=True, uploaded=False):
    session = bag_dir / name
    session.mkdir()
    if finalized:
        (session / "metadata.yaml").write_text("")
    if uploaded:
        (session / fu.UPLOADED_MARKER).write_text("")
    return session


def test_find_pending_sessions_includes_finalized_unuploaded(tmp_path):
    session = _make_session(tmp_path, "precharge_1")
    assert fu.find_pending_sessions(tmp_path) == [session]


def test_find_pending_sessions_skips_unfinalized(tmp_path):
    _make_session(tmp_path, "precharge_1", finalized=False)
    assert fu.find_pending_sessions(tmp_path) == []


def test_find_pending_sessions_skips_already_uploaded(tmp_path):
    _make_session(tmp_path, "precharge_1", uploaded=True)
    assert fu.find_pending_sessions(tmp_path) == []


def test_find_pending_sessions_missing_bag_dir(tmp_path):
    assert fu.find_pending_sessions(tmp_path / "does_not_exist") == []
