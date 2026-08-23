#!/usr/bin/env python3
"""Generate C++ alternatives that share an LC problem with an existing candidate."""

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
TEMPLATE = WORKSPACE / "hushuqi算法竞赛模板"
MANIFEST = ROOT / "variants.json"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def expand(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    out = []
    for line in path.read_text(encoding="utf-8").replace("\r\n", "\n").splitlines(True):
        if line.strip() == "#pragma once":
            continue
        match = INCLUDE_RE.match(line.rstrip("\n"))
        out.append(expand(path.parent / match.group(1), seen) if match else line)
    return "".join(out)


@dataclass(frozen=True)
class Spec:
    variant_id: str
    problem: str
    title: str
    category: str
    topic: str
    header: str
    test: str
    adapter: str
    complexity: str


def source_header(spec: Spec) -> tuple[str, list[dict[str, str]]]:
    header = TEMPLATE / "include" / "full" / spec.header
    if not header.is_file():
        header = TEMPLATE / "include" / spec.header
    if not header.is_file():
        raise FileNotFoundError(header)
    return expand(header, set()), [{
        "path": "hushuqi算法竞赛模板/" + (
            "include/full/" + spec.header
            if (TEMPLATE / "include" / "full" / spec.header).is_file()
            else "include/" + spec.header
        ),
        "sha256": digest(header),
    }]


ORDERED_ADAPTER = r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; TREE_TYPE<int> tr;
    while (n--) { int x; cin >> x; tr.add(x); }
    while (q--)
    {
        int t, x; cin >> t >> x;
        if (t == 0) { if (tr.rank(x + 1) == tr.rank(x)) tr.add(x); }
        else if (t == 1) tr.erase(x);
        else if (t == 2) cout << (x > tr.size(tr.root) ? -1 : tr.kth(x - 1)) << '\n';
        else if (t == 3) cout << tr.rank(x + 1) << '\n';
        else if (t == 4) { int k = tr.rank(x + 1); cout << (k ? tr.kth(k - 1) : -1) << '\n'; }
        else { int k = tr.rank(x); cout << (k < tr.size(tr.root) ? tr.kth(k) : -1) << '\n'; }
    }
}
'''


def ordered_adapter(tree: str) -> str:
    return ORDERED_ADAPTER.replace("TREE_TYPE", tree)


def fhq_adapter() -> str:
    return ORDERED_ADAPTER.replace("TREE_TYPE<int> tr", "FHQ<int> tr").replace(
        "tr.add(x)", "tr.insert(x)"
    )


def lca_adapter(kind: str) -> str:
    common = """
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<vector<int>> g(n);
    for (int v = 1; v < n; ++v) { int p; cin >> p; g[p].push_back(v); g[v].push_back(p); }
    vector<pair<int, int>> queries; while (q--) { int u, v; cin >> u >> v; queries.push_back({u, v}); }
    ANSWER
}
"""
    if kind == "rmq":
        return common.replace("ANSWER", "LCARMQ lca(g); for (auto [u, v] : queries) cout << lca.lca(u, v) << '\\n';")
    return common.replace("ANSWER", "for (int x : offlineLCA(g, 0, queries)) cout << x << '\\n';")


def online_lca_adapter(tree: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<vector<int>> g(n);
    for (int v = 1; v < n; ++v) {{ int p; cin >> p; g[p].push_back(v); g[v].push_back(p); }}
    {tree} lca(g); while (q--) {{ int u, v; cin >> u >> v; cout << lca.lca(u, v) << '\n'; }}
}}
'''


def suffix_double_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s; vector<int> a; for (unsigned char c : s) a.push_back(c);
    auto sa = saDouble(a);
    for (int i = 0; i < (int)sa.size(); ++i) cout << (i ? " " : "") << sa[i];
    cout << '\n';
}
'''


def convolution_adapter(call: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m; vector<Z> a(n), b(m);
    for (auto &x : a) {{ int v; cin >> v; x = v; }}
    for (auto &x : b) {{ int v; cin >> v; x = v; }}
    auto c = {call}(move(a), move(b));
    for (int i = 0; i < (int)c.size(); ++i) cout << c[i].val() << " \n"[i + 1 == (int)c.size()];
}}
'''


