#!/usr/bin/env python3
"""Bag -> CSV: suspension travel sampled on the IMU clock.

One row per /imu/data message whose suspension values changed (repeats are dropped, so a value holds until the next row), timestamped with the IMU's own header.stamp.
Suspension values (susp_* fields of /pwt/aqt4 = front and /data/aqt7 = rear) are held from
the latest sample at or before that IMU time (zero-order hold).

Columns: imu_time (local, human readable), imu_stamp_s (epoch seconds), one per sensor.
Columns to the right (summary_*) hold min/max per sensor, with the sensor and when it happened.

Writes <out_dir>/<bag name>/{<bag name>_susp.csv, <bag name>_tuning.png} and prints the key numbers.
Usage: bag_susp_csv.py <bag_dir|file.mcap> [out_dir=.]   (ROS is sourced automatically)
"""
import csv
import glob
import os
import sys
from datetime import datetime
from bisect import bisect_right

def _source_ros():
    """Re-exec under bash with ROS + this workspace sourced, so it works from any shell."""
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    cmd = (f'source /opt/ros/jazzy/setup.bash; source "{root}/install/setup.bash"; '
           'exec python3 "$0" "$@"')
    os.execvpe('bash', ['bash', '-c', cmd, os.path.abspath(__file__), *sys.argv[1:]],
               {**os.environ, 'BAG_SUSP_SOURCED': '1'})


try:
    import numpy as np
    import rosbag2_py
    from rclpy.serialization import deserialize_message
    from rosidl_runtime_py.utilities import get_message
    import lart_msgs  # noqa: F401  # probe: re-exec if workspace not sourced
except ImportError:
    if os.environ.get('BAG_SUSP_SOURCED'):
        raise
    _source_ros()

IMU_TOPIC = '/imu/data'
SUSP_TOPICS = {'/pwt/aqt4': 'front', '/data/aqt7': 'rear'}  # board 4 has steering angle, board 7 rear brake pressure


def human(t):
    return datetime.fromtimestamp(t).strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]  # local time, ms


def readers(bag):
    """Open the bag. A recording cut off by a power loss leaves an empty metadata.yaml and rosbag2 refuses the
    folder, so then read its .mcap files directly (the bag folder itself is never modified)."""
    conv = rosbag2_py.ConverterOptions('cdr', 'cdr')
    reader = rosbag2_py.SequentialReader()
    try:
        reader.open(rosbag2_py.StorageOptions(uri=bag), conv)
        yield reader
        return
    except RuntimeError as e:
        files = sorted(glob.glob(os.path.join(bag, '*.mcap'))) if os.path.isdir(bag) else []
        if not files:
            raise
        print(f'{bag}: metadata unreadable ({e}); reading {len(files)} .mcap file(s) directly')
    for f in files:
        reader = rosbag2_py.SequentialReader()
        reader.open(rosbag2_py.StorageOptions(uri=f), conv)
        yield reader


def read(bag):
    imu = []   # IMU header stamps [s]
    acc = []   # (stamp, ax, ay) [m/s^2]
    steer = ([], [])  # (bag log time s, steering angle) from the front board
    susp = {}  # column -> ([bag log time s], [value]); aqt headers are zero, so log time is the clock
    for reader in readers(bag):
        types = {t.name: get_message(t.type) for t in reader.get_all_topics_and_types()}
        while reader.has_next():
            name, data, log_ns = reader.read_next()
            if name == IMU_TOPIC:
                s = deserialize_message(data, types[name]).header.stamp
                imu.append(s.sec + s.nanosec * 1e-9)
                a = deserialize_message(data, types[name]).linear_acceleration
                acc.append((imu[-1], a.x, a.y))
            elif name in SUSP_TOPICS:
                msg = deserialize_message(data, types[name])
                if hasattr(msg, 'st_angle'):
                    steer[0].append(log_ns * 1e-9)
                    steer[1].append(msg.st_angle)
                for field in msg.get_fields_and_field_types():
                    if field.startswith('susp'):
                        col = f'{SUSP_TOPICS[name]}_{field}'
                        ts, vs = susp.setdefault(col, ([], []))
                        ts.append(log_ns * 1e-9)
                        vs.append(getattr(msg, field))
    return imu, susp, np.array(acc), steer


