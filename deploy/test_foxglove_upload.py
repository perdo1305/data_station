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


class FakeClient:
    def __init__(self, result=None, raises=None):
        self._result = result or {"link": "https://x/y", "text": "", "code": 200}
        self._raises = raises
        self.calls = []

    def upload_data(self, *, filename, data, key):
        self.calls.append({"filename": filename, "data": data, "key": key})
        if self._raises:
            raise self._raises
        return self._result


def test_upload_session_success_writes_marker(tmp_path):
    session = _make_session(tmp_path, "precharge_1")
    (session / "precharge_1_0.mcap").write_bytes(b"data")
    client = FakeClient(result={"link": "l", "text": "ok", "code": 200})

    assert fu.upload_session(client, session) is True
    assert (session / fu.UPLOADED_MARKER).exists()
    assert client.calls == [
        {"filename": "precharge_1_0.mcap", "data": b"data", "key": "precharge_1"}
    ]


def test_upload_session_http_failure_no_marker(tmp_path):
    session = _make_session(tmp_path, "precharge_1")
    (session / "precharge_1_0.mcap").write_bytes(b"data")
    client = FakeClient(result={"link": "l", "text": "server error", "code": 500})

    assert fu.upload_session(client, session) is False
    assert not (session / fu.UPLOADED_MARKER).exists()


def test_upload_session_missing_mcap_no_marker(tmp_path):
    session = _make_session(tmp_path, "precharge_1")
    client = FakeClient()

    assert fu.upload_session(client, session) is False
    assert not client.calls
    assert not (session / fu.UPLOADED_MARKER).exists()


def test_main_skips_without_api_key(tmp_path, monkeypatch):
    monkeypatch.setattr(fu, "ENV_PATH", tmp_path / "no.env")
    assert fu.main() == 0


def test_main_skips_when_offline(tmp_path, monkeypatch):
    env_path = tmp_path / ".env"
    env_path.write_text("FOXGLOVE_API_KEY=abc123\n")
    monkeypatch.setattr(fu, "ENV_PATH", env_path)
    monkeypatch.setattr(fu, "check_connectivity", lambda: False)
    assert fu.main() == 0


def test_main_uploads_pending_sessions(tmp_path, monkeypatch):
    env_path = tmp_path / ".env"
    env_path.write_text("FOXGLOVE_API_KEY=abc123\n")
    monkeypatch.setattr(fu, "ENV_PATH", env_path)
    monkeypatch.setattr(fu, "check_connectivity", lambda: True)

    bag_dir = tmp_path / "bags"
    bag_dir.mkdir()
    session = _make_session(bag_dir, "precharge_1")
    (session / "precharge_1_0.mcap").write_bytes(b"data")

    config_path = tmp_path / "rpi_config.yaml"
    config_path.write_text(
        f"bag_recorder:\n  ros__parameters:\n    bag_dir: {bag_dir}\n"
    )
    monkeypatch.setattr(fu, "RPI_CONFIG_PATH", config_path)

    fake_client = FakeClient()
    monkeypatch.setattr(fu, "Client", lambda token: fake_client)

    assert fu.main() == 0
    assert (session / fu.UPLOADED_MARKER).exists()
    assert fake_client.calls