def weighted_blossom_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m; vector<tuple<int, int, long long>> edge;
    while (m--) { int u, v; cin >> u >> v; edge.push_back({u, v, 1}); }
    WBlossom<long long> matching(n); auto [weight, answer] = matching.solve(edge);
    cout << answer.size() << '\n'; for (auto [u, v] : answer) cout << u << ' ' << v << '\n';
}
'''


def matching_adapter(flow_type: str, add_returns_index: bool, capacity_field: str = "cap") -> str:
    add_edge = (
        "int id = f.add(u, l + v, 1);"
        if add_returns_index
        else "int id = f.e[u].size(); f.add(u, l + v, 1);"
    )
    edge_array = "f.e" if flow_type != "Flow<long long>" else "f.e"
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int l, r, m; cin >> l >> r >> m; int s = l + r, t = s + 1;
    {flow_type} f(t + 1);
    for (int u = 0; u < l; ++u) f.add(s, u, 1);
    for (int v = 0; v < r; ++v) f.add(l + v, t, 1);
    vector<array<int, 3>> edge;
    while (m--) {{ int u, v; cin >> u >> v; {add_edge} edge.push_back({{u, v, id}}); }}
    auto value = f.flow(s, t); cout << value << '\n';
    for (auto [u, v, id] : edge) if ({edge_array}[u][id].{capacity_field} == 0) cout << u << ' ' << v << '\n';
}}
'''


def scc_adapter(function: str, reverse: bool = False) -> str:
    code = rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m; vector<vector<int>> g(n);
    while (m--) {{ int u, v; cin >> u >> v; g[u].push_back(v); }}
    auto bel = {function}(g); int k = *max_element(bel.begin(), bel.end()) + 1;
    vector<vector<int>> part(k); for (int u = 0; u < n; ++u) part[SCC_ID].push_back(u);
    cout << k << '\n'; for (auto &v : part) {{ cout << v.size(); for (int u : v) cout << ' ' << u; cout << '\n'; }}
}}
'''
    return code.replace("SCC_ID", "k - 1 - bel[u]" if reverse else "bel[u]")


def hk_adapter(class_name: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int l, r, m; cin >> l >> r >> m; {class_name} h(l, r);
    while (m--) {{ int u, v; cin >> u >> v; h.addEdge(u, v); }}
    cout << h.matching() << '\n'; for (int u = 0; u < l; ++u) if (h.leftMatch()[u] != -1) cout << u << ' ' << h.leftMatch()[u] << '\n';
}}
'''


def two_sat_adapter(class_name: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string p, cnf; int n, m; cin >> p >> cnf >> n >> m; {class_name} sat(n);
    while (m--) {{ int x, y, z; cin >> x >> y >> z; sat.addOr(abs(x) - 1, x > 0, abs(y) - 1, y > 0); }}
    auto ans = sat.solve(); if (!ans) {{ cout << "s UNSATISFIABLE\n"; return 0; }}
    cout << "s SATISFIABLE\nv"; for (int i = 0; i < n; ++i) cout << ' ' << ((*ans)[i] ? i + 1 : -i - 1); cout << " 0\n";
}}
'''


def kruskal_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m; vector<MEdge> e(m); map<array<int, 3>, vector<int>> ids;
    for (int i = 0; i < m; ++i) { cin >> e[i].x >> e[i].y >> e[i].w; ids[{e[i].x, e[i].y, e[i].w}].push_back(i); }
    auto ans = kruskal(n, e); assert(ans); cout << ans->first << '\n'; vector<int> out;
    for (auto x : ans->second) { auto &v = ids[{x.x, x.y, x.w}]; out.push_back(v.back()); v.pop_back(); }
    for (int i = 0; i < (int)out.size(); ++i) cout << (i ? " " : "") << out[i]; cout << '\n';
}
'''


def assignment_adapter(flow_type: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; int s = 2 * n, t = s + 1; {flow_type} f(t + 1);
    for (int i = 0; i < n; ++i) f.add(s, i, 1, 0);
    for (int j = 0; j < n; ++j) f.add(n + j, t, 1, 0);
    vector<vector<int>> id(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
    {{ long long c; cin >> c; id[i][j] = f.e[i].size(); f.add(i, n + j, 1, c); }}
    auto [sent, cost] = f.flow(s, t, n); assert(sent == n); cout << cost << '\n';
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (f.e[i][id[i][j]].cap == 0)
    {{ cout << j << " \n"[i + 1 == n]; break; }}
}}
'''


def staticrmq_adapter(kind: str) -> str:
    if kind == "dst":
        return r'''
