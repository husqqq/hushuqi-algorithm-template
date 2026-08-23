#!/usr/bin/env python3
"""在临时目录中比较当前候选和只读参考的同机性能。"""

from __future__ import annotations

import argparse
import ctypes
import hashlib
import json
import os
import statistics
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import submit_yosupo as yosupo
import verify_local


class BenchmarkError(RuntimeError):
    pass


def local_path(root: Path, path: Path, label: str) -> Path:
    # root 是题解目录；返回 .local 内的显式状态文件路径。
    target = path.resolve() if path.is_absolute() else (root / path).resolve()
    local = (root / ".local").resolve()
    try:
        target.relative_to(local)
    except ValueError as error:
        raise BenchmarkError(f"{label} 必须位于 .local 内：{path}") from error
    return target


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compiler_version(compiler: str) -> str:
    # compiler 是 C++ 编译器命令；返回其第一行版本字符串。
    result = subprocess.run(
        [compiler, "--version"],
        capture_output=True,
        check=False,
        timeout=30,
    )
    if result.returncode:
        raise BenchmarkError(f"无法读取编译器版本：{compiler}")
    return result.stdout.decode("utf-8", errors="replace").splitlines()[0]


def peak_working_set(process: subprocess.Popen[bytes]) -> int | None:
    # process 是正在运行的子进程；Windows 下返回其截至当前的峰值工作集字节数。
    if os.name != "nt":
        return None

    class Counters(ctypes.Structure):
        _fields_ = [
            ("cb", ctypes.c_ulong),
            ("PageFaultCount", ctypes.c_ulong),
            ("PeakWorkingSetSize", ctypes.c_size_t),
            ("WorkingSetSize", ctypes.c_size_t),
            ("QuotaPeakPagedPoolUsage", ctypes.c_size_t),
            ("QuotaPagedPoolUsage", ctypes.c_size_t),
            ("QuotaPeakNonPagedPoolUsage", ctypes.c_size_t),
            ("QuotaNonPagedPoolUsage", ctypes.c_size_t),
            ("PagefileUsage", ctypes.c_size_t),
            ("PeakPagefileUsage", ctypes.c_size_t),
            ("PrivateUsage", ctypes.c_size_t),
        ]

    counters = Counters()
    counters.cb = ctypes.sizeof(counters)
    ok = ctypes.windll.psapi.GetProcessMemoryInfo(
        ctypes.c_void_p(process._handle),
        ctypes.byref(counters),
        counters.cb,
    )
    return int(counters.PeakWorkingSetSize) if ok else None


def run_program(
    command: list[str],
    input_path: Path | None,
    output_path: Path,
    error_path: Path,
    cwd: Path,
    timeout: float,
) -> dict:
    # command 是待运行命令；input_path 可为空；返回墙钟时间、输出哈希和峰值工作集。
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with (
        input_path.open("rb") if input_path is not None else open(os.devnull, "rb") as inp,
        output_path.open("wb") as out,
        error_path.open("wb") as err,
    ):
        started = time.perf_counter()
        process = subprocess.Popen(
            command,
            stdin=inp,
            stdout=out,
            stderr=err,
            cwd=str(cwd),
        )
        peak = peak_working_set(process)
        while process.poll() is None:
            current = peak_working_set(process)
            if current is not None:
                peak = max(peak or 0, current)
            if time.perf_counter() - started > timeout:
                process.kill()
                process.wait()
                raise BenchmarkError(f"命令超时：{' '.join(command)}")
            time.sleep(0.002)
        current = peak_working_set(process)
        if current is not None:
            peak = max(peak or 0, current)
        seconds = time.perf_counter() - started
    if process.returncode:
        detail = error_path.read_bytes().decode("utf-8", errors="replace")[-2000:]
        raise BenchmarkError(
            f"命令退出码 {process.returncode}：{' '.join(command)}\n{detail}"
        )
    return {
        "seconds": round(seconds, 6),
        "peak_working_set_bytes": peak,
        "output_sha256": sha256_file(output_path),
    }


def choose_generator(info: dict, requested: str | None) -> str:
    # info 是官方题目信息；requested 非空时验证并返回该 generator，否则偏好 max_random。
    tests = verify_local._tests(info.get("tests", []))
    generators = [name for name, _ in tests if Path(name).suffix == ".cpp"]
    if requested is not None:
        if requested not in generators:
            raise BenchmarkError(f"官方 info.toml 未声明 generator：{requested}")
        return requested
    if not generators:
        raise BenchmarkError("官方题目没有 C++ generator，无法建立性能输入")

    def key(name: str) -> tuple[int, str]:
        stem = Path(name).stem.lower()
        if stem == "max_random":
            return (0, stem)
        if "max" in stem and "random" in stem:
            return (1, stem)
        if "max" in stem:
            return (2, stem)
        if "random" in stem:
            return (3, stem)
        return (4, stem)

    return min(generators, key=key)


