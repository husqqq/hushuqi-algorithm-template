#!/usr/bin/env python3
"""Compile both header branches and verify the generated print branch."""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
GENERATOR = ROOT / "tools" / "generate_print_headers.py"


def compiler() -> str:
    for name in ("g++", "clang++"):
        path = shutil.which(name)
        if path:
            return path
    raise RuntimeError("g++ or clang++ is required")


def compile_headers(branch: str, jobs: int) -> None:
    cxx = compiler()
    folder = ROOT / "include" / branch
    headers = sorted(folder.glob("*.hpp"))
    failures: list[str] = []
    with tempfile.TemporaryDirectory(prefix=f"template-{branch}-") as td:
        temp = Path(td)
        tasks = []
        for index, header in enumerate(headers):
            source = temp / f"header_{index}.cpp"
            relative = header.relative_to(ROOT).as_posix()
            source.write_text(
                f'#include "{relative}"\nsigned main() {{ return 0; }}\n',
                encoding="utf-8",
            )
            command = [cxx, "-std=c++20", "-O0", "-fsyntax-only", "-I", str(ROOT), str(source)]
            tasks.append((relative, command))
        with ThreadPoolExecutor(max_workers=jobs) as pool:
            future_to_name = {
                pool.submit(subprocess.run, command, capture_output=True): relative
                for relative, command in tasks
            }
            for done, future in enumerate(as_completed(future_to_name), 1):
                relative = future_to_name[future]
                result = future.result()
                if result.returncode:
                    diagnostic = result.stderr.decode("utf-8", errors="replace")
                    failures.append(f"{relative}\n{diagnostic}")
                if done % 50 == 0:
                    print(f"{branch}: {done}/{len(headers)}", flush=True)
    if failures:
        raise RuntimeError("\n".join(failures[:20]))
    print(f"{branch}: compiled {len(headers)} headers")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--branch", choices=("full", "print", "both"), default="both")
    parser.add_argument("--jobs", type=int, default=min(16, max(1, os.cpu_count() or 1)))
    args = parser.parse_args()
    subprocess.run([sys.executable, str(GENERATOR), "--check"], check=True)
    branches = ("full", "print") if args.branch == "both" else (args.branch,)
    for branch in branches:
        compile_headers(branch, args.jobs)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
