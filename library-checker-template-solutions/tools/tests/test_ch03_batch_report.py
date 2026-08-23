import copy
import importlib.util
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parents[1]
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))
SPEC = importlib.util.spec_from_file_location(
    "ch03_batch_report",
    TOOLS / "ch03_batch_report.py",
)
report = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = report
SPEC.loader.exec_module(report)


class PerformanceGateTest(unittest.TestCase):
    def performance(self):
        output = "a" * 64

        def side(order, path, source_sha256, seconds, peak):
            return {
                "path": path,
                "sha256": source_sha256,
                "runs": [
                    {
                        "seconds": value,
                        "peak_working_set_bytes": peak,
                        "output_sha256": output,
                        "order": order,
                    }
                    for value in seconds
                ],
                "median_seconds": sorted(seconds)[len(seconds) // 2],
                "peak_working_set_bytes": peak,
            }

        return {
            "schema_version": 1,
            "kind": "template_performance_review",
            "problem_id": "data_structure/p",
            "official_repository_commit": "b" * 40,
            "input": {
                "generator": "gen/max_random.cpp",
                "generator_sha256": "c" * 64,
                "seed": 0,
                "sha256": "d" * 64,
                "bytes": 1,
            },
            "environment": {
                "platform": sys.platform,
                "compiler": "test compiler",
                "compile_flags": [
                    *report.verify_local.COMPILE_FLAGS,
                    *report.yosupo.local_linker_args(),
                ],
                "measurement": "wall_clock_seconds; Windows PeakWorkingSetSize sampled every 2ms",
            },
            "candidate": side("candidate", "data_structure/p/main.cpp", "e" * 64, [1.0] * 5, 10),
            "reference": side("reference", "reference/main.cpp", "f" * 64, [1.0] * 5, 11),
            "threshold": 1.25,
            "candidate_over_reference_median": 1.0,
            "status": "passed",
        }

    def validate(self, value):
        report.validate_performance(
            value,
            "p",
            "data_structure/p",
            "b" * 40,
            "data_structure/p/main.cpp",
            "e" * 64,
            "reference/main.cpp",
            "f" * 64,
        )

    def test_accepts_consistent_five_round_evidence(self):
        self.validate(self.performance())

    def test_rejects_forged_runs_or_ratio(self):
        malformed = copy.deepcopy(self.performance())
        malformed["candidate"]["runs"] = ["not a run"] * 5
        with self.assertRaisesRegex(report.BatchError, "计时记录不是对象"):
            self.validate(malformed)

        ratio = copy.deepcopy(self.performance())
        ratio["candidate_over_reference_median"] = 0.5
        with self.assertRaisesRegex(report.BatchError, "倍率与媒值不一致"):
            self.validate(ratio)

    def test_rejects_relaxed_threshold(self):
        value = self.performance()
        value["threshold"] = 1.26
        with self.assertRaisesRegex(report.BatchError, "阈值无效或放宽"):
            self.validate(value)


if __name__ == "__main__":
    unittest.main()
