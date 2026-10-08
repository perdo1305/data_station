"""Regeneration must preserve hand-written ROS interfaces and match DBC fields."""
from pathlib import Path
import re
import shutil
import subprocess
import sys

import cantools


def test_dashboard_message_headers_have_registered_interfaces():
    """Catch a stale lart_msgs submodule when dashboard messages change."""
    root = Path(__file__).resolve().parents[1]
    package = root / "src/lart_msgs"
    cmake = (package / "CMakeLists.txt").read_text()
    registered_headers = set()
    for relative_path in re.findall(r'"((?:dbc_msgs|msg)/[^"\n]+\.msg)"', cmake):
        interface = package / relative_path
        assert interface.is_file(), f"Registered interface is missing: {relative_path}"
        name = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', interface.stem)
        registered_headers.add(re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', name).lower())

    ui = root / "LART_Car_Dashboard_v1/src/ui"
    consumers = [ui / "generated/can_bridge_impl.hpp", *ui.glob("dbc_api_sub_*.cpp")]
    required_headers = set()
    for consumer in consumers:
        required_headers.update(re.findall(r'<lart_msgs/msg/([^>]+)\.hpp>', consumer.read_text()))
    assert required_headers, "No dashboard message includes found"
    missing = required_headers - registered_headers
    assert not missing, f"Dashboard headers have no registered ROS interface: {sorted(missing)}"


def test_committed_aqt7_interface_matches_data_dbc():
    root = Path(__file__).resolve().parents[1]
    message = cantools.database.load_file(root / "dbc_signals/data_t26.dbc").get_message_by_name("AQT7")
    interface = root / "src/lart_msgs/dbc_msgs/Aqt7.msg"
    fields = {parts[1] for line in interface.read_text().splitlines()
              if len(parts := line.split()) >= 2}
    missing = {signal.name.lower() for signal in message.signals} - fields
    assert not missing, f"Aqt7.msg is missing DBC fields: {sorted(missing)}"


def test_api_generation_preserves_ros_package(tmp_path):
    root = Path(__file__).resolve().parents[1]
    ui = tmp_path / "LART_Car_Dashboard_v1/src/ui"
    ui.mkdir(parents=True)
    shutil.copy(root / "LART_Car_Dashboard_v1/src/ui/generate_dbc_api.py", ui)
    shutil.copytree(root / "src/lart_msgs", tmp_path / "src/lart_msgs")
    dbc_dir = tmp_path / "dbc_signals"
    dbc_dir.mkdir()
    # Small valid input isolates generator behavior from the incoming DBC layout.
    dbc = 'VERSION ""\nNS_ :\nBS_:\nBU_: Board\nBO_ 1904 AQT7: 4 Board\n SG_ SUSP_L : 0|16@1- (0.1,0) [0|0] "mm" Board\n SG_ SUSP_R : 16|16@1- (0.1,0) [0|0] "mm" Board\n'
    for name in ("data_t26", "powertrain_t26", "autonomous_t26"):
        (dbc_dir / f"{name}.dbc").write_text(dbc)
        cantools.database.load_file(dbc_dir / f"{name}.dbc")
    cmake = tmp_path / "src/lart_msgs/CMakeLists.txt"
    original = cmake.read_text()
    interface = tmp_path / "src/lart_msgs/dbc_msgs/Aqt7.msg"
    interface.write_text("std_msgs/Header header\n\nint16 susp_l\nfloat32 removed_signal\nint16 LIMIT = 100\n")
    subprocess.run([sys.executable, str(ui / "generate_dbc_api.py")], check=True)
    result = cmake.read_text()
    assert '"msg/ASStatus.msg"' in result
    assert '"srv/Heartbeat.srv"' in result
    assert 'find_package(geometry_msgs REQUIRED)' in result
    assert '"dbc_msgs/Aqt7.msg"' in result
    assert '"msg/Aqt7.msg"' not in result
    assert result.split('rosidl_generate_interfaces')[0] == original.split('rosidl_generate_interfaces')[0]
    assert (tmp_path / "src/lart_msgs/dbc_msgs/Aqt7.msg").read_text() == 'std_msgs/Header header\n\nint16 susp_l\nint16 LIMIT = 100\nfloat32 susp_r\n'
    subprocess.run([sys.executable, str(ui / "generate_dbc_api.py")], check=True)
    assert cmake.read_text() == result
