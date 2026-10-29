# ARM64 Build Cache Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `build-arm64.yml` faster by caching the expensive build parts, while every published `lart-dashboard-arm64.tar.gz` stays a complete, self-contained build that the Pi can swap in safely.

**Architecture:** Caches only feed the CI build; they never replace it. `lart_msgs` (the rosidl package, the slowest colcon package) is cached by submodule content and skipped on a hit; every other C/C++ object, including LVGL and the `dbc_api_sub_*` units in the dashboard, comes from a ccache directory. After the build, CI assembles the full `install/` + `ui-build/` tree, so the tarball always holds every file. A new `scripts/check_arm64_bundle.sh` checks that the bundle is complete. CI runs it before and after `tar`, and `scripts/pull_arm64_build.sh` runs it on a staging extract before touching the Pi's current build.

**Tech Stack:** GitHub Actions (`actions/cache@v4`, local composite action `./.github/actions/setup-arm64`), colcon, CMake, ccache, bash, pytest + PyYAML.

## Global Constraints

- The release asset name stays `lart-dashboard-arm64.tar.gz`, and the release tags stay `arm64-${{ github.sha }}` and `latest-arm64` (pinned by `tests/test_arm64_workflows.py`).
- The tarball layout stays `install/` + `LART_Car_Dashboard_v1/build/ui-build/` at the archive root, because `autostart_dashboard.sh` runs `build/ui-build/ui_runner` and `build/ui-build/can_bridge`.
- Only `build-arm64.yml` publishes releases (pinned by `test_only_normal_build_publishes_releases`).
- No cache step restores a whole `install` or `build` tree. A restored tree with stale or missing packages is how a partial bundle gets shipped.
- CLAUDE.md: update the workflow tests whenever you edit the workflows.
- Test runner: `/tmp/dbc-venv/bin/python -m pytest` (create it with `python3 -m venv /tmp/dbc-venv && /tmp/dbc-venv/bin/pip install -r scripts/requirements-dbc.txt`).

## Background (read first)

- The current `build-arm64.yml` caches `build install log` with `restore-keys: colcon-arm64-`, which gives little speed-up. `actions/checkout` gives every source file a fresh mtime, so make rebuilds anyway. The real cost is `lart_msgs` rosidl generation plus compiling the dashboard (`LVGL`, `dbc_api_sub_*.cpp`). That build is not cached at all today.
- The earlier attempts (the deleted `build-arm64-faster.yml` and commit `c868330`, reverted in `978e0f9`) split the build across jobs or excluded paths from caches. The tarball then lost files, so `pull_arm64_build.sh` (`rm -rf install ui-build`, then extract) left the Pi with a broken build.
- `LART_Car_Dashboard_v1/src/ui/CMakeLists.txt` uses `find_package(lart_msgs QUIET)` and only builds `can_bridge` when `lart_msgs_FOUND`. If `can_bridge` is missing from the bundle, `lart_msgs` was not visible to the dashboard build. The check script relies on this.
- `.github/actions/setup-arm64/action.yml` already installs the toolchain (including `ccache`) and outputs `cache-prefix`, a hash of all installed dpkg versions. `validate-dbc-arm64.yml` uses it. Reuse it here.

## File Structure

- Create `scripts/check_arm64_bundle.sh`: given a bundle root, exits non-zero and lists every missing required path.
- Create `tests/test_arm64_bundle.py`: tests for the check script and for the pull script's swap.
- Modify `scripts/pull_arm64_build.sh`: stage, check, then swap.
- Modify `.github/workflows/build-arm64.yml`: use the composite action, cache `lart_msgs` and ccache, check the bundle around `tar`.
- Modify `tests/test_arm64_workflows.py`: pin the new cache and check behaviour.
- Modify `README.md` (Build section): one paragraph on caching and the safe pull.

---

### Task 1: Bundle completeness check

**Files:**
- Create: `scripts/check_arm64_bundle.sh`
- Test: `tests/test_arm64_bundle.py`

