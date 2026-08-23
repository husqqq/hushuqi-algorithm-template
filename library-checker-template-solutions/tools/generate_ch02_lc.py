#!/usr/bin/env python3
"""Generate or verify the Chapter 02 standalone Library Checker solution."""

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
NAME = "longest_increasing_subsequence"
HEADER = "2_2_010.hpp"
TEST = "tests/test_ch02_lis.cpp"
TITLE = "Longest Increasing Subsequence"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')

ADAPTER = r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto ans = lis(a);
    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++)
    {
        cout << ans[i] << " \n"[i + 1 == (int)ans.size()];
    }
}
'''


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


def generated_source() -> bytes:
    return (
        expand(TEMPLATE / "include" / "full" / HEADER, set()).rstrip()
        + "\n\n"
        + ADAPTER.strip()
        + "\n"
    ).encode("utf-8")


def template_inputs() -> list[dict[str, str]]:
    """Return the source and test files that define this candidate."""
    paths = [
        TEMPLATE / "include" / "full" / HEADER,
        TEMPLATE / "include" / "base.hpp",
        TEMPLATE / TEST,
    ]
    return [
        {"path": path.relative_to(ROOT).as_posix(), "sha256": sha256(path)}
        for path in sorted(paths)
    ]


def generation_evidence(digest: str) -> dict:
    """Return stable deterministic-generation evidence for the current source."""
    tool = Path(__file__).resolve()
    return {
        "schema_version": 2,
        "problem_id": f"other/{NAME}",
        "source_path": f"library-checker-template-solutions/other/{NAME}/main.cpp",
        "source_sha256": digest,
        "generator_path": tool.relative_to(ROOT).as_posix(),
        "generator_sha256": sha256(tool),
        "working_directory": ".",
        "runner": f"Python {sys.version.splitlines()[0]}",
        "argv": ["--write"],
        "inputs": template_inputs(),
        "generated_sha256": digest,
        "byte_equal": True,
    }


def entry(digest: str) -> dict:
    return {
        "name": NAME,
        "title": TITLE,
        "category": "other",
        "source": f"other/{NAME}/main.cpp",
        "source_sha256": digest,
        "meta": f"other/{NAME}/meta.json",
        "state": "template_solution",
    }


def expected_meta(digest: str) -> dict:
    evidence = generation_evidence(digest)
    return {
        "schema_version": 3,
        "name": NAME,
        "title": TITLE,
        "category": "other",
        "provenance": {
            "kind": "template_solution",
            "source": f"other/{NAME}/main.cpp",
            "source_sha256": digest,
            "evidence": {
                "method": "deterministic_generation",
                "generator": {
                    "path": evidence["generator_path"],
                    "sha256": evidence["generator_sha256"],
                    "working_directory": evidence["working_directory"],
                    "runner": evidence["runner"],
                    "argv": evidence["argv"],
                    "generation_evidence": evidence,
                    "generation_evidence_sha256": hashlib.sha256(
                        json.dumps(
                            evidence,
                            ensure_ascii=False,
                            sort_keys=True,
                            separators=(",", ":"),
                        ).encode("utf-8")
                    ).hexdigest(),
                },
            },
        },
        "official_reference": {
            "commit": COMMIT,
            "category": "other",
            "problem": NAME,
        },
        "reference": {
            "kind": "read_only_baseline",
            "source": f"library-checker-solutions/other/{NAME}/main.cpp",
            "source_sha256": sha256(ROOT / "library-checker-solutions" / "other" / NAME / "main.cpp"),
        },
        "validation": {
            "status": "local_only",
            "current_source_sha256": digest,
            "checks": ["source_cxx20_compile", f"hushuqi算法竞赛模板/{TEST}"],
        },
        "submission_attempts": [],
    }


def read_json(path: Path) -> dict | None:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError):
        return None
    return data if isinstance(data, dict) else None


def current_meta(target: Path, digest: str, source_changed: bool) -> dict:
    baseline = expected_meta(digest)
    previous = read_json(target.parent / "meta.json")
    if previous is None or source_changed:
        if previous and isinstance(previous.get("submission_attempts"), list):
            baseline["submission_attempts"] = previous["submission_attempts"]
        return baseline
    previous.update({
        "schema_version": 3,
        "name": NAME,
        "title": TITLE,
        "category": "other",
        "provenance": baseline["provenance"],
        "official_reference": baseline["official_reference"],
        "reference": baseline["reference"],
    })
    validation = previous.get("validation")
    if not isinstance(validation, dict):
        validation = baseline["validation"]
    else:
        # 源码字节未变时保留既有线上认证，只刷新生成器负责的本地检查列表。
        validation["checks"] = baseline["validation"]["checks"]
        validation.setdefault("current_source_sha256", digest)
    previous["validation"] = validation
    return previous


def check_current(manifest: dict) -> list[str]:
    entries = {item["name"]: item for item in manifest.get("problems", []) if isinstance(item, dict)}
    data = generated_source()
    digest = hashlib.sha256(data).hexdigest()
    target = OUT / "other" / NAME / "main.cpp"
    meta = read_json(target.parent / "meta.json")
    errors = []
    if not target.is_file() or target.read_bytes() != data:
        errors.append(f"source out of date: {target.relative_to(ROOT)}")
    if entries.get(NAME) != entry(digest):
        errors.append(f"manifest out of date: other/{NAME}")
    expected = expected_meta(digest)
    if meta is None:
        errors.append(f"missing or invalid metadata: other/{NAME}")
    elif (meta.get("provenance") != expected["provenance"]
          or meta.get("official_reference") != expected["official_reference"]
          or meta.get("reference") != expected["reference"]):
        errors.append(f"metadata provenance out of date: other/{NAME}")
    return errors


def write_current(manifest: dict) -> None:
    data = generated_source()
    digest = hashlib.sha256(data).hexdigest()
    target = OUT / "other" / NAME / "main.cpp"
    source_changed = not target.is_file() or target.read_bytes() != data
    target.parent.mkdir(parents=True, exist_ok=True)
    if source_changed:
        target.write_bytes(data)
    meta = current_meta(target, digest, source_changed)
    (target.parent / "meta.json").write_text(
        json.dumps(meta, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )
    entries = {item["name"]: item for item in manifest["problems"]}
    entries[NAME] = entry(digest)
    manifest["problems"] = [entries[name] for name in sorted(entries)]
    manifest["problem_count"] = len(manifest["problems"])
    manifest["counts"] = {"template_solution": len(manifest["problems"])}
    (OUT / "manifest.json").write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )


def main() -> None:
    parser = argparse.ArgumentParser(description="生成或核验第 02 章独立 Library Checker 源码")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true")
    mode.add_argument("--write", action="store_true")
    args = parser.parse_args()
    manifest = json.loads((OUT / "manifest.json").read_text(encoding="utf-8"))
    if args.check:
        errors = check_current(manifest)
        if errors:
            print("CH02_GENERATION_OUT_OF_DATE", file=sys.stderr)
            print("\n".join(errors), file=sys.stderr)
            raise SystemExit(1)
        print("CH02_GENERATION_CURRENT")
        return
    write_current(manifest)


if __name__ == "__main__":
    main()
