#!/usr/bin/env python3
"""Generate the QOJ 3936 template catalogue from the repository's tested sources."""

from __future__ import annotations

import json
import re
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "qoj模板代码"
TEMPLATE_ROOT = ROOT / "hushuqi算法竞赛模板" / "include" / "full"
LC_ROOT = ROOT / "library-checker-template-solutions"
MANIFEST = CATALOG / "manifest.json"

LOCAL_INCLUDE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')


def resolve_include(current: Path, include: str) -> Path:
    candidate = (current.parent / include).resolve()
    if candidate.is_file():
        return candidate
    candidate = (TEMPLATE_ROOT / include).resolve()
    if candidate.is_file():
        return candidate
    support = (TEMPLATE_ROOT.parent / include.removeprefix("../")).resolve()
    if support.is_file():
        return support
    raise FileNotFoundError(f"cannot resolve {include!r} from {current}")


def bundle(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    output: list[str] = []
    for line in path.read_text(encoding="utf-8").splitlines():
        match = LOCAL_INCLUDE.match(line)
        if match:
            output.append(bundle(resolve_include(path, match.group(1)), seen))
        elif line.strip() != "#pragma once":
            output.append(line)
    return "\n".join(output).rstrip() + "\n"


def safe_name(label: str, title: str) -> str:
    cleaned = re.sub(r'[<>:"/\\|?*]', "_", title).strip().rstrip(".")
    return f"{label}_{cleaned}"


def write_problem(entry: dict[str, object]) -> None:
    destination = CATALOG / safe_name(str(entry["label"]), str(entry["title"]))
    destination.mkdir(parents=True, exist_ok=True)

    topics = [str(topic) for topic in entry.get("topics", [])]
    pieces = [
        "// Generated from hushuqi算法竞赛模板. Do not edit by hand.\n",
        "// QOJ contest 3936: " + str(entry["label"]) + " " + str(entry["title"]) + "\n",
    ]
    seen: set[Path] = set()
    for topic in topics:
        pieces.append(bundle(TEMPLATE_ROOT / f"{topic.replace('.', '_')}.hpp", seen))
    (destination / "template.hpp").write_text("\n".join(pieces), encoding="utf-8", newline="\n")

    source = entry.get("lc_source")
    if source:
        shutil.copyfile(LC_ROOT / str(source) / "main.cpp", destination / "main.cpp")

    status = "附同题型的已验证 main.cpp；提交 QOJ 前仍需核对输入输出" if source else "提供自包含算法模板，仍需按 QOJ 题面补适配层"
    readme = (
        f"# {entry['label']}. {entry['title']}\n\n"
        f"- QOJ: {entry['url']}\n"
        f"- 状态：{status}\n"
        f"- 模板条目：{', '.join(topics) if topics else '暂无完全对应条目'}\n"
    )
    if entry.get("note"):
        readme += f"- 备注：{entry['note']}\n"
    (destination / "README.md").write_text(readme, encoding="utf-8", newline="\n")


def write_index(problems: list[dict[str, object]]) -> None:
    ready = sum(bool(problem.get("lc_source")) for problem in problems)
    lines = [
        "# QOJ 3936 模板代码",
        "",
        "本目录对应 [XXIV 赛前模板训练赛](https://qoj.ac/contest/3936) 的 75 道题。",
        "每题目录中的 `template.hpp` 是从本仓库权威模板依赖展开得到的自包含代码；",
        "存在 `main.cpp` 时，它来自 `library-checker-template-solutions` 中同题型、已验证的独立提交源码。",
        "",
        f"当前共有 {ready} 道附同题型已验证实现，{len(problems) - ready} 道仅提供算法模板。",
        "`main.cpp` 的输入输出仍以对应 Library Checker 题为准；题目 README 标注了需要适配的差异。",
        "",
        "生成命令：`python qoj模板代码/tools/generate.py`",
        "",
        "| 题号 | 题目 | QOJ ID | 模板条目 | 内容 |",
        "| --- | --- | ---: | --- | --- |",
    ]
    for problem in problems:
        name = safe_name(str(problem["label"]), str(problem["title"]))
        kind = "main.cpp + template.hpp" if problem.get("lc_source") else "template.hpp"
        topics = ", ".join(str(topic) for topic in problem.get("topics", [])) or "-"
        lines.append(
            f"| {problem['label']} | [{problem['title']}]({name}/) | {problem['id']} | {topics} | {kind} |"
        )
    (CATALOG / "README.md").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def main() -> None:
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    for entry in data["problems"]:
        write_problem(entry)
    write_index(data["problems"])
    print(f"generated {len(data['problems'])} QOJ problem directories")


if __name__ == "__main__":
    main()