struct MinOp
{
    long long operator()(long long a, long long b) const
    {
        return min(a, b);
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<long long> a(n);
    for (auto &x : a) cin >> x;
    DST<long long, MinOp> st(a, MinOp{});
    while (q--)
    {
        int l, r; cin >> l >> r; cout << st.query(l, r) << '\n';
    }
}
'''
    raise ValueError(kind)


def block_st_adapter() -> str:
    return r'''
struct MinOp
{
    long long operator()(long long a, long long b) const
    {
        return min(a, b);
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<long long> a(n);
    for (auto &x : a) cin >> x;
    BlockST<long long, MinOp> st(a, MinOp{});
    while (q--)
    {
        int l, r; cin >> l >> r; cout << st.query(l, r) << '\n';
    }
}
'''


def fixed_hash_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int q; cin >> q; FixedHashMap<long long, long long, 21> value;
    while (q--)
    {
        int t; long long key; cin >> t >> key;
        if (t == 0)
        {
            long long x; cin >> x; value[key] = x;
        }
        else
        {
            cout << value.get(key) << '\n';
        }
    }
}
'''


def fast_bit_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<long long> a(n);
    for (auto &x : a) cin >> x;
    FastBIT<long long> bit(a);
    while (q--)
    {
        int t, x, y; cin >> t >> x >> y;
        if (t == 0) bit.add(x, y);
        else cout << bit.sum(x, y) << '\n';
    }
}
'''


def fixed_fwt_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int k; cin >> k; uint32_t n = 1U << k;
    vector<uint32_t> a(n), b(n);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    auto c = bitConvMod<998244353, BitOp::And>(move(a), move(b));
    for (uint32_t i = 0; i < n; i++) cout << c[i] << " \n"[i + 1 == n];
}
'''


def fast_det_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; vector a(n, vector<long long>(n));
    for (auto &row : a) for (auto &x : row) cin >> x;
    cout << detPrimeFast<998244353>(a) << '\n';
}
'''


def decimal_bigint_add_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (BigInt(a) + BigInt(b)).toDecimal() << '\n';
    }
}
'''


def lis_values_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; vector<int> a(n);
    for (auto &x : a) cin >> x;
    auto value = lis(a);
    vector<int> answer;
    int p = 0;
    for (int x : value)
    {
        while (a[p] != x) ++p;
        answer.push_back(p++);
    }
    cout << answer.size() << '\n';
    for (int i = 0; i < (int)answer.size(); ++i)
        cout << answer[i] << " \n"[i + 1 == (int)answer.size()];
}
'''


def persistent_kth_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<int> a(n), value;
    for (auto &x : a) cin >> x;
    value = a; sort(value.begin(), value.end());
    value.erase(unique(value.begin(), value.end()), value.end());
    PerSeg st(value.size()); vector<int> root(n + 1);
    for (int i = 0; i < n; ++i)
        root[i + 1] = st.add(root[i], lower_bound(value.begin(), value.end(), a[i]) - value.begin(), 1);
    while (q--)
    {
        int l, r, k; cin >> l >> r >> k;
        cout << value[st.kth(root[l], root[r], k)] << '\n';
    }
}
'''


def pbds_hash_adapter() -> str:
    return r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int q; cin >> q; pbds_hash::HashMap<long long, long long> value;
    while (q--)
    {
        int t; long long key; cin >> t >> key;
        if (t == 0)
        {
            long long x; cin >> x; value[key] = x;
        }
        else
        {
            auto it = value.find(key);
            cout << (it == value.end() ? 0 : it->second) << '\n';
        }
    }
}
'''


def depq_adapter(tree: str, insert: str) -> str:
    return rf'''
