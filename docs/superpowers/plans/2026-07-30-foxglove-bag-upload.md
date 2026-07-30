# Foxglove Bag Auto-Upload Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Automatically upload finalized precharge bag sessions to Foxglove whenever the Pi has internet, with no manual step and no crash/log-spam when offline.

**Architecture:** A standalone `deploy/foxglove_upload.py` script (no ROS2 dependency), run every 5 minutes by a systemd timer. It checks connectivity, reads `bag_dir` from the existing `rpi_config.yaml`, finds finalized-but-unuploaded sessions, and uploads each session's `.mcap` file via the official `foxglove-client` Python package.

**Tech Stack:** Python 3, `foxglove-client` (PyPI, official Foxglove SDK — confirmed via `github.com/foxglove/foxglove-python`, `foxglove.client.Client.upload_data`), `PyYAML`, `pytest` + `unittest.mock` for tests, systemd (`Type=oneshot` service + timer).

## Global Constraints

- Script must never exit nonzero or raise for expected/transient conditions: missing API key, offline, HTTP upload failure. Only genuine local bugs may raise. (Spec: Error Handling)
- `bag_dir` is read from `src/lart_bringup/config/rpi_config.yaml` (`bag_recorder.ros__parameters.bag_dir`) — no separate `BAG_DIR` env var. (Spec: Data Flow step 3)
- `deploy/.env` holds the real `FOXGLOVE_API_KEY` and must be gitignored; `deploy/.env.example` is the tracked template. (Spec: Components)
- A session is "finalized" only if `metadata.yaml` exists in its folder (matches `bag_recorder`'s own clean-shutdown marker). (Spec: Data Flow step 4)
- Upload success is marked by writing a `.uploaded` file in the session folder, written only after a confirmed 2xx response. (Spec: Data Flow step 6)
- With `split_duration_s: 0` (current `rpi_config.yaml` default), each finalized session has exactly one `.mcap` file; sessions with zero or multiple `.mcap` files are skipped with a logged warning rather than guessed at.
- Connectivity check targets `api.foxglove.dev:443` directly (the actual upload target), not a generic internet check. (Spec: Data Flow step 2)

---

## File Structure

- `requirements.txt` (modify) — add `foxglove-client`, `PyYAML`
- `.gitignore` (modify) — add `deploy/.env`
- `deploy/.env.example` (create) — template for the API key
- `deploy/foxglove_upload.py` (create) — all upload logic, built incrementally across Tasks 2–4
- `deploy/test_foxglove_upload.py` (create) — pytest unit tests, built alongside the script
- `deploy/foxglove-upload.service` (create) — systemd oneshot unit
- `deploy/foxglove-upload.timer` (create) — systemd timer, 5 min interval

---

### Task 1: Scaffolding — dependencies, gitignore, env template

**Files:**
- Modify: `requirements.txt`
- Modify: `.gitignore`
- Create: `deploy/.env.example`

**Interfaces:**
- Produces: `FOXGLOVE_API_KEY` env-file convention (`KEY=value` line format) that Task 2's `load_api_key()` parses.

- [ ] **Step 1: Add new dependencies to `requirements.txt`**

Append to the end of `requirements.txt`:

```
# Foxglove bag auto-upload (deploy/foxglove_upload.py)
# Official Foxglove Data Platform client — github.com/foxglove/foxglove-python
foxglove-client>=0.19.0
PyYAML>=6.0
```

- [ ] **Step 2: Ignore the real env file**

In `.gitignore`, add a new line after the existing `.vscode/` line:

```
deploy/.env
```

- [ ] **Step 3: Create the tracked env template**

Create `deploy/.env.example`:

```
# Foxglove Data Platform API key. Create one from your Foxglove org's
# Settings -> API Keys page (needs the recordings-upload capability).
# Copy this file to .env in the same directory and fill in the value —
# deploy/.env is gitignored and never committed.
FOXGLOVE_API_KEY=
```

- [ ] **Step 4: Commit**

```bash
git add requirements.txt .gitignore deploy/.env.example
git commit -m "build: add foxglove-client/PyYAML deps and env template for bag upload"
```

---

### Task 2: API key loading + connectivity check

**Files:**
- Create: `deploy/foxglove_upload.py`
- Create: `deploy/test_foxglove_upload.py`

**Interfaces:**
- Consumes: nothing from earlier tasks.
- Produces:
  - `load_api_key(env_path: Path) -> str | None`
  - `check_connectivity(host: str = FOXGLOVE_HOST, port: int = FOXGLOVE_PORT, timeout: float = 5.0) -> bool`
  - Module constants `FOXGLOVE_HOST = "api.foxglove.dev"`, `FOXGLOVE_PORT = 443`, `UPLOADED_MARKER = ".uploaded"`, `SCRIPT_DIR`, `ENV_PATH`, `RPI_CONFIG_PATH` (used by later tasks).

- [ ] **Step 1: Write the failing tests**

Create `deploy/test_foxglove_upload.py`:

```python
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
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: FAIL with `ModuleNotFoundError: No module named 'foxglove_upload'`

- [ ] **Step 3: Write the implementation**

Create `deploy/foxglove_upload.py`:

```python
#!/usr/bin/env python3
"""Uploads finalized precharge bag sessions to Foxglove when online.

Run every 5 minutes by foxglove-upload.timer (systemd). Each run:
  1. Loads FOXGLOVE_API_KEY from deploy/.env (skips if unset).
  2. Checks api.foxglove.dev is reachable (skips if not).
  3. Reads bag_dir from src/lart_bringup/config/rpi_config.yaml — the same
     directory bag_recorder writes sessions into.
  4. Uploads any session that has metadata.yaml (finalized by bag_recorder)
     and no .uploaded marker yet.

Never exits nonzero for expected/transient conditions (offline, no key,
upload failure) — only for genuine local bugs — so systemd doesn't
accumulate failed-unit spam for states that just mean "try again later".
"""

import socket
import sys
from datetime import datetime
from pathlib import Path
from typing import List, Optional

import requests
import yaml
from foxglove.client import Client

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent
ENV_PATH = SCRIPT_DIR / ".env"
RPI_CONFIG_PATH = REPO_ROOT / "src" / "lart_bringup" / "config" / "rpi_config.yaml"

FOXGLOVE_HOST = "api.foxglove.dev"
FOXGLOVE_PORT = 443
UPLOADED_MARKER = ".uploaded"


def load_api_key(env_path: Path) -> Optional[str]:
    if not env_path.is_file():
        return None
    for line in env_path.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, _, value = line.partition("=")
        if key.strip() != "FOXGLOVE_API_KEY":
            continue
        value = value.strip().strip('"').strip("'")
        return value or None
    return None


def check_connectivity(
    host: str = FOXGLOVE_HOST, port: int = FOXGLOVE_PORT, timeout: float = 5.0
) -> bool:
    try:
        with socket.create_connection((host, port), timeout=timeout):
            return True
    except OSError:
        return False


def main() -> int:
    api_key = load_api_key(ENV_PATH)
    if not api_key:
        print("FOXGLOVE_API_KEY not set — skipping.", file=sys.stderr)
        return 0

    if not check_connectivity():
        print("Foxglove unreachable — skipping.")
        return 0

    return 0


if __name__ == "__main__":
    sys.exit(main())
```

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: PASS (7 tests)

- [ ] **Step 5: Commit**

```bash
git add deploy/foxglove_upload.py deploy/test_foxglove_upload.py
git commit -m "feat: add API key loading and connectivity check for bag upload"
```

---

### Task 3: Bag directory scanning

**Files:**
- Modify: `deploy/foxglove_upload.py`
- Modify: `deploy/test_foxglove_upload.py`

**Interfaces:**
- Consumes: `RPI_CONFIG_PATH`, `UPLOADED_MARKER` from Task 2.
- Produces:
  - `read_bag_dir(config_path: Path) -> Path`
  - `find_mcap_file(session_dir: Path) -> Path | None` (returns `None` if not exactly one `.mcap` file)
  - `find_pending_sessions(bag_dir: Path) -> List[Path]`

- [ ] **Step 1: Write the failing tests**

Append to `deploy/test_foxglove_upload.py`:

```python
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
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: FAIL — `AttributeError: module 'foxglove_upload' has no attribute 'read_bag_dir'` (and similarly for `find_mcap_file`, `find_pending_sessions`)

- [ ] **Step 3: Write the implementation**

In `deploy/foxglove_upload.py`, add these functions after `check_connectivity` and before `main`:

```python
def read_bag_dir(config_path: Path) -> Path:
    with config_path.open() as f:
        config = yaml.safe_load(f)
    raw = config["bag_recorder"]["ros__parameters"]["bag_dir"]
    return Path(raw).expanduser()


def find_mcap_file(session_dir: Path) -> Optional[Path]:
    mcaps = sorted(session_dir.glob("*.mcap"))
    if len(mcaps) != 1:
        return None
    return mcaps[0]


def find_pending_sessions(bag_dir: Path) -> List[Path]:
    if not bag_dir.is_dir():
        return []
    pending = []
    for session in sorted(bag_dir.iterdir()):
        if not session.is_dir():
            continue
        if not (session / "metadata.yaml").exists():
            continue
        if (session / UPLOADED_MARKER).exists():
            continue
        pending.append(session)
    return pending
```

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: PASS (15 tests)

- [ ] **Step 5: Commit**

```bash
git add deploy/foxglove_upload.py deploy/test_foxglove_upload.py
git commit -m "feat: add bag directory scanning for pending upload sessions"
```

---

### Task 4: Upload + orchestration

**Files:**
- Modify: `deploy/foxglove_upload.py`
- Modify: `deploy/test_foxglove_upload.py`

**Interfaces:**
- Consumes: `find_mcap_file`, `find_pending_sessions`, `read_bag_dir`, `load_api_key`, `check_connectivity`, `UPLOADED_MARKER`, `ENV_PATH`, `RPI_CONFIG_PATH` from Tasks 2–3. Uses `foxglove.client.Client` — constructed as `Client(token=api_key)`; `client.upload_data(filename=str, data=bytes, key=str)` returns `{"link": str, "text": str, "code": int}` and does **not** raise on a failed PUT (only the initial link request can raise `requests.exceptions.HTTPError`/`FoxgloveException` — see `github.com/foxglove/foxglove-python` `foxglove/client/api.py:upload_data`). The `key` argument de-duplicates server-side on repeat uploads with the same key — pass the session folder name so a lost-then-retried upload can't create a duplicate recording.
- Produces:
  - `upload_session(client: Client, session_dir: Path) -> bool`
  - `main() -> int` (final version — replaces the Task 2 stub body)

- [ ] **Step 1: Write the failing tests**

Append to `deploy/test_foxglove_upload.py`:

```python
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
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: FAIL — `AttributeError: module 'foxglove_upload' has no attribute 'upload_session'` (and the `test_main_*` cases fail because `main()` currently always returns before touching bags)

- [ ] **Step 3: Write the implementation**

In `deploy/foxglove_upload.py`, add `upload_session` after `find_pending_sessions`, and replace `main`:

```python
def upload_session(client: Client, session_dir: Path) -> bool:
    mcap_path = find_mcap_file(session_dir)
    if mcap_path is None:
        print(
            f"{session_dir.name}: expected exactly one .mcap file, skipping",
            file=sys.stderr,
        )
        return False

    data = mcap_path.read_bytes()
    try:
        result = client.upload_data(filename=mcap_path.name, data=data, key=session_dir.name)
    except (requests.exceptions.RequestException, Exception) as exc:  # noqa: BLE001
        print(f"{session_dir.name}: upload error: {exc}", file=sys.stderr)
        return False

    if not (200 <= result["code"] < 300):
        print(
            f"{session_dir.name}: upload failed ({result['code']}): {result['text']}",
            file=sys.stderr,
        )
        return False

    (session_dir / UPLOADED_MARKER).write_text(datetime.now().isoformat() + "\n")
    print(f"{session_dir.name}: uploaded.")
    return True


def main() -> int:
    api_key = load_api_key(ENV_PATH)
    if not api_key:
        print("FOXGLOVE_API_KEY not set — skipping.", file=sys.stderr)
        return 0

    if not check_connectivity():
        print("Foxglove unreachable — skipping.")
        return 0

    bag_dir = read_bag_dir(RPI_CONFIG_PATH)
    pending = find_pending_sessions(bag_dir)
    if not pending:
        return 0

    client = Client(token=api_key)
    for session in pending:
        upload_session(client, session)

    return 0


if __name__ == "__main__":
    sys.exit(main())
```

Note: `upload_session` catches broad `Exception` deliberately — an upload error for one session (network blip mid-upload, malformed response) must not crash the loop or skip remaining pending sessions.

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd deploy && python3 -m pytest test_foxglove_upload.py -v`
Expected: PASS (21 tests)

- [ ] **Step 5: Commit**

```bash
git add deploy/foxglove_upload.py deploy/test_foxglove_upload.py
git commit -m "feat: upload pending bag sessions to Foxglove and mark them done"
```

---

### Task 5: systemd service + timer

**Files:**
- Create: `deploy/foxglove-upload.service`
- Create: `deploy/foxglove-upload.timer`

**Interfaces:**
- Consumes: `deploy/foxglove_upload.py` (Task 4) as the `ExecStart` target. Deploy path on the car is `/home/lart2026/GIT/data_station/` (per project convention).

- [ ] **Step 1: Create the service unit**

Create `deploy/foxglove-upload.service`:

```ini
# Install on the car (as root):
#   cp deploy/foxglove-upload.service deploy/foxglove-upload.timer /etc/systemd/system/
#   systemctl daemon-reload
#   systemctl enable --now foxglove-upload.timer
[Unit]
Description=Upload finalized precharge bags to Foxglove
After=network-online.target
Wants=network-online.target

[Service]
Type=oneshot
WorkingDirectory=/home/lart2026/GIT/data_station/deploy
ExecStart=/usr/bin/python3 /home/lart2026/GIT/data_station/deploy/foxglove_upload.py
```

- [ ] **Step 2: Create the timer unit**

Create `deploy/foxglove-upload.timer`:

```ini
[Unit]
Description=Run foxglove-upload.service every 5 minutes

[Timer]
OnBootSec=2min
OnUnitActiveSec=5min
Unit=foxglove-upload.service

[Install]
WantedBy=timers.target
```

- [ ] **Step 3: Validate unit syntax**

Run: `systemd-analyze verify deploy/foxglove-upload.service deploy/foxglove-upload.timer`
Expected: no output (no errors). If `systemd-analyze` isn't available on this machine, skip this step — it will be validated on the Pi in Task 6.

- [ ] **Step 4: Commit**

```bash
git add deploy/foxglove-upload.service deploy/foxglove-upload.timer
git commit -m "build: add systemd timer to run bag upload every 5 minutes"
```

---

### Task 6: Manual end-to-end verification (on the Pi)

**Files:** none (verification only).

**Interfaces:**
- Consumes: everything from Tasks 1–5.

- [ ] **Step 1: Install dependencies and configure the key**

```bash
cd /home/lart2026/GIT/data_station
pip install -r requirements.txt --break-system-packages
cp deploy/.env.example deploy/.env
# edit deploy/.env, set FOXGLOVE_API_KEY to a real key with recordings-upload capability
```

- [ ] **Step 2: Run once against a real completed session**

Ensure at least one folder under the configured `bag_dir` has `metadata.yaml` (a finished precharge recording — trigger one via `dbc_sim.launch.py` if needed, per the bag_recorder spec's manual test).

```bash
python3 deploy/foxglove_upload.py
```

Expected: prints `<session_name>: uploaded.`; the session's recording appears in the Foxglove Data Platform UI; a `.uploaded` file now exists in that session's folder.

- [ ] **Step 3: Verify idempotency**

```bash
python3 deploy/foxglove_upload.py
```

Expected: no output, no new upload (session already has `.uploaded`).

- [ ] **Step 4: Verify offline skip**

Disconnect the Pi's internet (or temporarily block `api.foxglove.dev` via `/etc/hosts`), then:

```bash
python3 deploy/foxglove_upload.py; echo "exit=$?"
```

Expected: prints `Foxglove unreachable — skipping.`, `exit=0`.

- [ ] **Step 5: Enable the timer**

```bash
sudo cp deploy/foxglove-upload.service deploy/foxglove-upload.timer /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now foxglove-upload.timer
systemctl list-timers foxglove-upload.timer
```

Expected: timer listed with a `NEXT` time within 5 minutes.

---

## Self-Review Notes

- Spec coverage: API key loading (Task 2), connectivity check (Task 2), `bag_dir` from `rpi_config.yaml` (Task 3), finalized+unuploaded scanning (Task 3), upload + marker (Task 4), never-nonzero error handling (Tasks 2 & 4, verified manually in Task 6 step 3), systemd 5-min timer (Task 5) — all spec sections have a task.
- The exact Foxglove upload API shape (`POST /v1/data/upload` → presigned `link` → `PUT` raw bytes, wrapped by `foxglove.client.Client.upload_data`) was confirmed against `github.com/foxglove/foxglove-python` source (`foxglove/client/api.py`) and PyPI (`foxglove-client==0.19.2`) rather than assumed, per the spec's explicit requirement.
- Type/signature consistency checked: `upload_session(client, session_dir)` used identically in Task 4's implementation and tests; `main()`'s use of `Client`, `ENV_PATH`, `RPI_CONFIG_PATH`, `check_connectivity` are all monkeypatchable module attributes, matching how Task 4's tests patch them.