**Interfaces:**
- Produces: `bash scripts/check_arm64_bundle.sh <bundle-root>`. Exits 0 when complete. Otherwise exits 1 and writes one `missing: <relative path>` line per missing path to stderr. The script holds a `PACKAGES=(...)` line, a space-separated list of colcon package names.
- Produces (test helpers in `tests/test_arm64_bundle.py`, reused by Task 2): `script_packages() -> list[str]`, `make_bundle(root: Path, skip: tuple[str, ...] = ()) -> None`, `run_check(root: Path) -> subprocess.CompletedProcess`.

- [ ] **Step 1: Write the failing tests**

Create `tests/test_arm64_bundle.py`:

```python
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
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_bundle.py`
Expected: FAIL. The file is missing, so `CHECK.read_text()` raises `FileNotFoundError`.

- [ ] **Step 3: Write the check script**

Create `scripts/check_arm64_bundle.sh`:

```bash
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
```

Then: `chmod +x scripts/check_arm64_bundle.sh`

- [ ] **Step 4: Run tests to verify they pass**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_bundle.py`
Expected: `5 passed` (or `4 passed, 1 skipped` without submodules).

- [ ] **Step 5: Check against a real local build, if you have one**

If `install/` and `LART_Car_Dashboard_v1/build/ui-build/` exist locally from a full build, run:
`bash scripts/check_arm64_bundle.sh .`
Expected: exit 0 and no output. If it reports a path that a correct build really does produce somewhere else, fix the path in the script and the test, not the build.

- [ ] **Step 6: Commit**

```bash
git add scripts/check_arm64_bundle.sh tests/test_arm64_bundle.py
git commit -m "feat(ci): add arm64 bundle completeness check"
```

---

### Task 2: Pull script stages, checks, then swaps

**Files:**
- Modify: `scripts/pull_arm64_build.sh` (whole file, 23 lines)
- Test: `tests/test_arm64_bundle.py` (append)

**Interfaces:**
- Consumes: `scripts/check_arm64_bundle.sh <bundle-root>` and the test helpers `make_bundle`, `UI_BUILD` from Task 1.
- Produces: `scripts/pull_arm64_build.sh`. Same CLI (no arguments). Exits 1 and leaves `install/` and `ui-build/` untouched when the downloaded bundle is incomplete.

- [ ] **Step 1: Write the failing tests**

Append to `tests/test_arm64_bundle.py`:

```python
import os
import shutil
import tarfile

PULL = ROOT / "scripts" / "pull_arm64_build.sh"
FAKE_CURL = """#!/usr/bin/env bash
while [ $# -gt 0 ]; do
    if [ "$1" = -o ]; then cp "$FAKE_TARBALL" "$2"; exit 0; fi
    shift
done
exit 1
"""


def run_pull(tmp_path, skip=()):
    ws = tmp_path / "ws"
    (ws / "scripts").mkdir(parents=True)
    shutil.copy(PULL, ws / "scripts")
    shutil.copy(CHECK, ws / "scripts")
    for old in (ws / "install", ws / UI_BUILD):
        old.mkdir(parents=True)
        (old / "OLD").write_text("")

    bundle = tmp_path / "bundle"
    make_bundle(bundle, skip=skip)
    tarball = tmp_path / "lart-dashboard-arm64.tar.gz"
    with tarfile.open(tarball, "w:gz") as tar:
        tar.add(bundle / "install", "install")
        tar.add(bundle / "LART_Car_Dashboard_v1", "LART_Car_Dashboard_v1")

    bin_dir = tmp_path / "bin"
    bin_dir.mkdir()
    (bin_dir / "curl").write_text(FAKE_CURL)
    (bin_dir / "curl").chmod(0o755)
    env = {**os.environ, "PATH": f"{bin_dir}:{os.environ['PATH']}",
           "FAKE_TARBALL": str(tarball)}
    result = subprocess.run(["bash", str(ws / "scripts/pull_arm64_build.sh")],
                            capture_output=True, text=True, env=env)
    return ws, result


def test_pull_replaces_build_with_complete_bundle(tmp_path):
    ws, result = run_pull(tmp_path)
    assert result.returncode == 0, result.stderr
    assert not (ws / "install/OLD").exists()
    assert not (ws / UI_BUILD / "OLD").exists()
    assert (ws / "install/lart_msgs/share/lart_msgs/package.xml").exists()
    assert os.access(ws / UI_BUILD / "ui_runner", os.X_OK)
    assert not list(ws.glob(".arm64-pull.*"))


