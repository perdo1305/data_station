import unittest
from pathlib import Path

import yaml


ROOT = Path(__file__).resolve().parents[1]
WORKFLOWS = ROOT / ".github" / "workflows"


class Arm64WorkflowTests(unittest.TestCase):
    def release_steps(self, filename):
        workflow = yaml.load(
            (WORKFLOWS / filename).read_text(), Loader=yaml.BaseLoader
        )
        jobs = workflow["jobs"].values()
        steps = [step for job in jobs for step in job["steps"]]
        return {
            step["name"]: step
            for step in steps
            if step.get("uses", "").startswith("softprops/action-gh-release@")
        }

    def test_normal_build_preserves_each_version_and_updates_latest(self):
        releases = self.release_steps("build-arm64.yml")

        self.assertEqual(
            releases["Publish versioned release"]["with"]["tag_name"],
            "arm64-${{ github.sha }}",
        )
        self.assertEqual(
            releases["Update latest release"]["with"]["tag_name"],
            "latest-arm64",
        )

    def test_only_normal_build_publishes_releases(self):
        # The faster build workflow was deleted; DBC validation/sync must not
        # introduce another publisher while the normal workflow owns releases.
        publishers = sorted(path.name for path in WORKFLOWS.glob('*.yml')
                            if self.release_steps(path.name))
        self.assertEqual(publishers, ['build-arm64.yml'])

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
        self.assertEqual(step["with"]["path"].split(), ["install/lart_msgs"])
        self.assertIn("hashFiles('src/lart_msgs/**')", step["with"]["key"])
        self.assertIn("steps.setup.outputs.cache-prefix", step["with"]["key"])

    def test_build_compiles_through_ccache(self):
        steps = self.build_steps()
        self.assertEqual(
            steps["Cache compiler output"]["with"]["path"].strip(), ".ccache"
        )
        keys = steps["Cache compiler output"]["with"]["restore-keys"].splitlines()
        self.assertIn("ccache-${{ steps.setup.outputs.cache-prefix }}-", keys)
        self.assertIn("ccache-", keys)
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


if __name__ == "__main__":
    unittest.main()
