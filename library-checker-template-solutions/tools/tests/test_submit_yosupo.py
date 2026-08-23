#!/usr/bin/env python3
import importlib.util
import io
import json
import base64
import shutil
import sys
import tempfile
import time
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "submit_yosupo.py"
SPEC = importlib.util.spec_from_file_location("submit_yosupo", SCRIPT)
submit_yosupo = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = submit_yosupo
SPEC.loader.exec_module(submit_yosupo)


class FakeResponse(io.BytesIO):
    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()


class YosupoSubmitTest(unittest.TestCase):
    def write(self, root, name, content, encoding="utf-8"):
        path = root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("w", encoding=encoding, newline="\n") as file:
            file.write(content)
        return path

    def make_prepared(
        self,
        root,
        source="int main() { return 0; }\n",
        category="",
    ):
        path = self.write(root, "main.cpp", source)
        meta_path = self.write(
            root,
            "meta.json",
            json.dumps({
                "schema_version": 3,
                "name": "aplusb",
                "validation": {"status": "local_only"},
                "submission_attempts": [],
            }),
        )
        item = submit_yosupo.SourceItem(
            "aplusb",
            "A + B",
            path,
            meta_path,
            submit_yosupo.sha256_file(path),
            category,
        )
        return submit_yosupo.prepare_sources([item])[0]

    def make_solution(self, workspace, name="aplusb", source=None):
        root = workspace / "library-checker-template-solutions"
        source = source or "#include <bits/stdc++.h>\nint main() { return 0; }\n"
        source_name = f"sample/{name}/main.cpp"
        meta_name = f"sample/{name}/meta.json"
        path = self.write(root, source_name, source)
        source_sha256 = submit_yosupo.sha256_file(path)
        meta = {
            "schema_version": 3,
            "name": name,
            "title": "A + B",
            "category": "sample",
            "provenance": {
                "kind": "template_solution",
                "source": source_name,
                "source_sha256": source_sha256,
            },
            "validation": {"status": "local_only"},
            "submission_attempts": [],
        }
        self.write(root, meta_name, json.dumps(meta))
        manifest = {
            "schema_version": 3,
            "problem_count": 1,
            "counts": {"template_solution": 1},
            "problems": [{
                "name": name,
                "title": "A + B",
                "category": "sample",
                "source": source_name,
                "source_sha256": source_sha256,
                "meta": meta_name,
                "state": "template_solution",
            }],
        }
        return root, manifest, path

    def detail(
        self,
        prepared,
        submission_id,
        status,
        source=None,
        lang="cpp20",
        problem="aplusb",
        title="A + B",
        is_latest=True,
    ):
        return {
            "overview": {
                "id": submission_id,
                "problem_name": problem,
                "problem_title": title,
                "lang": lang,
                "is_latest": is_latest,
                "status": status,
                "time": 0.125,
                "memory": 123456,
                "submission_time": "2026-08-16T00:00:00Z",
            },
            "source": prepared.source if source is None else source,
        }

    def final_attempt(self, prepared, submission_id=42, verdict="AC"):
        return {
            "submission_id": submission_id,
            "source_sha256": prepared.source_sha256,
            "server_source_sha256": prepared.source_sha256,
            "lang": "cpp20",
            "verdict": verdict,
            "last_status": verdict,
            "time": 0.125,
            "memory": 123456,
            "submission_time": "2026-08-16T00:00:00Z",
            "is_latest": True,
            "compile_standard": "gnu++20",
            "submitted_at": "2026-08-16T00:00:00Z",
            "recorded_at": "2026-08-16T00:00:01Z",
        }

    def test_source_is_read_verbatim_with_one_hash(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = "#include <vector>\nconst char *text = \"中文\";\nint main() {}\n"
            path = self.write(root, "main.cpp", source)
            prepared = self.make_prepared(root, source)

            self.assertEqual(prepared.source, source)
            self.assertEqual(prepared.source_sha256, submit_yosupo.sha256_file(path))
            self.assertEqual(prepared.source_sha256, prepared.item.source_sha256)

    def test_source_rejects_utf8_bom(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "main.cpp"
            path.write_bytes(b"\xef\xbb\xbfint main() {}\n")
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "BOM"):
                submit_yosupo.read_submission_source(path)

    def test_source_rejects_crlf(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "main.cpp"
            path.write_bytes(b"int main() {}\r\n")
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "CRLF"):
                submit_yosupo.read_submission_source(path)

    def test_source_rejects_any_quoted_include(self):
        cases = [
            '#include "local.hpp"\n',
            ' # include /* comment */ "local.hpp"\n',
            '\t#include\t"../local.hpp"\n',
            '#include \\\n"continued.hpp"\n',
        ]
        for index, source in enumerate(cases):
            with self.subTest(index=index), tempfile.TemporaryDirectory() as directory:
                path = self.write(Path(directory), "main.cpp", source)
                with self.assertRaisesRegex(
                    submit_yosupo.SubmissionError,
                    "本地 include",
                ):
                    submit_yosupo.read_submission_source(path)

    def test_source_rejects_invalid_utf8(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "main.cpp"
            path.write_bytes(b"\xff")
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "有效 UTF-8"):
                submit_yosupo.read_submission_source(path)

    def test_source_limit_is_checked(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            path = self.write(root, "main.cpp", "x" * (submit_yosupo.SOURCE_LIMIT + 1))
            meta = self.write(root, "meta.json", "{}\n")
            item = submit_yosupo.SourceItem(
                "large",
                "Large",
                path,
                meta,
                submit_yosupo.sha256_file(path),
            )
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "1 MiB"):
                submit_yosupo.prepare_sources([item])

    def test_loads_only_schema3_template_solution(self):
        with tempfile.TemporaryDirectory() as directory:
            root, manifest, path = self.make_solution(Path(directory))

            items = submit_yosupo.load_items(
                root,
                manifest,
                None,
                solution_only=True,
            )
            prepared = submit_yosupo.prepare_sources(items)[0]

            self.assertEqual([item.name for item in items], ["aplusb"])
            self.assertEqual(prepared.source, path.read_bytes().decode("utf-8"))
            self.assertEqual(prepared.source_sha256, submit_yosupo.sha256_file(path))

    def test_solution_tree_does_not_need_sibling_template(self):
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            root, manifest, _ = self.make_solution(workspace)
            self.assertFalse((workspace / "hushuqi算法竞赛模板").exists())

            items = submit_yosupo.load_items(
                root,
                manifest,
                None,
                solution_only=True,
            )
            submit_yosupo.prepare_sources(items)

    def test_manifest_and_meta_contract_is_strict(self):
        with tempfile.TemporaryDirectory() as directory:
            root, manifest, _ = self.make_solution(Path(directory))
            meta_path = root / manifest["problems"][0]["meta"]
            base_meta = json.loads(meta_path.read_text(encoding="utf-8"))
            cases = [
                ("manifest schema", lambda m, _: m.update(schema_version=2), "schema 3"),
                ("problem count", lambda m, _: m.update(problem_count=2), "problem_count"),
                ("counts", lambda m, _: m.update(counts={}), "counts"),
                (
                    "state",
                    lambda m, _: m["problems"][0].update(state="template_adapter"),
                    "state",
                ),
                ("meta schema", lambda _, j: j.update(schema_version=2), "schema 3"),
                (
                    "kind",
                    lambda _, j: j["provenance"].update(kind="template_adapter"),
                    "非当前模板题解",
                ),
                (
                    "template headers",
                    lambda _, j: j["provenance"].update(template_headers=["x.hpp"]),
                    "template_headers",
                ),
                (
                    "meta hash",
                    lambda _, j: j["provenance"].update(source_sha256="0" * 64),
                    "source_sha256",
                ),
            ]
            for label, mutate, message in cases:
                with self.subTest(label=label):
                    current_manifest = json.loads(json.dumps(manifest))
                    current_meta = json.loads(json.dumps(base_meta))
                    mutate(current_manifest, current_meta)
                    self.write(root, manifest["problems"][0]["meta"], json.dumps(current_meta))
                    with self.assertRaisesRegex(submit_yosupo.SubmissionError, message):
                        submit_yosupo.load_items(
                            root,
                            current_manifest,
                            None,
                            solution_only=True,
                        )
            self.write(root, manifest["problems"][0]["meta"], json.dumps(base_meta))

    def test_manifest_rejects_invalid_or_duplicate_problem_names(self):
        cases = [
            ([], "problems 数组"),
            ([1], "非对象"),
            ([{"name": "../x"}], "无效题目名"),
            ([{"name": "x"}, {"name": "x"}], "重复题目"),
        ]
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for entries, message in cases:
                manifest = {
                    "schema_version": 3,
                    "problem_count": len(entries),
                    "counts": {"template_solution": len(entries)},
                }
                if entries:
                    manifest["problems"] = entries
                with self.subTest(message=message):
                    with self.assertRaisesRegex(submit_yosupo.SubmissionError, message):
                        submit_yosupo.load_items(root, manifest, None)

    def test_manifest_accepts_official_problem_name_with_uppercase(self):
        with tempfile.TemporaryDirectory() as directory:
            root, manifest, _ = self.make_solution(
                Path(directory),
                name="convolution_F_2_64",
            )
            items = submit_yosupo.load_items(
                root,
                manifest,
                None,
                solution_only=True,
            )
            self.assertEqual([item.name for item in items], ["convolution_F_2_64"])

    def test_compile_uses_gnu20_and_removes_temporary_build(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            build_dirs = []

            def run(command, **kwargs):
                self.assertIn("-std=gnu++20", command)
                self.assertIn("-DONLINE_JUDGE", command)
                self.assertTrue(
                    all(arg in command for arg in submit_yosupo.local_linker_args())
                )
                source_path = Path(command[-3])
                output_path = Path(command[-1])
                self.assertEqual(
                    source_path.read_bytes(),
                    prepared.source.encode("utf-8"),
                )
                output_path.write_text("executable", encoding="utf-8")
                build_dirs.append(source_path.parent)
                return submit_yosupo.subprocess.CompletedProcess(command, 0, "", "")

            with patch.object(submit_yosupo.subprocess, "run", side_effect=run):
                with redirect_stdout(io.StringIO()):
                    submit_yosupo.compile_sources([prepared], compiler="fake-g++")

            self.assertEqual(len(build_dirs), 1)
            self.assertFalse(build_dirs[0].exists())

    @unittest.skipUnless(shutil.which("g++"), "g++ is not installed")
    def test_real_gnu20_full_compile(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            with redirect_stdout(io.StringIO()):
                submit_yosupo.compile_sources([prepared], timeout=30)

    def test_submit_uses_cpp20_and_requires_submission_id(self):
        captured = {}

        def urlopen(request, timeout):
            captured.update(json.loads(request.data.decode()))
            return FakeResponse(b'{"id": 42}')

        with patch.object(submit_yosupo, "urlopen", side_effect=urlopen):
            submission_id = submit_yosupo.submit(
                "https://api.example",
                "token",
                "aplusb",
                "int main() {}",
            )
        self.assertEqual(submission_id, 42)
        self.assertEqual(captured, {
            "problem": "aplusb",
            "source": "int main() {}",
            "lang": "cpp20",
            "tle_knockout": True,
        })

        with patch.object(
            submit_yosupo,
            "urlopen",
            return_value=FakeResponse(b"{}"),
        ):
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "submission id"):
                submit_yosupo.submit(
                    "https://api.example",
                    "token",
                    "aplusb",
                    "int main() {}",
                )

    def test_waits_through_all_current_progress_statuses(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            statuses = iter(["WJ", "-", "Fetching", "Compiling", "0/1", "AC"])

            def read_json(url):
                return self.detail(prepared, 42, next(statuses))

            stdout = io.StringIO()
            with patch.object(submit_yosupo, "read_json", side_effect=read_json), patch.object(
                submit_yosupo.time,
                "sleep",
            ), redirect_stdout(stdout):
                result = submit_yosupo.wait_submission(
                    "https://api.example",
                    prepared,
                    42,
                    timeout=5,
                    interval=0,
                )
            self.assertEqual(result["overview"]["status"], "AC")
            for status in ["WJ", "-", "Fetching", "Compiling", "0/1", "AC"]:
                self.assertIn(status, stdout.getvalue())

    def test_submission_identity_language_and_exact_source_are_verified(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            cases = [
                (self.detail(prepared, 42, "AC", problem="other"), "题目名"),
                (self.detail(prepared, 42, "AC", title="Other"), "题目标题"),
                (self.detail(prepared, 42, "AC", lang="cpp"), "语言"),
                (self.detail(prepared, 42, "AC", source="changed"), "SHA256"),
            ]
            for data, message in cases:
                with self.subTest(message=message):
                    with self.assertRaisesRegex(submit_yosupo.SubmissionError, message):
                        submit_yosupo.validate_submission(data, prepared, 42)

    def test_metadata_is_atomic_idempotent_and_records_online_ac(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            prepared = self.make_prepared(root)
            pending = self.final_attempt(prepared)
            pending.update({
                "server_source_sha256": None,
                "verdict": None,
                "last_status": "SUBMITTED",
                "time": None,
                "memory": None,
                "submission_time": None,
                "is_latest": None,
            })
            self.assertEqual(submit_yosupo.record_attempt(prepared, pending), "submitted")

            final = self.final_attempt(prepared)
            self.assertEqual(submit_yosupo.record_attempt(prepared, final), "online_ac")
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            self.assertEqual(len(meta["submission_attempts"]), 1)
            self.assertEqual(meta["validation"]["current_source_sha256"], prepared.source_sha256)
            self.assertEqual(meta["validation"]["server_source_sha256"], prepared.source_sha256)
            self.assertNotIn("current_adapter_sha256", meta["validation"])
            self.assertNotIn("current_expanded_sha256", meta["validation"])
            self.assertEqual(list(root.glob(".meta.json.*.tmp")), [])

    def test_source_change_during_judging_is_not_certified(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            prepared = self.make_prepared(root)
            self.write(root, "main.cpp", "int main() { return 1; }\n")

            status = submit_yosupo.record_attempt(
                prepared,
                self.final_attempt(prepared),
            )
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            self.assertEqual(status, "local_only")
            self.assertIsNone(meta["validation"]["current_source_online_verdict"])
            self.assertNotEqual(
                meta["validation"]["current_source_sha256"],
                prepared.source_sha256,
            )

    def test_invalid_online_attempt_is_not_certified(self):
        cases = [("lang", "cpp"), ("submission_id", -1)]
        for field, value in cases:
            with self.subTest(field=field), tempfile.TemporaryDirectory() as directory:
                prepared = self.make_prepared(Path(directory))
                attempt = self.final_attempt(prepared)
                attempt[field] = value
                self.assertEqual(
                    submit_yosupo.record_attempt(prepared, attempt),
                    "local_only",
                )

    def test_latest_recorded_attempt_wins_after_parallel_completion(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            newer = self.final_attempt(prepared, submission_id=43, verdict="AC")
            newer["recorded_at"] = "2026-08-16T00:00:03Z"
            older = self.final_attempt(prepared, submission_id=42, verdict="WA")
            older["recorded_at"] = "2026-08-16T00:00:02Z"

            submit_yosupo.record_attempt(prepared, newer)
            status = submit_yosupo.record_attempt(prepared, older)
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            self.assertEqual(status, "online_ac")
            self.assertEqual(meta["validation"]["current_source_submission_id"], 43)

    def test_only_matching_online_ac_is_skipped(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            meta["validation"] = {
                "status": "online_ac",
                "current_source_sha256": prepared.source_sha256,
                "server_source_sha256": prepared.source_sha256,
                "current_source_online_verdict": "AC",
                "current_source_is_latest": True,
                "current_source_submission_id": 42,
                "current_source_time": 0.125,
                "current_source_memory": 123456,
                "current_source_submission_time": "2026-08-16T00:00:00Z",
            }
            prepared.item.meta_path.write_text(json.dumps(meta), encoding="utf-8")
            self.assertFalse(submit_yosupo.already_certified(prepared))

            meta["submission_attempts"] = [self.final_attempt(prepared)]
            prepared.item.meta_path.write_text(json.dumps(meta), encoding="utf-8")
            self.assertTrue(submit_yosupo.already_certified(prepared))

            for field, attempt_field in (
                ("current_source_time", "time"),
                ("current_source_memory", "memory"),
                ("current_source_submission_time", "submission_time"),
            ):
                with self.subTest(field=field):
                    changed = json.loads(json.dumps(meta))
                    changed["submission_attempts"][0][attempt_field] = "changed"
                    prepared.item.meta_path.write_text(json.dumps(changed), encoding="utf-8")
                    self.assertFalse(submit_yosupo.already_certified(prepared))

            changed = json.loads(json.dumps(meta))
            changed["submission_attempts"][0]["lang"] = "cpp"
            prepared.item.meta_path.write_text(json.dumps(changed), encoding="utf-8")
            self.assertFalse(submit_yosupo.already_certified(prepared))

            changed = json.loads(json.dumps(meta))
            changed["validation"]["current_source_sha256"] = "0" * 64
            prepared.item.meta_path.write_text(json.dumps(changed), encoding="utf-8")
            self.assertFalse(submit_yosupo.already_certified(prepared))

    def test_reconcile_restores_matching_historical_ac(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            meta["submission_attempts"] = [self.final_attempt(prepared)]
            meta["validation"] = {
                "status": "local_only",
                "current_source_sha256": prepared.source_sha256,
                "current_source_online_verdict": None,
                "server_source_sha256": None,
            }
            prepared.item.meta_path.write_text(json.dumps(meta), encoding="utf-8")

            self.assertEqual(
                submit_yosupo.reconcile_validation(prepared),
                "online_ac",
            )
            self.assertTrue(submit_yosupo.already_certified(prepared))

    def test_reconcile_local_only_does_not_mutate_batch_bound_meta(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            before = prepared.item.meta_path.read_bytes()
            self.assertEqual(submit_yosupo.reconcile_validation(prepared), "local_only")
            self.assertEqual(prepared.item.meta_path.read_bytes(), before)

    def test_source_change_before_post_is_not_submitted(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            prepared = self.make_prepared(root)
            self.write(root, "main.cpp", "int changed;\n")
            with patch.object(submit_yosupo, "submit") as submit_mock:
                with self.assertRaisesRegex(
                    submit_yosupo.SubmissionError,
                    "提交前发生变化",
                ):
                    submit_yosupo.judge_one(prepared, "token", 5, 0)
            submit_mock.assert_not_called()
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            self.assertEqual(meta["submission_attempts"], [])

    def test_judge_one_records_final_verdict_not_post_success(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            stdout = io.StringIO()
            with patch.object(
                submit_yosupo,
                "submit",
                return_value=42,
            ) as submit_mock, patch.object(
                submit_yosupo,
                "wait_submission",
                return_value=self.detail(prepared, 42, "AC"),
            ), redirect_stdout(stdout):
                certified = submit_yosupo.judge_one(prepared, "token", 5, 0)
            output = stdout.getvalue()
            self.assertTrue(certified)
            self.assertIn("SUBMITTED aplusb: submission 42", output)
            self.assertIn("VERDICT aplusb: AC", output)
            self.assertNotIn("ACCEPTED", output)
            submit_mock.assert_called_once_with(
                submit_yosupo.API,
                "token",
                "aplusb",
                prepared.source,
            )

    def test_post_failure_is_recorded_without_submission_id(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            with patch.object(
                submit_yosupo,
                "submit",
                side_effect=submit_yosupo.SubmissionError("network failed"),
            ):
                with self.assertRaisesRegex(
                    submit_yosupo.SubmissionError,
                    "network failed",
                ):
                    submit_yosupo.judge_one(prepared, "token", 5, 0)
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            attempt = meta["submission_attempts"][0]
            self.assertIsNone(attempt["submission_id"])
            self.assertEqual(attempt["last_status"], "SUBMISSION_UNKNOWN")
            self.assertTrue(attempt["submission_unknown"])
            self.assertEqual(attempt["source_sha256"], prepared.source_sha256)
            self.assertNotIn("adapter_sha256", attempt)
            self.assertNotIn("expanded_sha256", attempt)
            self.assertEqual(meta["validation"]["status"], "submission_unknown")
            self.assertIsNone(meta["validation"]["current_source_submission_id"])

    def test_poll_failure_keeps_confirmed_submission_pending(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            with patch.object(
                submit_yosupo,
                "submit",
                return_value=42,
            ), patch.object(
                submit_yosupo,
                "wait_submission",
                side_effect=submit_yosupo.SubmissionError("poll failed"),
            ):
                with self.assertRaisesRegex(
                    submit_yosupo.SubmissionError,
                    "poll failed",
                ):
                    submit_yosupo.judge_one(prepared, "token", 5, 0)
            meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
            attempt = meta["submission_attempts"][0]
            self.assertEqual(attempt["submission_id"], 42)
            self.assertEqual(attempt["last_status"], "SUBMITTED")
            self.assertEqual(attempt["error"], "poll failed")
            self.assertEqual(meta["validation"]["status"], "submitted")
            self.assertEqual(meta["validation"]["current_source_submission_id"], 42)
            self.assertIsNone(meta["validation"]["current_source_online_verdict"])

    def test_unidentified_legacy_post_attempt_is_migrated_to_unknown(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            attempt = {
                "submission_id": None,
                "source_sha256": prepared.source_sha256,
                "server_source_sha256": None,
                "lang": "cpp20",
                "verdict": None,
                "last_status": "POST_ERROR",
                "time": None,
                "memory": None,
                "submission_time": None,
                "is_latest": None,
                "recorded_at": "2026-08-20T00:00:00Z",
                "error": "old network error",
            }
            self.assertEqual(
                submit_yosupo.record_attempt(prepared, attempt),
                "submission_unknown",
            )

    def test_unresolved_current_source_never_reposts(self):
        argv = [
            str(SCRIPT),
            "--submit",
            "--only",
            "exp_of_formal_power_series",
        ]
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "reconcile_validation",
            return_value="submission_unknown",
        ), patch.object(
            submit_yosupo,
            "browser_auth",
        ) as browser_auth, patch.object(
            submit_yosupo,
            "judge_one",
        ) as judge_one, redirect_stdout(io.StringIO()):
            with self.assertRaisesRegex(SystemExit, "拒绝自动重交"):
                submit_yosupo.main()
        browser_auth.assert_not_called()
        judge_one.assert_not_called()

    def test_submitted_current_source_only_resumes_polling(self):
        argv = [
            str(SCRIPT),
            "--submit",
            "--only",
            "exp_of_formal_power_series",
        ]
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "reconcile_validation",
            return_value="submitted",
        ), patch.object(
            submit_yosupo,
            "latest_submission_id",
            return_value=42,
        ), patch.object(
            submit_yosupo,
            "poll_pending_one",
            return_value=True,
        ) as poll_pending, patch.object(
            submit_yosupo,
            "browser_auth",
        ) as browser_auth, patch.object(
            submit_yosupo,
            "judge_one",
        ) as judge_one, redirect_stdout(io.StringIO()):
            submit_yosupo.main()
        poll_pending.assert_called_once()
        browser_auth.assert_not_called()
        judge_one.assert_not_called()

    def test_explicit_unknown_retry_does_not_allow_other_online_states(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(Path(directory))
            submit_yosupo.block_automatic_resubmission(
                prepared,
                "submission_unknown",
                allow_retry_unknown=True,
            )
            for status in ("submitted", "online_failed", "online_stale"):
                with self.subTest(status=status):
                    with self.assertRaises(submit_yosupo.SubmissionError):
                        submit_yosupo.block_automatic_resubmission(
                            prepared,
                            status,
                            allow_retry_unknown=True,
                        )

    def test_token_freshness_rejects_malformed_and_expired_jwts(self):
        def token(payload):
            data = json.dumps(payload).encode("utf-8")
            part = base64.urlsafe_b64encode(data).decode().rstrip("=")
            return f"header.{part}.signature"

        self.assertFalse(submit_yosupo.token_is_fresh("not-a-jwt"))
        self.assertFalse(submit_yosupo.token_is_fresh(token({"exp": 0})))
        self.assertTrue(submit_yosupo.token_is_fresh(token({"exp": time.time() + 3600})))

    def test_refresh_token_returns_only_a_fresh_id_token(self):
        def token(payload):
            data = json.dumps(payload).encode("utf-8")
            part = base64.urlsafe_b64encode(data).decode().rstrip("=")
            return f"header.{part}.signature"

        class Response:
            def __enter__(self):
                return self

            def __exit__(self, *args):
                return False

            def read(self):
                return json.dumps({"id_token": token({"exp": time.time() + 3600})}).encode()

        session = {"api_key": "key", "refresh_token": "refresh", "access_token": "old"}
        with patch.object(submit_yosupo, "urlopen", return_value=Response()) as urlopen:
            self.assertTrue(submit_yosupo.refresh_token(session))
        request = urlopen.call_args.args[0]
        self.assertIn("securetoken.googleapis.com", request.full_url)
        self.assertNotIn("refresh", request.full_url)

    def test_data_structure_batch_is_required_and_revalidated(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            prepared = self.make_prepared(root, category="data_structure")
            with self.assertRaisesRegex(submit_yosupo.SubmissionError, "必须提供 --batch"):
                submit_yosupo.validate_ch03_batch(root, prepared, None)

            batch = self.write(root, ".local/batch.json", "{}\n")
            result = submit_yosupo.subprocess.CompletedProcess([], 0, "", "")
            with patch.object(
                submit_yosupo.subprocess,
                "run",
                return_value=result,
            ) as run:
                submit_yosupo.validate_ch03_batch(root, prepared, batch)
            command = run.call_args.args[0]
            self.assertEqual(
                command,
                [
                    sys.executable,
                    str(root / "tools" / "ch03_batch_report.py"),
                    "--only",
                    "aplusb",
                    "--revalidate-batch",
                    str(batch),
                ],
            )
            self.assertEqual(run.call_args.kwargs["cwd"], str(root.parent))

    def test_data_structure_submit_without_batch_stops_before_login(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(
                Path(directory),
                category="data_structure",
            )
            argv = [
                str(SCRIPT),
                "--submit",
                "--only",
                "aplusb",
            ]
            with patch.object(sys, "argv", argv), patch.object(
                submit_yosupo,
                "load_items",
                return_value=[prepared.item],
            ), patch.object(
                submit_yosupo,
                "prepare_sources",
                return_value=[prepared],
            ), patch.object(
                submit_yosupo,
                "reconcile_validation",
                return_value="local_only",
            ), patch.object(
                submit_yosupo,
                "already_certified",
                return_value=False,
            ), patch.object(
                submit_yosupo,
                "browser_auth",
            ) as browser_auth, patch.object(
                submit_yosupo,
                "judge_one",
            ) as judge_one, redirect_stdout(io.StringIO()):
                with self.assertRaisesRegex(SystemExit, "必须提供 --batch"):
                    submit_yosupo.main()
            browser_auth.assert_not_called()
            judge_one.assert_not_called()

    def test_data_structure_batch_is_revalidated_before_and_after_login(self):
        with tempfile.TemporaryDirectory() as directory:
            prepared = self.make_prepared(
                Path(directory),
                category="data_structure",
            )
            argv = [
                str(SCRIPT),
                "--submit",
                "--only",
                "aplusb",
                "--batch",
                "batch.json",
            ]
            with patch.object(sys, "argv", argv), patch.object(
                submit_yosupo,
                "load_items",
                return_value=[prepared.item],
            ), patch.object(
                submit_yosupo,
                "prepare_sources",
                return_value=[prepared],
            ), patch.object(
                submit_yosupo,
                "reconcile_validation",
                return_value="local_only",
            ), patch.object(
                submit_yosupo,
                "already_certified",
                return_value=False,
            ), patch.object(
                submit_yosupo,
                "validate_ch03_batch",
            ) as validate_batch, patch.object(
                submit_yosupo,
                "check_receipt",
            ), patch.object(
                submit_yosupo,
                "compile_sources",
            ), patch.object(
                submit_yosupo,
                "check_cpp20",
            ), patch.object(
                submit_yosupo,
                "browser_auth",
                return_value="token",
            ), patch.object(
                submit_yosupo,
                "judge_one",
                return_value=True,
            ) as judge_one, patch.object(Path, "mkdir"), redirect_stdout(io.StringIO()):
                submit_yosupo.main()
            self.assertEqual(validate_batch.call_count, 3)
            for call in validate_batch.call_args_list:
                self.assertEqual(call.args[1], prepared)
                self.assertEqual(call.args[2], Path("batch.json"))
            judge_one.assert_called_once_with(prepared, "token", 1800, 3)

    def test_submit_requires_explicit_scope(self):
        stderr = io.StringIO()
        with patch.object(sys, "argv", [str(SCRIPT), "--submit"]), redirect_stderr(stderr):
            with self.assertRaises(SystemExit) as raised:
                submit_yosupo.main()
        self.assertEqual(raised.exception.code, 2)
        self.assertIn("--only", stderr.getvalue())

    def test_submit_rejects_all_and_multiple_problem_scope(self):
        cases = [
            ([str(SCRIPT), "--submit", "--all"], "--only"),
            ([str(SCRIPT), "--submit", "--only", "a,b"], "一次只能提交一道题"),
            ([str(SCRIPT), "--submit", "--only", "a", "--jobs", "2"], "--jobs 1"),
        ]
        for argv, message in cases:
            stderr = io.StringIO()
            with self.subTest(argv=argv), patch.object(sys, "argv", argv), redirect_stderr(
                stderr
            ), redirect_stdout(io.StringIO()):
                with self.assertRaises(SystemExit) as raised:
                    submit_yosupo.main()
            self.assertIn(message, stderr.getvalue() + str(raised.exception))

    def test_removed_expansion_flags_are_rejected(self):
        for flag in ("--check-expanded", "--output-expanded"):
            argv = [str(SCRIPT), "--dry-run", "--all", flag]
            if flag == "--output-expanded":
                argv.append("out")
            with self.subTest(flag=flag), patch.object(sys, "argv", argv), redirect_stderr(
                io.StringIO()
            ):
                with self.assertRaises(SystemExit) as raised:
                    submit_yosupo.main()
                self.assertEqual(raised.exception.code, 2)

    def test_compile_failure_happens_before_authentication(self):
        argv = [
            str(SCRIPT),
            "--submit",
            "--only",
            "exp_of_formal_power_series",
        ]
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "already_certified",
            return_value=False,
        ), patch.object(
            submit_yosupo,
            "check_receipt",
        ), patch.object(
            submit_yosupo,
            "compile_sources",
            side_effect=submit_yosupo.SubmissionError("compile failed"),
        ), patch.object(submit_yosupo, "check_cpp20") as check_cpp20, patch.object(
            submit_yosupo,
            "browser_auth",
        ) as browser_auth, redirect_stdout(io.StringIO()):
            with self.assertRaises(SystemExit):
                submit_yosupo.main()
        check_cpp20.assert_not_called()
        browser_auth.assert_not_called()

    def test_receipt_failure_happens_before_compile_and_authentication(self):
        argv = [
            str(SCRIPT),
            "--submit",
            "--only",
            "exp_of_formal_power_series",
        ]
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "already_certified",
            return_value=False,
        ), patch.object(
            submit_yosupo,
            "check_receipt",
            side_effect=submit_yosupo.SubmissionError("receipt failed"),
        ) as check_receipt, patch.object(
            submit_yosupo,
            "compile_sources",
        ) as compile_sources, patch.object(
            submit_yosupo,
            "check_cpp20",
        ) as check_cpp20, patch.object(
            submit_yosupo,
            "browser_auth",
        ) as browser_auth, redirect_stdout(io.StringIO()):
            with self.assertRaisesRegex(SystemExit, "receipt failed"):
                submit_yosupo.main()
        check_receipt.assert_called_once()
        compile_sources.assert_not_called()
        check_cpp20.assert_not_called()
        browser_auth.assert_not_called()

    def test_receipt_is_checked_again_after_login_before_submission(self):
        argv = [
            str(SCRIPT),
            "--submit",
            "--only",
            "exp_of_formal_power_series",
        ]
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "already_certified",
            return_value=False,
        ), patch.object(
            submit_yosupo,
            "check_receipt",
            side_effect=[None, submit_yosupo.SubmissionError("receipt changed")],
        ) as check_receipt, patch.object(
            submit_yosupo,
            "compile_sources",
        ), patch.object(
            submit_yosupo,
            "check_cpp20",
        ), patch.object(
            submit_yosupo,
            "browser_auth",
            return_value="token",
        ), redirect_stdout(io.StringIO()):
            with self.assertRaisesRegex(SystemExit, "receipt changed"):
                submit_yosupo.main()
        self.assertEqual(check_receipt.call_count, 2)

    def test_dry_run_checks_stored_main_without_authentication(self):
        argv = [
            str(SCRIPT),
            "--dry-run",
            "--only",
            "exp_of_formal_power_series",
        ]
        stdout = io.StringIO()
        with patch.object(sys, "argv", argv), patch.object(
            submit_yosupo,
            "browser_auth",
        ) as browser_auth, redirect_stdout(stdout):
            submit_yosupo.main()
        output = stdout.getvalue()
        browser_auth.assert_not_called()
        self.assertIn("polynomial/exp_of_formal_power_series/main.cpp", output)
        self.assertIn("独立源码有效", output)
        self.assertIn("SHA256", output)


if __name__ == "__main__":
    unittest.main()
