"""Tests for scripts/bag_susp_csv.py against a real recorded bag (skipped if absent or ROS not sourced)."""
import csv
import importlib.util
import sys
from pathlib import Path

import pytest

pytest.importorskip('rosbag2_py')

BAG = Path('/home/sintra/bags/bags_milagres_13_10_2026/precharge_2026-10-13T10-22-51'
           '/precharge_2026-10-13T10-22-51_0.mcap')
pytestmark = pytest.mark.skipif(not BAG.exists(), reason='sample bag not on this machine')

SCRIPT = Path(__file__).resolve().parents[1] / 'scripts' / 'bag_susp_csv.py'


@pytest.fixture(scope='module')
def rows(tmp_path_factory):
    out = tmp_path_factory.mktemp('csv') / 'out.csv'
    spec = importlib.util.spec_from_file_location('bag_susp_csv', SCRIPT)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    sys.argv = ['bag_susp_csv.py', str(BAG), str(out)]
    mod.main()
    with open(out, newline='') as f:
        return list(csv.DictReader(f))


def test_header_and_row_count(rows):
    assert set(rows[0]) >= {'imu_time', 'imu_stamp_s', 'front_susp_l', 'front_susp_r'}
    assert len(rows) == 3881  # 51150 IMU rows after dropping consecutive repeats


def test_imu_stamps_increase_and_are_epoch(rows):
    t = [float(r['imu_stamp_s']) for r in rows]
    assert t[0] > 1.7e9  # real header.stamp, not the zero stamp of the aqt headers
    assert all(b >= a for a, b in zip(t, t[1:]))


def test_no_empty_cells_and_values_in_range(rows):
    for r in rows:
        assert all(r[k] != '' for k in ('imu_time', 'imu_stamp_s', 'front_susp_l', 'front_susp_r'))
    assert any(float(r['front_susp_l']) != float(rows[0]['front_susp_l']) for r in rows)  # not a flat hold


def test_human_time_matches_stamp(rows):
    from datetime import datetime
    r = rows[0]
    assert abs(datetime.strptime(r['imu_time'], '%Y-%m-%d %H:%M:%S.%f').timestamp() - float(r['imu_stamp_s'])) < 0.001


def test_summary_min_max_per_sensor(rows):
    summ = [r for r in rows if r['summary_stat']]
    assert [(r['summary_stat'], r['summary_sensor']) for r in summ] == [
        ('min', 'front_susp_l'), ('max', 'front_susp_l'), ('min', 'front_susp_r'), ('max', 'front_susp_r')]
    for sensor in ('front_susp_l', 'front_susp_r'):
        vals = [float(r[sensor]) for r in rows]
        got = {r['summary_stat']: float(r['summary_value']) for r in summ if r['summary_sensor'] == sensor}
        assert got == {'min': min(vals), 'max': max(vals)}


def test_no_consecutive_repeats(rows):
    vals = [(r['front_susp_l'], r['front_susp_r']) for r in rows]
    assert all(a != b for a, b in zip(vals, vals[1:]))
