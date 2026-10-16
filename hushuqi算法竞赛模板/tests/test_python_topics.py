from __future__ import annotations

import bisect
import io
import random
import re
import sys
import tempfile
from pathlib import Path
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
DOC = ROOT / "14-其他实用模板.md"


def block(topic_id: str) -> str:
    text = DOC.read_text(encoding="utf-8")
    hit = re.search(
        rf"^### {re.escape(topic_id)} .+?^```python\n(.*?)^```",
        text,
        re.MULTILINE | re.DOTALL,
    )
    assert hit, topic_id
    return hit.group(1)


def naive_conv(a: list[int], b: list[int], mod: int | None = None) -> list[int]:
    ans = [0] * (len(a) + len(b) - 1)
    for i, x in enumerate(a):
        for j, y in enumerate(b):
            ans[i + j] += x * y
            if mod is not None:
                ans[i + j] %= mod
    return ans


def main() -> None:
    for topic_id in ("14.1.011", "14.1.012", "14.1.013", "14.1.014"):
        compile(block(topic_id), topic_id, "exec")

    ns: dict[str, object] = {}
    out = io.StringIO()
    with patch.object(sys, "stdin", io.TextIOWrapper(io.BytesIO(b"1 2\n3\n"))), patch.object(sys, "stdout", out):
        exec(block("14.1.011"), ns)
        assert ns["tokens"]() == [b"1", b"2", b"3"]
    assert out.getvalue() == "19\n"
    assert ns["lo"] == 1 and ns["hi"] == 3
    assert ns["root"] == 10**50 and ns["joined"] == "acm/template"

    ns = {}
    out = io.StringIO()
    limit = sys.getrecursionlimit()
    try:
        with patch.object(sys, "stdin", io.TextIOWrapper(io.BytesIO(b"7 8\n"))), patch.object(sys, "stdout", out):
            exec(block("14.1.014"), ns)
    finally:
        sys.setrecursionlimit(limit)
    assert out.getvalue() == "29/21 22\n"
    assert ns["data"] == [b"7", b"8"] and ns["stamp"] == "2026/07/24 12:30:00"
    assert ns["day_diff"](2000, 2, 28, 2000, 3, 1) == 2
    assert ns["day_diff"](1900, 2, 28, 1900, 3, 1) == 1
    with tempfile.TemporaryDirectory(prefix="ch14-python-") as temp:
        path = Path(temp) / "日期.txt"
        ns["write_text"](path, "算法\n29/21\n")
        assert ns["read_text"](path) == "算法\n29/21\n"

    ns: dict[str, object] = {}
    exec(block("14.1.012"), ns)

    def check_pairs(net):
        for u, edges in enumerate(net.g):
            for i, e in enumerate(edges):
                rev = net.g[e[0]][e[1]]
                assert rev is not e and rev[:2] == [u, i]
                assert e[2] >= 0
                if len(e) == 4:
                    assert rev[3] == -e[3]

    for cls in ("Dinic", "MinCostFlow"):
        net = ns[cls](2)
        for u, v, c in [(0, 0, 5), (0, 0, 0), (0, 1, 3), (1, 0, 2), (1, 1, 7)]:
            net.add(u, v, c, 0) if cls == "MinCostFlow" else net.add(u, v, c)
        check_pairs(net)
        total = sum(e[2] for edges in net.g for e in edges)
        assert net.flow(0, 1) == ((3, 0) if cls == "MinCostFlow" else 3)
        check_pairs(net)
        assert sum(e[2] for edges in net.g for e in edges) == total

    net = ns["MinCostFlow"](2)
    net.add(0, 0, 1, -1)
    net.add(0, 1, 1, 0)
    try:
        net.flow(0, 1)
    except ValueError:
        pass
    else:
        raise AssertionError("negative self-loop must be rejected")

    dsu = ns["DSU"](6)
    assert dsu.merge(0, 1) and dsu.merge(1, 2)
    assert dsu.find(0) == dsu.find(2) and not dsu.merge(0, 2)

    rng = random.Random(0x20260723)
    a = [rng.randrange(-100, 101) for _ in range(80)]
    seg = ns["SegTree"](a)
    for _ in range(1000):
        if rng.randrange(2):
            p = rng.randrange(len(a))
            x = rng.randrange(-100, 101)
            a[p] = x
            seg.set(p, x)
        else:
            l = rng.randrange(len(a) + 1)
            r = rng.randrange(l, len(a) + 1)
            assert seg.prod(l, r) == sum(a[l:r])

    dinic = ns["Dinic"](4)
    dinic.add(0, 1, 3)
    dinic.add(0, 2, 2)
    dinic.add(1, 2, 1)
    dinic.add(1, 3, 2)
    dinic.add(2, 3, 4)
    assert dinic.flow(0, 3) == 5

    mcf = ns["MinCostFlow"](4)
    mcf.add(0, 1, 2, -1)
    mcf.add(0, 2, 1, 2)
    mcf.add(1, 2, 1, 0)
    mcf.add(1, 3, 1, 3)
    mcf.add(2, 3, 2, 1)
    assert mcf.flow(0, 3, 3) == (3, 5)

    for _ in range(200):
        n = rng.randrange(1, 30)
        m = rng.randrange(1, 30)
        a = [rng.randrange(-50, 51) for _ in range(n)]
        b = [rng.randrange(-50, 51) for _ in range(m)]
        got = ns["conv_fft"](a, b)
        want = naive_conv(a, b)
        assert all(abs(x - y) < 1E-7 for x, y in zip(got, want))
        aa = [x % 998244353 for x in a]
        bb = [x % 998244353 for x in b]
        assert ns["conv_ntt"](aa, bb) == naive_conv(aa, bb, 998244353)

    ns = {}
    exec(block("14.1.013"), ns)
    tr = ns["Splay"]()
    ref: list[int] = []
    for _ in range(5000):
        op = rng.randrange(4)
        x = rng.randrange(-100, 101)
        if op == 0:
            tr.insert(x)
            bisect.insort(ref, x)
        elif op == 1:
            got = tr.erase(x)
            p = bisect.bisect_left(ref, x)
            want = p < len(ref) and ref[p] == x
            assert got == want
            if want:
                ref.pop(p)
        elif op == 2:
            assert tr.rank(x) == bisect.bisect_left(ref, x)
        else:
            k = rng.randrange(-1, len(ref) + 1)
            want = ref[k] if 0 <= k < len(ref) else None
            assert tr.kth(k) == want

    print("OK")


if __name__ == "__main__":
    main()
