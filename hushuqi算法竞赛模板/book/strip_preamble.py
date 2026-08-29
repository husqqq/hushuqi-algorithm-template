#!/usr/bin/env python3
# 从合订 md 里删掉各章「## 本章公共前导」小节（标题行 → 下一个 # / ## 标题之前的全部内容）。
# 合订后第 1 章的全局前导（头文件 / int long long / INF / eps）已覆盖全书，
# 各章再逐个重印「本章公共前导」在整本书里就是冗余，故只在合订本里剔除；
# 单章源文件保持不变（单独看某一章时仍自带前导）。
import sys, io, re
from pathlib import Path

path = sys.argv[1] if len(sys.argv) > 1 else str(Path(__file__).resolve().parent / "hushuqi算法竞赛模板-合订.md")
with io.open(path, encoding="utf-8") as f:
    lines = f.readlines()

out, skip, removed = [], False, 0
for line in lines:
    if re.match(r"^##\s+本章公共前导", line):
        skip = True
        removed += 1
        continue
    if skip and re.match(r"^#{1,2}\s", line):   # 下一个「# 章」或「## 节」标题结束剔除
        skip = False
    if not skip:
        out.append(line)

text = "".join(out)

# PDF 只保留章标题和正文；每章标题后的第一段章首说明属于阅读提示，
# 单章 Markdown 仍保留，便于直接阅读和复制。
lines = text.splitlines(keepends=True)
clean, i = [], 0
while i < len(lines):
    line = lines[i]
    clean.append(line)
    i += 1
    if not re.match(r"^#\s+\d+\.", line):
        continue
    while i < len(lines) and not lines[i].strip():
        i += 1
    if i < len(lines) and lines[i].startswith(">"):
        while i < len(lines) and lines[i].startswith(">"):
            i += 1
        while i < len(lines) and not lines[i].strip():
            i += 1

with io.open(path, "w", encoding="utf-8") as f:
    f.writelines(clean)

sys.stderr.write("stripped %d 公共前导 section(s)\n" % removed)
