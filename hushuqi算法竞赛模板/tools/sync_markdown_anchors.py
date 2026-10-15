#!/usr/bin/env python3
"""Keep GitHub numeric anchors in sync with the PDF heading filter."""
from pathlib import Path
import argparse
import re

ROOT = Path(__file__).resolve().parents[1]


def sync(text):
    out = []
    active = None
    for line in text.splitlines(keepends=True):
        mark = re.match(r'^ {0,3}(`{3,}|~{3,})(.*)$', line)
        if mark:
            run, tail = mark.groups()
            if active is None:
                active = run
            elif run[0] == active[0] and len(run) >= len(active) and not tail.strip():
                active = None
        if active is None:
            if re.fullmatch(r'<a id="(?:c\d+|\d+)"></a>\s*', line):
                continue
            head = re.match(r'^(#{1,3}) (\d+(?:\.\d+)*)(?:\.)?\s+', line)
            if head:
                num = head[2].replace('.', '')
                ident = 'c'+num if len(head[1]) == 1 else num
                while out and not out[-1].strip():
                    out.pop()
                if out:
                    out.append('\n')
                out.extend([f'<a id="{ident}"></a>\n', '\n'])
        out.append(line)
    if active:
        raise ValueError('Unclosed code fence')
    return ''.join(out)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    changed = []
    ids = set()
    for path in sorted(ROOT.glob('[0-9][0-9]-*.md')):
        text = path.read_text(encoding='utf-8')
        updated = sync(text)
        for ident in re.findall(r'<a id="([^"]+)"></a>', updated):
            if ident in ids:
                raise ValueError(f'Duplicate numeric anchor: {ident}')
            ids.add(ident)
        if text != updated:
            changed.append(path.name)
            if not args.check:
                path.write_text(updated, encoding='utf-8')
    if args.check and changed:
        raise SystemExit('Stale anchors: '+', '.join(changed))
    print(f'NUMERIC_ANCHORS_OK count={len(ids)} updated={len(changed)}')


if __name__ == '__main__':
    main()
