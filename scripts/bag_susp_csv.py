#!/usr/bin/env python3
"""Bag -> CSV: suspension travel sampled on the IMU clock.

One row per /imu/data message whose suspension values changed (repeats are dropped, so a value holds until the next row), timestamped with the IMU's own header.stamp.
Suspension values (susp_* fields of /pwt/aqt4 = front and /data/aqt7 = rear) are held from
the latest sample at or before that IMU time (zero-order hold).

Columns: imu_time (local, human readable), imu_stamp_s (epoch seconds), one per sensor.
Columns to the right (summary_*) hold min/max per sensor, with the sensor and when it happened.

Usage (ROS sourced): bag_susp_csv.py <bag_dir> [out.csv]
"""
import csv
import sys
from datetime import datetime
from bisect import bisect_right

import rosbag2_py
from rclpy.serialization import deserialize_message
from rosidl_runtime_py.utilities import get_message

IMU_TOPIC = '/imu/data'
SUSP_TOPICS = {'/pwt/aqt4': 'front', '/data/aqt7': 'rear'}  # board 4 has steering angle, board 7 rear brake pressure


def human(t):
    return datetime.fromtimestamp(t).strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]  # local time, ms


def read(bag):
    reader = rosbag2_py.SequentialReader()
    reader.open(rosbag2_py.StorageOptions(uri=bag), rosbag2_py.ConverterOptions('cdr', 'cdr'))
    types = {t.name: get_message(t.type) for t in reader.get_all_topics_and_types()}
    imu = []   # IMU header stamps [s]
    susp = {}  # column -> ([bag log time s], [value]); aqt headers are zero, so log time is the clock
    while reader.has_next():
        name, data, log_ns = reader.read_next()
        if name == IMU_TOPIC:
            s = deserialize_message(data, types[name]).header.stamp
            imu.append(s.sec + s.nanosec * 1e-9)
        elif name in SUSP_TOPICS:
            msg = deserialize_message(data, types[name])
            for field in msg.get_fields_and_field_types():
                if field.startswith('susp'):
                    col = f'{SUSP_TOPICS[name]}_{field}'
                    ts, vs = susp.setdefault(col, ([], []))
                    ts.append(log_ns * 1e-9)
                    vs.append(getattr(msg, field))
    return imu, susp


def main():
    bag = sys.argv[1]
    out = sys.argv[2] if len(sys.argv) > 2 else bag.rstrip('/') + '_susp.csv'
    imu, susp = read(bag)
    if not imu or not susp:
        sys.exit(f'need {IMU_TOPIC} and a susp_* field in {SUSP_TOPICS}: imu={len(imu)} susp={list(susp)}')
    cols = sorted(susp)
    table = []  # (imu time, values)
    for t in imu:
        row = []
        for c in cols:
            ts, vs = susp[c]
            i = bisect_right(ts, t) - 1
            row.append(vs[i] if i >= 0 else None)
        if None in row:  # skip until every sensor has a sample
            continue
        if not table or row != table[-1][1]:  # drop repeats: a value holds until the next row
            table.append((t, row))
    summary = []  # (stat, sensor, value, time)
    for j, c in enumerate(cols):
        lo = min(table, key=lambda r: r[1][j])
        hi = max(table, key=lambda r: r[1][j])
        summary += [('min', c, lo[1][j], human(lo[0])), ('max', c, hi[1][j], human(hi[0]))]
    with open(out, 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['imu_time', 'imu_stamp_s'] + cols + ['', 'summary_stat', 'summary_sensor', 'summary_value', 'summary_imu_time'])
        for k, (t, row) in enumerate(table):
            extra = ['', *summary[k]] if k < len(summary) else []
            w.writerow([human(t), f'{t:.6f}'] + row + extra)
        for srow in summary[len(table):]:  # fewer data rows than summary rows
            w.writerow([''] * (2 + len(cols)) + ['', *srow])
    n = len(table)
    print(f'{n} rows -> {out}')


if __name__ == '__main__':
    main()
