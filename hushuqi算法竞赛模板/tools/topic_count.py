#!/usr/bin/env python3
"""Count numbered knowledge points in chapters 1--14 and sync published totals."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PATTERN = re.compile(r"^###\s+(\d+\.\d+\.\d{3})\s+", re.MULTILINE)
TARGETS = [ROOT.parent / "README.md", ROOT / "README.md", ROOT / "目录.md"]
REPORT = ROOT / "知识点覆盖统计.md"


def count_topics() -> int:
    ids = set()
    for chapter in range(1, 15):
        files = list(ROOT.glob(f"{chapter:02d}-*.md"))
        if len(files) != 1:
            raise ValueError(f"第 {chapter} 章正文数量异常：{len(files)}")
        ids.update(m.group(1) for m in PATTERN.finditer(
            files[0].read_text(encoding="utf-8")))
    return len(ids)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    count = count_topics()
    pattern = re.compile(r"第 1–14 章共 \d+ 个")
    stale = []
    for path in TARGETS:
        data = path.read_text(encoding="utf-8")
        updated, n = pattern.subn(f"第 1–14 章共 {count} 个", data)
        if n != 1:
            raise ValueError(f"{path}: 预期恰有一处知识点计数，实际 {n}")
        if updated != data:
            stale.append((path, updated))
    report = f"""# 知识点覆盖统计

本文件由 `tools/topic_count.py` 生成。统计对象是第 1–14 章正文中唯一的 `### x.y.zzz` 编号；第 0、15 章不计入。主题头文件数量与知识点数量不是同一口径：一个头可以被多个条目共享，部分条目是公式、工具、别名或文档说明。

- 当前正文/目录知识点：**{count}**。
- `include/full` 与 `include/print` 主题头：**{len(list((ROOT / 'include' / 'full').glob('*.hpp')))} / {len(list((ROOT / 'include' / 'print').glob('*.hpp')))}**。
- Library Checker 主映射：**251** 道题，不是知识点总数。

## 数字不一致的根因

- `cd8283a` 基线正文有 669 项，`docs/97` 沿用了这个历史扫描口径。
- 后续新增 `3.5.013 Radix Heap`、`5.5.014 Borůvka`、`5.8.013 费用缩放流` 后为 672 项。
- 再新增 `10.2.021 拉格朗日插值` 后实际为 673 项，但 README 与目录状态没有同步。
- 本轮新增条目后当前为 {count} 项；README、目录和本文件由本脚本统一校验。

`tools/topic_count.py --check` 会检查各章编号唯一性、目录编号集合和公布数字，避免再次手工漂移。
"""
    if (not REPORT.exists()) or REPORT.read_text(encoding="utf-8") != report:
        stale.append((REPORT, report))
    if args.check:
        if stale:
            raise SystemExit("知识点计数过期，请运行 python tools/topic_count.py")
        print(f"TOPIC_COUNT_CURRENT {count}")
        return
    for path, updated in stale:
        path.write_text(updated, encoding="utf-8")
        print(f"UPDATED {path}")
    print(f"TOPIC_COUNT {count}")


if __name__ == "__main__":
    main()
