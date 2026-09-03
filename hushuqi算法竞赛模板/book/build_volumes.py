#!/usr/bin/env python3
"""构建三册打印版；章节 Markdown 和 TeX 中间文件只写入系统临时目录。"""
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parent.parent
BOOK = ROOT / "book"
VOLUME_DIR = BOOK / "分册版"
FILTERS = [
    "--lua-filter=" + str(BOOK / "anchors.lua"),
    "--lua-filter=" + str(BOOK / "numberlines.lua"),
    "--include-in-header=" + str(BOOK / "head.tex"),
]
PANDOC_COMMON = [
    "--toc-depth=3",
    *FILTERS,
    "-V", "documentclass=ctexart",
    "-V", "geometry:a4paper",
    "-V", "geometry:margin=2cm",
    "-V", "colorlinks=true",
    "-V", "linkcolor=teal",
    "-V", "urlcolor=blue",
    "-V", "toccolor=black",
    "--highlight-style=" + str(BOOK / "jblight.theme"),
    "--pdf-engine=xelatex",
]


def strip_preamble(text):
    """删除合订本中重复的「本章公共前导」小节。"""
    out = []
    skip = False
    for line in text.splitlines(keepends=True):
        if re.match(r"^##\s+本章公共前导", line):
            skip = True
            continue
        if skip and re.match(r"^#{1,2}\s", line):
            skip = False
        if not skip:
            out.append(line)
    return "".join(out)


def strip_chapter_intro(text):
    """删除每章标题后的第一段章首说明，源 Markdown 不受影响。"""
    lines = text.splitlines(keepends=True)
    out, i = [], 0
    while i < len(lines):
        line = lines[i]
        out.append(line)
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
    return "".join(out)


def cover(title, path, reset_body=False):
    """写入指定分册封皮。"""
    text = rf"""% 分册封面
\begin{{titlepage}}
\thispagestyle{{empty}}
\centering
\vspace*{{\stretch{{2}}}}

{{\fontsize{{40}}{{48}}\selectfont\bfseries {title}\par}}

\vspace{{2.4em}}
\rule{{0.55\linewidth}}{{1.1pt}}\par
\vspace{{1.1em}}
{{\Large\itshape ICPC/CCPC Algorithm Templates\par}}

\vspace*{{\stretch{{3}}}}
\end{{titlepage}}
\clearpage
"""
    if reset_body:
        text += r"""
% 目录使用罗马页码，正文恢复为 1；这样目录可补印到旧纸质分册。
\pagenumbering{roman}
% \tableofcontents 自己会调用无编号 section，故在它结束后才打开正文重置开关。
\apptocmd{\tableofcontents}{\global\resetbodytrue}{}{}
"""
    path.write_text(text, encoding="utf-8")


def build_one(name, files, with_toc, tmp):
    """构建一册 PDF；files 是按顺序排列的章节文件。"""
    text = "\n\n".join(p.read_text(encoding="utf-8") for p in files)
    md = tmp / f"volume-{name}.md"
    md.write_text(strip_chapter_intro(strip_preamble(text)) + "\n\n", encoding="utf-8")
    cover_path = tmp / f"cover-{name}.tex"
    cover(f"hushuqi算法竞赛模板({name})", cover_path, reset_body=name != "上")
    VOLUME_DIR.mkdir(exist_ok=True)
    out = VOLUME_DIR / f"hushuqi算法竞赛模板({name}).pdf"
    cmd = ["pandoc", str(md)]
    if with_toc:
        cmd.append("--toc")
    cmd += PANDOC_COMMON
    cmd += ["--include-before-body=" + str(cover_path), "-o", str(out)]
    subprocess.run(cmd, cwd=ROOT, check=True)
    print(f"{name}: {out}")


def main():
    if shutil.which("pandoc") is None:
        raise SystemExit("pandoc not found; cannot build volume PDFs")
    files = sorted(ROOT.glob("[0-9][0-9]-*.md"), key=lambda p: p.name)
    if len(files) != 16:
        raise SystemExit(f"expected 16 chapter files, found {len(files)}")
    groups = [("上", files[:5], True), ("中", files[5:9], True), ("下", files[9:], True)]
    with tempfile.TemporaryDirectory(prefix="hushuqi-volumes-") as path:
        tmp = Path(path)
        for name, part, with_toc in groups:
            build_one(name, part, with_toc, tmp)


if __name__ == "__main__":
    main()
