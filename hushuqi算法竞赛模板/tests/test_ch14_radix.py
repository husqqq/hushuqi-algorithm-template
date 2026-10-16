"""Check actual radix-conversion copies against Python integers in all configurations."""
from __future__ import annotations

import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler, branch_source
from test_ch14_widths import expand

DIGIT = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"


def encode(x: int, base: int) -> str:
    if not x:
        return "0"
    neg = x < 0
    x = abs(x)
    out = []
    while x:
        x, r = divmod(x, base)
        out.append(DIGIT[r])
    return ("-" if neg else "") + "".join(reversed(out))


BODY = r'''
signed main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int from, to;
        string s;
        cin >> from >> to >> s;
        cout << convertBase(s, from, to) << '\n';
    }
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string s;
        int e;
        cin >> s >> e;
        cout << bigPow(BigInt(s), e).str() << '\n';
    }
    // Long inputs exercise the linear branches without building a second solver in C++.
    assert(convertBase("-" + string(1000000, '0'), 62, 2) == "0");
    assert(convertBase("+" + string(1000000, '0') + "Az", 62, 62) == "Az");
    assert(convertBase(string(100000, 'V'), 32, 2) == string(500000, '1'));
    assert(convertBase(string(500000, '1'), 2, 32) == string(100000, 'V'));
    assert(bigPow(BigInt(-1), numeric_limits<int>::max()).str() == "-1");
    cout << "LONG_OK\n";
}
'''


def main() -> None:
    rng = random.Random(14006)
    rows = []
    want = []
    for source in range(2, 63):
        for target in range(2, 63):
            # Both signs and every radix pair, with noncanonical input padding.
            for sign in (1, -1):
                value = sign * rng.getrandbits(rng.randrange(1, 513))
                raw = encode(abs(value), source)
                prefix = "-" if value < 0 else "+"
                text = prefix + "0" * rng.randrange(5) + raw
                rows.append((source, target, text))
                want.append(encode(value, target))
    for source in (2, 4, 8, 16, 32):
        for target in (2, 4, 8, 16, 32):
            for bits in range(1, 67):
                # Powers of two cross every source/target group boundary.
                for delta in (-1, 0, 1):
                    value = (1 << bits) + delta
                    rows.append((source, target, encode(value, source)))
                    want.append(encode(value, target))
    for base in range(2, 63):
        for sign in ("", "+", "-"):
            rows.append((base, 63 - base if base < 62 else 2, sign + "00000"))
            want.append("0")
    for source, target, text in [(62, 10, "Aaz"), (37, 36, "aA"), (16, 62, "FF")]:
        value = 0
        for c in text:
            value = value * source + DIGIT.index(c)
        rows.append((source, target, text))
        want.append(encode(value, target))
    powers = [(0, 0), (0, 1), (-1, 0), (-1, 2147483647), (1, 2147483647),
              (-9223372036854775808, 1), (-9223372036854775808, 2)]
    powers += [(rng.randrange(-10**35, 10**35), rng.randrange(31)) for _ in range(300)]
    data = str(len(rows)) + "\n" + "".join(f"{a} {b} {s}\n" for a, b, s in rows)
    data += str(len(powers)) + "\n" + "".join(f"{a} {e}\n" for a, e in powers)
    expected = want + [str(pow(a, e)) for a, e in powers] + ["LONG_OK"]
    markdown = (ROOT / "14-其他实用模板.md").read_text(encoding="utf-8")
    printed = "#include <bits/stdc++.h>\nusing namespace std;\n"
    for topic in ("001", "006"):
        found = re.search(rf"^### 14\.1\.{topic} .*?^```cpp\n(.*?)^```", markdown, re.M | re.S)
        assert found, topic
        printed += found[1] + "\n"
    sources = {"markdown": printed}
    for branch in ("full", "print"):
        sources[branch] = expand(ROOT / "include" / branch / "14_1_006.hpp", set())
    with tempfile.TemporaryDirectory(prefix="ch14-radix-") as temp:
        for branch, source in sources.items():
            source = re.sub(r"^#define int long long\s*$", "", source, flags=re.M)
            for enabled in (False, True):
                stem = f"{branch}_{int(enabled)}"
                path = Path(temp) / (stem + ".cpp")
                exe = path.with_suffix(".exe")
                prefix = "#include <bits/stdc++.h>\n" + ("#define int long long\n" if enabled else "")
                path.write_text(prefix + source + BODY, encoding="utf-8")
                result = subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)],
                                        capture_output=True, timeout=180)
                assert result.returncode == 0, result.stderr.decode(errors="replace")
                result = subprocess.run([str(exe)], input=data, text=True, capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr)
                got = result.stdout.splitlines()
                assert len(got) == len(expected), (stem, len(got), len(expected))
                for i, (actual, value) in enumerate(zip(got, expected)):
                    assert actual == value, (stem, i, actual, value)
                print(f"{stem}: {len(rows)} radix, {len(powers)} power and long-input checks OK", flush=True)
        for branch in ("full", "print"):
            path = Path(temp) / f"core_{branch}.cpp"
            path.write_text(branch_source(ROOT / "tests/test_ch14_core.cpp", branch), encoding="utf-8")
            exe = path.with_suffix(".exe")
            subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)], check=True, timeout=180)
            result = subprocess.run([str(exe)], capture_output=True, timeout=180)
            assert result.returncode == 0 and result.stdout.strip() == b"OK", (branch, result.stderr)
            print(f"{branch}: existing chapter core checks OK", flush=True)


if __name__ == "__main__":
    main()