def plot_tuning(susp, acc, steer, png):
    """Tuning plots from the raw samples (repeats kept). Travel is in the sensor's unit (mm assumed)."""
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    cols = sorted(susp)
    t0 = min(susp[c][0][0] for c in cols)
    # common 20 Hz grid so sensors, steering and IMU line up; zero-order hold like the CSV
    t = np.arange(max(susp[c][0][0] for c in cols), min(susp[c][0][-1] for c in cols), 0.05)
    x = {c: np.array(susp[c][1])[np.clip(np.searchsorted(susp[c][0], t, 'right') - 1, 0, None)] for c in cols}
    x = {c: v - np.median(v) for c, v in x.items()}  # travel relative to the median ride height
    rate = len(susp[cols[0]][0]) / (susp[cols[0]][0][-1] - susp[cols[0]][0][0])
    acc = acc[np.any(acc[:, 1:] != 0, axis=1)]  # IMU publishes exact 0,0,0 when it has no fix
    ax_, ay_ = (np.interp(t, acc[:, 0], acc[:, i]) for i in (1, 2))
    ax_, ay_ = ax_ - np.median(ax_), ay_ - np.median(ay_)  # drop mounting offset / gravity component
    st = np.interp(t, *steer) if steer[0] else None
    v = {c: np.gradient(x[c], t) for c in cols}
    fig, a = plt.subplots(2, 3, figsize=(18, 9))
    fig.suptitle(f'Suspension tuning ({png.rsplit("/", 1)[-1]}) - sensor rate ~{rate:.1f} Hz: velocities/PSD are a lower bound', fontsize=11)
    for c in cols:
        a[0, 0].plot(t - t0, x[c], lw=.7, label=c)
        a[0, 1].hist(x[c], 60, alpha=.5, label=c)
        a[0, 2].hist(v[c], 80, alpha=.5, label=c, log=True)
        f = np.fft.rfftfreq(len(t), .05)
        a[1, 2].semilogy(f[1:], np.abs(np.fft.rfft((x[c] - x[c].mean()) * np.hanning(len(t))))[1:] ** 2, lw=.8, label=c)
    if st is not None:
        s2 = a[0, 0].twinx(); s2.plot(t - t0, st, color='gray', lw=.4, alpha=.5); s2.set_ylabel('steering angle')
    a[0, 0].set(title='Travel vs time (rel. median)', xlabel='s', ylabel='travel'); a[0, 0].legend()
    a[0, 1].set(title='Travel distribution (bump +/rebound -; check against bump stop)', xlabel='travel'); a[0, 1].legend()
    a[0, 2].set(title='Damper velocity distribution (damper curve usage)', xlabel='travel/s'); a[0, 2].legend()
    pairs = [c for c in cols if c.endswith('_l')]
    if pairs and pairs[0][:-2] + '_r' in x:  # roll: (L-R) against lateral acceleration
        roll = x[pairs[0]] - x[pairs[0][:-2] + '_r']
        a[1, 0].scatter(ay_, roll, s=2, alpha=.3)
        k = np.polyfit(ay_, roll, 1); g = np.linspace(ay_.min(), ay_.max(), 2)
        a[1, 0].plot(g, np.polyval(k, g), 'r', label=f'{k[0]:.2f} travel per m/s^2')
        a[1, 0].set(title='Roll: (L-R) vs lateral accel (slope = roll gain, lower = stiffer)', xlabel='IMU ay [m/s^2]', ylabel='L-R travel'); a[1, 0].legend()
        print(f'roll gain {k[0]:.3f} travel per m/s^2 (corr {np.corrcoef(ay_, roll)[0, 1]:.2f})')
    heave = np.mean([x[c] for c in cols], axis=0)
    a[1, 1].scatter(ax_, heave, s=2, alpha=.3)
    k = np.polyfit(ax_, heave, 1); g = np.linspace(ax_.min(), ax_.max(), 2)
    a[1, 1].plot(g, np.polyval(k, g), 'r', label=f'{k[0]:.2f} travel per m/s^2')
    a[1, 1].set(title='Pitch: mean travel vs longitudinal accel (dive/squat)', xlabel='IMU ax [m/s^2]', ylabel='mean travel'); a[1, 1].legend()
    a[1, 2].set(title='Travel PSD (limit: ~2.6 Hz real, 5 Hz sensor)', xlabel='Hz', ylabel='power'); a[1, 2].legend()
    print(f'pitch gain {k[0]:.3f} travel per m/s^2 (corr {np.corrcoef(ax_, heave)[0, 1]:.2f})')
    for c in cols:
        p5, p95 = np.percentile(x[c], [5, 95])
        print(f'{c}: range {x[c].min():.1f}..{x[c].max():.1f}, p5/p95 {p5:.1f}/{p95:.1f}, '
              f'|v| p95 {np.percentile(abs(v[c]), 95):.1f}/s, bump vel p99 {np.percentile(v[c], 99):.1f}, rebound vel p1 {np.percentile(v[c], 1):.1f}')
    fig.tight_layout(); fig.savefig(png, dpi=110)
    print(f'plots -> {png}')


