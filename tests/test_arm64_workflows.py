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

    def test_fast_build_preserves_each_version_and_updates_latest(self):
        releases = self.release_steps("build-arm64-faster.yml")

        self.assertEqual(
            releases["Publish versioned release"]["with"]["tag_name"],
            "faster-arm64-${{ github.sha }}",
        )
        self.assertEqual(
            releases["Update latest release"]["with"]["tag_name"],
            "faster-arm64",
        )


if __name__ == "__main__":
    unittest.main()
