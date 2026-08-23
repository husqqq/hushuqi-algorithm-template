from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TEMPLATE = next(p for p in ROOT.iterdir() if p.is_dir() and p.name.startswith("hushuqi"))
OUT = ROOT / "library-checker-template-solutions"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
INC = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')

SPECS = {
    "aho_corasick": ("4_2_002.hpp", r'''
signed main()
{
    int n; cin >> n;
    AC<26> ac;
    vector<int> end(n);
    for (auto &x : end) { string s; cin >> s; x = ac.add(s); }
    ac.build();
    auto &nodes = ac.nodes();
    vector<int> par(nodes.size(), -1);
    for (int u = 0; u < (int)nodes.size(); u++)
        for (int c = 0; c < 26; c++) if (nodes[u].ch[c] != -1) par[nodes[u].ch[c]] = u;
    cout << nodes.size() << '\n';
    for (int u = 1; u < (int)nodes.size(); u++) cout << par[u] << ' ' << nodes[u].fail << '\n';
    for (int i = 0; i < n; i++) cout << (i ? " " : "") << end[i];
    cout << '\n';
}

'''),
    "eertree": ("4_4_001.hpp", r'''
signed main()
{
    string s; cin >> s; PAM<26, 'a'> pam; vector<int> last;
    for (char c : s) last.push_back(pam.add(c));
    vector<int> par(pam.t.size(), -1);
    for (int u = 0; u < (int)pam.t.size(); u++)
        for (int c = 0; c < 26; c++) if (pam.t[u].ch[c]) par[pam.t[u].ch[c]] = u;
    cout << pam.t.size() - 2 << '\n';
    for (int u = 2; u < (int)pam.t.size(); u++) cout << par[u] - 1 << ' ' << pam.t[u].link - 1 << '\n';
    for (int i = 0; i < (int)s.size(); i++) cout << (i ? " " : "") << last[i] - 1;
    cout << '\n';
}
'''),
    "enumerate_palindromes": ("4_1_004.hpp", r'''
signed main()
{
    string s; cin >> s; Manacher m(s);
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (i) cout << ' ';
        cout << 2 * m.odd[i] - 1;
        if (i + 1 < (int)s.size()) cout << ' ' << 2 * m.even[i + 1];
    }
    cout << '\n';
}
'''),
    "longest_common_substring": ("4_3_004.hpp", r'''
signed main()
{
    string a, b; cin >> a >> b;
    auto ans = commonSubstr(a, b);
    cout << ans[0] << ' ' << ans[1] << ' ' << ans[2] << ' ' << ans[3] << '\n';
}
'''),
    "lyndon_factorization": ("4_1_006.hpp", r'''
signed main()
{
    string s; cin >> s; auto a = lyndon(s);
    for (int i = 0; i < (int)a.size(); i++) cout << (i ? " " : "") << a[i];
    cout << '\n';
}
'''),
    "number_of_substrings": ("4_3_004.hpp", r'''
signed main()
{
    string s; cin >> s; SAM<26, 'a'> sam(s); cout << sam.distinct() << '\n';
}
'''),
    "palindromes_in_deque": ("4_4_002.hpp", r'''
signed main()
{
    int q; cin >> q; DequePAM<26, 'a'> d;
    while (q--)
    {
        int type; cin >> type;
        if (type < 2) { char c; cin >> c; if (type == 0) d.pushFront(c); else d.pushBack(c); }
        else if (type == 2) d.popFront(); else d.popBack();
        cout << d.distinct() << ' ' << d.longPrefix() << ' ' << d.longSuffix() << '\n';
    }
}
'''),
    "runenumerate": ("4_1_007.hpp", r'''
signed main()
{
    string s; cin >> s; auto a = runs(s); cout << a.size() << '\n';
    for (auto x : a) cout << x.p << ' ' << x.l << ' ' << x.r << '\n';
}
'''),
    "suffixarray": ("4_3_002.hpp", r'''
signed main()
{
    string s; cin >> s; auto a = suffixArray(s);
    for (int i = 0; i < (int)a.size(); i++) cout << (i ? " " : "") << a[i];
    cout << '\n';
}
'''),
    "wildcard_pattern_matching": ("4_1_011.hpp", r'''
signed main()
{
    string s, t; cin >> s >> t; auto a = wildcard(s, t);
    for (bool x : a) cout << x;
    cout << '\n';
}
'''),
    "zalgorithm": ("4_1_003.hpp", r'''
signed main()
{
    string s; cin >> s; auto a = zfunc(s);
    for (int i = 0; i < (int)a.size(); i++) cout << (i ? " " : "") << a[i];
    cout << '\n';
}
'''),
}

TESTS = {
    "aho_corasick": "tests/test_ch04_ac.cpp",
    "eertree": "tests/test_ch04_pam.cpp",
    "enumerate_palindromes": "tests/test_ch04_manacher.cpp",
    "longest_common_substring": "tests/test_ch04_longest_common_substring.cpp",
    "lyndon_factorization": "tests/test_ch04_lyndon.cpp",
    "number_of_substrings": "tests/test_ch04_sam.cpp",
    "palindromes_in_deque": "tests/test_ch04_deque_pam.cpp",
    "runenumerate": "tests/test_ch04_runs.cpp",
    "suffixarray": "tests/test_ch04_suffix_array.cpp",
    "wildcard_pattern_matching": "tests/test_ch04_wildcard.cpp",
    "zalgorithm": "tests/test_ch04_z.cpp",
}

