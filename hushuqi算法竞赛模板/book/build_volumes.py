#!/usr/bin/env python3
"""构建三册打印版；章节 Markdown 和 TeX 中间文件只写入系统临时目录。"""
from pathlib import Path
import re
import shutil
import tempfile
from itertools import combinations
from pypdf import PdfReader
from render_pdf import render_pdf


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
]


def strip_maintenance_notes(text):
    """删除打印版中不应出现的仓库路径和生成器同步提示。"""
    return re.sub(
        r"^头文件：`?include/full/[0-9_]+\.hpp`?，打印版由生成器同步。\s*\n?",
        "",
        text,
        flags=re.MULTILINE,
    )


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


def unlink_foreign(text):
    """分册里指向其他分册知识点的链接没有落点，改成普通文字。"""
    present = {re.sub(r"\D", "", m) for m in re.findall(r"^#{1,3}\s+(\d+(?:\.\d+)*)", text, re.M)}

    def repl(m):
        return m[0] if m[2] in present else m[1]

    return re.sub(r"\[([^\]]+)\]\((?:[^)#\s]*\.md)?#(\d+)\)", repl, text)


def build_one(name, files, tmp, out):
    """构建一册到临时 out；files 是按顺序排列的章节文件，返回实际页数。"""
    text = "\n\n".join(p.read_text(encoding="utf-8") for p in files)
    md = tmp / f"volume-{name}.md"
    body = unlink_foreign(strip_maintenance_notes(strip_chapter_intro(strip_preamble(text))))
    md.write_text(body + "\n\n", encoding="utf-8")
    cover_path = tmp / f"cover-{name}.tex"
    cover(f"hushuqi算法竞赛模板({name})", cover_path, reset_body=name != "上")
    cmd = ["pandoc", str(md), "--toc", *PANDOC_COMMON,
           "--include-before-body=" + str(cover_path)]
    return render_pdf(cmd, out, ROOT)


def chapter_pages(files, full_pdf):
    """从本次合订本的一级书签取得每章正文页数；书签必须与当前章节一致。"""
    reader = PdfReader(full_pdf)
    chapters = [item for item in reader.outline if not isinstance(item, list)]
    titles = []
    for path in files:
        headings = re.findall(r"^#\s+(\d+\.\s+.+)$", path.read_text(encoding="utf-8"), re.M)
        if len(headings) != 1:
            raise ValueError(f"{path.name}: 必须恰有一个章标题")
        titles.append(headings[0])
    if [item.title for item in chapters] != titles:
        raise ValueError("合订本章书签与当前 Markdown 不一致，请先重建合订本")
    starts = [reader.get_destination_page_number(item) for item in chapters]
    if starts != sorted(set(starts)):
        raise ValueError("合订本章节页序异常")
    ends = starts[1:] + [len(reader.pages)]
    return [end - start for start, end in zip(starts, ends)]


def candidate_partitions(weights):
    """枚举三段非空连续章节，按最大正文页数、页数差、边界依次排序。"""
    if len(weights) < 3 or any(weight <= 0 for weight in weights):
        raise ValueError("至少需要三章，且每章页数必须为正")
    prefix = [0]
    for weight in weights:
        prefix.append(prefix[-1] + weight)
    candidates = []
    for first, second in combinations(range(1, len(weights)), 2):
        bounds = (0, first, second, len(weights))
        sizes = [prefix[end] - prefix[start] for start, end in zip(bounds, bounds[1:])]
        candidates.append((max(sizes), max(sizes) - min(sizes), bounds))
    return [bounds for _, _, bounds in sorted(candidates)]


def build_volumes(full_pdf):
    """按本次合订本估计最均衡边界；实际分册均不超 600 页才更新正式文件。"""
    files = sorted(ROOT.glob("[0-9][0-9]-*.md"))
    numbers = [int(path.name.split("-", 1)[0]) for path in files]
    if len(numbers) != len(set(numbers)):
        raise ValueError("章号重复")
    candidates = candidate_partitions(chapter_pages(files, full_pdf))
    names = ("上", "中", "下")
    with tempfile.TemporaryDirectory(prefix="hushuqi-volumes-") as path:
        tmp = Path(path)
        cached = {}
        for bounds in candidates:
            ranges = list(zip(bounds, bounds[1:]))
            keys = [(name, start, end) for name, (start, end) in zip(names, ranges)]
            if any(key in cached and cached[key][0] > 600 for key in keys):
                continue
            for key in keys:
                name, start, end = key
                if key not in cached:
                    out = tmp / f"volume-{name}-{start}-{end}.pdf"
                    pages = build_one(name, files[start:end], tmp, out)
                    cached[key] = (pages, out)
                if cached[key][0] > 600:
                    print(f"VOLUME_REBALANCE {name}:{start}..{end - 1} "
                          f"pages={cached[key][0]}", flush=True)
                    break
            else:
                VOLUME_DIR.mkdir(parents=True, exist_ok=True)
                for key in keys:
                    name, start, end = key
                    pages, source = cached[key]
                    out = VOLUME_DIR / f"hushuqi算法竞赛模板({name}).pdf"
                    pending = out.with_suffix(".pdf.tmp")
                    shutil.copyfile(source, pending)
                    pending.replace(out)
                    print(f"VOLUME_READY {name}:{files[start].stem}.."
                          f"{files[end - 1].stem} pages={pages}", flush=True)
                return
        raise ValueError("没有满足每册不超过 600 页的连续章节划分；正式分册未更新")


def main():
    # 单独运行时也先构建当前合订本，避免用旧 PDF 的页数划分新章节。
    from build import build_full
    build_volumes(build_full())


if __name__ == "__main__":
    main()
