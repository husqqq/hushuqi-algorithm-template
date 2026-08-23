#!/usr/bin/env python3
"""生成第 03 章单题在线提交后的可认证结果对象。"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

import submit_yosupo as yosupo


class ResultError(RuntimeError):
    pass


VALIDATION_KEYS = {
    "checked_at",
    "current_source_sha256",
    "current_adapter_sha256",
    "current_expanded_sha256",
    "status",
    "current_source_online_verdict",
    "current_source_submission_id",
    "current_source_time",
    "current_source_memory",
    "current_source_submission_time",
    "current_source_is_latest",
    "server_source_sha256",
    "note",
}


def canonical_bytes(value: object) -> bytes:
    # value 是 JSON 可序列化对象，返回流程规定的规范化 UTF-8 字节串。
    return json.dumps(
        value,
        ensure_ascii=False,
        sort_keys=True,
        separators=(",", ":"),
    ).encode("utf-8")


def canonical_sha256(value: object) -> str:
    # value 是 JSON 可序列化对象，返回其规范化 SHA256。
    return hashlib.sha256(canonical_bytes(value)).hexdigest()


def sha256_file(path: Path) -> str:
    # path 是常规文件，返回其原始字节 SHA256。
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path: Path) -> dict:
    # path 是 JSON 文件，返回对象根；格式错误时抛出 ResultError。
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise ResultError(f"无法读取 JSON：{path}") from error
    if not isinstance(value, dict):
        raise ResultError(f"JSON 根节点不是对象：{path}")
    return value


def local_path(root: Path, path: Path, label: str) -> Path:
    # root 是题解目录；返回 .local 内的显式结果状态文件路径。
    target = path.resolve() if path.is_absolute() else (root / path).resolve()
    local = (root / ".local").resolve()
    try:
        target.relative_to(local)
    except ValueError as error:
        raise ResultError(f"{label} 必须位于 .local 内：{path}") from error
    return target


def pointer_escape(key: str) -> str:
    # key 是 JSON 对象键，返回 JSON Pointer 中的转义片段。
    return key.replace("~", "~0").replace("/", "~1")


def changed_pointers(before: object, after: object, pointer: str = "") -> list[str]:
    # before、after 是 JSON 值，返回所有语义不同的最小 JSON Pointer。
    if type(before) is not type(after):
        return [pointer or "/"]
    if isinstance(before, dict):
        result = []
        for key in sorted(set(before) | set(after)):
            child = f"{pointer}/{pointer_escape(key)}"
            if key not in before or key not in after:
                result.append(child)
            else:
                result.extend(changed_pointers(before[key], after[key], child))
        return result
    if isinstance(before, list):
        if before == after:
            return []
        return [pointer or "/"]
    return [] if before == after else [pointer or "/"]


def append_only(before: list, after: list) -> tuple[bool, int]:
    # before、after 是提交尝试数组，检查 after 是否以 before 为前缀并返回新增数。
    if len(after) < len(before) or after[:len(before)] != before:
        return False, 0
    return True, len(after) - len(before)


def batch_problem(batch: dict, name: str | None) -> dict:
    # batch 是提交前批次对象；name 可选，返回唯一的题目对象并做身份校验。
    if batch.get("schema_version") != 2 or batch.get("chapter") != "03":
        raise ResultError("批次不是第 03 章 schema 2 对象")
    problems = batch.get("problems")
    if not isinstance(problems, list) or len(problems) != 1 or not isinstance(problems[0], dict):
        raise ResultError("提交前批次必须恰有一道题")
    problem = problems[0]
    if name is not None and problem.get("name") != name:
        raise ResultError("--only 与批次题目不一致")
    problem_id = problem.get("problem_id")
    scope = batch.get("authorization", {}).get("scope")
    if not isinstance(problem_id, str) or not isinstance(scope, dict):
        raise ResultError("批次缺少题目身份或授权范围")
    if scope.get("problem_id") != problem_id or problem_id != f"data_structure/{problem.get('name')}":
        raise ResultError("批次题目身份不一致")
    return problem


def validation_changes(before: dict, after: dict) -> list[str]:
    # before、after 是提交前后 validation，返回不允许的字段变化指针。
    result = []
    for key in sorted(set(before) | set(after)):
        if before.get(key) != after.get(key) and key not in VALIDATION_KEYS:
            result.append(f"/validation/{pointer_escape(key)}")
    return result


def result_fields(meta: dict, source_sha256: str) -> dict:
    # meta 是提交后的元数据，source_sha256 是当前源码哈希，提取当前线上结果字段。
    validation = meta.get("validation")
    attempts = meta.get("submission_attempts")
    if not isinstance(validation, dict) or not isinstance(attempts, list):
        raise ResultError("提交后元数据缺少 validation 或 submission_attempts")
    status = validation.get("status")
    allowed = {
        "online_ac",
        "online_failed",
        "online_stale",
        "submitted",
        "submission_unknown",
        "submission_error",
    }
    if status not in allowed:
        raise ResultError(f"提交后状态不是可报告在线状态：{status!r}")
    submission_id = validation.get("current_source_submission_id")
    verdict = validation.get("current_source_online_verdict")
    is_latest = validation.get("current_source_is_latest")
    matching = [
        attempt for attempt in attempts
        if isinstance(attempt, dict) and attempt.get("source_sha256") == source_sha256
    ]
    if status == "submission_unknown":
        submission_id = None
    if status == "submission_error":
        submission_id = None
    if submission_id is not None and type(submission_id) is not int:
        raise ResultError("在线提交编号不是整数或 null")
    if status == "submitted" and (submission_id is None or verdict is not None):
        raise ResultError("submitted 状态的提交编号或 verdict 不符合约束")
    if status in {"online_ac", "online_failed", "online_stale"}:
        if submission_id is None or not isinstance(verdict, str):
            raise ResultError("最终在线状态缺少提交编号或 verdict")
    if status == "online_ac":
        if not matching or not yosupo.attempt_is_ac(matching[-1], source_sha256):
            raise ResultError("online_ac 未对应当前源码的可认证 AC 尝试")
    return {
        "status": status,
        "submission_id": submission_id,
        "verdict": verdict,
        "is_latest": is_latest,
        "language": "cpp20" if matching else None,
        "time": validation.get("current_source_time"),
        "memory": validation.get("current_source_memory"),
        "submission_time": validation.get("current_source_submission_time"),
        "server_source_sha256": validation.get("server_source_sha256"),
    }


def make_result(root: Path, batch_path: Path, pre_meta_path: Path, name: str | None) -> dict:
    # root 是题解目录，读取单题批次和提交前元数据快照，生成提交后结果对象。
    batch = read_json(batch_path)
    problem = batch_problem(batch, name)
    batch_sha256 = canonical_sha256(batch)
    pre_meta = read_json(pre_meta_path)
    expected_pre_sha256 = problem.get("meta_pre_submit_sha256")
    if sha256_file(pre_meta_path) != expected_pre_sha256:
        raise ResultError("提交前元数据快照与批次 SHA256 不一致")
    source_sha256 = problem.get("source_sha256")
    source_path = root.parent / problem.get("source_path", "")
    meta_path = root.parent / problem.get("meta_path", "")
    audit_path = root.parent / batch.get("coverage_audit", {}).get("path", "")
    if not isinstance(source_sha256, str) or not source_path.is_file() or not meta_path.is_file() or not audit_path.is_file():
        raise ResultError("批次引用的源码、元数据或审计文件不存在")
    if sha256_file(source_path) != source_sha256:
        raise ResultError("当前源码哈希已变化")
    post_meta = read_json(meta_path)
    pre_attempts = pre_meta.get("submission_attempts")
    post_attempts = post_meta.get("submission_attempts")
    pre_validation = pre_meta.get("validation")
    post_validation = post_meta.get("validation")
    if not isinstance(pre_attempts, list) or not isinstance(post_attempts, list):
        raise ResultError("提交前后 submission_attempts 必须是数组")
    if not isinstance(pre_validation, dict) or not isinstance(post_validation, dict):
        raise ResultError("提交前后 validation 必须是对象")
    unchanged = []
    for key in sorted(set(pre_meta) | set(post_meta)):
        if key in {"validation", "submission_attempts"}:
            continue
        if pre_meta.get(key) != post_meta.get(key):
            unchanged.extend(changed_pointers(pre_meta.get(key), post_meta.get(key), f"/{pointer_escape(key)}"))
    unexpected = [*unchanged, *validation_changes(pre_validation, post_validation)]
    is_append_only, delta = append_only(pre_attempts, post_attempts)
    appended = post_attempts[len(pre_attempts):] if is_append_only else []
    if any(
        not isinstance(attempt, dict) or attempt.get("source_sha256") != source_sha256
        for attempt in appended
    ):
        unexpected.append("/submission_attempts")
    ids = [attempt.get("submission_id") for attempt in appended if isinstance(attempt, dict) and attempt.get("submission_id") is not None]
    if len(ids) > 1 or any(type(value) is not int for value in ids):
        unexpected.append("/submission_attempts")
    result = result_fields(post_meta, source_sha256)
    if result["status"] == "online_ac" and result["server_source_sha256"] != source_sha256:
        unexpected.append("/validation/server_source_sha256")
    return {
        "schema_version": 1,
        "kind": "yosupo_single_problem_result",
        "batch_sha256": batch_sha256,
        "problem_id": problem["problem_id"],
        "source_sha256": source_sha256,
        "meta_pre_submit_sha256": expected_pre_sha256,
        "meta_post_submit_sha256": sha256_file(meta_path),
        "coverage_audit_post_sha256": sha256_file(audit_path),
        "meta_change_check": {
            "unchanged_outside_validation_and_submission_attempts": not unchanged,
            "submission_attempts_are_append_only": is_append_only,
            "submission_attempt_delta": delta,
            "unexpected_json_pointers": sorted(set(unexpected)),
        },
        "result": result,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description="生成第 03 章单题在线结果对象")
    parser.add_argument("--batch", required=True, type=Path, help="提交前单题批次 JSON")
    parser.add_argument("--pre-meta", required=True, type=Path, help="提交前 meta.json 临时快照")
    parser.add_argument("--only", help="与批次一致的唯一题目名")
    parser.add_argument(
        "--output",
        type=Path,
        help="将结果 JSON 直接以 UTF-8/LF 写入 .local 内的状态文件",
    )
    args = parser.parse_args()
    try:
        root = Path(__file__).resolve().parent.parent
        batch_path = local_path(root, args.batch, "提交前批次")
        pre_meta_path = local_path(root, args.pre_meta, "提交前元数据快照")
        result = make_result(root, batch_path, pre_meta_path, args.only)
        if args.output is not None:
            target = local_path(root, args.output, "结果输出路径")
            target.parent.mkdir(parents=True, exist_ok=True)
            yosupo.atomic_write_json(target, result)
    except (OSError, ValueError, ResultError, yosupo.SubmissionError) as error:
        sys.exit(str(error))
    if args.output is not None:
        print(f"RESULT WRITTEN {local_path(root, args.output, '结果输出路径')}")
    else:
        print(json.dumps(result, ensure_ascii=False, indent=2))
    print(f"RESULT_SHA256 {canonical_sha256(result)}", file=sys.stderr)


if __name__ == "__main__":
    main()
