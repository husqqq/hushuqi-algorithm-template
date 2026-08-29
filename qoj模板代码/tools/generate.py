#!/usr/bin/env python3
"""Generate the QOJ 3936 template catalogue from the repository's tested sources."""

from __future__ import annotations

import json
import re
from pathlib import Path
from urllib.parse import quote


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "qoj模板代码"
TEMPLATE_ROOT = ROOT / "hushuqi算法竞赛模板" / "include" / "full"
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
    if not entry.get("pending_template"):
        seen: set[Path] = set()
        for topic in topics:
            pieces.append(bundle(TEMPLATE_ROOT / f"{topic.replace('.', '_')}.hpp", seen))
        (destination / "template.hpp").write_text("\n".join(pieces), encoding="utf-8", newline="\n")

    # QOJ 的题意和 Library Checker 经常只有算法相同、I/O 协议不同。
    # main.cpp 必须逐题按 QOJ 题面适配；生成器只更新模板和索引，
    # 既不覆盖已有入口，也不自动复制 Library Checker 的提交源码。

    has_main = (destination / "main.cpp").exists()
    if entry.get("pending_template"):
        status = "QOJ 候选 template.hpp + 适配入口，待批准写入正式模板"
    elif str(entry["label"]) == "EX1" and has_main:
        status = "已补高性能本地候选，尚待 QOJ 目标机验证"
    elif has_main:
        status = "main.cpp 已按 QOJ 题面适配输入输出"
    elif str(entry["label"]) == "46":
        status = "函数接口题：按题面实现 init(p) 与 inv(x)，不应提供 main.cpp"
    else:
        status = "仅有相关算法模板，尚无完整 QOJ 提交实现"
    readme = (
        f"# {entry['label']}. {entry['title']}\n\n"
        f"- QOJ: {entry['url']}\n"
        f"- 状态：{status}\n"
        f"- 模板条目：{', '.join(topics) if topics else '暂无完全对应条目'}\n"
    )
    if entry.get("note"):
        readme += f"- 备注：{entry['note']}\n"
    readme_path = destination / "README.md"
    marker = "\n## 做法\n"
    if str(entry["label"]) == "EX1" and readme_path.exists():
        old_readme = readme_path.read_text(encoding="utf-8")
        if marker in old_readme:
            readme += marker + old_readme.split(marker, 1)[1]
    readme_path.write_text(readme, encoding="utf-8", newline="\n")


def write_index(problems: list[dict[str, object]]) -> None:
    def has_main(problem: dict[str, object]) -> bool:
        name = safe_name(str(problem["label"]), str(problem["title"]))
        return (CATALOG / name / "main.cpp").exists()

    ready = sum(has_main(problem) for problem in problems)
    lines = [
        "# QOJ 3936 模板代码",
        "",
        "本目录对应 [XXIV 赛前模板训练赛](https://qoj.ac/contest/3936) 的 75 道题。",
        "通常每题目录中的 `template.hpp` 从本仓库权威模板依赖展开；带 pending_template 标记的条目则是待批准候选，生成器会保留而不覆盖；",
        "算法主体可复用本仓库模板或 Library Checker 实现，但 `main.cpp` 的输入输出、下标和特殊约定均逐题按 QOJ 题面适配。",
        "生成器不会创建或覆盖 `main.cpp`；入口代码的来源分类见 `来源审计.md`。",
        "",
        f"当前共有 {ready} 道提供按 QOJ 题面适配的 `main.cpp`。",
        "不存在 `main.cpp` 的目录只提供相关算法模板，不能直接提交；46 为函数接口题，本来就不应包含 main。",
        "EX1 已补高性能本地候选，尚待 QOJ 验证；其余未完成项及原因见 `未完成清单.md`。",
        "",
        "生成命令：`python qoj模板代码/tools/generate.py`",
        "",
        "| 题号 | 题目 | QOJ ID | 模板条目 | 内容 |",
        "| --- | --- | ---: | --- | --- |",
    ]
    for problem in problems:
        name = safe_name(str(problem["label"]), str(problem["title"]))
        link = quote(name, safe="")
        if problem.get("pending_template"):
            kind = "候选 template.hpp + QOJ main.cpp"
        elif str(problem["label"]) == "EX1" and has_main(problem):
            kind = "高性能候选 main.cpp + template.hpp"
        else:
            kind = "QOJ main.cpp + template.hpp" if has_main(problem) else ("函数接口 template.hpp" if str(problem["label"]) == "46" else "仅 template.hpp")
        topics = ", ".join(str(topic) for topic in problem.get("topics", [])) or "-"
        lines.append(
            f"| {problem['label']} | [{problem['title']}]({link}/) | {problem['id']} | {topics} | {kind} |"
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
