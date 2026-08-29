#!/usr/bin/env python3
"""按两位章号合并模板正文，确保 Windows 下中文文件名按 UTF-8 读取。"""
from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
out = Path(sys.argv[1]) if len(sys.argv) > 1 else root / "book" / "hushuqi算法竞赛模板-合订.md"
files = sorted(root.glob("[0-9][0-9]-*.md"), key=lambda p: p.name)
text = "\n\n".join(p.read_text(encoding="utf-8") for p in files)
out.write_text(text + "\n\n", encoding="utf-8")
print(f"combined {len(files)} chapters -> {out}")
