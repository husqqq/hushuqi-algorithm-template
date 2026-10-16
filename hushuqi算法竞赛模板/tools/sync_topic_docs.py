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
    "13.1.002", "13.1.004", "13.1.007", "13.1.008", "13.1.010", "13.1.011", "13.1.012",
    "13.2.003", "13.2.004", "13.3.003", "13.3.004",
    "1.1.008", "2.1.009", "2.1.010", "2.2.002", "2.2.015", "3.1.001", "3.1.004", "3.1.005",
    "3.1.006", "3.1.007", "3.1.008", "3.2.005", "3.2.006", "3.2.008", "3.2.009", "3.2.010",
    "3.3.001", "3.3.002", "3.3.003", "3.3.004", "3.3.005", "3.3.009", "3.3.010", "3.3.011",
    "3.3.012", "3.3.017", "3.3.018", "3.3.019", "3.3.021", "3.4.003", "3.4.004", "3.4.008",
    "3.4.011", "3.4.013", "3.5.001", "3.5.002", "3.5.003", "3.5.004", "3.5.006", "3.5.007", "3.5.008",
    "3.5.009", "3.5.010", "3.5.011", "3.5.012", "3.5.013", "3.6.001", "3.6.002", "3.6.003",
    "3.6.007", "3.6.009", "3.6.012", "3.6.013", "3.6.014", "3.7.001", "3.7.003", "3.7.005",
    "3.7.007", "3.7.008", "3.7.009", "3.7.010", "3.8.001", "3.8.003", "3.10.001", "3.10.002",
    "3.10.003", "3.10.004", "4.1.003", "4.1.004", "4.1.005", "4.1.006", "4.1.007", "4.1.009", "4.1.011",
    "4.1.012", "4.1.013", "4.2.002", "4.3.002", "4.3.004", "4.3.005", "4.3.008", "4.3.010", "4.3.011", "4.3.012",
    "4.4.001", "4.4.002", "4.4.003", "4.4.004", "4.5.004", "4.5.007", "5.1.001", "5.1.007",
    "5.2.011", "5.2.013", "5.3.010", "5.3.011", "5.3.013", "5.4.001", "5.4.002", "5.4.003",
    "5.5.002", "5.5.003", "5.5.004", "5.5.005", "5.5.006", "5.5.010", "5.5.012", "5.6.001",
    "5.6.002", "5.6.003", "5.6.004", "5.6.005", "5.6.006", "5.6.007", "5.6.008", "5.6.009",
    "5.6.010", "5.7.001", "5.7.002", "5.7.005", "5.7.007", "5.8.001", "5.8.002", "5.8.003",
    "5.8.004", "5.8.006", "5.8.007", "5.8.008", "5.8.009", "5.8.010", "5.8.012", "5.8.013",
    "5.8.014", "5.9.001", "5.9.003", "5.9.005", "5.9.006", "5.9.008", "5.9.009", "5.10.001",
    "5.10.002", "5.10.003", "5.10.004", "5.10.005", "5.10.006", "5.10.010", "5.10.011", "5.10.012",
    "5.10.013", "5.10.014", "5.10.015", "5.10.017", "5.10.018", "6.1.011", "6.3.001", "6.3.005",
    "6.4.006", "6.4.007", "6.5.004", "6.5.005", "6.5.006", "7.1.008", "7.1.010", "7.1.014",
    "7.1.017", "7.2.001", "7.3.003", "7.3.005", "7.4.007", "7.4.008", "7.4.009", "7.5.001",
    "7.5.003", "8.1.001", "8.1.002", "8.1.006", "8.1.007", "8.1.008", "8.1.009", "8.1.010",
    "8.2.002", "8.2.003", "8.2.006", "8.2.011", "8.2.012", "8.2.013", "8.2.016", "8.2.017",
    "8.3.002", "8.3.003", "8.3.004", "8.3.005", "9.1.003", "9.1.004", "9.1.013", "9.1.014",
    "9.2.007", "9.2.009", "9.3.001", "9.3.002", "9.3.008", "10.1.001", "10.1.007", "10.1.008",
    "10.1.015", "10.2.017", "10.2.021", "10.2.022", "10.3.001", "10.3.002", "10.3.003", "10.3.004",
    "10.4.003", "11.1.001", "11.1.002", "11.1.003", "11.1.005", "11.1.006", "11.1.007", "11.1.008",
    "11.1.009", "11.1.010", "11.1.011", "11.1.012", "11.2.002", "11.2.006", "11.2.007", "11.2.011",
    "11.2.012", "11.3.002", "11.3.004", "11.3.005", "11.3.007", "11.3.008", "11.3.009", "11.3.011",
    "11.3.012", "11.3.013", "11.3.014", "11.4.003", "11.4.005", "11.4.008", "11.4.009", "11.4.011",
    "11.5.001", "11.5.002", "11.5.003", "11.5.005", "11.5.006", "11.5.007", "11.6.001", "11.6.002",
    "11.6.003", "11.6.004", "11.6.005", "11.6.008", "11.6.009", "12.1.011", "12.3.006", "12.3.007",
    "2.4.007", "7.3.014", "7.3.015", "8.1.013", "8.2.018", "13.1.001", "13.2.005", "13.3.009", "14.1.001", "14.1.006", "14.1.008", "14.1.020", "14.1.021",
}

def snippet(name: str) -> str:
    data = (ROOT / "include/print" / name).read_text(encoding="utf-8")
    if name == "3_4_011.hpp":
        data = data.replace('#include "support/offline.hpp"',
                            '// 依赖 2.2.002 离散化与坐标压缩\n// 依赖 2.3.001 普通莫队')
    data = re.sub(r'^#pragma once\s*\n', '', data, flags=re.M)
    data = re.sub(r'^#include (?:<[^>]+>|"support/(?:checks|base|topic_common)\.hpp")\s*\n', '', data, flags=re.M)
    # 头文件保护宏只用于仓库内重复包含，正文不印。
    guard = re.match(r'\s*#ifndef (\w+)\n#define \1\n', data)
    if guard:
        data = data[guard.end():]
        data = re.sub(r'\n#endif\s*$', '\n', data)
    data = re.sub(r'^using namespace std;\s*\n|^#define int long long\s*\n', '', data, flags=re.M)
    # 打印版不出现仓库路径：依赖的头改写成「依赖 编号 标题」注释。
    data = re.sub(r'^#include "support/[^"\n]+" // 正文依赖 (\d+\.\d+\.\d{3})',
                  lambda m: f"// 依赖 {m[1]} {TITLES[m[1]].replace('`', '')}", data, flags=re.M)
    data = re.sub(r'^#include "(\d+)_(\d+)_(\d+)\.hpp"(?: // 正文依赖 (\d+\.\d+\.\d{3}))?',
                  lambda m: f"// 依赖 {m[4] or f'{m[1]}.{m[2]}.{m[3]}'} {TITLES.get(m[4] or f'{m[1]}.{m[2]}.{m[3]}', '').replace('`', '')}".rstrip(),
                  data, flags=re.M)
    # 依赖注释与代码之间不留空行，与手写正文的格式一致。
    data = re.sub(r'^(// 依赖 .*\n)\n+', r'\1', data.strip(), flags=re.M)
    # 手写接口说明已覆盖同一依赖时，不再重复打印由 include 生成的通用说明。
    data = re.sub(r'^// 依赖 (\d+\.\d+\.\d{3})[^\n]*\n(?=// 依赖 \1\b)',
                  '', data, flags=re.M)
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
