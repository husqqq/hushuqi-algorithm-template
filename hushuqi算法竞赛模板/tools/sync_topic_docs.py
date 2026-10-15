#!/usr/bin/env python3
"""Refresh the first C++ block of selected topics from generated print headers."""
from __future__ import annotations
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOPIC_HEADING = re.compile(r"^###\s+(\d+\.\d+\.\d{3})\s+.*$", re.M)
ANY_HEADING = re.compile(r"^#{1,3} .*$", re.M)
# 目录中的「编号 → 标题」，用于把依赖头改写成可读的注释。
TITLES = dict(re.findall(r"^- \*\*(\d+\.\d+\.\d{3})\*\* (.+?)\s*$",
                         (ROOT / "目录.md").read_text(encoding="utf-8"), re.M))
# 这些知识点的第一个代码块由 include/print/<编号>.hpp 生成；正文散文与自测代码手工维护。
SYNCED = {
    "2.1.010", "2.2.015", "3.4.013", "4.1.013", "4.3.012", "5.2.013", "5.8.014", "5.9.009",
    "5.10.017", "5.10.018", "6.4.007", "7.4.009", "9.1.013", "9.1.014", "9.3.008", "10.2.022",
    "12.1.011", "12.3.007", "6.4.006", "7.5.003", "7.4.008",
}

def snippet(name: str) -> str:
    data = (ROOT / "include/print" / name).read_text(encoding="utf-8")
    data = re.sub(r'^#pragma once\s*\n', '', data, flags=re.M)
    data = re.sub(r'^#include (?:<[^>]+>|"support/(?:checks|base|topic_common)\.hpp")\s*\n', '', data, flags=re.M)
    # 头文件保护宏只用于仓库内重复包含，正文不印。
    guard = re.match(r'\s*#ifndef (\w+)\n#define \1\n', data)
    if guard:
        data = data[guard.end():]
        data = re.sub(r'\n#endif\s*$', '\n', data)
    data = re.sub(r'^using namespace std;\s*\n|^#define int long long\s*\n', '', data, flags=re.M)
    # 打印版不出现仓库路径：依赖的头改写成「依赖 编号 标题」注释。
    data = re.sub(r'^#include "(\d+)_(\d+)_(\d+)\.hpp"',
                  lambda m: f"// 依赖 {m[1]}.{m[2]}.{m[3]} {TITLES.get(f'{m[1]}.{m[2]}.{m[3]}', '').replace('`', '')}".rstrip(),
                  data, flags=re.M)
    return "```cpp\n" + data.strip() + "\n```"

def sync_topics(data: str) -> tuple[str, set[str]]:
    seen = set()
    heads = list(ANY_HEADING.finditer(data))
    for i in range(len(heads) - 1, -1, -1):
        match = TOPIC_HEADING.match(heads[i][0])
        if not match or match[1] not in SYNCED:
            continue
        topic = match[1]
        start, end = heads[i].start(), heads[i + 1].start() if i + 1 < len(heads) else len(data)
        section = data[start:end]
        fence = re.search(r"```cpp\n.*?\n```", section, re.S)
        if not fence:
            raise ValueError(f"{topic}: missing C++ block")
        code = snippet(topic.replace(".", "_") + ".hpp")
        section = section[:fence.start()] + code + section[fence.end():]
        data = data[:start] + section + data[end:]
        seen.add(topic)
    return data, seen

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    changed, seen = [], set()
    for path in sorted(ROOT.glob("[0-9][0-9]-*.md")):
        data = path.read_text(encoding="utf-8")
        updated, found = sync_topics(data)
        seen |= found
        if data != updated:
            changed.append(path.name)
            if not args.check:
                path.write_text(updated, encoding="utf-8", newline="")
    if SYNCED - seen:
        raise SystemExit("missing synced topics: " + ", ".join(sorted(SYNCED - seen)))
    if args.check and changed:
        raise SystemExit("stale chapter snippets: " + ", ".join(changed))
    print("TOPIC_DOCS_CURRENT" if args.check else f"updated {len(changed)} chapter snippets")

if __name__ == "__main__":
    main()
