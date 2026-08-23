#!/usr/bin/env bash
# 一键：合订 14 章 -> 生成带封面 / 目录 / 自动页码的 PDF。
# 用法： bash book/build.sh          (默认出 PDF)
#        bash book/build.sh tex      (只出中间 .tex，快，便于排查)
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"

MD="$HERE/hushuqi算法竞赛模板-合订.md"

# 1) 合订：按章号顺序拼接，并在章间补空行，避免章标题粘到上一章末行
: > "$MD"
for file in 0[1-9]-*.md 1[0-4]-*.md; do
  cat "$file" >> "$MD"
  printf '\n\n' >> "$MD"
done
# 剔除各章「本章公共前导」冗余小节（ch01 全局前导已覆盖全书）
python "$HERE/strip_preamble.py" "$MD" 2>&1 || python3 "$HERE/strip_preamble.py" "$MD"
echo "combined -> $MD"

# 2) 渲染（标题不加 LaTeX 编号——正文里已带 7.1.003 这类编号，避免重复编号）
PANDOC=(pandoc "$MD"
  --toc --toc-depth=3
  --lua-filter="$HERE/anchors.lua"
  --lua-filter="$HERE/numberlines.lua"
  --include-in-header="$HERE/head.tex"
  --include-before-body="$HERE/cover.tex"
  -V documentclass=ctexart
  -V geometry:a4paper -V geometry:margin=2cm
  -V colorlinks=true -V linkcolor=teal -V urlcolor=blue -V toccolor=black
  --highlight-style="$HERE/jblight.theme"
)

if [ "${1:-pdf}" = "tex" ]; then
  "${PANDOC[@]}" -s -o "$HERE/hushuqi算法竞赛模板.tex"
  echo "TEX -> $HERE/hushuqi算法竞赛模板.tex"
else
  "${PANDOC[@]}" --pdf-engine=xelatex -o "$HERE/hushuqi算法竞赛模板.pdf"
  echo "PDF -> $HERE/hushuqi算法竞赛模板.pdf"
fi