def test_pull_keeps_old_build_when_bundle_incomplete(tmp_path):
    ws, result = run_pull(
        tmp_path, skip=("install/lart_msgs/share/lart_msgs/package.xml",)
    )
    assert result.returncode != 0
    assert (ws / "install/OLD").exists()
    assert (ws / UI_BUILD / "OLD").exists()
    assert "missing: install/lart_msgs" in result.stderr
    assert not list(ws.glob(".arm64-pull.*"))
```

Move the new `import` lines to the top of the file with the others.

- [ ] **Step 2: Run tests to verify they fail**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_bundle.py -k pull`
Expected: `test_pull_keeps_old_build_when_bundle_incomplete` FAILS on `assert result.returncode != 0`, because the current script deletes the old build and extracts the partial one.

- [ ] **Step 3: Rewrite the pull script**

Replace `scripts/pull_arm64_build.sh` with:

```bash
#!/usr/bin/env bash
set -euo pipefail

REPO="perdo1305/data_station"
TAG="latest-arm64"
ASSET="lart-dashboard-arm64.tar.gz"
WS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
UI_BUILD="LART_Car_Dashboard_v1/build/ui-build"

# Stage inside the workspace so the final mv stays on one filesystem.
TMP="$(mktemp -d "$WS_DIR/.arm64-pull.XXXXXX")"
trap 'rm -rf "$TMP"' EXIT

echo "Downloading ${ASSET} from ${REPO}@${TAG}..."
curl -fsSL "https://github.com/${REPO}/releases/download/${TAG}/${ASSET}" -o "$TMP/${ASSET}"

echo "Extracting into staging..."
mkdir "$TMP/stage"
tar xzf "$TMP/${ASSET}" -C "$TMP/stage"

if ! bash "$WS_DIR/scripts/check_arm64_bundle.sh" "$TMP/stage"; then
    echo "Downloaded build is incomplete; keeping the current build." >&2
    exit 1
fi

echo "Replacing old build output..."
rm -rf "$WS_DIR/install" "$WS_DIR/$UI_BUILD"
mkdir -p "$(dirname "$WS_DIR/$UI_BUILD")"
mv "$TMP/stage/install" "$WS_DIR/install"
mv "$TMP/stage/$UI_BUILD" "$WS_DIR/$UI_BUILD"

echo "Done. Run: source $WS_DIR/install/setup.bash"
```

