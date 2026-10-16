"""Run real Chapter 14 code with both int-macro settings and all copy paths."""
from __future__ import annotations

import datetime
import random
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_test_variants import compiler


def expand(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    out = []
    for line in path.read_text(encoding="utf-8").splitlines(True):
        match = re.match(r'^#include "([^"]+)"', line)
        if match:
            out.append(expand(path.parent / match[1], seen))
        elif not line.startswith("#pragma once"):
            out.append(line)
    return "".join(out)


def main() -> None:
    text = (ROOT / "14-其他实用模板.md").read_text(encoding="utf-8")
    topics = ["001", "005", "016", "020", "021"]
    printed = "#include <bits/stdc++.h>\nusing namespace std;\n"
    for topic in topics:
        match = re.search(rf"^### 14\.1\.{topic} .*?^```cpp\n(.*?)^```", text, re.M | re.S)
        assert match, topic
        printed += match[1] + "\n"
    rng = random.Random(14005)
    cases = []
    epoch = datetime.date(1970, 1, 1)
    for y, m, d in [(1, 1, 1), (1900, 3, 1), (2000, 2, 29), (9999, 12, 31)]:
        cases.append((y, m, d))
    for _ in range(1000):
        dt = datetime.date.fromordinal(rng.randrange(1, datetime.date.max.toordinal() + 1))
        cases.append((dt.year, dt.month, dt.day))
    body = 'signed main()\n{\n'
    for y, m, d in cases:
        dt = datetime.date(y, m, d)
        body += f'assert((Date{{{y},{m},{d}}}.days()=={(dt-epoch).days}));\n'
        body += f'assert((Date{{{y},{m},{d}}}.weekday()=={(dt.weekday()+1)%7}));\n'
    body += '''
    for (long long z : {LLONG_MIN, LLONG_MIN + 1, -719528LL, 0LL, LLONG_MAX - 1, LLONG_MAX})
    {
        Date d = Date::fromDays(z);
        assert(d.valid() && d.days() == z);
    }
    Decimal::setPrecision(20);
    Decimal a("5.55"), b("0.11");
    Decimal::setPrecision(0);
    assert((a % b).str() == "0.05");
    assert((-a % b).str() == "-0.05");
    const string expected = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680";
    assert(piMachin(100).str(100) == expected);
    assert(piChudnovsky(100).str(100) == expected);
    assert(Decimal::getPrecision() == 0);
    cout << "OK\\n";
}
'''
    sources = {"markdown": printed}
    for branch in ("full", "print"):
        seen: set[Path] = set()
        sources[branch] = "".join(expand(ROOT / "include" / branch / f"14_1_{t}.hpp", seen) for t in ("005", "021"))
    with tempfile.TemporaryDirectory(prefix="ch14-widths-") as temp:
        for branch, source in sources.items():
            source = re.sub(r"^#define int long long\s*$", "", source, flags=re.M)
            for enabled in (False, True):
                stem = f"{branch}_{int(enabled)}"
                path = Path(temp) / (stem + ".cpp")
                exe = path.with_suffix(".exe")
                # Standard headers must be parsed before enabling the macro.
                prefix = "#include <bits/stdc++.h>\n" + ("#define int long long\n" if enabled else "")
                path.write_text(prefix + source + body, encoding="utf-8")
                result = subprocess.run([compiler(), "-std=c++20", "-O2", str(path), "-o", str(exe)], capture_output=True, timeout=180)
                assert result.returncode == 0, result.stderr.decode(errors="replace")
                result = subprocess.run([str(exe)], capture_output=True, timeout=180)
                assert result.returncode == 0, (stem, result.stdout, result.stderr)
                assert result.stdout.strip() == b"OK", stem
                print(stem + ": OK", flush=True)


if __name__ == "__main__":
    main()
