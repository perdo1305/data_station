# ARM64 CI Build for Pi Offload

Date: 2026-07-26

## Problem

The Raspberry Pi 5 currently compiles everything locally before each run: the ROS2
colcon workspace (including `lart_msgs` rosidl codegen) and the LVGL/rclcpp
`ui_runner` C++ dashboard binary (`LART_Car_Dashboard_v1/src/ui`). Both are slow
and memory-constrained on the Pi (the Makefile already caps parallel jobs to
avoid OOM-killing `cc1plus`). We want to move this compilation off the Pi and
onto GitHub Actions, since the Pi should only need to pull a prebuilt artifact
and run it.

## Scope

In scope: producing arm64 build output (`install/` from colcon, `ui_runner` from
cmake) via GitHub Actions, publishing it as a downloadable release asset, and a
Pi-side script to fetch and install it.

Out of scope: Python runtime dependencies (`python-can`, `cantools`, `gpiod`,
Adafruit libs) — these are pip-installed directly on the Pi already and are not
a build-time bottleneck. Source files tracked in git (fonts, `dbc_signals/`,
launch configs, package sources) continue to be delivered via normal `git pull`
— only compiled/generated output is replaced by the CI artifact.

## Architecture

New workflow: `.github/workflows/build-arm64.yml`.

- `runs-on: ubuntu-24.04-arm` — GitHub's native arm64-hosted runner (free for
  public repos). This matches the Pi's target OS (Ubuntu 24.04 / ROS2 Jazzy)
  exactly, so no QEMU emulation or cross-compilation toolchain is needed.
- Trigger: `push` to `master`.

### Build steps

1. `actions/checkout` with `submodules: recursive` (needed for the LVGL
   submodule at `LART_Car_Dashboard_v1/src/ui/third_party/lvgl`).
2. Add the ROS2 apt repository for Ubuntu Noble (24.04) and install:
   `ros-jazzy-ros-base`, `ros-jazzy-rclcpp`, `ros-jazzy-rclpy`,
   `ros-jazzy-std-msgs`, `ros-jazzy-rosidl-default-generators`,
   `ros-jazzy-rosidl-default-runtime`, `python3-colcon-common-extensions`.
   This is a narrower package set than the existing amd64 Dockerfile's
   `ros-jazzy-desktop` (which pulls in GUI/sim tooling not needed to compile
   this workspace), chosen to keep CI apt-install time down.
3. Install UI build tooling: `build-essential cmake pkg-config libsdl2-dev`.
4. `colcon build` at the workspace root (**not** `--symlink-install` — a plain
   build copies real files into `install/`, making it self-contained and
   extractable on a different machine than it was built on).
5. `source install/setup.bash`, then build the dashboard UI:
   `cmake -S LART_Car_Dashboard_v1/src/ui -B LART_Car_Dashboard_v1/build/ui-build`
   followed by `cmake --build LART_Car_Dashboard_v1/build/ui-build`. This links
   against the `lart_msgs` headers/libraries produced in step 4, matching how
   `make display-local` already sequences things locally.
6. Package `install/` and `LART_Car_Dashboard_v1/build/ui-build/` into
   `lart-dashboard-arm64.tar.gz`.

### Publishing

- Use `softprops/action-gh-release` to publish the tarball to a fixed release
  tag `latest-arm64`. Each push to `master` overwrites the asset on that same
  tag, so there is always exactly one "latest known-good arm64 build" to pull
  — no per-commit release sprawl to manage.

### Pi-side pull

New script: `scripts/pull_arm64_build.sh`.

- Downloads `lart-dashboard-arm64.tar.gz` from the `latest-arm64` release via
  plain `curl` (public repo, no authentication required).
- Extracts it over the Pi's local workspace, replacing `install/` and
  `LART_Car_Dashboard_v1/build/ui-build/` with the prebuilt versions.
- Expected Pi workflow becomes: `git pull` (source files) →
  `scripts/pull_arm64_build.sh` (compiled output) → `source install/setup.bash`
  → run — with no local `colcon build` or `cmake --build` step required.

## Error handling

- If the arm64 CI build fails (compile error, apt package unavailable), the
  release asset on `latest-arm64` is simply not updated — the Pi's pull script
  continues to fetch the last known-good build rather than a broken one.
- `pull_arm64_build.sh` should fail loudly (non-zero exit) if the download or
  extraction fails, rather than silently leaving a stale/partial `install/`.

## Testing

- Verify the workflow runs end-to-end on a push to `master` and produces a
  release asset.
- Verify `scripts/pull_arm64_build.sh` on the actual Pi: fetch, extract,
  `source install/setup.bash`, and confirm `ros2 launch lart_bringup
  sim.launch.py` and the UI (`ui_runner`) both start using the CI-built
  artifacts, with no local build step invoked.
