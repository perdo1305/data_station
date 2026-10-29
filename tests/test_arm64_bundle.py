import re
import subprocess
from pathlib import Path

import pytest


ROOT = Path(__file__).resolve().parents[1]
CHECK = ROOT / "scripts" / "check_arm64_bundle.sh"
UI_BUILD = "LART_Car_Dashboard_v1/build/ui-build"
EXECUTABLES = [
    f"{UI_BUILD}/ui_runner",
    f"{UI_BUILD}/can_bridge",
    "install/led_controller/lib/led_controller/led_controller",
]


def script_packages():
    match = re.search(r"^PACKAGES=\((.*)\)$", CHECK.read_text(), re.MULTILINE)
    return match.group(1).split()


def make_bundle(root, skip=()):
    files = ["install/setup.bash"] + [
        f"install/{pkg}/share/{pkg}/package.xml" for pkg in script_packages()
    ]
    for rel in files + EXECUTABLES:
        if rel in skip:
            continue
        path = root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text("")
        if rel in EXECUTABLES:
            path.chmod(0o755)


def run_check(root):
    return subprocess.run(
        ["bash", str(CHECK), str(root)], capture_output=True, text=True
    )


def test_complete_bundle_passes(tmp_path):
    make_bundle(tmp_path)
    assert run_check(tmp_path).returncode == 0


def test_missing_lart_msgs_fails_and_names_path(tmp_path):
    missing = "install/lart_msgs/share/lart_msgs/package.xml"
    make_bundle(tmp_path, skip=(missing,))
    result = run_check(tmp_path)
    assert result.returncode != 0
    assert f"missing: {missing}" in result.stderr


def test_missing_can_bridge_fails(tmp_path):
    make_bundle(tmp_path, skip=(f"{UI_BUILD}/can_bridge",))
    assert run_check(tmp_path).returncode != 0


def test_non_executable_ui_runner_fails(tmp_path):
    make_bundle(tmp_path)
    (tmp_path / UI_BUILD / "ui_runner").chmod(0o644)
    assert run_check(tmp_path).returncode != 0


def test_package_list_matches_workspace():
    manifests = sorted((ROOT / "src").glob("*/package.xml"))
    if not (ROOT / "src/lart_msgs/package.xml").exists():
        pytest.skip("src/lart_msgs submodule not initialised")
    names = sorted(
        re.search(r"<name>(.*?)</name>", p.read_text()).group(1) for p in manifests
    )
    assert sorted(script_packages()) == names