def load_one(root: Path, name: str) -> tuple[dict, yosupo.PreparedSource]:
    # root 是题解目录，name 是唯一题目名；返回 manifest 项和当前准备好的候选。
    manifest = verify_local.read_manifest(root)
    prepared = verify_local.load_sources(root, manifest, {name})
    if len(prepared) != 1:
        raise BenchmarkError(f"{name}: 当前候选数不为 1")
    entries = {entry["name"]: entry for entry in manifest["problems"]}
    return entries[name], prepared[0]


def check_output(
    checker: Path,
    input_path: Path,
    actual_path: Path,
    expected_path: Path,
    cwd: Path,
    timeout: float,
    label: str,
) -> None:
    # 用官方 checker 验证 actual_path；即使有非唯一输出也不依赖字节相等。
    verify_local.run_cmd(
        [str(checker), str(input_path), str(actual_path), str(expected_path)],
        label,
        timeout,
        cwd=cwd,
    )


def main() -> None:
    parser = argparse.ArgumentParser(description="当前候选/参考五轮性能门")
    parser.add_argument("--only", required=True, help="唯一官方题目名")
    parser.add_argument("--category", default="data_structure", help="官方分类，默认 data_structure")
    parser.add_argument("--generator", help="官方 gen/ 下的 C++ generator 名")
    parser.add_argument("--seed", type=int, default=0, help="官方 generator 的种子")
    parser.add_argument("--rounds", type=int, default=5, help="有效交替轮数，至少 5")
    parser.add_argument("--warmups", type=int, default=1, help="每侧预热次数，至少 1")
    parser.add_argument("--threshold", type=float, default=1.25, help="候选/参考中位数允许倍率")
    parser.add_argument("--compiler", default="g++", help="GNU C++20 编译器")
    parser.add_argument("--compile-timeout", type=float, default=180.0)
    parser.add_argument("--run-timeout", type=float, default=120.0)
    parser.add_argument(
        "--output",
        type=Path,
        help="将性能 JSON 直接以 UTF-8/LF 写入 .local 内的状态文件",
    )
    args = parser.parse_args()
    if args.rounds < 5 or args.warmups < 1 or args.threshold <= 0:
        parser.error("rounds 至少为 5，warmups 至少为 1，threshold 必须为正数")

    root = Path(__file__).resolve().parent.parent
    workspace = root.parent
    try:
        entry, candidate = load_one(root, args.only)
        if entry.get("category") != args.category:
            raise BenchmarkError(f"{args.only}: 不属于 {args.category}")
        meta = json.loads(candidate.item.meta_path.read_text(encoding="utf-8"))
        reference = meta.get("reference")
        if not isinstance(reference, dict) or not isinstance(reference.get("source"), str):
            raise BenchmarkError(f"{args.only}: 缺少受控参考源码")
        reference_path = (workspace / reference["source"]).resolve()
        if not reference_path.is_file():
            raise BenchmarkError(f"{args.only}: 参考源码不存在：{reference_path}")
        reference_sha256 = sha256_file(reference_path)
        if reference.get("source_sha256") != reference_sha256:
            raise BenchmarkError(f"{args.only}: 参考源码 SHA256 已变化")

        manifest = verify_local.read_manifest(root)
        commit = verify_local.official_commit(manifest)
        problems_root = verify_local.validate_official_root(
            verify_local.default_problems_path(root), commit
        )
        compiler = compiler_version(args.compiler)
        with tempfile.TemporaryDirectory(prefix="ch03-performance-") as directory:
            work = Path(directory)
            problem = verify_local.stage_problem(
                problems_root,
                commit,
                verify_local.official_problem_rel(entry),
                work / "snapshot",
                args.compile_timeout,
            )
            info = verify_local.read_info(problem / "info.toml")
            verify_local.validate_official_title(entry, info)
            verify_local.write_params(problem, info)
            common = work / "snapshot" / "common"
            generator_name = choose_generator(info, args.generator)
            bin_dir = work / "bin"
            generator = verify_local._exe(bin_dir / "generator")
            correct = verify_local._exe(bin_dir / "correct")
            checker = verify_local._exe(bin_dir / "checker")
            candidate_bin = verify_local._exe(bin_dir / "candidate")
            reference_bin = verify_local._exe(bin_dir / "reference")
            verify_local.compile_cpp(
                problem / "gen" / generator_name,
                generator,
                args.compiler,
                args.compile_timeout,
                include_dirs=(common,),
                label=f"{args.only}: 编译性能 generator",
            )
            verify_local.compile_cpp(
                problem / "sol" / "correct.cpp",
                correct,
                args.compiler,
                args.compile_timeout,
                include_dirs=(common,),
                label=f"{args.only}: 编译性能 correct",
            )
            checker_name = info.get("checker", "checker.cpp")
            if not isinstance(checker_name, str):
                raise BenchmarkError(f"{args.only}: checker 配置无效")
            verify_local.compile_cpp(
                problem / checker_name,
                checker,
                args.compiler,
                args.compile_timeout,
                include_dirs=(common,),
                label=f"{args.only}: 编译性能 checker",
            )
            verify_local.compile_cpp(
                candidate.item.path,
                candidate_bin,
                args.compiler,
                args.compile_timeout,
                label=f"{args.only}: 编译当前候选",
            )
            verify_local.compile_cpp(
                reference_path,
                reference_bin,
                args.compiler,
                args.compile_timeout,
                label=f"{args.only}: 编译只读参考",
            )

            input_path = work / "input.txt"
            run_program(
                [str(generator), str(args.seed)],
                None,
                input_path,
                work / "generator.err",
                problem,
                args.run_timeout,
            )
            expected_path = work / "expected.txt"
            run_program(
                [str(correct)],
                input_path,
                expected_path,
                work / "correct.err",
                problem,
                args.run_timeout,
            )

            def one(kind: str, index: int, phase: str) -> dict:
                # kind 选择 candidate/reference，index 是本阶段序号，phase 是 warmup 或 measured。
                binary = candidate_bin if kind == "candidate" else reference_bin
                actual = work / f"{phase}-{kind}-{index}.txt"
                result = run_program(
                    [str(binary)],
                    input_path,
                    actual,
                    work / f"{phase}-{kind}-{index}.err",
                    problem,
                    args.run_timeout,
                )
                check_output(
                    checker,
                    input_path,
                    actual,
                    expected_path,
                    problem,
                    args.run_timeout,
                    f"{args.only}: {phase} {kind} 官方 checker",
                )
                return result | {"order": kind}

            for index in range(args.warmups):
                one("candidate", index, "warmup")
                one("reference", index, "warmup")

            candidate_runs = []
            reference_runs = []
            for index in range(args.rounds):
                order = ("candidate", "reference") if index % 2 == 0 else ("reference", "candidate")
                for kind in order:
                    result = one(kind, index, "measured")
                    (candidate_runs if kind == "candidate" else reference_runs).append(result)
            candidate_median = statistics.median(run["seconds"] for run in candidate_runs)
            reference_median = statistics.median(run["seconds"] for run in reference_runs)
            ratio = candidate_median / reference_median if reference_median else None
            result = {
                "schema_version": 1,
                "kind": "template_performance_review",
                "problem_id": f"{args.category}/{args.only}",
                "official_repository_commit": commit,
                "input": {
                    "generator": f"gen/{generator_name}",
                    "generator_sha256": sha256_file(problem / "gen" / generator_name),
                    "seed": args.seed,
                    "sha256": sha256_file(input_path),
                    "bytes": input_path.stat().st_size,
                },
                "environment": {
                    "platform": sys.platform,
                    "compiler": compiler,
                    "compile_flags": [
                        *verify_local.COMPILE_FLAGS,
                        *yosupo.local_linker_args(),
                    ],
                    "measurement": "wall_clock_seconds; Windows PeakWorkingSetSize sampled every 2ms",
                },
                "candidate": {
                    "path": entry["source"],
                    "sha256": candidate.source_sha256,
                    "runs": candidate_runs,
                    "median_seconds": candidate_median,
                    "peak_working_set_bytes": max(
                        (run["peak_working_set_bytes"] or 0 for run in candidate_runs),
                        default=0,
                    ) or None,
                },
                "reference": {
                    "path": reference["source"],
                    "sha256": reference_sha256,
                    "runs": reference_runs,
                    "median_seconds": reference_median,
                    "peak_working_set_bytes": max(
                        (run["peak_working_set_bytes"] or 0 for run in reference_runs),
                        default=0,
                    ) or None,
                },
                "threshold": args.threshold,
                "candidate_over_reference_median": ratio,
                "status": "passed" if ratio is not None and ratio <= args.threshold else "slower_than_threshold",
            }
    except (BenchmarkError, OSError, ValueError, verify_local.LocalVerifyError, yosupo.SubmissionError) as error:
        sys.exit(str(error))
    if args.output is not None:
        target = local_path(root, args.output, "性能结果路径")
        target.parent.mkdir(parents=True, exist_ok=True)
        yosupo.atomic_write_json(target, result)
        print(f"PERFORMANCE WRITTEN {target}")
    else:
        print(json.dumps(result, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
