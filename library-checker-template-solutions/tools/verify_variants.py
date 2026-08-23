#!/usr/bin/env python3
"""Verify additional template variants against official Library Checker cases."""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import submit_yosupo as yosupo
import verify_local as local


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
MANIFEST_PATH = ROOT / "variants.json"
RECEIPT_PATH = ROOT / ".local" / "yosupo-variant-verification.json"
PYTHON_TEST = ROOT.parent / "hushuqi算法竞赛模板" / "tests" / "test_python_topics.py"


def canonical_hash(value: object) -> str:
    if isinstance(value, dict):
        value = dict(value)
        value.pop("updated_at", None)
    data = json.dumps(
        value, ensure_ascii=False, sort_keys=True, separators=(",", ":")
    ).encode("utf-8")
    return hashlib.sha256(data).hexdigest()


def read_manifest() -> dict:
    value = json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))
    if value.get("schema_version") != 1 or value.get("kind") != "template_variants":
        raise local.LocalVerifyError("variants.json schema is invalid")
    entries = value.get("variants")
    if not isinstance(entries, list) or value.get("variant_count") != len(entries):
        raise local.LocalVerifyError("variants.json count is invalid")
    return value


def selected(manifest: dict, wanted: set[str] | None) -> list[tuple[dict, yosupo.PreparedSource]]:
    result = []
    seen = set()
    for entry in manifest["variants"]:
        variant_id = entry.get("variant_id")
        if not isinstance(variant_id, str) or not variant_id:
            raise local.LocalVerifyError("variant_id is invalid")
        if variant_id in seen:
            raise local.LocalVerifyError(f"duplicate variant_id: {variant_id}")
        seen.add(variant_id)
        if wanted is not None and variant_id not in wanted:
            continue
        source = (ROOT / entry["source"]).resolve()
        meta = (ROOT / entry["meta"]).resolve()
        source.relative_to(ROOT)
        meta.relative_to(ROOT)
        if not source.is_file() or not meta.is_file():
            raise local.LocalVerifyError(f"missing variant files: {variant_id}")
        text, digest = yosupo.read_submission_source(source)
        if digest != entry.get("source_sha256"):
            raise local.LocalVerifyError(f"stale source hash: {variant_id}")
        metadata = json.loads(meta.read_text(encoding="utf-8"))
        checks = {
            "variant_id": variant_id,
            "name": entry.get("problem"),
            "title": entry.get("title"),
            "category": entry.get("category"),
            "language": entry.get("language"),
            "source": entry.get("source"),
            "source_sha256": digest,
            "template_items": entry.get("template_items"),
        }
        for key, expected in checks.items():
            if metadata.get(key) != expected:
                raise local.LocalVerifyError(f"{variant_id}: meta {key} is stale")
        if entry.get("language") not in {"pypy3", "cpp20"}:
            raise local.LocalVerifyError(f"unsupported variant language: {entry.get('language')}")
        item = yosupo.SourceItem(
            name=entry["problem"],
            title=entry["title"],
            path=source,
            meta_path=meta,
            source_sha256=digest,
            category=entry["category"],
            language=entry["language"],
        )
        result.append((entry, yosupo.PreparedSource(item, text, digest)))
    if wanted is not None:
        missing = wanted - {entry["variant_id"] for entry, _ in result}
        if missing:
            raise local.LocalVerifyError("unknown variants: " + ", ".join(sorted(missing)))
    if not result:
        raise local.LocalVerifyError("no variants selected")
    return result


def run_python_test(timeout: float) -> dict:
    started = time.monotonic()
    result = subprocess.run(
        [sys.executable, "-I", str(PYTHON_TEST)],
        cwd=PYTHON_TEST.parent,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
        timeout=timeout,
        check=False,
    )
    if result.returncode != 0:
        raise local.LocalVerifyError(
            "Python template test failed\n" + (result.stderr or result.stdout)[-8000:]
        )
    if result.stdout.strip() != "OK":
        raise local.LocalVerifyError("Python template test did not print OK")
    return {
        "path": "hushuqi算法竞赛模板/tests/test_python_topics.py",
        "sha256": yosupo.sha256_file(PYTHON_TEST),
        "status": "passed",
        "seconds": round(time.monotonic() - started, 6),
    }


def write_receipt(receipt: dict, ids: set[str]) -> None:
    try:
        old = json.loads(RECEIPT_PATH.read_text(encoding="utf-8"))
    except FileNotFoundError:
        old = None
    if (
        isinstance(old, dict)
        and old.get("schema_version") == receipt["schema_version"]
        and old.get("kind") == receipt["kind"]
        and old.get("official_repository") == receipt["official_repository"]
        and isinstance(old.get("variants"), dict)
    ):
        manifest = read_manifest()
        current = {entry["variant_id"]: entry for entry in manifest["variants"]}
        receipt["variants"] = {
            key: value for key, value in old["variants"].items()
            if key not in ids
            and key in current
            and isinstance(value, dict)
            and value.get("source_sha256") == current[key].get("source_sha256")
            and value.get("problem") == current[key].get("problem")
            and value.get("language") == current[key].get("language")
        } | receipt["variants"]
    RECEIPT_PATH.parent.mkdir(parents=True, exist_ok=True)
    yosupo.atomic_write_json(RECEIPT_PATH, receipt)