- [ ] **Step 4: Run tests to verify they pass**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_bundle.py`
Expected: `7 passed` (or `6 passed, 1 skipped`).

- [ ] **Step 5: Ignore the staging dir**

If a pull is killed with SIGKILL, the trap does not run and `.arm64-pull.*` stays behind. Add it to `.gitignore`:

```bash
printf '.arm64-pull.*/\n' >> .gitignore
```

- [ ] **Step 6: Commit**

```bash
git add scripts/pull_arm64_build.sh tests/test_arm64_bundle.py .gitignore
git commit -m "fix(scripts): only replace Pi build after bundle check passes"
```

---

### Task 3: Cache lart_msgs and ccache in build-arm64.yml

**Files:**
- Modify: `.github/workflows/build-arm64.yml` (whole file)
- Modify: `tests/test_arm64_workflows.py` (add methods to `Arm64WorkflowTests`)
- Modify: `README.md` (Build section, after the paragraph ending "instead of compiling locally:")

**Interfaces:**
- Consumes: `./.github/actions/setup-arm64` (output `cache-prefix`), `scripts/check_arm64_bundle.sh` from Task 1.
- Produces: workflow step names `Cache lart_msgs`, `Cache compiler output`, `Build ROS2 workspace`, `Build dashboard UI`, `Package artifact`. The tests pin these names.

- [ ] **Step 1: Write the failing workflow tests**

Add to the `Arm64WorkflowTests` class in `tests/test_arm64_workflows.py`:

```python
    def build_steps(self):
        workflow = yaml.load(
            (WORKFLOWS / "build-arm64.yml").read_text(), Loader=yaml.BaseLoader
        )
        return {s["name"]: s for s in workflow["jobs"]["build-arm64"]["steps"]}

    def test_build_reuses_shared_toolchain_setup(self):
        uses = [s.get("uses", "") for s in self.build_steps().values()]
        self.assertIn("./.github/actions/setup-arm64", uses)

    def test_build_caches_lart_msgs_by_submodule_content(self):
        step = self.build_steps()["Cache lart_msgs"]
        self.assertIn("install/lart_msgs", step["with"]["path"].split())
        self.assertIn("hashFiles('src/lart_msgs/**')", step["with"]["key"])
        self.assertIn("steps.setup.outputs.cache-prefix", step["with"]["key"])

    def test_build_compiles_through_ccache(self):
        steps = self.build_steps()
        self.assertEqual(
            steps["Cache compiler output"]["with"]["path"].strip(), ".ccache"
        )
        for name in ("Build ROS2 workspace", "Build dashboard UI"):
            self.assertIn("-DCMAKE_CXX_COMPILER_LAUNCHER=ccache", steps[name]["run"])

    def test_no_cache_restores_whole_install_or_build_tree(self):
        # A restored stale tree is how partial bundles got published before.
        for step in self.build_steps().values():
            if step.get("uses", "").startswith("actions/cache"):
                paths = step["with"]["path"].split()
                self.assertNotIn("install", paths)
                self.assertNotIn("build", paths)

    def test_package_checks_bundle_before_and_after_tar(self):
        run = self.build_steps()["Package artifact"]["run"]
        self.assertEqual(run.count("scripts/check_arm64_bundle.sh"), 2)
        self.assertLess(run.index("check_arm64_bundle.sh ."), run.index("tar czf"))
        self.assertIn(
            "tar czf lart-dashboard-arm64.tar.gz install LART_Car_Dashboard_v1/build/ui-build",
            run,
        )
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_workflows.py`
Expected: the 2 existing tests pass. The 5 new ones fail with `KeyError: 'Cache lart_msgs'` / `AssertionError`.

- [ ] **Step 3: Rewrite the workflow**

Replace `.github/workflows/build-arm64.yml` with:

```yaml
name: Build ARM64

on:
  push:
    branches: [master]

permissions:
  contents: write

concurrency:
  group: build-arm64-normal
  cancel-in-progress: true

jobs:
  build-arm64:
    runs-on: ubuntu-24.04-arm
    env:
      CCACHE_DIR: ${{ github.workspace }}/.ccache
      CCACHE_MAXSIZE: 1G
    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          submodules: recursive

      - name: Set up ARM64 toolchain
        id: setup
        uses: ./.github/actions/setup-arm64

      # Slowest colcon package; only changes with the submodule or toolchain.
      - name: Cache lart_msgs
        id: msgs-cache
        uses: actions/cache@v4
        with:
          path: |
            build/lart_msgs
            install/lart_msgs
          key: msgs-${{ steps.setup.outputs.cache-prefix }}-${{ hashFiles('src/lart_msgs/**') }}

      # Objects only: every run still builds and links the full tree from them.
      - name: Cache compiler output
        uses: actions/cache@v4
        with:
          path: .ccache
          key: ccache-${{ steps.setup.outputs.cache-prefix }}-${{ github.sha }}
          restore-keys: |
            ccache-${{ steps.setup.outputs.cache-prefix }}-

      - name: Build ROS2 workspace
        env:
          MSGS_CACHE_HIT: ${{ steps.msgs-cache.outputs.cache-hit }}
        run: |
          source /opt/ros/jazzy/setup.bash
          skip=()
          if [[ "$MSGS_CACHE_HIT" == true && -f install/lart_msgs/share/lart_msgs/package.bash ]]; then
            echo "Reusing cached lart_msgs."
            source install/lart_msgs/share/lart_msgs/package.bash
            skip=(--packages-skip lart_msgs)
          else
            rm -rf build/lart_msgs install/lart_msgs
          fi
          ccache --zero-stats
          colcon build --parallel-workers "$(nproc)" "${skip[@]}" \
            --cmake-args -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

      - name: Build dashboard UI
        run: |
          source /opt/ros/jazzy/setup.bash
          source install/setup.bash
          cmake -S LART_Car_Dashboard_v1/src/ui -B LART_Car_Dashboard_v1/build/ui-build \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_C_COMPILER_LAUNCHER=ccache \
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
          cmake --build LART_Car_Dashboard_v1/build/ui-build --parallel "$(nproc)"
          ccache --show-stats

      - name: Package artifact
        run: |
          bash scripts/check_arm64_bundle.sh .
          tar czf lart-dashboard-arm64.tar.gz install LART_Car_Dashboard_v1/build/ui-build
          mkdir -p "$RUNNER_TEMP/bundle-check"
          tar xzf lart-dashboard-arm64.tar.gz -C "$RUNNER_TEMP/bundle-check"
          bash scripts/check_arm64_bundle.sh "$RUNNER_TEMP/bundle-check"

      - name: Get short commit SHA
        id: version
        run: echo "short_sha=${GITHUB_SHA::7}" >> "$GITHUB_OUTPUT"

      - name: Publish versioned release
        uses: softprops/action-gh-release@v2
        with:
          tag_name: arm64-${{ github.sha }}
          name: "ARM64 build ${{ steps.version.outputs.short_sha }}"
          files: lart-dashboard-arm64.tar.gz

      - name: Update latest release
        uses: softprops/action-gh-release@v2
        with:
          tag_name: latest-arm64
          name: "Latest ARM64 build"
          files: lart-dashboard-arm64.tar.gz