def compression_strokes(ts, vs, min_travel=2.0):
    """Compression strokes (trough -> next peak, value rising) as (duration ms, travel). Repeats are dropped first.
    shortcut: duration resolution is the sensor period (~200 ms at 5 Hz), so values are quantised."""
    t, v = [ts[0]], [vs[0]]
    for a, b in zip(ts[1:], vs[1:]):
        if b != v[-1]:
            t.append(a); v.append(b)
    out, i = [], 0
    while i < len(v) - 1:
        if v[i + 1] <= v[i]:
            i += 1; continue
        j = i
        while j + 1 < len(v) and v[j + 1] > v[j]:
            j += 1
        if v[j] - v[i] >= min_travel:
            out.append(((t[j] - t[i]) * 1e3, v[j] - v[i]))
        i = j
    return out


def plot_compression(susp, png, max_ms=2000):
    """One panel per sensor: stroke (x, 0 centred: compression +, rebound -) vs stroke time (y), strokes <= max_ms."""
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    cols = sorted(susp)
    fig, a = plt.subplots(1, len(cols), figsize=(4.5 * len(cols), 5), sharex=True, sharey=True, squeeze=False)
    for ax, c in zip(a[0], cols):
        ts, vs = susp[c]
        reb = [(d, -x) for d, x in compression_strokes(ts, [-v for v in vs])]  # falling runs, negative travel
        s = np.array([(x, d) for d, x in compression_strokes(ts, vs) + reb if d <= max_ms])
        if len(s):
            ax.scatter(s[:, 0], s[:, 1], s=8, alpha=.4)
        ax.axvline(0, color='k', lw=.8)
        ax.set(title=f'{c} ({len(s)} strokes)', xlabel='stroke [mm]  (- rebound | + compression)')
    lim = max(abs(x) for x in a[0, 0].get_xlim())
    a[0, 0].set_xlim(-lim, lim)
    a[0, 0].set_ylabel('stroke time [ms]')
    fig.tight_layout(); fig.savefig(png, dpi=110)
    print(f'plots -> {png}')


def main():
    bag = sys.argv[1]
    name = os.path.basename(os.path.dirname(os.path.abspath(bag)) if bag.endswith('.mcap') else os.path.abspath(bag))
    folder = os.path.join(sys.argv[2] if len(sys.argv) > 2 else '.', name)
    os.makedirs(folder, exist_ok=True)
    out = os.path.join(folder, f'{name}_susp.csv')
    imu, susp, acc, steer = read(bag)
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
    plot_tuning(susp, acc, steer, os.path.join(folder, f'{name}_tuning.png'))
    plot_compression(susp, os.path.join(folder, f'{name}_compression.png'))


if __name__ == '__main__':
    main()