def verify(items: list[tuple[dict, yosupo.PreparedSource]], run_timeout: float) -> None:
    manifest = read_manifest()
    commit = manifest["official_repository"]["commit"]
    problems_root = ROOT / ".local" / "library-checker-problems"
    local.validate_official_root(problems_root, commit)
    template_test = run_python_test(run_timeout) if any(
        entry["language"] == "pypy3" for entry, _ in items
    ) else None
    records = {}
    with tempfile.TemporaryDirectory(prefix="yosupo-variant-") as directory:
        work = Path(directory)
        for entry, prepared in items:
            binary = None
            source_compile = {"status": "passed", "interpreter": "pypy3"}
            if entry["language"] == "pypy3":
                compile(prepared.source, str(prepared.item.path), "exec")
            else:
                binary = work / entry["variant_id"] / "source.exe"
                binary.parent.mkdir(parents=True, exist_ok=True)
                seconds = local.compile_cpp(
                    prepared.item.path,
                    binary,
                    "g++",
                    180.0,
                    (),
                    online_judge=True,
                    label=f"{entry['variant_id']}: compile",
                )
                source_compile = {"status": "passed", "standard": "gnu++20", "seconds": round(seconds, 6)}
                test_path = WORKSPACE / entry["test"]
                test_binary = work / entry["variant_id"] / "test.exe"
                local.compile_cpp(test_path, test_binary, "g++", 180.0, (), label=f"{entry['variant_id']}: test compile")
                local.run_cmd([str(test_binary)], f"{entry['variant_id']}: template test", timeout=run_timeout, cwd=test_path.parent)
                template_record = {"path": entry["test"], "sha256": yosupo.sha256_file(test_path), "status": "passed"}
            cases = local.run_official_cases(
                ROOT,
                prepared,
                prepared.item.path,
                {
                    "name": entry["problem"],
                    "title": entry["title"],
                    "category": entry["category"],
                },
                problems_root,
                commit,
                work / entry["variant_id"],
                "g++",
                (),
                180.0,
                run_timeout,
                solution_command=(
                    [sys.executable, "-I", str(prepared.item.path)]
                    if entry["language"] == "pypy3"
                    else [str(binary)]
                ),
            )
            _, current = yosupo.read_submission_source(prepared.item.path)
            if current != prepared.source_sha256:
                raise local.LocalVerifyError(f"source changed during verification: {entry['variant_id']}")
            records[entry["variant_id"]] = {
                "status": "passed",
                "verified_at": yosupo.utc_now(),
                "problem": entry["problem"],
                "language": entry["language"],
                "source_sha256": current,
                "source_compile": source_compile,
                "source_syntax": (
                    {"status": "passed", "interpreter": sys.version.split()[0]}
                    if entry["language"] == "pypy3" else None
                ),
                "source_checks": [template_test if entry["language"] == "pypy3" else template_record],
                "cases": cases,
            }
    receipt = {
        "schema_version": 1,
        "kind": "yosupo_template_variant_local_verification",
        "status": "passed",
        "verified_at": yosupo.utc_now(),
        "manifest_sha256": canonical_hash(manifest),
        "official_repository": {"commit": commit},
        "variants": records,
    }
    write_receipt(receipt, set(records))
    print(f"LOCAL VERIFIED: {RECEIPT_PATH}")


def require_receipt(manifest: dict, entry: dict, prepared: yosupo.PreparedSource) -> dict:
    try:
        receipt = json.loads(RECEIPT_PATH.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise local.LocalVerifyError(f"cannot read variant receipt: {error}") from error
    if (
        receipt.get("schema_version") != 1
        or receipt.get("kind") != "yosupo_template_variant_local_verification"
        or receipt.get("status") != "passed"
        or receipt.get("manifest_sha256") != canonical_hash(manifest)
        or receipt.get("official_repository") != {
            "commit": manifest["official_repository"]["commit"]
        }
    ):
        raise local.LocalVerifyError("variant receipt is stale")
    record = receipt.get("variants", {}).get(entry["variant_id"])
    if (
        not isinstance(record, dict)
        or record.get("status") != "passed"
        or record.get("problem") != entry["problem"]
        or record.get("language") != entry["language"]
        or record.get("source_sha256") != prepared.source_sha256
    ):
        raise local.LocalVerifyError(f"no current receipt for {entry['variant_id']}")
    cases = record.get("cases")
    if not isinstance(cases, list) or {case.get("kind") for case in cases} != {
        "sample", "boundary", "random"
    } or any(case.get("status") != "passed" for case in cases):
        raise local.LocalVerifyError(f"incomplete official cases: {entry['variant_id']}")
    checks = record.get("source_checks")
    if (
        not isinstance(checks, list)
        or len(checks) != 1
        or checks[0].get("sha256") != yosupo.sha256_file(
            PYTHON_TEST if entry["language"] == "pypy3" else ROOT.parent / entry["test"]
        )
        or checks[0].get("status") != "passed"
    ):
        raise local.LocalVerifyError(f"stale template test: {entry['variant_id']}")
    return record


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--only", help="comma-separated variant ids")
    parser.add_argument("--run-timeout", type=float, default=120.0)
    args = parser.parse_args()
    if args.run_timeout <= 0:
        parser.error("run timeout must be positive")
    try:
        manifest = read_manifest()
        wanted = yosupo.parse_wanted(args.only)
        items = selected(manifest, wanted)
        print(f"verifying {len(items)} variants")
        verify(items, args.run_timeout)
    except (local.LocalVerifyError, yosupo.SubmissionError, subprocess.TimeoutExpired) as error:
        sys.exit(str(error))


if __name__ == "__main__":
    main()
