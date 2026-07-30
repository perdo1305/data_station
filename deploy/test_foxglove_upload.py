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
