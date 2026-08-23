#!/usr/bin/env python3
"""Submit one Python template variant at a time after local verification."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

import submit_yosupo as yosupo
import verify_variants as variants


ROOT = variants.ROOT
PROFILE = ROOT / ".local" / "yosupo-profile"


def write_meta(path: Path, meta: dict) -> None:
    yosupo.atomic_write_json(path, meta)


def submit_one(entry: dict, prepared: yosupo.PreparedSource, timeout: float, interval: float) -> bool:
    manifest = variants.read_manifest()
    variants.require_receipt(manifest, entry, prepared)
    meta = json.loads(prepared.item.meta_path.read_text(encoding="utf-8"))
    attempts = meta.setdefault("submission_attempts", [])
    if not isinstance(attempts, list):
        raise yosupo.SubmissionError("submission_attempts must be an array")
    for old in reversed(attempts):
        if (
            isinstance(old, dict)
            and old.get("source_sha256") == prepared.source_sha256
            and old.get("lang") == prepared.item.language
            and old.get("verdict") == "AC"
            and old.get("is_latest") is True
            and old.get("server_source_sha256") == prepared.source_sha256
        ):
            print(f"SKIP {entry['variant_id']}: already online AC as {old.get('submission_id')}")
            return True
        if (
            isinstance(old, dict)
            and old.get("source_sha256") == prepared.source_sha256
            and old.get("submission_id") is not None
            and old.get("verdict") is None
        ):
            raise yosupo.SubmissionError(
                f"{entry['variant_id']}: existing submitted id {old['submission_id']} must be polled first"
            )

    token = yosupo.browser_auth(PROFILE)
    yosupo.check_language(yosupo.API, prepared.item.language)
    yosupo.check_current(prepared)
    submission_id = yosupo.submit(
        yosupo.API,
        token,
        prepared.item.name,
        prepared.source,
        prepared.item.language,
    )
    submitted_at = yosupo.utc_now()
    attempt = {
        "submission_id": submission_id,
        "source_sha256": prepared.source_sha256,
        "server_source_sha256": None,
        "lang": prepared.item.language,
        "verdict": None,
        "last_status": "SUBMITTED",
        "time": None,
        "memory": None,
        "submission_time": None,
        "is_latest": None,
        "compile_standard": None,
        "submitted_at": submitted_at,
        "recorded_at": submitted_at,
    }
    attempts.append(attempt)
    write_meta(prepared.item.meta_path, meta)
    print(f"SUBMITTED {entry['variant_id']}: submission {submission_id}")
    data = yosupo.wait_submission(
        yosupo.API,
        prepared,
        submission_id,
        timeout=timeout,
        interval=interval,
    )
    overview, server_hash = yosupo.validate_submission(data, prepared, submission_id)
    attempt.update({
        "server_source_sha256": server_hash,
        "verdict": overview.get("status"),
        "last_status": overview.get("status"),
        "time": overview.get("time"),
        "memory": overview.get("memory"),
        "submission_time": overview.get("submission_time"),
        "is_latest": overview.get("is_latest"),
        "recorded_at": yosupo.utc_now(),
    })
    validation = meta.setdefault("validation", {})
    validation.update({
        "status": (
            "online_ac"
            if attempt["verdict"] == "AC"
            and attempt["is_latest"] is True
            and server_hash == prepared.source_sha256
            else "online_failed"
        ),
        "checked_at": yosupo.utc_now(),
        "current_source_sha256": prepared.source_sha256,
        "current_source_online_verdict": attempt["verdict"],
        "current_source_submission_id": submission_id,
        "current_source_time": attempt["time"],
        "current_source_memory": attempt["memory"],
        "current_source_submission_time": attempt["submission_time"],
        "current_source_is_latest": attempt["is_latest"],
        "server_source_sha256": server_hash,
        "note": "当前变体源码已通过 Library Checker 在线评测。"
        if attempt["verdict"] == "AC" and attempt["is_latest"] is True
        else "当前变体在线评测未通过。",
    })
    write_meta(prepared.item.meta_path, meta)
    print(
        f"VERDICT {entry['variant_id']}: {attempt['verdict']}, "
        f"time {attempt['time']} s, memory {attempt['memory']} B"
    )
    return validation["status"] == "online_ac"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--only", required=True, help="one variant id")
    parser.add_argument("--judge-timeout", type=float, default=1800.0)
    parser.add_argument("--poll-interval", type=float, default=3.0)
    args = parser.parse_args()
    if "," in args.only:
        parser.error("受控流程每次只允许提交一个变体")
    try:
        manifest = variants.read_manifest()
        items = variants.selected(manifest, {args.only})
        entry, prepared = items[0]
        if not submit_one(entry, prepared, args.judge_timeout, args.poll_interval):
            raise SystemExit(2)
    except (variants.local.LocalVerifyError, yosupo.SubmissionError, OSError, json.JSONDecodeError) as error:
        sys.exit(str(error))


if __name__ == "__main__":
    main()
