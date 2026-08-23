#!/usr/bin/env python3
"""Promote LC mappings only after every reproducible evidence gate passes."""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
TEMPLATE = WORKSPACE / "hushuqi算法竞赛模板"
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
MANIFEST = ROOT / "manifest.json"
PROBLEMS = ROOT / ".local" / "library-checker-problems"
RECEIPT = ROOT / ".local" / "yosupo-local-verification.json"
SCOPED = ROOT / ".local" / "scoped-receipts"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
SHARED_HEADERS = {
    ("tree", "dynamic_tree_vertex_add_subtree_sum", "6_4_001.hpp"),
}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path: Path) -> dict:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise ValueError(f"{path}: JSON root must be an object")
    return value


def load_generator():
    path = ROOT / "tools" / "generate_lc_gaps.py"
    spec = importlib.util.spec_from_file_location("generate_lc_gaps", path)
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load generate_lc_gaps.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def read_rows() -> list[list[str]]:
    rows = []
    for line_no, line in enumerate(MAPPING.read_text(encoding="utf-8").splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) != 4:
            raise ValueError(f"{MAPPING}:{line_no}: expected four TSV columns")
        rows.append(fields)
    return rows


def receipt_records() -> dict[str, dict]:
    records: dict[str, dict] = {}
    paths = [RECEIPT]
    if SCOPED.is_dir():
        paths.extend(sorted(SCOPED.glob("*.json")))
    for path in paths:
        if not path.is_file():
            continue
        receipt = read_json(path)
        if (receipt.get("status") != "passed"
                or receipt.get("official_repository") != {"commit": COMMIT}):
            continue
        current = receipt.get("problems")
        if not isinstance(current, dict):
            continue
        for name, record in current.items():
            if not isinstance(record, dict):
                continue
            previous = records.get(name)
            if (not isinstance(previous, dict)
                    or record.get("verified_at", "") >= previous.get("verified_at", "")):
                records[name] = record
    return records


def chapter_markdown(item: str) -> Path:
    major = item.split(".", 1)[0].zfill(2)
    matches = [path for path in TEMPLATE.glob(f"{major}-*.md") if path.name != "目录.md"]
    if len(matches) != 1:
        raise ValueError(f"template item {item}: chapter markdown count is {len(matches)}")
    return matches[0]


def online_current(meta: dict, source_sha: str) -> bool:
    validation = meta.get("validation")
    return bool(
        isinstance(validation, dict)
        and validation.get("status") == "online_ac"
        and validation.get("current_source_sha256") == source_sha
        and validation.get("current_source_online_verdict") == "AC"
        and validation.get("current_source_is_latest") is True
        and validation.get("server_source_sha256") == source_sha
        and type(validation.get("current_source_submission_id")) is int
    )


def audit_row(row: list[str], entries: dict[str, dict], records: dict[str, dict], generator) -> list[str]:
    chapter, item, problem, confidence = row
    if confidence == "audited":
        return row
    if confidence != "classified":
        raise ValueError(f"{problem}: unsupported mapping confidence {confidence}")
    category, name = problem.split("/", 1)
    failures = []
    official = PROBLEMS / category / name
    for filename in ("task.md", "checker.cpp"):
        if not (official / filename).is_file():
            failures.append(f"official {filename} missing")
    reference = WORKSPACE / "library-checker-solutions" / category / name / "main.cpp"
    if not reference.is_file():
        failures.append("controlled reference missing")

    entry = entries.get(problem)
    source_sha = None
    meta = {}
    if not isinstance(entry, dict):
        failures.append("manifest entry missing")
    else:
        source = ROOT / entry.get("source", "")
        source_sha = entry.get("source_sha256")
        if not source.is_file() or not isinstance(source_sha, str) or sha256(source) != source_sha:
            failures.append("current source hash mismatch")
        meta_path = ROOT / entry.get("meta", "")
        if not meta_path.is_file():
            failures.append("meta missing")
        else:
            meta = read_json(meta_path)
            provenance = meta.get("provenance")
            if (not isinstance(provenance, dict)
                    or provenance.get("kind") != "template_solution"
                    or provenance.get("source_sha256") != source_sha):
                failures.append("template provenance mismatch")

    record = records.get(name)
    if (not isinstance(record, dict) or record.get("status") != "passed"
            or record.get("official_commit") != COMMIT
            or record.get("source_sha256") != source_sha):
        failures.append("current local receipt missing")
    if not online_current(meta, source_sha or ""):
        failures.append("current online AC missing")

    spec = generator.SPECS.get((category, name))
    if spec is None:
        if category != "sample":
            failures.append("deterministic generator spec missing")
    else:
        header, _title, test, adapter = spec
        expected_header = item.replace(".", "_") + ".hpp"
        expected_stem = expected_header.removesuffix(".hpp")
        supplemental = header.startswith(expected_stem + "_")
        shared = (category, name, header) in SHARED_HEADERS
        if header != expected_header and not supplemental and not shared:
            failures.append(f"header {header} does not match item {item}")
        header_path = TEMPLATE / "include" / "full" / header
        test_path = TEMPLATE / test
        if not header_path.is_file():
            failures.append(f"header missing: {header}")
        if not test_path.is_file():
            failures.append(f"test missing: {test}")
        if header_path.is_file() and isinstance(entry, dict):
            expected = (generator.expand(header_path, set()).rstrip()
                        + "\n\n" + adapter.strip() + "\n").encode()
            source_path = ROOT / entry.get("source", "")
            if not source_path.is_file() or source_path.read_bytes() != expected:
                failures.append("source is not the deterministic template expansion")
        checks = meta.get("validation", {}).get("checks", []) if isinstance(meta, dict) else []
        if f"hushuqi算法竞赛模板/{test}" not in checks:
            failures.append("meta does not bind the mapped template test")

    markdown = chapter_markdown(item)
    body = markdown.read_text(encoding="utf-8")
    heading = re.compile(rf"^###\s+{re.escape(item)}(?:\s|$)", re.MULTILINE)
    if not heading.search(body):
        failures.append(f"template markdown item missing in {markdown.name}")
    if failures:
        raise RuntimeError(f"{problem}: " + "; ".join(failures))
    return [chapter, item, problem, "audited"]


def main() -> None:
    parser = argparse.ArgumentParser(description="Finalize reproducible LC mapping audits")
    parser.add_argument("--write", action="store_true", help="promote passing classified rows")
    args = parser.parse_args()
    manifest = read_json(MANIFEST)
    entries = {f"{entry['category']}/{entry['name']}": entry
               for entry in manifest.get("problems", [])}
    records = receipt_records()
    generator = load_generator()
    rows = read_rows()
    audited = []
    failures = []
    for row in rows:
        try:
            audited.append(audit_row(row, entries, records, generator))
        except RuntimeError as error:
            failures.append(str(error))
            audited.append(row)
    if failures:
        raise RuntimeError("mapping audit failed:\n" + "\n".join(failures))
    promoted = sum(a != b for a, b in zip(rows, audited))
    if args.write:
        comments = [line for line in MAPPING.read_text(encoding="utf-8").splitlines()
                    if not line or line.startswith("#")]
        text = "\n".join(comments + ["\t".join(row) for row in audited]) + "\n"
        MAPPING.write_text(text, encoding="utf-8")
        print(f"PROMOTED: {promoted}; AUDITED: {len(audited)}")
    else:
        print(f"AUDITABLE: {promoted}; ALREADY_AUDITED: {len(rows) - promoted}")


if __name__ == "__main__":
    main()
