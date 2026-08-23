#!/usr/bin/env python3
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch


TOOLS = Path(__file__).resolve().parents[1]
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))
SCRIPT = TOOLS / "verify_local.py"
SPEC = importlib.util.spec_from_file_location("verify_local", SCRIPT)
verify_local = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = verify_local
SPEC.loader.exec_module(verify_local)
yosupo = verify_local.yosupo


class LocalVerifyTest(unittest.TestCase):
    def write(self, root, name, content):
        path = root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("w", encoding="utf-8", newline="\n") as file:
            file.write(content)
        return path

    def prepared(self, workspace, source="int main() { return 0; }\n"):
        root = workspace / "library-checker-template-solutions"
        path = self.write(root, "sample/p/main.cpp", source)
        meta = {
            "validation": {"checks": [verify_local.COMPILE_MARKER]},
        }
        meta_path = self.write(root, "sample/p/meta.json", json.dumps(meta))
        item = yosupo.SourceItem(
            "p",
            "P",
            path,
            meta_path,
            yosupo.sha256_file(path),
        )
        return root, yosupo.prepare_sources([item])[0]

    def receipt(self, manifest, prepared):
        cases = []
        for kind in verify_local.CASE_KINDS:
            cases.append({
                "kind": kind,
                "name": f"{kind}_00",
                "source": f"{kind}.cpp",
                "index": 0,
                "seed": verify_local.RANDOM_SEED if kind == "random" else None,
                "status": "passed",
                "input_sha256": "1" * 64,
                "expected_sha256": "2" * 64,
                "actual_sha256": "3" * 64,
            })
        commit = manifest["official_repository"]["commit"]
        return {
            "schema_version": verify_local.RECEIPT_SCHEMA,
            "kind": verify_local.RECEIPT_KIND,
            "status": "passed",
            "verified_at": "2026-08-17T00:00:00Z",
            "manifest_sha256": verify_local.manifest_sha256(manifest),
            "official_repository": {"commit": commit},
            "compiler": {
                "command": "g++",
                "args": [],
                "linker_args": list(yosupo.local_linker_args()),
                "standard": "gnu++20",
                "optimization": "-O2",
                "ndebug": False,
            },
            "problems": {
                prepared.item.name: {
                    "status": "passed",
                    "verified_at": "2026-08-17T00:00:00Z",
                    "official_commit": commit,
                    "source_sha256": prepared.source_sha256,
                    "source_compile": {"status": "passed"},
                    "source_checks": [],
                    "cases": cases,
                },
            },
        }

    def test_selects_sample_boundary_and_exact_random(self):
        tests = [
            {"name": "example.in", "number": 2},
            {"name": "max_random.cpp", "number": 3},
            {"name": "small_degree.cpp", "number": 5},
            {"name": "random.cpp", "number": 3},
        ]

        cases = verify_local.select_cases(tests)

        self.assertEqual(
            [(case.kind, case.source) for case in cases],
            [
                ("sample", "example.in"),
                ("boundary", "small_degree.cpp"),
                ("random", "random.cpp"),
            ],
        )
        self.assertEqual(cases[2].seed, 0)

    def test_selects_semantic_boundary_even_when_name_contains_random(self):
        tests = [
            {"name": "example.in", "number": 1},
            {"name": "random.cpp", "number": 2},
            {"name": "abnormal_random.cpp", "number": 2},
            {"name": "max_random.cpp", "number": 2},
        ]

        cases = verify_local.select_cases(tests)

        self.assertEqual(cases[1].source, "abnormal_random.cpp")
        self.assertEqual(cases[2].source, "random.cpp")

    def test_selection_rejects_missing_boundary(self):
        tests = [
            {"name": "example.in", "number": 1},
            {"name": "random.cpp", "number": 1},
        ]
        with self.assertRaisesRegex(
            verify_local.LocalVerifyError,
            "边界测试",
        ):
            verify_local.select_cases(tests)

    def test_selection_uses_explicit_local_boundary_when_official_set_has_none(self):
        tests = [
            {"name": "example.in", "number": 1},
            {"name": "random.cpp", "number": 1},
        ]
        fixture = Path("boundary.in")
        cases = verify_local.select_cases(tests, fixture, "template/tests/boundary.in")
        self.assertEqual(cases[1].source, "template/tests/boundary.in")
        self.assertEqual(cases[1].input_path, fixture)

    def test_selection_falls_back_to_fixed_input_without_semantic_name(self):
        tests = [
            {"name": "example.in", "number": 1},
            {"name": "hand.in", "number": 1},
            {"name": "random.cpp", "number": 1},
            {"name": "max_random.cpp", "number": 1},
        ]

        cases = verify_local.select_cases(tests)

        self.assertEqual(cases[1].source, "hand.in")

    def test_selection_falls_back_to_nonrandom_generator(self):
        tests = [
            {"name": "example.in", "number": 1},
            {"name": "random.cpp", "number": 1},
            {"name": "max_random.cpp", "number": 1},
            {"name": "path.cpp", "number": 1},
        ]

        cases = verify_local.select_cases(tests)

        self.assertEqual(cases[1].source, "path.cpp")

    def test_local_linker_args_expand_all_windows_compilations(self):
        expected = (f"-Wl,--stack,{yosupo.WINDOWS_STACK_BYTES}",)
        if verify_local.os.name == "nt":
            self.assertEqual(yosupo.local_linker_args(), expected)
        else:
            self.assertEqual(yosupo.local_linker_args(), ())

    def test_official_title_must_match_manifest(self):
        verify_local.validate_official_title(
            {"name": "p", "title": "Official Title"},
            {"title": "Official Title"},
        )
        with self.assertRaisesRegex(
            verify_local.LocalVerifyError,
            "标题不一致",
        ):
            verify_local.validate_official_title(
                {"name": "p", "title": "Manifest Title"},
                {"title": "Official Title"},
            )

    def test_load_sources_wraps_submission_error(self):
        root = Path("workspace")
        manifest = {"problem_count": 1}
        with patch.object(
            yosupo,
            "load_items",
            side_effect=yosupo.SubmissionError("invalid source"),
        ):
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "invalid source",
            ):
                verify_local.load_sources(root, manifest, None)

    def test_receipt_accepts_current_source_and_rejects_later_change(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(workspace)
            manifest = {"official_repository": {"commit": "a" * 40}}
            receipt_path = workspace / "receipt.json"
            receipt_path.write_text(
                json.dumps(self.receipt(manifest, prepared)),
                encoding="utf-8",
            )

            verify_local.require_receipt(
                root,
                [prepared],
                manifest,
                receipt_path,
            )
            with prepared.item.path.open(
                "w",
                encoding="utf-8",
                newline="\n",
            ) as file:
                file.write("int main() { return 1; }\n")

            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "发生变化",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

    def test_receipt_relative_path_is_based_on_solution_root(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(workspace)
            manifest = {"official_repository": {"commit": "e" * 40}}
            receipt_path = root / ".local/custom-receipt.json"
            receipt_path.parent.mkdir(parents=True)
            receipt_path.write_text(
                json.dumps(self.receipt(manifest, prepared)),
                encoding="utf-8",
            )

            result = verify_local.require_receipt(
                root,
                [prepared],
                manifest,
                Path(".local/custom-receipt.json"),
            )
            self.assertEqual(result["status"], "passed")

    def test_receipt_rejects_missing_compiler_and_case_hash(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(workspace)
            manifest = {"official_repository": {"commit": "c" * 40}}
            receipt_path = workspace / "receipt.json"

            receipt = self.receipt(manifest, prepared)
            receipt.pop("compiler")
            receipt_path.write_text(json.dumps(receipt), encoding="utf-8")
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "编译配置",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

            receipt = self.receipt(manifest, prepared)
            receipt["problems"]["p"]["cases"][0].pop("input_sha256")
            receipt_path.write_text(json.dumps(receipt), encoding="utf-8")
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "sample 用例记录",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

    def test_receipt_rejects_duplicate_source_check(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(workspace)
            check = self.write(workspace, "check.cpp", "int main() {}\n")
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            meta["validation"]["checks"].append("check.cpp")
            prepared.item.meta_path.write_text(json.dumps(meta), encoding="utf-8")
            manifest = {"official_repository": {"commit": "d" * 40}}
            receipt = self.receipt(manifest, prepared)
            record = {
                "path": "check.cpp",
                "sha256": yosupo.sha256_file(check),
                "status": "passed",
            }
            receipt["problems"]["p"]["source_checks"] = [record, record]
            receipt_path = workspace / "receipt.json"
            receipt_path.write_text(json.dumps(receipt), encoding="utf-8")

            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "模板测试集合",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

    def test_receipt_rejects_source_hash_and_incomplete_cases(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(workspace)
            manifest = {"official_repository": {"commit": "b" * 40}}
            receipt_path = workspace / "receipt.json"
            receipt = self.receipt(manifest, prepared)
            receipt["problems"]["p"]["source_sha256"] = "0" * 64
            receipt_path.write_text(json.dumps(receipt), encoding="utf-8")
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "源码哈希",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

            receipt = self.receipt(manifest, prepared)
            receipt["problems"]["p"]["cases"].pop()
            receipt_path.write_text(json.dumps(receipt), encoding="utf-8")
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "三类用例",
            ):
                verify_local.require_receipt(
                    root,
                    [prepared],
                    manifest,
                    receipt_path,
                )

    def test_official_root_rejects_wrong_commit(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            with patch.object(
                verify_local,
                "_git_text",
                side_effect=[str(root), "c" * 40],
            ):
                with self.assertRaisesRegex(
                    verify_local.LocalVerifyError,
                    "提交不匹配",
                ):
                    verify_local.validate_official_root(root, "d" * 40)

    def test_official_problem_path_rejects_traversal(self):
        with self.assertRaisesRegex(
            verify_local.LocalVerifyError,
            "category 无效",
        ):
            verify_local.official_problem_rel({
                "name": "p",
                "category": "../outside",
            })

    def test_run_cmd_reports_nonzero_and_timeout(self):
        failed = SimpleNamespace(returncode=7, stdout=b"", stderr=b"boom")
        with patch.object(verify_local.subprocess, "run", return_value=failed):
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "(?s)退出码 7.*boom",
            ):
                verify_local.run_cmd(["tool"], "运行工具", 1)

        with patch.object(
            verify_local.subprocess,
            "run",
            side_effect=subprocess.TimeoutExpired(["tool"], 1),
        ):
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "超过 1 秒",
            ):
                verify_local.run_cmd(["tool"], "运行工具", 1)

    def test_compile_flags_keep_assertions_enabled(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = self.write(root, "main.cpp", "int main() {}\n")
            output = root / "main.exe"
            ok = verify_local.RunResult(b"", 0.1)
            with patch.object(verify_local, "run_cmd", return_value=ok) as run:
                verify_local.compile_cpp(
                    source,
                    output,
                    "g++",
                    1,
                    compiler_args=("-march=native",),
                )
            command = run.call_args.args[0]
            self.assertIn("-std=gnu++20", command)
            self.assertIn("-O2", command)
            self.assertNotIn("-DNDEBUG", command)
            self.assertEqual(
                tuple(command[-len(yosupo.local_linker_args()):])
                if yosupo.local_linker_args()
                else (),
                yosupo.local_linker_args(),
            )

            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "NDEBUG",
            ):
                verify_local.compile_cpp(
                    source,
                    output,
                    "g++",
                    1,
                    compiler_args=("-DNDEBUG",),
                )

    def test_compile_sources_uses_prepared_source_text(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, prepared = self.prepared(
                workspace,
                "int main() { return 7; }\n",
            )
            build = root / "build"
            compiled = []

            def compile_source(source, *_args, **_kwargs):
                compiled.append(source.read_text(encoding="utf-8"))
                return 0.25

            with patch.object(
                verify_local,
                "compile_cpp",
                side_effect=compile_source,
            ):
                binaries, seconds = verify_local.compile_sources(
                    [prepared],
                    build,
                    "g++",
                    (),
                    1,
                )

            self.assertEqual(compiled, [prepared.source])
            self.assertEqual(set(binaries), {"p"})
            self.assertEqual(seconds, {"p": 0.25})

    def test_default_paths_are_inside_solution_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "library-checker-template-solutions"
            private = root / ".local"
            self.assertEqual(
                verify_local.default_receipt_path(root),
                private / verify_local.RECEIPT_NAME,
            )
            self.assertEqual(
                verify_local.default_problems_path(root),
                private / "library-checker-problems",
            )

    def test_relative_custom_paths_stay_inside_solution_local(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "library-checker-template-solutions"
            self.assertEqual(
                verify_local.local_arg_path(
                    root,
                    Path(".local/custom-receipt.json"),
                    "本地预检收据",
                ),
                (root / ".local/custom-receipt.json").resolve(),
            )
            with self.assertRaisesRegex(
                verify_local.LocalVerifyError,
                "必须位于本目录 .local 下",
            ):
                verify_local.local_arg_path(
                    root,
                    Path("../.local/custom-receipt.json"),
                    "本地预检收据",
                )

    def test_absolute_custom_path_is_an_explicit_external_override(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "library-checker-template-solutions"
            external = (Path(directory) / "external/receipt.json").resolve()
            self.assertEqual(
                verify_local.local_arg_path(root, external, "本地预检收据"),
                external,
            )

    def test_subset_receipt_preserves_compatible_problem_records(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "receipt.json"
            base = {
                "schema_version": verify_local.RECEIPT_SCHEMA,
                "kind": verify_local.RECEIPT_KIND,
                "status": "passed",
                "manifest_sha256": "a" * 64,
                "official_repository": {"commit": "b" * 40},
                "compiler": {"standard": "gnu++20", "optimization": "-O2"},
            }
            old = base | {
                "problems": {
                    "a": {"version": "old-a"},
                    "b": {"version": "old-b"},
                    "removed": {"version": "old-removed"},
                },
            }
            current = base | {
                "problems": {"a": {"version": "new-a"}},
            }
            path.write_text(json.dumps(old), encoding="utf-8")

            verify_local.merge_receipt(path, current, {"a", "b"})

            self.assertEqual(
                current["problems"],
                {
                    "a": {"version": "new-a"},
                    "b": {"version": "old-b"},
                },
            )


if __name__ == "__main__":
    unittest.main()