TITLES = {
    "aho_corasick": "Aho Corasick", "eertree": "Eertree", "enumerate_palindromes": "Enumerate Palindromes",
    "longest_common_substring": "Longest Common Substring", "lyndon_factorization": "Lyndon Factorization",
    "number_of_substrings": "Number of Substrings", "palindromes_in_deque": "Palindromes in Deque",
    "runenumerate": "Run Enumerate", "suffixarray": "Suffix Array", "wildcard_pattern_matching": "Wildcard Pattern Matching",
    "zalgorithm": "Z Algorithm",
}

def expand(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen: return ""
    seen.add(path)
    out = []
    for line in path.read_text(encoding="utf-8").replace("\r\n", "\n").splitlines(True):
        if line.strip() == "#pragma once": continue
        m = INC.match(line.rstrip("\n"))
        out.append(expand(path.parent / m.group(1), seen) if m else line)
    return "".join(out)

def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()

def expected_meta(name: str, digest: str) -> dict:
    source_rel = f"string/{name}/main.cpp"
    return {
        "schema_version": 3,
        "name": name,
        "title": TITLES[name],
        "category": "string",
        "provenance": {
            "kind": "template_solution",
            "source": source_rel,
            "source_sha256": digest,
        },
        "official_reference": {
            "commit": COMMIT,
            "category": "string",
            "problem": name,
        },
        "validation": {
            "status": "local_only",
            "current_source_sha256": digest,
            "checks": ["source_cxx20_compile", "hushuqi算法竞赛模板/" + TESTS[name]],
        },
        "submission_attempts": [],
    }


def generated_source(name: str, header: str, adapter: str) -> bytes:
    return (
        expand(TEMPLATE / "include" / "full" / header, set()).rstrip()
        + "\n\n"
        + adapter.strip()
        + "\n"
    ).encode()


def generated_entry(name: str, digest: str) -> dict:
    return {
        "name": name,
        "title": TITLES[name],
        "category": "string",
        "source": f"string/{name}/main.cpp",
        "source_sha256": digest,
        "meta": f"string/{name}/meta.json",
        "state": "template_solution",
    }


def read_json(path: Path) -> dict | None:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return None


def current_meta_or_reset(name: str, target: Path, digest: str, source_changed: bool) -> dict:
    """Keep verdict history when refreshing an unchanged generated source.

    A changed source invalidates current local/online evidence, but historical
    submission attempts remain useful audit evidence and must not be erased.
    """
    baseline = expected_meta(name, digest)
    previous = read_json(target.parent / "meta.json")
    if previous is None or source_changed:
        if previous and isinstance(previous.get("submission_attempts"), list):
            baseline["submission_attempts"] = previous["submission_attempts"]
        return baseline
    previous.update({
        "schema_version": 3,
        "name": name,
        "title": TITLES[name],
        "category": "string",
        "provenance": baseline["provenance"],
        "official_reference": baseline["official_reference"],
    })
    return previous


def check_current(manifest: dict) -> list[str]:
    entries = {x["name"]: x for x in manifest.get("problems", [])}
    problems: list[str] = []
    for name, (header, adapter) in SPECS.items():
        data = generated_source(name, header, adapter)
        digest = hashlib.sha256(data).hexdigest()
        target = OUT / "string" / name / "main.cpp"
        meta = read_json(target.parent / "meta.json")
        expected = generated_entry(name, digest)
        if not target.is_file() or target.read_bytes() != data:
            problems.append(f"source out of date: {target.relative_to(ROOT)}")
        if entries.get(name) != expected:
            problems.append(f"manifest out of date: string/{name}")
        if meta is None:
            problems.append(f"missing or invalid metadata: string/{name}")
        elif (
            meta.get("provenance") != expected_meta(name, digest)["provenance"]
            or meta.get("official_reference") != expected_meta(name, digest)["official_reference"]
        ):
            problems.append(f"metadata provenance out of date: string/{name}")
    return problems


def write_current(manifest: dict) -> None:
    manifest_path = OUT / "manifest.json"
    entries = {x["name"]: x for x in manifest["problems"]}
    for name, (header, adapter) in SPECS.items():
        data = generated_source(name, header, adapter)
        target = OUT / "string" / name / "main.cpp"
        source_changed = not target.is_file() or target.read_bytes() != data
        target.parent.mkdir(parents=True, exist_ok=True)
        if source_changed:
            target.write_bytes(data)
        digest = hashlib.sha256(data).hexdigest()
        meta = current_meta_or_reset(name, target, digest, source_changed)
        (target.parent / "meta.json").write_text(
            json.dumps(meta, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
        )
        entries[name] = generated_entry(name, digest)
    manifest["problems"] = [entries[k] for k in sorted(entries)]
    manifest["problem_count"] = len(manifest["problems"])
    manifest["counts"] = {"template_solution": len(manifest["problems"])}
    manifest_path.write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate or verify chapter 04 standalone Library Checker sources."
    )
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true", help="verify without writing")
    mode.add_argument("--write", action="store_true", help="refresh generated files explicitly")
    args = parser.parse_args()
    manifest_path = OUT / "manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if args.check:
        problems = check_current(manifest)
        if problems:
            print("CH04_GENERATION_OUT_OF_DATE", file=sys.stderr)
            print("\n".join(problems), file=sys.stderr)
            raise SystemExit(1)
        print("CH04_GENERATION_CURRENT")
        return
    write_current(manifest)

if __name__ == "__main__": main()
