# Foxglove Bag Auto-Upload — Design

**Date:** 2026-07-30
**Status:** Approved

## Purpose

Automatically push completed rosbag2 sessions (recorded by `bag_recorder`,
[docs/superpowers/specs/2026-07-06-precharge-bag-recorder-design.md](2026-07-06-precharge-bag-recorder-design.md))
to Foxglove's Data Platform whenever the Pi has internet access, with no
manual step and no errors when offline or mid-recording.

## Approach

Standalone script + systemd timer, decoupled from ROS2. No long-lived node —
a `Type=oneshot` service run every 5 minutes by a systemd timer. Runs as
plain `python3` (PyYAML ships with the ROS2 install already on the Pi;
`requests` is a new dependency).

## Components

- `deploy/.env.example` — git-tracked template: `FOXGLOVE_API_KEY=`
- `deploy/.env` — real key, **gitignored**
- `deploy/foxglove_upload.py` — uploader script
- `deploy/foxglove-upload.service` — oneshot unit invoking the script
- `deploy/foxglove-upload.timer` — `OnUnitActiveSec=5min`, triggers the service

## Data Flow (per run)

1. Load `FOXGLOVE_API_KEY` from `deploy/.env`. Missing/empty → log, exit 0.
2. Connectivity check: TCP connect to `api.foxglove.dev:443`, 5 s timeout.
   Checks the actual upload target is reachable, not generic internet.
   Fail → log, exit 0.
3. Read `bag_recorder.bag_dir` from
   `src/lart_bringup/config/rpi_config.yaml` (path resolved relative to the
   script's own location) — single source of truth shared with the recorder,
   no separate `BAG_DIR` env var to drift out of sync.
4. Scan `bag_dir` for session folders that have `metadata.yaml` (finalized —
   `bag_recorder` only writes this on clean shutdown, so mid-recording or
   unrepaired sessions are skipped) and no `.uploaded` marker.
5. For each pending session, upload its `.mcap` file to Foxglove as a
   Recording import. Exact request/response shape is verified against
   current Foxglove Data Platform API docs at implementation time (not
   assumed from memory — API surfaces change).
6. On a successful (2xx) response: write `.uploaded` marker in the session
   folder immediately, before any further work, to minimize the window
   where a real server-side success could go unmarked and get re-uploaded
   on the next tick.
7. On failure: log the error, leave the session unmarked for retry on the
   next run.

## Error Handling

- No API key configured yet: log and skip, not a crash — lets the timer run
  unattended before the Pi has been provisioned with a key.
- Offline / Foxglove unreachable: expected, frequent state — log at info
  level, exit 0, no error.
- Upload HTTP failure (4xx/5xx/timeout): log at error level with status/
  detail, leave session unmarked, retried automatically next tick.
- Concurrent runs: not handled explicitly — relies on systemd's default
  behavior of not starting a new instance of an already-active oneshot
  service, so overlapping uploads can't happen even if one run takes longer
  than 5 minutes.
- Script never exits nonzero for any of the above — only for a genuine
  local bug — so systemd doesn't accumulate failed-unit spam for expected
  transient states.

## Testing

- Manual: run the script once against a completed test session + valid key,
  verify the upload lands in Foxglove and `.uploaded` is written.
- Manual: block network (or point at an unreachable host) and verify a
  clean skip with no exception/nonzero exit.
- Manual: rerun against an already-`.uploaded` session, verify it's skipped
  and not re-uploaded.
