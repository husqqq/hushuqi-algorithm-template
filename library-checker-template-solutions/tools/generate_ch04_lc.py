from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TEMPLATE = next(p for p in ROOT.iterdir() if p.is_dir() and p.name.startswith("hushuqi"))
OUT = ROOT / "library-checker-template-solutions"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
INC = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*(?://.*)?$')
# 仅对已逐题复核的条目强制完整来源证明；其余条目继续公开同步缺口。
PROVEN = {"aho_corasick", "eertree", "number_of_substrings", "longest_common_substring", "runenumerate", "palindromes_in_deque", "zalgorithm", "enumerate_palindromes", "lyndon_factorization"}

SPECS = {
    "aho_corasick": ("4_2_002.hpp", r'''
signed main()
{
    // 无参数；按题面编号输出 Trie 父边、fail 边和各模式终态，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    AC<26> ac;
    vector<int> ends(n);
    for (auto &x : ends)
    {
        string s;
        cin >> s;
        x = ac.add(s);
    }
    ac.build();
    const auto &nodes = ac.nodes();
    vector<int> par(nodes.size(), -1);
    for (int u = 0; u < (int)nodes.size(); u++)
    {
        for (int c = 0; c < 26; c++)
        {
            if (nodes[u].ch[c] != -1)
            {
                par[nodes[u].ch[c]] = u;
            }
        }
    }
    cout << nodes.size() << '\n';
    for (int u = 1; u < (int)nodes.size(); u++)
    {
        cout << par[u] << ' ' << nodes[u].fail << '\n';
    }
    for (int i = 0; i < n; i++)
    {
        cout << (i ? " " : "") << ends[i];
    }
    cout << '\n';
}

'''),
    "eertree": ("4_4_001.hpp", r'''
signed main()
{
    // 无参数；输出回文树父边、后缀链接和每个前缀的最长回文后缀，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    PAM<26, 'a'> pam;
    vector<int> last;
    for (char c : s)
    {
        last.push_back(pam.add(c));
    }
    const auto &nodes = pam.t;
    vector<int> par(nodes.size(), -1);
    for (int u = 0; u < (int)nodes.size(); u++)
    {
        for (int c = 0; c < 26; c++)
        {
            if (nodes[u].ch[c])
            {
                par[nodes[u].ch[c]] = u;
            }
        }
    }
    cout << nodes.size() - 2 << '\n';
    for (int u = 2; u < (int)nodes.size(); u++)
    {
        cout << par[u] - 1 << ' ' << nodes[u].link - 1 << '\n';
    }
    for (int i = 0; i < (int)s.size(); i++)
    {
        cout << (i ? " " : "") << last[i] - 1;
    }
    cout << '\n';
}
'''),
    "enumerate_palindromes": ("4_1_004.hpp", r'''
signed main()
{
    // 无参数；按字符中心、相邻字符缝中心的顺序输出最长回文长度，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    Manacher m(s);
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
    // 无参数；输出两串的一组最长公共子串半开坐标，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string a, b;
    cin >> a >> b;
    auto ans = commonSubstr(a, b);
    cout << ans[0] << ' ' << ans[1] << ' ' << ans[2] << ' ' << ans[3] << '\n';
}
'''),
    "lyndon_factorization": ("4_1_006.hpp", r'''
signed main()
{
    // 无参数；输出 Lyndon 分解左端点及末尾哨兵，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto a = lyndon(s);
    for (int i = 0; i < (int)a.size(); i++) cout << (i ? " " : "") << a[i];
    cout << '\n';
}
'''),
    "number_of_substrings": ("4_3_010.hpp", r'''
signed main()
{
    // 无参数；输出小写串的本质不同非空子串数，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    cout << subDistinct(s, 0, s.size()) << '\n';
}
'''),
    "palindromes_in_deque": ("4_4_002.hpp", r'''
signed main()
{
    // 无参数；逐次输出当前非空回文种数、最长回文前缀和后缀长度，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    DequePAM<26, 'a'> d;
    while (q--)
    {
        int type;
        cin >> type;
        if (type < 2)
        {
            char c;
            cin >> c;
            if (type == 0) d.pushFront(c);
            else d.pushBack(c);
        }
        else if (type == 2) d.popFront();
        else d.popBack();
        cout << d.distinct() << ' ' << d.longPrefix() << ' ' << d.longSuffix() << '\n';
    }
}
'''),
    "runenumerate": ("4_1_007.hpp", r'''
signed main()
{
    // 无参数；按最小周期、左端点、右端点的字典序输出全部极大重复，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto a = runs(s);
    cout << a.size() << '\n';
    for (auto [p, l, r] : a)
    {
        cout << p << ' ' << l << ' ' << r << '\n';
    }
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
    // 无参数；输出输入串的 Z 数组，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto z = zfunc(s);
    for (int i = 0; i < (int)z.size(); i++)
    {
        cout << (i ? " " : "") << z[i];
    }
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
    "number_of_substrings": "tests/test_ch04_range_substrings.cpp",
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

def check_documentation(header: str) -> None:
    """核对已复核条目的正文代码与打印头，不只记录文件哈希。"""
    chapter = (TEMPLATE / "04-字符串.md").read_text(encoding="utf-8")
    topic = header.removesuffix(".hpp").replace("_", ".")
    section = re.search(rf"^### {re.escape(topic)} .*?(?=^### |\Z)", chapter, re.M | re.S)
    fence = re.search(r"```cpp\n(.*?)\n```", section[0], re.S) if section else None
    path = TEMPLATE / "tools/sync_topic_docs.py"
    spec = importlib.util.spec_from_file_location("ch04_doc_sync", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    if fence is None or "```cpp\n" + fence[1] + "\n```" != module.snippet(header):
        raise ValueError(f"{topic}: documentation differs from print header")


def generation_evidence(name: str, digest: str) -> dict:
    header, adapter = SPECS[name]
    check_documentation(header)
    seen: set[Path] = set()
    expand(TEMPLATE / "include/full" / header, seen)
    printed: set[Path] = set()
    expand(TEMPLATE / "include/print" / header, printed)
    # 两个独立系统临时目录各自重新展开，不保留第二棵候选源码。
    for _ in range(2):
        with tempfile.TemporaryDirectory(prefix="template-ch04-proof-") as td:
            candidate = Path(td) / "main.cpp"
            data = generated_source(name, header, adapter)
            candidate.write_bytes(data)
            if candidate.read_bytes() != data or sha(candidate) != digest:
                raise ValueError(f"{name}: nondeterministic generation")
    def record(path: Path) -> dict:
        return {"path": path.relative_to(ROOT).as_posix(), "sha256": sha(path)}
    proof = {
        "method": "deterministic_generation",
        "generator": {
            **record(Path(__file__).resolve()),
            "argv": ["python", "library-checker-template-solutions/tools/generate_ch04_lc.py",
                     "--only", name, "--write"],
            "working_directory": ".",
            "runner": f"Python {sys.version.split()[0]}",
        },
        "template_inputs": [record(p) for p in sorted(seen)],
        "documentation_inputs": [record(TEMPLATE / "04-字符串.md"),
                                 record(TEMPLATE / "tools/sync_topic_docs.py"),
                                 *[record(p) for p in sorted(printed)]],
        "adapter_sha256": hashlib.sha256(adapter.strip().encode()).hexdigest(),
        "controlled_reference": record(ROOT / "library-checker-solutions/string" / name / "main.cpp"),
        "temporary_regenerations": 2,
        "byte_identical": True,
        "source_sha256": digest,
    }
    proof["evidence_sha256"] = hashlib.sha256(
        json.dumps(proof, sort_keys=True, ensure_ascii=False, separators=(",", ":")).encode()
    ).hexdigest()
    return proof


def expected_meta(name: str, digest: str) -> dict:
    source_rel = f"string/{name}/main.cpp"
    result = {
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
    if name in PROVEN:
        result["provenance"]["evidence"] = generation_evidence(name, digest)
    return result


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


def check_current(manifest: dict, names=None) -> list[str]:
    entries = {x["name"]: x for x in manifest.get("problems", [])}
    problems: list[str] = []
    for name in SPECS if names is None else names:
        header, adapter = SPECS[name]
        data = generated_source(name, header, adapter)
        digest = hashlib.sha256(data).hexdigest()
        target = OUT / "string" / name / "main.cpp"
        meta = read_json(target.parent / "meta.json")
        expected = generated_entry(name, digest)
        baseline = expected_meta(name, digest)
        if not target.is_file() or target.read_bytes() != data:
            problems.append(f"source out of date: {target.relative_to(ROOT)}")
        if entries.get(name) != expected:
            problems.append(f"manifest out of date: string/{name}")
        if meta is None:
            problems.append(f"missing or invalid metadata: string/{name}")
        elif (
            meta.get("provenance") != baseline["provenance"]
            or meta.get("official_reference") != baseline["official_reference"]
        ):
            problems.append(f"metadata provenance out of date: string/{name}")
    return problems


def write_current(manifest: dict, names=None) -> None:
    manifest_path = OUT / "manifest.json"
    entries = {x["name"]: x for x in manifest["problems"]}
    for name in SPECS if names is None else names:
        header, adapter = SPECS[name]
        data = generated_source(name, header, adapter)
        target = OUT / "string" / name / "main.cpp"
        source_changed = not target.is_file() or target.read_bytes() != data
        target.parent.mkdir(parents=True, exist_ok=True)
        digest = hashlib.sha256(data).hexdigest()
        meta = current_meta_or_reset(name, target, digest, source_changed)
        if source_changed:
            target.write_bytes(data)
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
    parser.add_argument("--only", help="comma-separated problem names; default is all")
    args = parser.parse_args()
    names = list(dict.fromkeys(args.only.split(","))) if args.only is not None else None
    if names is not None and any(name not in SPECS for name in names):
        parser.error("unknown problem in --only")
    manifest_path = OUT / "manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if args.check:
        problems = check_current(manifest, names)
        if problems:
            print("CH04_GENERATION_OUT_OF_DATE", file=sys.stderr)
            print("\n".join(problems), file=sys.stderr)
            raise SystemExit(1)
        print("CH04_GENERATION_CURRENT")
        return
    write_current(manifest, names)

if __name__ == "__main__": main()
