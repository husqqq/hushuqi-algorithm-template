from __future__ import annotations

import hashlib
import json
import re
import shutil
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
MAPPING = ROOT / "LC_TEMPLATE_MAPPING.tsv"
MANIFEST = ROOT / "manifest.json"
RECEIPT = ROOT / ".local" / "yosupo-local-verification.json"
SCOPED_RECEIPTS = ROOT / ".local" / "scoped-receipts"
OUT = ROOT / "不可完全置信清单"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"


def file_sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def json_sha256(data: object) -> str:
    raw = json.dumps(data, ensure_ascii=False, sort_keys=True,
                     separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(raw).hexdigest()


def read_json(path: Path) -> dict:
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"{path} 的根节点不是对象")
    return data


def read_mapping() -> list[dict[str, str]]:
    rows = []
    for line_no, raw in enumerate(MAPPING.read_text(encoding="utf-8").splitlines(), 1):
        if not raw or raw.startswith("#"):
            continue
        fields = raw.split("\t")
        if len(fields) != 4:
            raise ValueError(f"{MAPPING.name}:{line_no}: 不是四列 TSV")
        chapter, item, problem, confidence = fields
        rows.append({"chapter": chapter, "template_item": item,
                     "problem": problem, "mapping_confidence": confidence})
    return rows


def source_entries(manifest: dict) -> dict[str, dict]:
    result = {}
    for entry in manifest.get("problems", []):
        if not isinstance(entry, dict):
            continue
        category, name = entry.get("category"), entry.get("name")
        if not isinstance(category, str) or not isinstance(name, str):
            continue
        problem = f"{category}/{name}"
        if problem in result:
            raise ValueError(f"manifest 重复题目：{problem}")
        result[problem] = entry
    return result


def receipt_records(manifest: dict) -> dict[str, dict]:
    """Return valid default records plus each named scoped receipt's own item."""
    def records_from(path: Path, require_manifest: bool) -> dict[str, dict]:
        if not path.is_file():
            return {}
        receipt = read_json(path)
        current = (
            receipt.get("status") == "passed"
            and receipt.get("official_repository") == {"commit": COMMIT}
            and (not require_manifest
                 or receipt.get("manifest_sha256") == json_sha256(manifest))
        )
        records = receipt.get("problems")
        return records if current and isinstance(records, dict) else {}

    # 默认收据覆盖整份清单，必须与当前 manifest 绑定。单题 scoped
    # 收据的有效性由下方逐项核对的当前源码 SHA 和官方 commit 决定；
    # 其他题写回线上元数据不应使未改动题目的本地证据失效。
    records = records_from(RECEIPT, require_manifest=True)
    if SCOPED_RECEIPTS.is_dir():
        for path in sorted(SCOPED_RECEIPTS.glob("*.json")):
            for name, record in records_from(path, require_manifest=False).items():
                if not isinstance(record, dict):
                    continue
                previous = records.get(name)
                if (
                    not isinstance(previous, dict)
                    or record.get("verified_at", "") >= previous.get("verified_at", "")
                ):
                    records[name] = record
    return records


def inspect_row(row: dict[str, str], entries: dict[str, dict],
                receipts: dict[str, dict]) -> dict[str, str]:
    reasons = []
    if row["mapping_confidence"] != "audited":
        reasons.append("mapping_review_pending")

    entry = entries.get(row["problem"])
    source_state = local_state = online_state = "missing"
    submission_id = source_sha = "null"
    if entry is None:
        reasons.extend(["source_missing", "verification_missing"])
    else:
        source = entry.get("source")
        declared_sha = entry.get("source_sha256")
        source_path = ROOT / source if isinstance(source, str) else None
        if (source_path is not None and source_path.is_file()
                and isinstance(declared_sha, str)
                and file_sha256(source_path) == declared_sha):
            source_state, source_sha = "current", declared_sha
        else:
            source_state = "stale"
            reasons.append("source_stale")

        record = receipts.get(entry.get("name"))
        if (source_state == "current" and isinstance(record, dict)
                and record.get("status") == "passed"
                and record.get("official_commit") == COMMIT
                and record.get("source_sha256") == source_sha):
            local_state = "current_verified"

        meta_path = ROOT / entry.get("meta", "")
        meta = read_json(meta_path) if meta_path.is_file() else None
        validation = meta.get("validation") if isinstance(meta, dict) else None
        if isinstance(validation, dict) and validation.get("status") == "online_ac":
            if (source_state == "current"
                    and validation.get("current_source_sha256") == source_sha
                    and validation.get("current_source_online_verdict") == "AC"
                    and validation.get("current_source_is_latest") is True
                    and validation.get("server_source_sha256") == source_sha
                    and type(validation.get("current_source_submission_id")) is int):
                online_state = "current_ac"
                submission_id = str(validation["current_source_submission_id"])
            else:
                online_state = "stale"

        # 本地官方 checker 与同哈希线上 AC 都能独立证明当前源码正确。
        # 提交前仍由提交器强制检查本地收据；这里只判定事后置信状态。
        if local_state != "current_verified" and online_state != "current_ac":
            reasons.append("verification_missing")

    return row | {"source_state": source_state, "local_state": local_state,
                  "online_state": online_state, "submission_id": submission_id,
                  "source_sha256": source_sha,
                  "reasons": ",".join(dict.fromkeys(reasons)) or "fully_trusted"}


