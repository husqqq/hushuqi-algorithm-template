#!/usr/bin/env python3
"""Parse GFM, verify local link fragments, and inspect the published PDFs."""
from __future__ import annotations
import argparse
from html.parser import HTMLParser
import json
import re
import subprocess
import tempfile
from pathlib import Path
from urllib.parse import unquote, urlsplit

ROOT = Path(__file__).resolve().parents[1]

class HtmlIDs(HTMLParser):
    def __init__(self):
        super().__init__()
        self.ids = set()
    def handle_starttag(self, tag, attrs):
        for key, value in attrs:
            if key in ('id', 'name') and value:
                self.ids.add(value)

def walk(value):
    if isinstance(value, dict):
        yield value
        for child in value.values():
            yield from walk(child)
    elif isinstance(value, list):
        for child in value:
            yield from walk(child)

def parse(path):
    result = subprocess.run(['pandoc', str(path), '-f', 'gfm+tex_math_dollars', '-t', 'json'],
                            check=True, capture_output=True, encoding='utf-8')
    ast = json.loads(result.stdout)
    ids = set()
    links = []
    html = HtmlIDs()
    for node in walk(ast):
        kind, data = node.get('t'), node.get('c')
        if kind == 'Header':
            ids.add(data[1][0])
        elif kind in ('RawBlock', 'RawInline') and data[0] == 'html':
            html.feed(data[1])
        elif kind in ('Link', 'Image'):
            links.append(data[2][0])
    return ids | html.ids, links

def check_pdf(path, volume=False):
    from pypdf import PdfReader
    reader = PdfReader(path, strict=True)
    pages = len(reader.pages)
    if pages < 2 or (volume and pages > 600):
        raise ValueError(f'{path.name}: abnormal page count {pages}')
    # Resolve every page and its content streams, not merely the file header.
    for page in reader.pages:
        assert float(page.mediabox.width) > 0 and float(page.mediabox.height) > 0
        if page.get_contents() is not None:
            page.get_contents().get_data()
    if not reader.outline:
        raise ValueError(f'{path.name}: missing PDF outline')
    print(f'PDF_OK {path.name} pages={pages}', flush=True)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--pdf', action='store_true')
    args = parser.parse_args()
    paths = sorted(ROOT.glob('[0-9][0-9]-*.md')) + [ROOT/'README.md', ROOT/'目录.md', ROOT/'知识点关系.md']
    parsed = {p.resolve(): parse(p) for p in paths}
    errors = []
    for path, (_, links) in parsed.items():
        for raw in links:
            url = urlsplit(raw)
            if url.scheme or url.netloc:
                continue
            target = (path.parent / unquote(url.path)).resolve() if url.path else path
            if not target.exists():
                errors.append(f'{path.name}: missing target {raw}')
            elif url.fragment and target.suffix.lower() == '.md':
                if target not in parsed:
                    parsed_target = parse(target)
                else:
                    parsed_target = parsed[target]
                if unquote(url.fragment) not in parsed_target[0]:
                    errors.append(f'{path.name}: missing fragment {raw}')
    if errors:
        print('\n'.join(errors))
        return 1
    # Exercise the HTML writer too; this is a parser check, not a browser layout audit.
    with tempfile.TemporaryDirectory(prefix='template-gfm-') as tmp:
        for path in paths:
            subprocess.run(['pandoc', str(path), '-f', 'gfm+tex_math_dollars', '-t', 'html5',
                            '--mathjax', '-o', str(Path(tmp)/(path.stem+'.html'))], check=True)
    if args.pdf:
        check_pdf(ROOT/'book'/'hushuqi算法竞赛模板.pdf')
        for name in ('上', '中', '下'):
            check_pdf(ROOT/'book'/'分册版'/f'hushuqi算法竞赛模板({name}).pdf', True)
    print(f'MARKDOWN_LINKS_AND_PARSE_OK files={len(paths)}')
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
