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
