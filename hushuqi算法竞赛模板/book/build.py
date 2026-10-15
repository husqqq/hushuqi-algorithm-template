#!/usr/bin/env python3
"""Build the full PDF and all volumes; preserve diagnostics and fail on errors."""
from pathlib import Path
import argparse
import subprocess
import sys
from render_pdf import render_pdf

ROOT = Path(__file__).resolve().parents[1]
BOOK = ROOT / 'book'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('format', nargs='?', choices=('pdf', 'tex'), default='pdf')
    args = parser.parse_args()
    subprocess.run([sys.executable, str(BOOK/'combine.py')], cwd=ROOT, check=True)
    subprocess.run([sys.executable, str(BOOK/'strip_preamble.py')], cwd=ROOT, check=True)
    common = ['pandoc', str(BOOK/'hushuqi算法竞赛模板-合订.md'), '--toc', '--toc-depth=3',
              '--lua-filter='+str(BOOK/'anchors.lua'), '--lua-filter='+str(BOOK/'numberlines.lua'),
              '--include-in-header='+str(BOOK/'head.tex'), '--include-before-body='+str(BOOK/'cover.tex'),
              '-V', 'documentclass=ctexart', '-V', 'geometry:a4paper', '-V', 'geometry:margin=2cm',
              '-V', 'colorlinks=true', '-V', 'linkcolor=teal', '-V', 'urlcolor=blue', '-V', 'toccolor=black',
              '--highlight-style='+str(BOOK/'jblight.theme')]
    if args.format == 'tex':
        subprocess.run(common+['-s', '-o', str(BOOK/'hushuqi算法竞赛模板.tex')], cwd=ROOT, check=True)
        return
    render_pdf(common, BOOK/'hushuqi算法竞赛模板.pdf', ROOT)
    subprocess.run([sys.executable, str(BOOK/'build_volumes.py')], cwd=ROOT, check=True)
    subprocess.run([sys.executable, str(ROOT/'tools'/'check_markdown_render.py'), '--pdf'], cwd=ROOT, check=True)
    print('BOOK_BUILD_OK', flush=True)

if __name__ == '__main__':
    main()
