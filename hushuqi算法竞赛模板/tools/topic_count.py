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
    topics = {}
    for chapter in range(1, 15):
        files = list(ROOT.glob(f"{chapter:02d}-*.md"))
        if len(files) != 1:
            raise ValueError(f"第 {chapter} 章正文数量异常：{len(files)}")
        data = files[0].read_text(encoding="utf-8")
        section = None
        last = (0, 0, 0)
        fence = None
        for line in data.splitlines():
            marker = re.match(r"^ {0,3}(`{3,}|~{3,})(.*)$", line)
            if marker:
                run, tail = marker.groups()
                if fence is None:
                    fence = run
                elif run[0] == fence[0] and len(run) >= len(fence) and not tail.strip():
                    fence = None
                continue
            if fence:
                continue
            match = re.match(r"^## (\d+)\.(\d+)\s+", line)
            if match:
                section = tuple(map(int, match.groups()))
                if section[0] != chapter:
                    raise ValueError(f"{files[0].name}: 错误小节编号 {match[0]}")
            match = re.match(r"^### (\d+\.\d+\.\d{3})\s+(.+?)\s*$", line)
            if not match:
                continue
            num, title = match.groups()
            key = tuple(map(int, num.split('.')))
            if key[:2] != section or key <= last or num in topics:
                raise ValueError(f"{files[0].name}: 编号重复、乱序或父小节错误：{num}")
            topics[num] = title
            last = key
    entries = re.findall(r"^- \*\*(\d+\.\d+\.\d{3})\*\* (.+?)\s*$",
                         (ROOT / '目录.md').read_text(encoding='utf-8'), re.M)
    entries = [(num, title) for num, title in entries
               if 1 <= int(num.split('.')[0]) <= 14]
    directory = dict(entries)
    if len(directory) != len(entries):
        raise ValueError("目录包含重复知识点编号")
    normalized = {num: re.sub(r"（兼容索引，正文见 1\.1\.008）$", "", title)
                  for num, title in directory.items()}
    if set(topics) != set(normalized):
        missing = topics.keys() ^ directory.keys()
        raise ValueError(f"目录与正文编号不一致：{sorted(missing)}")
    titles = [num for num in topics if topics[num] != normalized[num]]
    if titles:
        raise ValueError(f"目录与正文标题不一致：{titles}")
    return len(topics)


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
