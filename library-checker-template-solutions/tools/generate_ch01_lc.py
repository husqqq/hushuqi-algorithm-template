#!/usr/bin/env python3
"""Generate or verify Chapter 01 standalone Library Checker solutions."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
TEMPLATE = ROOT / "hushuqi算法竞赛模板"
OUT = ROOT / "library-checker-template-solutions"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')

SPECS = {
    "aplusb": {
        "item": "1.1.002",
        "title": "A + B",
        "test": "tests/test_ch01_ch02.cpp",
        "boundary_input": "hushuqi算法竞赛模板/tests/lc/ch01_aplusb_boundary.in",
        "adapter": r'''
signed main()
{
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}
''',
    },
    "many_aplusb": {
        "item": "1.2.001",
        "title": "Many A + B",
        "test": "tests/test_ch01_revised.cpp",
        "adapter": r'''
signed main()
{
    FastInput in;
    int t, a, b;
    in.read(t);
    while (t--)
    {
        in.read(a);
        in.read(b);
        cout << a + b << '\n';
    }
}
''',
    },
    "many_aplusb_128bit": {
        "item": "1.2.002",
        "title": "Many A + B (128 bit)",
        "test": "tests/test_ch01_revised.cpp",
        "adapter": r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        string sa, sb;
        cin >> sa >> sb;
        auto a = parseInt128(sa);
        auto b = parseInt128(sb);
        assert(a && b);
        cout << toString(*a + *b) << '\n';
    }
}
''',
    },
}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def expand(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    result: list[str] = []
    for line in path.read_text(encoding="utf-8").replace("\r\n", "\n").splitlines(True):
        if line.strip() == "#pragma once":
            continue
        match = INCLUDE_RE.match(line.rstrip("\n"))
        result.append(expand(path.parent / match.group(1), seen) if match else line)
    return "".join(result)


def generated_source(name: str) -> bytes:
    adapter = SPECS[name]["adapter"].strip()
    return (expand(TEMPLATE / "include" / "base.hpp", set()).rstrip() + "\n\n" + adapter + "\n").encode()


def entry(name: str, digest: str) -> dict:
    return {
        "name": name,
        "title": SPECS[name]["title"],
        "category": "sample",
        "source": f"sample/{name}/main.cpp",
        "source_sha256": digest,
        "meta": f"sample/{name}/meta.json",
        "state": "template_solution",
    }


def expected_meta(name: str, digest: str) -> dict:
    validation = {
        "status": "local_only",
        "current_source_sha256": digest,
        "checks": ["source_cxx20_compile", f"hushuqi算法竞赛模板/{SPECS[name]['test']}"],
    }
    if "boundary_input" in SPECS[name]:
        validation["boundary_input"] = SPECS[name]["boundary_input"]
    return {
        "schema_version": 3,
        "name": name,
        "title": SPECS[name]["title"],
        "category": "sample",
        "provenance": {
            "kind": "template_solution",
            "source": f"sample/{name}/main.cpp",
            "source_sha256": digest,
        },
        "official_reference": {
            "commit": COMMIT,
            "category": "sample",
            "problem": name,
        },
        "reference": {
            "kind": "read_only_baseline",
            "source": f"library-checker-solutions/sample/{name}/main.cpp",
            "source_sha256": sha256(ROOT / "library-checker-solutions" / "sample" / name / "main.cpp"),
        },
        "validation": validation,
        "submission_attempts": [],
    }


def read_json(path: Path) -> dict | None:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError):
        return None
    return data if isinstance(data, dict) else None


def current_meta_or_reset(name: str, target: Path, digest: str, source_changed: bool) -> dict:
    baseline = expected_meta(name, digest)
    previous = read_json(target.parent / "meta.json")
    if previous is None or source_changed:
        if previous and isinstance(previous.get("submission_attempts"), list):
            baseline["submission_attempts"] = previous["submission_attempts"]
        return baseline
    previous.update({
        "schema_version": 3,
        "name": name,
        "title": SPECS[name]["title"],
        "category": "sample",
        "provenance": baseline["provenance"],
        "reference": baseline["reference"],
        "official_reference": baseline["official_reference"],
    })
    validation = previous.get("validation")
    if not isinstance(validation, dict):
        validation = {}
    validation.update(baseline["validation"])
    if "boundary_input" not in baseline["validation"]:
        validation.pop("boundary_input", None)
    previous["validation"] = validation
    return previous


def check_current(manifest: dict) -> list[str]:
    entries = {item["name"]: item for item in manifest.get("problems", []) if isinstance(item, dict)}
    problems: list[str] = []
    for name in SPECS:
        data = generated_source(name)
        digest = hashlib.sha256(data).hexdigest()
        target = OUT / "sample" / name / "main.cpp"
        meta = read_json(target.parent / "meta.json")
        if not target.is_file() or target.read_bytes() != data:
            problems.append(f"source out of date: {target.relative_to(ROOT)}")
        if entries.get(name) != entry(name, digest):
            problems.append(f"manifest out of date: sample/{name}")
        expected = expected_meta(name, digest)
        if meta is None:
            problems.append(f"missing or invalid metadata: sample/{name}")
        elif meta.get("provenance") != expected["provenance"] or meta.get("official_reference") != expected["official_reference"]:
            problems.append(f"metadata provenance out of date: sample/{name}")
    return problems


def write_current(manifest: dict) -> None:
    manifest_path = OUT / "manifest.json"
    entries = {item["name"]: item for item in manifest["problems"]}
    for name in SPECS:
        data = generated_source(name)
        digest = hashlib.sha256(data).hexdigest()
        target = OUT / "sample" / name / "main.cpp"
        changed = not target.is_file() or target.read_bytes() != data
        target.parent.mkdir(parents=True, exist_ok=True)
        if changed:
            target.write_bytes(data)
        meta = current_meta_or_reset(name, target, digest, changed)
        (target.parent / "meta.json").write_text(
            json.dumps(meta, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
        )
        entries[name] = entry(name, digest)
    manifest["problems"] = [entries[name] for name in sorted(entries)]
    manifest["problem_count"] = len(manifest["problems"])
    manifest["counts"] = {"template_solution": len(manifest["problems"])}
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description="生成或核验第 01 章独立 Library Checker 源码")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true")
    mode.add_argument("--write", action="store_true")
    args = parser.parse_args()
    manifest = json.loads((OUT / "manifest.json").read_text(encoding="utf-8"))
    if args.check:
        errors = check_current(manifest)
        if errors:
            print("CH01_GENERATION_OUT_OF_DATE", file=sys.stderr)
            print("\n".join(errors), file=sys.stderr)
            raise SystemExit(1)
        print("CH01_GENERATION_CURRENT")
        return
    write_current(manifest)


if __name__ == "__main__":
    main()
