"""Compare the actual full, print and Markdown integer implementations with Python ints."""
from __future__ import annotations

import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler
from test_ch14_widths import expand


def blocks(text: str, topic: str) -> list[str]:
    section = re.search(rf"^### {re.escape(topic)} .*?(?=^### |\Z)", text, re.M | re.S)
    assert section, topic
    return re.findall(r"```cpp\n(.*?)\n```", section[0], re.S)


def main() -> None:
    sys.set_int_max_str_digits(0)
    rng = random.Random(14019)
    pairs = [(0, 1), (-(1 << 63), -1), ((1 << 63) - 1, -(1 << 63))]
    for digits in (1, 8, 9, 10, 63 * 9, 64 * 9, 65 * 9, 128 * 9, 257 * 9, 513 * 9):
        for divisor in (1, 64 * 9, 65 * 9, max(1, digits // 2), digits, digits + 1):
            x = int("9" * digits)
            y = int("1" + "0" * (divisor - 1)) + rng.randrange(1, 1000)
            for sx, sy in ((1, 1), (-1, 1), (1, -1), (-1, -1)):
                pairs.append((sx * x, sy * y))
    for _ in range(100):
        nx, ny = rng.randrange(1, 16000), rng.randrange(1, 8000)
        pairs.append((rng.getrandbits(nx) * rng.choice((-1, 1)),
                      max(1, rng.getrandbits(ny)) * rng.choice((-1, 1))))

    data, expected = [], []
    for x, y in pairs:
        q = abs(x) // abs(y) * (-1 if (x < 0) != (y < 0) else 1)
        r = x - q * y
        data.append(f"{x} {y}\n")
        expected.extend(str(z) for z in (x + y, x - y, x * y, q, r))
    body = r'''
signed main()
{
    static_assert(sizeof(int) == EXPECT_WIDTH);
    assert(FastBigInt(LLONG_MIN).toDecimal() == "-9223372036854775808");
    assert(HexBigInt(LLONG_MIN).toHex() == "-8000000000000000");
    assert(BigInt(LLONG_MIN).str() == "-9223372036854775808");
    for (string s : {"+0000", "-0000", "0000"})
    {
        assert(FastBigInt(s).toDecimal() == "0");
        assert(HexBigInt::fromDecimal(s).toDecimal() == "0");
    }
    string sa, sb;
    while (cin >> sa >> sb)
    {
        FastBigInt a(sa), b(sb);
        HexBigInt x = HexBigInt::fromDecimal(sa), y = HexBigInt::fromDecimal(sb);
        assert(a.toDecimal() == sa && x.toDecimal() == sa);
        auto [q, r] = divmod(a, b);
        auto [hq, hr] = divmod(x, y);
        cout << (a+b).toDecimal() << '\n' << (a-b).toDecimal() << '\n'
             << (a*b).toDecimal() << '\n' << q.toDecimal() << '\n' << r.toDecimal() << '\n';
        cerr << (x+y).toDecimal() << '\n' << (x-y).toDecimal() << '\n'
             << (x*y).toDecimal() << '\n' << hq.toDecimal() << '\n' << hr.toDecimal() << '\n';
    }
}
'''
    ch14 = (ROOT / "14-其他实用模板.md").read_text(encoding="utf-8")
    ch10 = (ROOT / "10-多项式、变换与线性递推.md").read_text(encoding="utf-8")
    ch1 = (ROOT / "01-基础与工程.md").read_text(encoding="utf-8")
    # All explicitly printed dependencies, followed by every implementation block of 14.1.019.
    printed = "\n".join([blocks(ch1, "1.1.008")[0], blocks(ch10, "10.1.002")[0],
                         blocks(ch10, "10.1.004")[0], blocks(ch14, "14.1.001")[0],
                         *blocks(ch14, "14.1.019")[:3]])
    sources = {"markdown": printed}
    for branch in ("full", "print"):
        seen: set[Path] = set()
        source = "".join(expand(ROOT / "include" / branch / f"14_1_{t}.hpp", seen)
                         for t in ("001", "019"))
        source = source.replace("// LC_GENERATOR_CH14_ONLY_BEGIN\n#undef int",
                                '// LC_GENERATOR_CH14_ONLY_BEGIN\n#pragma push_macro("int")\n#undef int')
        source = source.replace("#define int long long\n\n// LC_GENERATOR_CH14_ONLY_END",
                                '#pragma pop_macro("int")\n\n// LC_GENERATOR_CH14_ONLY_END')
        sources[branch] = source
    with tempfile.TemporaryDirectory(prefix="ch14-bigints-") as temp:
        for branch, source in sources.items():
            source = re.sub(r"^#define int long long\s*$", "", source, flags=re.M)
            for enabled in (False, True):
                stem = f"{branch}_{int(enabled)}"
                path = Path(temp) / (stem + ".cpp")
                exe = path.with_suffix(".exe")
                prefix = ("#include <bits/stdc++.h>\nusing namespace std;\n"
                          + ("#define int long long\n" if enabled else "")
                          + f"#define EXPECT_WIDTH {8 if enabled else 4}\n")
                path.write_text(prefix + source + body, encoding="utf-8")
                result = subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)],
                                        capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr.decode(errors="replace")[:6000])
                result = subprocess.run([str(exe)], input="".join(data), capture_output=True,
                                        text=True, timeout=180)
                assert result.returncode == 0, (stem, result.stderr[-3000:])
                for actual in (result.stdout.split(), result.stderr.split()):
                    if actual != expected:
                        idx = next((i for i, (a, b) in enumerate(zip(actual, expected)) if a != b),
                                   min(len(actual), len(expected)))
                        raise AssertionError((stem, "case", idx // 5, pairs[idx // 5], "operation", idx % 5))
                print(f"{stem}: {len(pairs)} independent cases OK", flush=True)


if __name__ == "__main__":
    main()