```

Notes for the implementer:
- `cache-hit` is `'true'` only on an exact key match. On a miss, `actions/cache@v4` saves `build/lart_msgs` and `install/lart_msgs` in its post-job step, after colcon has rebuilt them.
- The ccache key includes `github.sha`, so every run saves an updated cache. `restore-keys` picks up the newest previous one, and GitHub evicts old entries LRU.
- `.ccache` lives in the workspace but is not under `install/` or `ui-build/`, so it never ends up in the tarball.

- [ ] **Step 4: Run the workflow tests**

Run: `/tmp/dbc-venv/bin/python -m pytest -q tests/test_arm64_workflows.py tests/test_dbc_sync_workflow.py tests/test_arm64_bundle.py`
Expected: all pass.

- [ ] **Step 5: Lint the workflow if actionlint is installed**

Run: `command -v actionlint && actionlint .github/workflows/build-arm64.yml`
Expected: no output (or actionlint is not installed; then skip).

- [ ] **Step 6: Document it in README**

In `README.md`, after the line `recent build. The Pi pulls the latest release instead of compiling locally:`, insert a blank line and then:

```markdown
CI caches the `lart_msgs` install (keyed by the submodule content and toolchain)
and compiler output through ccache, but always builds and packages the complete
`install/` and `ui-build/` trees. `scripts/check_arm64_bundle.sh` verifies the
bundle before it is published, and `pull_arm64_build.sh` runs the same check on
a staging copy before replacing the Pi's current build, so an incomplete
download leaves the old build in place.
```

- [ ] **Step 7: Commit**

```bash
git add .github/workflows/build-arm64.yml tests/test_arm64_workflows.py README.md
git commit -m "ci: cache lart_msgs and ccache in arm64 build, verify bundle"
```

---

### Task 4: Verify on GitHub after merge

`build-arm64.yml` only runs on pushes to `master`, and adding `workflow_dispatch` to test it on a branch would overwrite `latest-arm64`. So this check happens after the merge.

- [ ] **Step 1: First (cold) run.** After merging, open the Actions run for the merge commit. Expected: the `Cache lart_msgs` log says `Cache not found`, `Package artifact` passes both checks, and the release is published.
- [ ] **Step 2: Second (warm) run.** Push any small commit to `master` that does not touch `src/lart_msgs`. Expected: the `Build ROS2 workspace` log contains `Reusing cached lart_msgs.`, the `ccache --show-stats` hit rate is high, and the total job time is clearly below the cold run.
- [ ] **Step 3: Pi pull.** On the Pi, run `./scripts/pull_arm64_build.sh`. Expected: `Done.` Then check that `LART_Car_Dashboard_v1/build/ui-build/ui_runner` and `can_bridge` exist and the dashboard starts.

---

## Out of scope

- `validate-dbc-arm64.yml` keeps its own colcon cache. It regenerates DBC outputs, which changes `lart_msgs` on every PR anyway. Moving it to ccache would be a separate change.
- Shrinking the tarball (shipping only the binaries instead of the whole `ui-build/` with its `.o` files). `autostart_dashboard.sh` only needs the two binaries, but changing the layout is a separate decision.