signed main()
{{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; {tree}<int> tr;
    while (n--) {{ int x; cin >> x; tr.{insert}(x); }}
    while (q--)
    {{
        int t; cin >> t;
        if (t == 0) {{ int x; cin >> x; tr.{insert}(x); }}
        else if (t == 1) {{ int x = tr.kth(0); cout << x << '\n'; tr.erase(x); }}
        else {{ int x = tr.kth(tr.size(tr.root) - 1); cout << x << '\n'; tr.erase(x); }}
    }}
}}
'''


SPECS = (
    Spec("12_2_007_lis_values_cpp20", "longest_increasing_subsequence", "Longest Increasing Subsequence", "other", "12.2.007", "12_2_007.hpp", "tests/test_ch12_dp.cpp", lis_values_adapter(), "O(N log N) time and O(N) memory"),
    Spec("3_3_008_persistent_seg_kth_cpp20", "range_kth_smallest", "Range Kth Smallest", "data_structure", "3.3.008", "3_3_008.hpp", "tests/test_ch03_perseg.cpp", persistent_kth_adapter(), "O((N+Q) log N) time and O(N log N) memory"),
    Spec("3_4_003_dst_cpp20", "staticrmq", "Static RMQ", "data_structure", "3.4.003", "3_4_003.hpp", "tests/test_ch03_dst.cpp", staticrmq_adapter("dst"), "O(N log N) preprocessing, O(1) query"),
    Spec("3_4_001_block_st_cpp20", "staticrmq", "Static RMQ", "data_structure", "3.4.001", "3_4_001.hpp", "tests/test_ch03_block_st.cpp", block_st_adapter(), "O(N + (N/B) log(N/B)) preprocessing and memory, O(B) same-block and O(1) cross-block query for fixed B=16"),
    Spec("3_5_001_treap_depq_cpp20", "double_ended_priority_queue", "Double-Ended Priority Queue", "data_structure", "3.5.001", "3_5_001.hpp", "tests/test_ch03_treap.cpp", depq_adapter("Treap", "add"), "O(log N) expected per operation"),
    Spec("3_5_002_fhq_depq_cpp20", "double_ended_priority_queue", "Double-Ended Priority Queue", "data_structure", "3.5.002", "3_5_002.hpp", "tests/test_ch03_fhq.cpp", depq_adapter("FHQ", "insert"), "O(log N) expected per operation"),
    Spec("3_5_004_splay_depq_cpp20", "double_ended_priority_queue", "Double-Ended Priority Queue", "data_structure", "3.5.004", "3_5_004.hpp", "tests/test_ch03_splay.cpp", depq_adapter("Splay", "add"), "O(log N) amortized per operation"),
    Spec("3_9_011_gp_hash_cpp20", "associative_array", "Associative Array", "data_structure", "3.9.011", "3_9_011.hpp", "tests/test_ch03_pbds_hash.cpp", pbds_hash_adapter(), "O(1) expected per operation"),
    Spec("3_6_006_fixed_hash_cpp20", "associative_array", "Associative Array", "data_structure", "3.6.006", "3_6_006.hpp", "tests/test_ch03_fixed_hash.cpp", fixed_hash_adapter(), "O(1) expected per operation and O(2^21) memory"),
    Spec("14_1_019_decimal_bigint_add_cpp20", "addition_of_big_integers", "Addition of Big Integers", "big_integer", "14.1.019", "14_1_019.hpp", "tests/test_ch14_hex_bigint.cpp", decimal_bigint_add_adapter(), "O(N) addition over base-1e9 limbs"),
    Spec("3_3_001_seg_cpp20", "point_add_range_sum", "Point Add Range Sum", "data_structure", "3.3.001", "3_3_001.hpp", "tests/test_ch03_seg.cpp", r'''
signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr);int n,q;cin>>n>>q;vector<Sum>a(n);for(auto&x:a)cin>>x.val;Seg<Sum>s(a);while(q--){int t,x,y;cin>>t>>x>>y;if(t==0){a[x].val+=y;s.set(x,a[x]);}else cout<<s.query(x,y).val<<'\n';}}
''', "O((N+Q) log N)"),
    Spec("3_3_005_zkw_seg_cpp20", "point_add_range_sum", "Point Add Range Sum", "data_structure", "3.3.005", "3_3_005.hpp", "tests/test_ch03_zkw_seg.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,q;cin>>n>>q;vector<int>a(n);for(int&x:a)cin>>x;ZkwSeg<int,plus<int>>s(a,0,plus<int>{});while(q--){int t,x,y;cin>>t>>x>>y;if(t==0){a[x]+=y;s.set(x,a[x]);}else cout<<s.prod(x,y)<<'\n';}}
''', "O((N+Q) log N)"),
    Spec("3_2_001_fast_bit_cpp20", "point_add_range_sum", "Point Add Range Sum", "data_structure", "3.2.001", "3_2_001.hpp", "tests/test_ch03_fast_variants.cpp", fast_bit_adapter(), "O(N) preprocessing, O(log N) per update or query, O(N) memory"),
    Spec("3_5_001_treap_cpp20", "ordered_set", "Ordered Set", "data_structure", "3.5.001", "3_5_001.hpp", "tests/test_ch03_treap.cpp", ordered_adapter("Treap"), "O(log N) expected per operation"),
    Spec("3_5_004_splay_cpp20", "ordered_set", "Ordered Set", "data_structure", "3.5.004", "3_5_004.hpp", "tests/test_ch03_splay.cpp", ordered_adapter("Splay"), "O(log N) amortized per operation"),
    Spec("3_5_002_fhq_cpp20", "ordered_set", "Ordered Set", "data_structure", "3.5.002", "3_5_002.hpp", "tests/test_ch03_fhq.cpp", fhq_adapter(), "O(log N) expected per operation, O(N) worst case"),
    Spec("6_1_004_lcarmq_cpp20", "lca", "Lowest Common Ancestor", "tree", "6.1.004", "6_1_004.hpp", "tests/test_ch06_uncovered.cpp", lca_adapter("rmq"), "O(N log N) preprocessing, O(1) per query"),
    Spec("6_1_005_offline_lca_cpp20", "lca", "Lowest Common Ancestor", "tree", "6.1.005", "6_1_005.hpp", "tests/test_ch06_uncovered.cpp", lca_adapter("offline"), "O((N+Q) alpha(N)) offline"),
    Spec("6_1_003_hld_lca_cpp20", "lca", "Lowest Common Ancestor", "tree", "6.1.003", "6_3_001.hpp", "tests/test_ch06_hld.cpp", online_lca_adapter("HLD"), "O(N) preprocessing, O(log N) per query"),
    Spec("6_3_002_reroot_hld_lca_cpp20", "lca", "Lowest Common Ancestor", "tree", "6.3.002", "6_3_002.hpp", "tests/test_ch06_uncovered.cpp", online_lca_adapter("RerootHLD"), "O(N) preprocessing, O(log N) per query"),
    Spec("4_3_001_sa_double_cpp20", "suffixarray", "Suffix Array", "string", "4.3.001", "4_3_002.hpp", "tests/test_ch04_sa_double.cpp", suffix_double_adapter(), "O(N log^2 N) time and O(N) memory"),
    Spec("10_1_003_plain_ntt_cpp20", "convolution_mod", "Convolution", "convolution", "10.1.003", "10_1_003.hpp", "tests/test_ch10_convolution.cpp", convolution_adapter("plain_ntt::conv"), "O((N+M) log(N+M)) time and O(bit_ceil(N+M)) memory"),
    Spec("10_1_006_fixed_fwt_and_cpp20", "bitwise_and_convolution", "Bitwise And Convolution", "convolution", "10.1.006", "10_1_006.hpp", "tests/test_ch10_transform_edges.cpp", fixed_fwt_adapter(), "O(N log N) time and O(N) memory"),
    Spec("9_1_006_fast_det_cpp20", "matrix_det", "Determinant of Matrix", "linear_algebra", "9.1.006", "9_1_006.hpp", "tests/test_ch09_revised.cpp", fast_det_adapter(), "O(N^3 + N log P) time and O(N^2) memory"),
    Spec("5_6_004_csr_hopcroft_karp_cpp20", "bipartitematching", "Matching on Bipartite Graph", "graph", "5.6.004", "5_6_004.hpp", "tests/test_ch05_matching_variants.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int l,r,m;cin>>l>>r>>m;HopKarpCSR h(l,r);while(m--){int u,v;cin>>u>>v;h.addEdge(u,v);}cout<<h.matching()<<'\n';for(int u=0;u<l;u++)if(h.leftMatch()[u]!=-1)cout<<u<<' '<<h.leftMatch()[u]<<'\n';}
''', "O(E sqrt(V))"),
    Spec("5_8_001_dinic_cpp20", "bipartitematching", "Matching on Bipartite Graph", "graph", "5.8.001", "5_8_001.hpp", "tests/test_ch05_flow.cpp", matching_adapter("Flow<long long>", True), "O(V^2 E) general Dinic bound"),
    Spec("5_8_002_push_relabel_cpp20", "bipartitematching", "Matching on Bipartite Graph", "graph", "5.8.002", "5_8_002.hpp", "tests/test_ch05_flow.cpp", matching_adapter("PushRelabel<long long>", False, "c"), "O(V^3) generic FIFO push-relabel bound"),
    Spec("5_8_009_isap_cpp20", "bipartitematching", "Matching on Bipartite Graph", "graph", "5.8.009", "5_8_009.hpp", "tests/test_ch05_flow.cpp", matching_adapter("ISAP<long long>", False), "O(V^2 E) generic ISAP bound"),
    Spec("5_3_002_kosaraju_cpp20", "scc", "Strongly Connected Components", "graph", "5.3.002", "5_3_002.hpp", "tests/test_ch05_components.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<vector<int>>g(n);while(m--){int u,v;cin>>u>>v;g[u].push_back(v);}auto b=kosarajuSCC(g);int k=*max_element(b.begin(),b.end())+1;vector<vector<int>>p(k);for(int i=0;i<n;i++)p[b[i]].push_back(i);cout<<k<<'\n';for(auto&v:p){cout<<v.size();for(int x:v)cout<<' '<<x;cout<<'\n';}}
''', "O(V+E)"),
    Spec("5_3_001_tarjan_cpp20", "scc", "Strongly Connected Components", "graph", "5.3.001", "5_3_001.hpp", "tests/test_ch05_components.cpp", scc_adapter("tarjanSCC", True), "O(V+E)"),
    Spec("5_6_003_hopcroft_karp_cpp20", "bipartitematching", "Matching on Bipartite Graph", "graph", "5.6.003", "5_6_003.hpp", "tests/test_ch05_matching_variants.cpp", hk_adapter("HopcroftKarp"), "O(E sqrt(V))"),
    Spec("5_5_013_prim_cpp20", "minimum_spanning_tree", "Minimum Spanning Tree", "graph", "5.5.013", "5_5_013.hpp", "tests/test_ch05_foundations.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<vector<pair<int,int>>>g(n);map<tuple<int,int,int>,vector<int>>ids;for(int i=0;i<m;i++){int u,v,w;cin>>u>>v>>w;g[u].push_back({v,w});g[v].push_back({u,w});ids[{min(u,v),max(u,v),w}].push_back(i);}auto z=primSparse(g);assert(z);cout<<z->sum<<'\n';for(int v=0;v<n;v++)if(z->fa[v]!=-1){int u=z->fa[v],w=LLONG_MAX;for(auto[x,c]:g[v])if(x==u)w=min(w,c);auto&a=ids[{min(u,v),max(u,v),w}];cout<<a.back()<<' ';a.pop_back();}cout<<'\n';}
''', "O(E log V)"),
    Spec("5_4_002_kosaraju_two_sat_cpp20", "two_sat", "2 Sat", "other", "5.4.002", "5_4_002.hpp", "tests/test_ch05_two_sat.cpp", r'''
signed main(){string p,cnf;int n,m;cin>>p>>cnf>>n>>m;Kosaraju2SAT s(n);while(m--){int x,y,z;cin>>x>>y>>z;s.addOr(abs(x)-1,x>0,abs(y)-1,y>0);}auto a=s.solve();if(!a)return cout<<"s UNSATISFIABLE\n",0;cout<<"s SATISFIABLE\nv";for(int i=0;i<n;i++)cout<<' '<<((*a)[i]?i+1:-i-1);cout<<" 0\n";}
''', "O(V+E)"),
    Spec("5_4_001_tarjan_two_sat_cpp20", "two_sat", "2 Sat", "other", "5.4.001", "5_4_001.hpp", "tests/test_ch05_two_sat.cpp", two_sat_adapter("TarjanTwoSAT"), "O(V+E)"),
    Spec("5_5_001_kruskal_cpp20", "minimum_spanning_tree", "Minimum Spanning Tree", "graph", "5.5.001", "5_5_001.hpp", "tests/test_ch05_foundations.cpp", kruskal_adapter(), "O(E log E)"),
    Spec("5_8_003_spfa_cost_flow_cpp20", "assignment", "Assignment Problem", "graph", "5.8.003", "5_8_003.hpp", "tests/test_ch05_flow.cpp", assignment_adapter("SpfaCostFlow<long long, long long>"), "O(FVE) worst case"),
    Spec("5_8_004_primal_dual_cost_flow_cpp20", "assignment", "Assignment Problem", "graph", "5.8.004", "5_8_004.hpp", "tests/test_ch05_flow.cpp", assignment_adapter("CostFlow<long long, long long>"), "O(VE + F E log V) including initial potentials"),
    Spec("5_7_006_weighted_blossom_unweighted_cpp20", "general_matching", "Matching on General Graph", "graph", "5.7.006", "5_7_006.hpp", "tests/check_weighted_blossom.cpp", weighted_blossom_adapter(), "O(N^3) time and O(N^2) memory"),
)


def write_json(path: Path, value: object) -> None:
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8", newline="\n")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    entries = {entry["variant_id"]: entry for entry in manifest["variants"]}
    entries.pop("5_8_010_hlpp_cpp20", None)
    generator_hash = digest(Path(__file__))
    for spec in SPECS:
        test = TEMPLATE / spec.test
        header_source, header_inputs = source_header(spec)
        source_data = (header_source.rstrip() + "\n\n" + spec.adapter.strip() + "\n").encode("utf-8")
        source_hash = hashlib.sha256(source_data).hexdigest()
        rel_dir = Path(spec.category) / spec.problem / "variants" / spec.variant_id
        source_rel, meta_rel = rel_dir / "main.cpp", rel_dir / "meta.json"
        source_path, meta_path = ROOT / source_rel, ROOT / meta_rel
        if args.check:
            if not source_path.is_file() or source_path.read_bytes() != source_data:
                raise SystemExit(f"SOURCE_OUT_OF_DATE: {spec.variant_id}")
        else:
            source_path.parent.mkdir(parents=True, exist_ok=True)
            source_path.write_bytes(source_data)
        old = json.loads(meta_path.read_text(encoding="utf-8")) if meta_path.is_file() else {}
        attempts = old.get("submission_attempts", [])
        validation = old.get("validation", {})
        if validation.get("current_source_sha256") != source_hash:
            validation = {"status": "local_only", "current_source_sha256": source_hash,
                          "current_source_online_verdict": None, "current_source_submission_id": None,
                          "server_source_sha256": None,
                          "checks": ["source_cxx20_compile", "hushuqi算法竞赛模板/" + spec.test]}
        meta = {"schema_version": 1, "kind": "template_variant", "variant_id": spec.variant_id,
                "name": spec.problem, "title": spec.title, "category": spec.category,
                "language": "cpp20", "source": source_rel.as_posix(), "source_sha256": source_hash,
                "template_items": [spec.topic], "complexity": spec.complexity,
                "provenance": {"method": "deterministic_generation",
                               "generator": "library-checker-template-solutions/tools/generate_cpp_variants.py",
                               "generator_sha256": generator_hash,
                               "inputs": header_inputs + [
                                   {"path": "hushuqi算法竞赛模板/" + spec.test, "sha256": digest(test)}
                               ],
                               "generated_sha256": source_hash},
                "validation": validation, "submission_attempts": attempts}
        if not args.check:
            write_json(meta_path, meta)
        entry = {"variant_id": spec.variant_id, "problem": spec.problem, "title": spec.title,
                 "category": spec.category, "language": "cpp20", "source": source_rel.as_posix(),
                 "source_sha256": source_hash, "meta": meta_rel.as_posix(),
                 "template_items": [spec.topic], "test": "hushuqi算法竞赛模板/" + spec.test}
        if args.check:
            if entries.get(spec.variant_id) != entry:
                raise SystemExit(f"MANIFEST_OUT_OF_DATE: {spec.variant_id}")
        else:
            entries[spec.variant_id] = entry
    if not args.check:
        manifest["variants"] = [entries[key] for key in sorted(entries)]
        manifest["variant_count"] = len(entries)
        manifest["updated_at"] = datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        write_json(MANIFEST, manifest)


if __name__ == "__main__":
    main()
