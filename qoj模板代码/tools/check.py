#!/usr/bin/env python3
"""Compile every generated header and every copied standalone solution."""

from __future__ import annotations

import json
import shutil
import subprocess
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "qoj模板代码"


def run(command: list[str]) -> None:
    result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(" ".join(command) + "\n" + result.stdout + result.stderr)


def main() -> None:
    compiler = shutil.which("g++")
    if compiler is None:
        raise RuntimeError("g++ was not found")
    problems = json.loads((CATALOG / "manifest.json").read_text(encoding="utf-8"))["problems"]
    header_count = 0
    main_count = 0
    with tempfile.TemporaryDirectory(prefix="qoj3936-") as temporary:
        temporary_path = Path(temporary)
        for problem in problems:
            name = __import__("generate").safe_name(str(problem["label"]), str(problem["title"]))
            directory = CATALOG / name
            stub = temporary_path / "stub.cpp"
            stub.write_text(f'#include "{(directory / "template.hpp").as_posix()}"\nsigned main() {{}}\n', encoding="utf-8")
            run([compiler, "-std=c++20", "-fsyntax-only", str(stub)])
            header_count += 1
            source = directory / "main.cpp"
            if source.exists():
                run([compiler, "-std=c++20", "-fsyntax-only", str(source)])
                main_count += 1
    print(f"compiled {header_count} template headers and {main_count} standalone mains")


if __name__ == "__main__":
    main()
