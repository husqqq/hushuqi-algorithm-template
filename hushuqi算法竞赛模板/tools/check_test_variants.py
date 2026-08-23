#!/usr/bin/env python3
"""Compile and run the C++ test suite against both header branches."""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
INCLUDE = ROOT / "include"
TESTS = ROOT / "tests"
GENERATOR = ROOT / "tools" / "generate_print_headers.py"
LOCAL_INCLUDE = re.compile(r'#include\s+"\.\./include/(full/)?([^\"]+)"')


def compiler() -> str:
    for name in ("g++", "clang++"):
        path = shutil.which(name)
        if path:
            return path
    raise RuntimeError("g++ or clang++ is required")


def test_sources() -> list[Path]:
    return sorted(
        path for path in TESTS.glob("*.cpp")
        if not path.name.startswith("benchmark_")
    )


def branch_source(source: Path, branch: str) -> str:
    text = source.read_text(encoding="utf-8")

    def redirect(match: re.Match[str]) -> str:
        full_prefix, name = match.groups()
        if full_prefix:
            target = INCLUDE / branch / name
        elif branch == "print":
            target = INCLUDE / "print" / "support" / name
        else:
            target = INCLUDE / name
        return f'#include "{target.as_posix()}"'

    return LOCAL_INCLUDE.sub(redirect, text)


def compile_tests(branch: str, jobs: int, temp: Path) -> list[tuple[str, Path]]:
    cxx = compiler()
    tests = test_sources()
    commands: list[tuple[str, Path, list[str]]] = []
    for index, source in enumerate(tests):
        generated = temp / f"{index:03d}_{source.name}"
        generated.write_text(branch_source(source, branch), encoding="utf-8")
        executable = temp / f"{index:03d}_{source.stem}.exe"
        command = [
            cxx, "-std=c++20", "-O2", "-pipe", "-I", str(ROOT),
            str(generated), "-o", str(executable),
        ]
        if branch == "print":
            command.insert(1, "-DTEMPLATE_PRINT_VARIANT")
        commands.append((source.name, executable, command))

    failures: list[str] = []
    with ThreadPoolExecutor(max_workers=jobs) as pool:
        future_to_test = {
            pool.submit(subprocess.run, command, capture_output=True): (name, executable)
            for name, executable, command in commands
        }
        for done, future in enumerate(as_completed(future_to_test), 1):
            name, _ = future_to_test[future]
            result = future.result()
            if result.returncode:
                diagnostic = result.stderr.decode("utf-8", errors="replace")
                failures.append(f"{name}\n{diagnostic}")
            if done % 20 == 0:
                print(f"{branch}: compiled {done}/{len(tests)} tests", flush=True)
    if failures:
        raise RuntimeError("\n".join(failures[:20]))
    print(f"{branch}: compiled {len(tests)} tests")
    return [(name, executable) for name, executable, _ in commands]


def run_tests(branch: str, tests: list[tuple[str, Path]], jobs: int, timeout: int) -> None:
    failures: list[str] = []

    def run_one(item: tuple[str, Path]) -> tuple[str, subprocess.CompletedProcess[bytes] | None, str]:
        name, executable = item
        try:
            result = subprocess.run(
                [str(executable)], cwd=TESTS, capture_output=True, timeout=timeout
            )
            return name, result, ""
        except subprocess.TimeoutExpired:
            return name, None, f"timed out after {timeout}s"

    with ThreadPoolExecutor(max_workers=jobs) as pool:
        futures = [pool.submit(run_one, item) for item in tests]
        for done, future in enumerate(as_completed(futures), 1):
            name, result, error = future.result()
            if result is None:
                failures.append(f"{name}: {error}")
            elif result.returncode:
                stdout = result.stdout.decode("utf-8", errors="replace")
                stderr = result.stderr.decode("utf-8", errors="replace")
                failures.append(
                    f"{name}: exit {result.returncode}\nstdout:\n{stdout}\nstderr:\n{stderr}"
                )
            if done % 20 == 0:
                print(f"{branch}: ran {done}/{len(tests)} tests", flush=True)
    if failures:
        raise RuntimeError("\n".join(failures[:20]))
    print(f"{branch}: passed {len(tests)} tests")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--branch", choices=("full", "print", "both"), default="both")
    parser.add_argument("--compile-jobs", type=int, default=min(8, max(1, os.cpu_count() or 1)))
    parser.add_argument("--run-jobs", type=int, default=min(4, max(1, os.cpu_count() or 1)))
    parser.add_argument("--timeout", type=int, default=300)
    args = parser.parse_args()
    subprocess.run([sys.executable, str(GENERATOR), "--check"], check=True)
    branches = ("full", "print") if args.branch == "both" else (args.branch,)
    with tempfile.TemporaryDirectory(prefix="template-tests-") as directory:
        temp_root = Path(directory)
        for branch in branches:
            temp = temp_root / branch
            temp.mkdir()
            executables = compile_tests(branch, args.compile_jobs, temp)
            run_tests(branch, executables, args.run_jobs, args.timeout)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
