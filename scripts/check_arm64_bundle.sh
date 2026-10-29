#!/usr/bin/env bash
# Fail if a CI bundle root is missing anything the Pi needs to run.
set -euo pipefail

ROOT="${1:?usage: check_arm64_bundle.sh <bundle-root>}"
# Keep in sync with src/*/package.xml (tests/test_arm64_bundle.py checks).
PACKAGES=(input_handler lart_bringup lart_msgs led_controller sim)
UI_BUILD="LART_Car_Dashboard_v1/build/ui-build"

missing=0
need() { [ -e "$ROOT/$1" ] || { echo "missing: $1" >&2; missing=1; }; }
need_exec() { [ -x "$ROOT/$1" ] || { echo "missing: $1" >&2; missing=1; }; }

need install/setup.bash
for pkg in "${PACKAGES[@]}"; do
    need "install/$pkg/share/$pkg/package.xml"
done
need_exec install/led_controller/lib/led_controller/led_controller
need_exec "$UI_BUILD/ui_runner"
# can_bridge is only built when the dashboard CMake found lart_msgs.
need_exec "$UI_BUILD/can_bridge"

exit "$missing"