def markdown_table(rows: list[dict[str, str]]) -> str:
    lines = [
        "| 模板条目 | LC 题目 | 映射 | 源码 | 本地 | 在线 | 未完成原因 |",
        "|---|---|---|---|---|---|---|",
    ]
    for row in rows:
        lines.append(
            f"| `{row['template_item']}` | `{row['problem']}` | "
            f"`{row['mapping_confidence']}` | `{row['source_state']}` | "
            f"`{row['local_state']}` | `{row['online_state']}` | "
            f"`{row['reasons']}` |"
        )
    return "\n".join(lines)


def main() -> None:
    manifest = read_json(MANIFEST)
    if manifest.get("official_repository", {}).get("commit") != COMMIT:
        raise ValueError("manifest 的官方题库 commit 与清单生成器不一致")
    entries = source_entries(manifest)
    receipts = receipt_records(manifest)
    rows = [inspect_row(row, entries, receipts)
            for row in read_mapping()]
    rows.sort(key=lambda row: (row["chapter"], row["template_item"], row["problem"]))
    untrusted = [row for row in rows if row["reasons"] != "fully_trusted"]
    if not untrusted:
        if OUT.is_dir():
            shutil.rmtree(OUT)
        print(f"PASS: {len(rows)} routed; all fully trusted; removed {OUT}")
        return

    by_chapter = defaultdict(list)
    for row in untrusted:
        by_chapter[row["chapter"]].append(row)

    OUT.mkdir(parents=True, exist_ok=True)
    headers = ["chapter", "template_item", "problem", "mapping_confidence",
               "source_state", "local_state", "online_state", "submission_id",
               "source_sha256", "reasons"]
    all_tsv = ["\t".join(headers)]
    all_tsv.extend("\t".join(row[key] for key in headers) for row in untrusted)
    (OUT / "ALL.tsv").write_text("\n".join(all_tsv) + "\n", encoding="utf-8")
    no_online = [row for row in rows if row["online_state"] != "current_ac"]
    no_online_tsv = ["\t".join(headers)]
    no_online_tsv.extend("\t".join(row[key] for key in headers) for row in no_online)
    (OUT / "NO_CURRENT_ONLINE.tsv").write_text(
        "\n".join(no_online_tsv) + "\n", encoding="utf-8"
    )

    total_by_chapter = Counter(row["chapter"] for row in rows)
    chapters = [f"{chapter:02d}" for chapter in range(1, 15)]
    chapters.extend(sorted(set(total_by_chapter) - set(chapters)))
    summary = ["# 不可完全置信清单汇总", "",
               f"固定官方题库 commit：`{COMMIT}`。本目录由 `tools/refresh_untrusted_inventory.py` 生成。",
               "", "| 章节 | TSV 总数 | 无当前线上 AC | 不可完全置信 | 已满足全部门禁 |",
               "|---|---:|---:|---:|---:|"]
    for chapter in chapters:
        bad, total = len(by_chapter[chapter]), total_by_chapter[chapter]
        no_online_count = sum(row["chapter"] == chapter for row in no_online)
        summary.append(
            f"| {chapter} | {total} | {no_online_count} | {bad} | {total - bad} |"
        )
    summary.extend([f"| 合计 | {len(rows)} | {len(no_online)} | {len(untrusted)} | {len(rows) - len(untrusted)} |",
                    "",
                    "“可完全置信”要求映射为 `audited`、源码为当前版本，并至少具备以下一种当前证据：官方本地验证收据，或 `is_latest=true` 且服务端源码 SHA256 与磁盘源码一致的线上 AC。"])
    (OUT / "SUMMARY.md").write_text("\n".join(summary) + "\n", encoding="utf-8")

    for chapter in chapters:
        chapter_rows = by_chapter[chapter]
        text = [f"# 第 {chapter} 章不可完全置信清单", "",
                f"权威范围来自 `../LC_TEMPLATE_MAPPING.tsv` 中 `chapter={chapter}` 的全部行。当前冻结映射没有缺口文件。", ""]
        text.append(markdown_table(chapter_rows) if chapter_rows
                    else "本章当前没有不可完全置信项目。")
        (OUT / f"chapter-{chapter}.md").write_text("\n".join(text) + "\n",
                                                        encoding="utf-8")

    readme = ["# 不可完全置信清单", "",
              "本目录只保存状态报告，不保存第二份题解源码。收到章节任务时仍必须先读取 `../LC_TEMPLATE_MAPPING.tsv`；这里用于快速定位映射、测试、候选或在线闭环尚未完成的项目，不能反向定义章节范围。", "",
              "运行以下命令刷新：", "", "```powershell",
              "python library-checker-template-solutions/tools/refresh_untrusted_inventory.py",
              "```", "", "状态含义：", "",
              "- `mapping_review_pending`：TSV 仅为 `classified`，尚无源码级映射审计。",
              "- `source_missing/source_stale`：没有当前唯一 `main.cpp`，或清单哈希与磁盘不一致。",
              "- `verification_missing`：当前源码既没有官方本地验证收据，也没有可认证的同哈希线上 AC。", "",
              "`NO_CURRENT_ONLINE.tsv` 精确保存当前没有可认证线上 AC 的全部题；`ALL.tsv` 保存仍缺映射、源码或任一当前正确性证据的项目。`SUMMARY.md` 给出按章计数，`chapter-NN.md` 是收到第 NN 章任务时的未完成视图。全部项目可置信时，本目录会被自动删除；任何源码、manifest、meta、收据、映射或官方 commit 改变后都必须重新生成。"]
    (OUT / "README.md").write_text("\n".join(readme) + "\n", encoding="utf-8")
    print(f"PASS: {len(rows)} routed; {len(untrusted)} not fully trusted; output {OUT}")


if __name__ == "__main__":
    main()
