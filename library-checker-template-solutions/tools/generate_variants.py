#!/usr/bin/env python3
"""Generate additional LC adapters without replacing each problem's main candidate."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
WORKSPACE = ROOT.parent
TEMPLATE = WORKSPACE / "hushuqi算法竞赛模板" / "14-其他实用模板.md"
PYTHON_TEST = WORKSPACE / "hushuqi算法竞赛模板" / "tests" / "test_python_topics.py"
MANIFEST = ROOT / "variants.json"


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def file_sha256(path: Path) -> str:
    return sha256(path.read_bytes())


def now() -> str:
    return datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")


def block(topic: str) -> str:
    text = TEMPLATE.read_text(encoding="utf-8")
    match = re.search(
        rf"^### {re.escape(topic)} .+?^```python\n(.*?)^```",
        text,
        re.MULTILINE | re.DOTALL,
    )
    if match is None:
        raise RuntimeError(f"missing Python block for {topic}")
    return match.group(1).rstrip() + "\n"


def prefix(topic: str) -> str:
    if topic == "14.1.011":
        return "import sys\nfrom collections import deque\n\n"
    return block(topic)


@dataclass(frozen=True)
class Variant:
    variant_id: str
    problem: str
    title: str
    category: str
    topic: str
    adapter: str
    complexity: str


VARIANTS = (
    Variant(
        "14_1_011_deque_pypy3", "deque", "Deque", "data_structure",
        "14.1.011",
        r'''
def main():
    data = sys.stdin.buffer.read().split()
    q = int(data[0])
    pos = 1
    a = deque()
    out = []
    for _ in range(q):
        t = int(data[pos])
        pos += 1
        if t == 0:
            a.appendleft(int(data[pos]))
            pos += 1
        elif t == 1:
            a.append(int(data[pos]))
            pos += 1
        elif t == 2:
            a.popleft()
        elif t == 3:
            a.pop()
        else:
            out.append(str(a[int(data[pos])]))
            pos += 1
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O(1) end operations; O(min(i,N-i)) indexed access in Python deque",
    ),
    Variant(
        "14_1_011_dict_pypy3", "associative_array", "Associative Array",
        "data_structure", "14.1.011",
        r'''
def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    q = data[0]
    pos = 1
    value = {}
    out = []
    for _ in range(q):
        t, key = data[pos], data[pos + 1]
        pos += 2
        if t == 0:
            value[key] = data[pos]
            pos += 1
        else:
            out.append(str(value.get(key, 0)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O(1) expected per operation and O(Q) memory",
    ),
    Variant(
        "14_1_011_bigint_add_pypy3", "addition_of_big_integers",
        "Addition of Big Integers", "big_integer", "14.1.011",
        r'''
def main():
    if hasattr(sys, "set_int_max_str_digits"):
        sys.set_int_max_str_digits(0)
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(str(int(data[2 * i + 1]) + int(data[2 * i + 2])))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision addition; decimal parsing/formatting is interpreter-dependent",
    ),
    Variant(
        "14_1_011_bigint_mul_pypy3", "multiplication_of_big_integers",
        "Multiplication of Big Integers", "big_integer", "14.1.011",
        r'''
def main():
    if hasattr(sys, "set_int_max_str_digits"):
        sys.set_int_max_str_digits(0)
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(str(int(data[2 * i + 1]) * int(data[2 * i + 2])))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision multiplication plus decimal conversion",
    ),
    Variant(
        "14_1_011_bigint_div_pypy3", "division_of_big_integers",
        "Division of Big Integers", "big_integer", "14.1.011",
        r'''
def main():
    if hasattr(sys, "set_int_max_str_digits"):
        sys.set_int_max_str_digits(0)
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        a = int(data[2 * i + 1])
        b = int(data[2 * i + 2])
        q, r = divmod(a, b)
        out.append(f"{q} {r}")
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision division plus decimal conversion",
    ),
    Variant(
        "14_1_011_hex_bigint_add_pypy3", "addition_of_hex_big_integers",
        "Addition of Hex Big Integers", "big_integer", "14.1.011",
        r'''
def hex_string(x):
    return ("-" if x < 0 else "") + format(abs(x), "X")


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(hex_string(int(data[2 * i + 1], 16) + int(data[2 * i + 2], 16)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision addition with linear-time hexadecimal conversion",
    ),
    Variant(
        "14_1_011_hex_bigint_mul_pypy3", "multiplication_of_hex_big_integers",
        "Multiplication of Hex Big Integers", "big_integer", "14.1.011",
        r'''
def hex_string(x):
    return ("-" if x < 0 else "") + format(abs(x), "X")


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(hex_string(int(data[2 * i + 1], 16) * int(data[2 * i + 2], 16)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision multiplication with hexadecimal conversion",
    ),
    Variant(
        "14_1_011_hex_bigint_div_pypy3", "division_of_hex_big_integers",
        "Division of Hex Big Integers", "big_integer", "14.1.011",
        r'''
def hex_string(x):
    return format(x, "X")


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        a = int(data[2 * i + 1], 16)
        b = int(data[2 * i + 2], 16)
        q, r = divmod(a, b)
        out.append(f"{hex_string(q)} {hex_string(r)}")
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "Python arbitrary-precision division with hexadecimal conversion",
    ),
    Variant(
        "14_1_012_dsu_pypy3", "unionfind", "Unionfind", "data_structure",
        "14.1.012",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n, q = next(it), next(it)
    dsu = DSU(n)
    out = []
    for _ in range(q):
        t, u, v = next(it), next(it), next(it)
        if t == 0:
            dsu.merge(u, v)
        else:
            out.append("1" if dsu.find(u) == dsu.find(v) else "0")
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O((N+Q) alpha(N)) time and O(N) memory",
    ),
    Variant(
        "14_1_012_segtree_pypy3", "point_add_range_sum",
        "Point Add Range Sum", "data_structure", "14.1.012",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n, q = next(it), next(it)
    a = [next(it) for _ in range(n)]
    seg = SegTree(a)
    out = []
    for _ in range(q):
        t, x, y = next(it), next(it), next(it)
        if t == 0:
            a[x] += y
            seg.set(x, a[x])
        else:
            out.append(str(seg.prod(x, y)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O((N+Q) log N) time and O(bit_ceil(N)) memory",
    ),
    Variant(
        "14_1_012_dinic_pypy3", "bipartitematching",
        "Matching on Bipartite Graph", "graph", "14.1.012",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    left, right, m = next(it), next(it), next(it)
    source, sink = left + right, left + right + 1
    dinic = Dinic(sink + 1)
    for u in range(left):
        dinic.add(source, u, 1)
    for v in range(right):
        dinic.add(left + v, sink, 1)
    edges = []
    for _ in range(m):
        u, v = next(it), next(it)
        index = len(dinic.g[u])
        dinic.add(u, left + v, 1)
        edges.append((u, v, index))
    value = dinic.flow(source, sink)
    out = [str(value)]
    out.extend(
        f"{u} {v}" for u, v, index in edges if dinic.g[u][index][2] == 0
    )
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O(V^2 E) general Dinic bound; unit bipartite instances are faster",
    ),
    Variant(
        "14_1_012_mincostflow_pypy3", "assignment", "Assignment Problem",
        "graph", "14.1.012",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n = next(it)
    source, sink = 2 * n, 2 * n + 1
    flow = MinCostFlow(sink + 1)
    for i in range(n):
        flow.add(source, i, 1, 0)
    for j in range(n):
        flow.add(n + j, sink, 1, 0)
    refs = [[] for _ in range(n)]
    for i in range(n):
        for j in range(n):
            cost = next(it)
            index = len(flow.g[i])
            flow.add(i, n + j, 1, cost)
            refs[i].append(index)
    sent, cost = flow.flow(source, sink, n)
    assert sent == n
    perm = []
    for i in range(n):
        perm.append(next(j for j, index in enumerate(refs[i]) if flow.g[i][index][2] == 0))
    sys.stdout.write(str(cost) + "\n" + " ".join(map(str, perm)))


if __name__ == "__main__":
    main()
''',
        "O(FVE) worst-case SPFA augmentation; O(N^2) memory for assignment",
    ),
    Variant(
        "14_1_012_ntt_pypy3", "convolution_mod", "Convolution",
        "convolution", "14.1.012",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    n, m = data[0], data[1]
    a = data[2:2 + n]
    b = data[2 + n:2 + n + m]
    sys.stdout.write(" ".join(map(str, conv_ntt(a, b))))


if __name__ == "__main__":
    main()
''',
        "O((N+M) log(N+M)) time and O(bit_ceil(N+M)) memory",
    ),
    Variant(
        "14_1_013_splay_pypy3", "ordered_set", "Ordered Set",
        "data_structure", "14.1.013",
        r'''
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n, q = next(it), next(it)
    tree = Splay()
    for _ in range(n):
        tree.insert(next(it))
    out = []
    for _ in range(q):
        t, x = next(it), next(it)
        if t == 0:
            if tree._find(x) == 0:
                tree.insert(x)
        elif t == 1:
            tree.erase(x)
        elif t == 2:
            value = tree.kth(x - 1)
            out.append(str(-1 if value is None else value))
        elif t == 3:
            out.append(str(tree.rank(x + 1)))
        elif t == 4:
            value = tree.kth(tree.rank(x + 1) - 1)
            out.append(str(-1 if value is None else value))
        else:
            value = tree.kth(tree.rank(x))
            out.append(str(-1 if value is None else value))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
''',
        "O(log N) amortized per operation, O(N) memory",
    ),
)


def write_json(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(value, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
        newline="\n",
    )


def generate(check: bool) -> None:
    generator_hash = file_sha256(Path(__file__))
    template_hash = file_sha256(TEMPLATE)
    test_hash = file_sha256(PYTHON_TEST)
    entries = []
    for item in VARIANTS:
        source = prefix(item.topic) + item.adapter.strip() + "\n"
        relative_dir = Path(item.category) / item.problem / "variants" / item.variant_id
        source_rel = relative_dir / "main.py"
        meta_rel = relative_dir / "meta.json"
        source_path = ROOT / source_rel
        meta_path = ROOT / meta_rel
        source_bytes = source.encode("utf-8")
        source_hash = sha256(source_bytes)
        if check:
            if not source_path.is_file() or source_path.read_bytes() != source_bytes:
                raise RuntimeError(f"generated source is stale: {source_rel.as_posix()}")
        else:
            source_path.parent.mkdir(parents=True, exist_ok=True)
            source_path.write_bytes(source_bytes)

        old = {}
        if meta_path.is_file():
            old = json.loads(meta_path.read_text(encoding="utf-8"))
        attempts = old.get("submission_attempts", []) if isinstance(old, dict) else []
        validation = old.get("validation", {}) if isinstance(old, dict) else {}
        if validation.get("current_source_sha256") != source_hash:
            validation = {
                "status": "local_only",
                "current_source_sha256": source_hash,
                "current_source_online_verdict": None,
                "current_source_submission_id": None,
                "server_source_sha256": None,
                "checks": [
                    "source_python_syntax",
                    "hushuqi算法竞赛模板/tests/test_python_topics.py",
                ],
            }
        meta = {
            "schema_version": 1,
            "kind": "template_variant",
            "variant_id": item.variant_id,
            "name": item.problem,
            "title": item.title,
            "category": item.category,
            "language": "pypy3",
            "source": source_rel.as_posix(),
            "source_sha256": source_hash,
            "template_items": [item.topic],
            "complexity": item.complexity,
            "provenance": {
                "method": "deterministic_generation",
                "generator": "library-checker-template-solutions/tools/generate_variants.py",
                "generator_sha256": generator_hash,
                "inputs": [
                    {"path": "hushuqi算法竞赛模板/14-其他实用模板.md", "sha256": template_hash},
                    {"path": "hushuqi算法竞赛模板/tests/test_python_topics.py", "sha256": test_hash},
                ],
                "generated_sha256": source_hash,
            },
            "validation": validation,
            "submission_attempts": attempts,
        }
        if check:
            if not meta_path.is_file():
                raise RuntimeError(f"missing metadata: {meta_rel.as_posix()}")
            current = json.loads(meta_path.read_text(encoding="utf-8"))
            comparable = dict(current)
            comparable.pop("submission_attempts", None)
            comparable.pop("validation", None)
            expected = dict(meta)
            expected.pop("submission_attempts", None)
            expected.pop("validation", None)
            if comparable != expected:
                raise RuntimeError(f"generated metadata is stale: {meta_rel.as_posix()}")
        else:
            write_json(meta_path, meta)
        entries.append({
            "variant_id": item.variant_id,
            "problem": item.problem,
            "title": item.title,
            "category": item.category,
            "language": "pypy3",
            "source": source_rel.as_posix(),
            "source_sha256": source_hash,
            "meta": meta_rel.as_posix(),
            "template_items": [item.topic],
        })

    manifest = {
        "schema_version": 1,
        "kind": "template_variants",
        "official_repository": {
            "commit": "04c8de378bab67be926325de2871f0babb8e6451",
            "url": "https://github.com/yosupo06/library-checker-problems",
        },
        "variant_count": len(entries),
        "variants": entries,
    }
    if check:
        current = json.loads(MANIFEST.read_text(encoding="utf-8"))
        current_entries = {
            entry.get("variant_id"): entry for entry in current.get("variants", [])
        }
        if (
            current.get("schema_version") != manifest["schema_version"]
            or current.get("kind") != manifest["kind"]
            or current.get("official_repository") != manifest["official_repository"]
            or any(current_entries.get(entry["variant_id"]) != entry for entry in entries)
        ):
            raise RuntimeError("Python entries in variants.json are stale")
    else:
        if MANIFEST.is_file():
            current = json.loads(MANIFEST.read_text(encoding="utf-8"))
            python_ids = {entry["variant_id"] for entry in entries}
            other = [
                entry for entry in current.get("variants", [])
                if entry.get("variant_id") not in python_ids
            ]
            entries = sorted(other + entries, key=lambda entry: entry["variant_id"])
            manifest["variants"] = entries
            manifest["variant_count"] = len(entries)
        manifest["updated_at"] = now()
        write_json(MANIFEST, manifest)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    generate(parser.parse_args().check)


if __name__ == "__main__":
    main()
