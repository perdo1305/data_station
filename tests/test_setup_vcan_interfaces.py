"""Behavioral tests for privileged virtual CAN interface setup."""

import os
from pathlib import Path
import subprocess


def test_setup_creates_each_requested_vcan_interface(tmp_path):
    calls = tmp_path / "calls"
    fake_bin = tmp_path / "bin"
    fake_bin.mkdir()

    (fake_bin / "ip").write_text(
        "#!/bin/sh\n"
        "if [ \"$1 $2\" = \"link show\" ]; then exit 1; fi\n"
        f"echo ip \"$@\" >> {calls}\n"
    )
    (fake_bin / "sudo").write_text(
        "#!/bin/sh\n"
        f"echo sudo \"$@\" >> {calls}\n"
    )
    (fake_bin / "ip").chmod(0o755)
    (fake_bin / "sudo").chmod(0o755)

    script = Path(__file__).parents[1] / "scripts" / "setup_vcan_interfaces.sh"
    env = {**os.environ, "PATH": f"{fake_bin}:{os.environ['PATH']}"}
    result = subprocess.run(
        [str(script), "vcan_data", "vcan_pwt", "vcan_auto"],
        capture_output=True,
        text=True,
        env=env,
    )

    assert result.returncode == 0, result.stderr
    assert calls.read_text().splitlines() == [
        "sudo modprobe vcan",
        "sudo ip link add vcan_data type vcan",
        "sudo ip link set vcan_data up",
        "sudo ip link add vcan_pwt type vcan",
        "sudo ip link set vcan_pwt up",
        "sudo ip link add vcan_auto type vcan",
        "sudo ip link set vcan_auto up",
    ]
