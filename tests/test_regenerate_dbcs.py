"""Exercise the complete generator chain with real databases and generators."""
import hashlib
import json
from pathlib import Path
import runpy
import shutil
import subprocess

import cantools
import pytest


ROOT = Path(__file__).resolve().parents[1]
NAMES = ("data_t26", "powertrain_t26", "autonomous_t26")


@pytest.fixture
def workspace(tmp_path):
    ui = tmp_path / "LART_Car_Dashboard_v1/src/ui"
    (ui / "generated").mkdir(parents=True)
    for relative in ("generate_dbc_api.py", "generated/generate_can_bridge.py"):
        shutil.copy(ROOT / "LART_Car_Dashboard_v1/src/ui" / relative, ui / relative)
    shutil.copytree(ROOT / "src/lart_msgs", tmp_path / "src/lart_msgs",
                    ignore=shutil.ignore_patterns(".git"))
    (tmp_path / "dbc_signals").mkdir()
    for name in NAMES:
        shutil.copy(ROOT / f"dbc_signals/{name}.dbc", tmp_path / "dbc_signals")
    return tmp_path


def regenerate(root, source=None):
    script = ROOT / "scripts/regenerate_dbcs.py"
    assert script.is_file(), "The complete regeneration entry point is missing"
    runpy.run_path(str(script))["regenerate"](root, source)


def snapshot(root):
    return {str(p.relative_to(root)): p.read_bytes()
            for p in root.rglob("*") if p.is_file()}


def test_regeneration_is_repeatable_and_compiles_every_decoder(workspace, tmp_path):
    regenerate(workspace)
    first = snapshot(workspace)
    regenerate(workspace)
    assert snapshot(workspace) == first, "Identical DBCs must not create new diffs"
    ui = workspace / "LART_Car_Dashboard_v1/src/ui"
    assert (ui / "dbc_api.h").is_file()
    assert (ui / "generated/can_bridge_impl.hpp").is_file()
    assert '"dbc_msgs/Aqt7.msg"' in (workspace / "src/lart_msgs/CMakeLists.txt").read_text()
    for name in NAMES:
        subprocess.run(["cc", "-std=c99", "-Wall", "-Wextra", "-Werror", "-c",
                        str(ui / f"generated/{name}.c"), "-o", str(tmp_path / f"{name}.o")],
                       check=True)


def test_invalid_source_leaves_current_inputs_and_outputs_untouched(workspace, tmp_path):
    source = tmp_path / "incoming"
    shutil.copytree(workspace / "dbc_signals", source)
    (source / "autonomous_t26.dbc").write_text("not a DBC")
    before = snapshot(workspace)
    with pytest.raises(cantools.database.errors.UnsupportedDatabaseFormatError):
        regenerate(workspace, source)
    assert snapshot(workspace) == before


def test_source_manifest_records_exact_commit_and_file_hashes(workspace, tmp_path):
    source = tmp_path / "incoming"
    shutil.copytree(workspace / "dbc_signals", source)
    subprocess.run(["git", "init", "-q", str(source)], check=True)
    subprocess.run(["git", "-C", str(source), "add", "."], check=True)
    subprocess.run(["git", "-C", str(source), "-c", "user.name=Test", "-c",
                    "user.email=test@example.com", "commit", "-qm", "DBC fixture"], check=True)
    revision = subprocess.check_output(["git", "-C", str(source), "rev-parse", "HEAD"],
                                       text=True).strip()
    regenerate(workspace, source)
    manifest = json.loads((workspace / "dbc_signals/source.json").read_text())
    assert manifest["commit"] == revision
    assert manifest["sha256"] == {
        f"{name}.dbc": hashlib.sha256((source / f"{name}.dbc").read_bytes()).hexdigest()
        for name in NAMES
    }
    regenerate(workspace)
    assert json.loads((workspace / "dbc_signals/source.json").read_text()) == manifest
    (workspace / "dbc_signals/data_t26.dbc").write_bytes(
        (workspace / "dbc_signals/data_t26.dbc").read_bytes() + b"\n")
    regenerate(workspace)
    assert json.loads((workspace / "dbc_signals/source.json").read_text())["commit"] is None
