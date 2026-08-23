from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "library-checker-template-solutions"
TEMPLATE = ROOT / "hushuqi算法竞赛模板"
COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
INC = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')

BOUNDARY_INPUTS = {
    "bernoulli_number": "hushuqi算法竞赛模板/tests/lc/ch07_bernoulli_boundary.in",
}

SPECS = {
    ("graph", "assignment"): (
        "5_6_005.hpp", "Assignment Problem", "tests/test_ch05_assignment.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<vector<long long>> a(n, vector<long long>(n));
    for (auto &row : a)
    {
        for (long long &x : row) cin >> x;
    }
    auto ans = assignment(a);
    assert(ans.has_value());
    cout << ans->cost << '\n';
    for (int i = 0; i < n; i++) cout << ans->col[i] << " \n"[i + 1 == n];
}
'''),
    ("graph", "bipartite_edge_coloring"): (
        "5_6_006.hpp", "Edge Coloring of Bipartite Graph", "tests/test_ch05_bipartite_edge_coloring.cpp", r'''
signed main()
{
    int nl, nr, m; cin >> nl >> nr >> m;
    BipEdgeColor g(nl, nr);
    for (int i = 0; i < m; i++)
    {
        int u, v; cin >> u >> v;
        g.add(u, v);
    }
    auto ans = g.work();
    cout << ans.k << '\n';
    for (int x : ans.c) cout << x << '\n';
}
'''),
    ("graph", "maximum_independent_set"): (
        "5_6_008.hpp", "Maximum Independent Set", "tests/test_ch05_maximum_independent_set.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = maxIS(n, e);
    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << " \n"[i + 1 == (int)ans.size()];
}
'''),
    ("graph", "general_matching"): (
        "5_7_001.hpp", "Matching on General Graph", "tests/test_ch05_general_matching.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    Blossom g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v; cin >> u >> v;
        g.add(u, v);
    }
    cout << g.work() << '\n';
    for (int u = 0; u < n; u++) if (u < g.match[u]) cout << u << ' ' << g.match[u] << '\n';
}
'''),
    ("graph", "general_weighted_matching"): (
        "5_7_006.hpp", "General Weighted Matching", "tests/test_ch05_general_matching.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<tuple<int, int, long long>> e(m);
    for (auto &[u, v, w] : e) cin >> u >> v >> w;
    WeightedBlossom<long long> g(n);
    auto [sum, ans] = g.solve(e);
    cout << ans.size() << ' ' << sum << '\n';
    for (auto [u, v] : ans) cout << u << ' ' << v << '\n';
}
'''),
    ("graph", "min_cost_b_flow"): (
        "5_8_011.hpp", "Minimum Cost b-flow", "tests/test_ch05_flow.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    MinCostBFlow<long long, long long, __int128> g(n);
    for (int u = 0; u < n; u++)
    {
        long long b; cin >> b;
        g.supply(u, b);
    }
    for (int i = 0; i < m; i++)
    {
        int u, v; long long l, r, c;
        cin >> u >> v >> l >> r >> c;
        g.add(u, v, l, r, c);
    }
    auto ans = g.solveCertificate();
    if (!ans)
    {
        cout << "infeasible\n";
        return 0;
    }
    auto print = [&](auto &&self, __int128 x) -> void
    {
        if (x < 0) cout << '-', x = -x;
        if (x >= 10) self(self, x / 10);
        cout << (char)('0' + x % 10);
    };
    print(print, ans->value);
    cout << '\n';
    for (long long x : ans->pot) cout << x << '\n';
    for (long long x : ans->flow) cout << x << '\n';
}
'''),
    ("graph", "eulerian_trail_directed"): (
        "5_9_001.hpp", "Eulerian Trail (Directed)", "tests/test_ch05_euler.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n, m; cin >> n >> m;
        vector<pair<int, int>> e(m);
        for (auto &[u, v] : e) cin >> u >> v;
        auto ans = directedEuler(n, e);
        if (!ans)
        {
            cout << "No\n";
            continue;
        }
        cout << "Yes\n";
        for (int i = 0; i <= m; i++) cout << ans->v[i] << " \n"[i == m];
        for (int i = 0; i < m; i++) cout << ans->id[i] << " \n"[i + 1 == m];
    }
}
'''),
    ("graph", "eulerian_trail_undirected"): (
        "5_9_001.hpp", "Eulerian Trail (Undirected)", "tests/test_ch05_euler.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n, m; cin >> n >> m;
        vector<pair<int, int>> e(m);
        for (auto &[u, v] : e) cin >> u >> v;
        auto ans = undirectedEuler(n, e);
        if (!ans)
        {
            cout << "No\n";
            continue;
        }
        cout << "Yes\n";
        for (int i = 0; i <= m; i++) cout << ans->v[i] << " \n"[i == m];
        for (int i = 0; i < m; i++) cout << ans->id[i] << " \n"[i + 1 == m];
    }
}
'''),
    ("graph", "cycle_detection"): (
        "5_9_007.hpp", "Cycle Detection (Directed)", "tests/test_ch05_cycles.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = directedCycleEdges(n, e);
    if (ans.empty()) cout << -1 << '\n';
    else
    {
        cout << ans.size() << '\n';
        for (int id : ans) cout << id << '\n';
    }
}
'''),
    ("graph", "cycle_detection_undirected"): (
        "5_9_007.hpp", "Cycle Detection (Undirected)", "tests/test_ch05_cycles.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = undirectedCycleEdges(n, e);
    if (ans.empty()) cout << -1 << '\n';
    else
    {
        cout << ans.size() << '\n';
        vector<int> v;
        for (int start : {e[ans[0]].first, e[ans[0]].second})
        {
            vector<int> cur{start};
            bool ok = true;
            for (int id : ans)
            {
                auto [a, b] = e[id];
                if (cur.back() == a) cur.push_back(b);
                else if (cur.back() == b) cur.push_back(a);
                else ok = false;
            }
            if (ok && cur.back() == start)
            {
                cur.pop_back();
                if (set<int>(cur.begin(), cur.end()).size() == cur.size()) v = move(cur);
            }
        }
        assert(!v.empty());
        for (int i = 0; i < (int)ans.size(); i++) cout << v[i] << " \n"[i + 1 == (int)ans.size()];
        for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << " \n"[i + 1 == (int)ans.size()];
    }
}
'''),
    ("graph", "counting_eulerian_circuits"): (
        "5_9_003.hpp", "Counting Eulerian Circuits", "tests/test_ch05_cycles.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << bestTheorem(n, e[0].first, e, true).val() << '\n';
}
'''),
    ("graph", "counting_c4"): (
        "5_9_006.hpp", "Counting $C _ 4$'s", "tests/test_ch05_cycles.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = fourCyclesPerEdge(n, e);
    for (int i = 0; i < m; i++) cout << ans[i] << " \n"[i + 1 == m];
}
'''),
    ("graph", "enumerate_triangles"): (
        "5_9_006.hpp", "Enumerate Triangles", "tests/test_ch05_cycles.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<long long> val(n);
    for (long long &x : val) cin >> x;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << weightedTriangles(n, val, e, 998244353) << '\n';
}
'''),
    ("graph", "counting_spanning_tree_directed"): (
        "8_2_011.hpp", "Counting Spanning Trees (Directed)", "tests/test_ch08_matrix.cpp", r'''
signed main()
{
    int n, m, root; cin >> n >> m >> root;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << arborescences(n, root, e).val() << '\n';
}
'''),
    ("graph", "counting_spanning_tree_undirected"): (
        "8_2_011.hpp", "Counting Spanning Trees (Undirected)", "tests/test_ch08_matrix.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << spanningTrees(n, e).val() << '\n';
}
'''),
    ("graph", "chordal_graph_recognition"): (
        "5_10_001.hpp", "Chordal Graph Recognition", "tests/test_ch05_special.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = chordalCertificate(n, e);
    if (ans.ok)
    {
        cout << "YES\n";
        for (int i = 0; i < n; i++) cout << ans.peo[i] << " \n"[i + 1 == n];
    }
    else
    {
        cout << "NO\n" << ans.cycle.size() << '\n';
        for (int i = 0; i < (int)ans.cycle.size(); i++) cout << ans.cycle[i] << " \n"[i + 1 == (int)ans.cycle.size()];
    }
}
'''),
    ("graph", "dominatortree"): (
        "5_10_003.hpp", "Dominator Tree", "tests/test_ch05_dominator.cpp", r'''
signed main()
{
    int n, m, s; cin >> n >> m >> s;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
    }
    auto ans = Dominator(g).work(s);
    for (int i = 0; i < n; i++) cout << ans[i] << " \n"[i + 1 == n];
}
'''),
    ("graph", "enumerate_cliques"): (
        "5_10_006.hpp", "Enumerate Cliques", "tests/test_ch05_special.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<long long> val(n);
    for (long long &x : val) cin >> x;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << weightedCliques(n, val, e, 998244353) << '\n';
}
'''),
    ("graph", "chromatic_number"): (
        "5_10_008.hpp", "Chromatic Number", "tests/test_ch05_coloring.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<unsigned long long> e(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        e[u] |= 1ULL << v;
        e[v] |= 1ULL << u;
    }
    cout << Dsatur(e).solve().k << '\n';
}
'''),
    ("graph", "chromatic_polynomial"): (
        "5_10_010.hpp", "Chromatic Polynomial", "tests/test_ch05_chromatic_poly.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<unsigned long long> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u] |= 1ULL << v;
        if (u != v) g[v] |= 1ULL << u;
    }
    auto ans = chromaticPoly(g);
    for (int i = 0; i <= n; i++) cout << ans[i].val() << " \n"[i == n];
}
'''),
    ("graph", "st_numbering"): (
        "5_10_013.hpp", "st-Numbering", "tests/test_ch05_st_numbering.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n, m, s, z; cin >> n >> m >> s >> z;
        vector<vector<int>> g(n);
        while (m--)
        {
            int u, v; cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        auto ans = stNumbering(g, s, z);
        if (!ans) cout << "No\n";
        else
        {
            cout << "Yes\n";
            for (int i = 0; i < n; i++) cout << (*ans)[i] << " \n"[i + 1 == n];
        }
    }
}
'''),
    ("graph", "tree_decomposition_width_2"): (
        "5_10_014.hpp", "Tree Decomposition (Width 2)", "tests/test_ch05_tree_decomposition.cpp", r'''
signed main()
{
    string p, q; int n, m; cin >> p >> q >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) { cin >> u >> v; --u; --v; }
    auto ans = treeDecomposition2(n, e);
    if (!ans) { cout << -1 << '\n'; return 0; }
    int k = ans->bag.size();
    cout << "s td " << k << " 2 " << n << '\n';
    for (int i = 0; i < k; i++)
    {
        cout << "b " << i + 1;
        for (int v : ans->bag[i]) cout << ' ' << v + 1;
        cout << '\n';
    }
    for (auto [u, v] : ans->edge) cout << u + 1 << ' ' << v + 1 << '\n';
}
'''),
    ("graph", "dynamic_graph_vertex_add_component_sum"): (
        "6_4_006.hpp", "Dynamic Graph Vertex Add Component Sum", "tests/test_ch06_dynamic_component_sum.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    vector<DynamicComponentOp> op(q);
    for (auto &[type, u, v, x] : op)
    {
        cin >> type >> u;
        if (type <= 1) cin >> v;
        else if (type == 2) cin >> x;
    }
    for (long long x : offlineDynamicComponentSums(a, op)) cout << x << '\n';
}
'''),
    ("graph", "shortest_path"): (
        "5_2_008.hpp", "Shortest Path", "tests/test_ch05_shortest.cpp", r'''
signed main()
{
    int n, m, s, t; cin >> n >> m >> s >> t;
    vector<vector<WEdge>> g(n);
    while (m--)
    {
        int u, v, w; cin >> u >> v >> w;
        g[u].push_back({v, w});
    }
    auto [d, par] = shortestPathTree(g, s);
    if (!d[t].has_value())
    {
        cout << -1 << '\n';
        return 0;
    }
    vector<int> path;
    for (int u = t; u != -1; u = par[u]) path.push_back(u);
    reverse(path.begin(), path.end());
    cout << *d[t] << ' ' << (int)path.size() - 1 << '\n';
    for (int i = 1; i < (int)path.size(); i++) cout << path[i - 1] << ' ' << path[i] << '\n';
}
'''),
    ("graph", "k_shortest_walk"): (
        "5_2_012.hpp", "K-Shortest Walk", "tests/test_ch05_shortest.cpp", r'''
signed main()
{
    int n, m, s, t, k; cin >> n >> m >> s >> t >> k;
    vector<EppsteinEdge> e(m);
    for (auto &[u, v, w] : e) cin >> u >> v >> w;
    auto ans = eppsteinWalks(n, e, s, t, k);
    for (int i = 0; i < k; i++) cout << (i < (int)ans.size() ? ans[i] : -1) << '\n';
}
'''),
    ("graph", "scc"): (
        "5_3_001.hpp", "Strongly Connected Components", "tests/test_ch05_components.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
    }
    auto bel = tarjanSCC(g);
    int cnt = 0;
    for (int x : bel) cnt = max(cnt, x + 1);
    vector<vector<int>> part(cnt);
    for (int u = 0; u < n; u++) part[cnt - 1 - bel[u]].push_back(u);
    cout << cnt << '\n';
    for (auto &v : part)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
'''),
    ("graph", "biconnected_components"): (
        "5_3_005.hpp", "Biconnected Components", "tests/test_ch05_components.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    VertexBCC bcc(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        bcc.addEdge(u, v);
    }
    bcc.build();
    auto ans = bcc.components();
    cout << ans.size() << '\n';
    for (auto &v : ans)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
'''),
    ("graph", "connected_components_of_complement_graph"): (
        "5_3_011.hpp", "Connected Components of Complement Graph", "tests/test_ch05_components.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto ans = complementCC(g);
    cout << ans.size() << '\n';
    for (auto &v : ans)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
'''),
    ("graph", "incremental_scc"): (
        "5_3_012.hpp", "Strongly Connected Components (Incremental)", "tests/test_ch05_incremental_scc.cpp", r'''
signed main()
{
    constexpr int mod = 998244353;
    int n, m; cin >> n >> m;
    vector<int> x(n);
    for (int &v : x) cin >> v;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto when = incrementalSCC(n, e);
    vector<vector<int>> add(m + 1);
    for (int i = 0; i < m; i++) if (when[i] <= m) add[when[i]].push_back(i);
    vector<int> par(n, -1);
    auto find = [&](auto &&self, int u) -> int
    {
        return par[u] < 0 ? u : par[u] = self(self, par[u]);
    };
    int ans = 0;
    for (int t = 1; t <= m; t++)
    {
        for (int id : add[t])
        {
            auto [u, v] = e[id];
            u = find(find, u);
            v = find(find, v);
            if (u == v) continue;
            ans = (ans + x[u] * x[v]) % mod;
            if (par[u] > par[v]) swap(u, v);
            par[u] += par[v];
            par[v] = u;
            x[u] = (x[u] + x[v]) % mod;
        }
        cout << ans << '\n';
    }
}
'''),
    ("graph", "directedmst"): (
        "5_5_008.hpp", "Directed MST", "tests/test_ch05_directed_mst.cpp", r'''
signed main()
{
    int n, m, root; cin >> n >> m >> root;
    vector<DirectedMSTFastEdge> e(m);
    for (auto &[u, v, w] : e) cin >> u >> v >> w;
    auto ans = directedMSTTree(n, root, e);
    assert(ans.has_value());
    cout << ans->weight << '\n';
    for (int i = 0; i < n; i++) cout << ans->parent[i] << " \n"[i + 1 == n];
}
'''),
    ("graph", "minimum_steiner_tree"): (
        "5_5_009.hpp", "Minimum Steiner Tree", "tests/test_ch05_steiner.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    Steiner st(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        st.add(u, v, w);
    }
    int k; cin >> k;
    vector<int> key(k);
    for (int &u : key) cin >> u;
    auto ans = st.solveTree(key);
    assert(ans.has_value());
    cout << ans->first << ' ' << ans->second.size() << '\n';
    for (int i = 0; i < (int)ans->second.size(); i++) cout << ans->second[i] << " \n"[i + 1 == (int)ans->second.size()];
    if (ans->second.empty()) cout << '\n';
}
'''),
    ("graph", "incremental_minimum_spanning_forest"): (
        "5_5_011.hpp", "Incremental Minimum Spanning Forest", "tests/test_ch05_incremental_msf.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    IncrementalMSF msf(n);
    msf.reserveEdges(m);
    for (int i = 0; i < m; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        int ans = msf.replaceId(u, v, w, i);
        cout << ans << " \n"[i + 1 == m];
    }
}
'''),
    ("graph", "minimum_diameter_spanning_tree"): (
        "5_5_012.hpp", "Minimum Diameter Spanning Tree", "tests/test_ch05_mdst.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    MinimumDiameterSpanningTree tree(n);
    map<pair<int, int>, pair<int, int>> best;
    for (int i = 0; i < m; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        tree.addEdge(u, v, w);
        if (u > v) swap(u, v);
        if (!best.count({u, v}) || w < best[{u, v}].first) best[{u, v}] = {w, i};
    }
    auto [diameter, e] = tree.solve();
    cout << diameter << '\n';
    for (int i = 0; i < (int)e.size(); i++)
    {
        auto [u, v] = e[i];
        if (u > v) swap(u, v);
        cout << best[{u, v}].second << " \n"[i + 1 == (int)e.size()];
    }
}
'''),
    ("graph", "minimum_spanning_tree"): (
        "5_5_001.hpp", "Minimum Spanning Tree", "tests/test_ch05_foundations.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<MEdge> e(m);
    map<array<int, 3>, vector<int>> pos;
    for (int i = 0; i < m; i++)
    {
        auto &x = e[i];
        cin >> x.x >> x.y >> x.w;
        pos[{x.x, x.y, x.w}].push_back(i);
    }
    auto ans = kruskal(n, e);
    assert(ans.has_value());
    vector<bool> used(m);
    vector<int> id;
    for (auto x : ans->second)
    {
        auto &v = pos[{x.x, x.y, x.w}];
        id.push_back(v.back());
        v.pop_back();
    }
    cout << ans->first << '\n';
    for (int i = 0; i < (int)id.size(); i++) cout << (i ? " " : "") << id[i];
    cout << '\n';
}
'''),
    ("graph", "two_edge_connected_components"): (
        "5_3_004.hpp", "Two-Edge-Connected Components", "tests/test_ch05_foundations.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    EdgeBCC bcc(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        bcc.addEdge(u, v);
    }
    bcc.build();
    auto bel = bcc.components();
    int cnt = 0;
    for (int x : bel) cnt = max(cnt, x + 1);
    vector<vector<int>> part(cnt);
    for (int u = 0; u < n; u++) part[bel[u]].push_back(u);
    cout << cnt << '\n';
    for (auto &v : part)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
'''),
    ("graph", "bipartitematching"): (
        "5_6_003.hpp", "Matching on Bipartite Graph", "tests/test_ch05_matching_variants.cpp", r'''
signed main()
{
    int l, r, m; cin >> l >> r >> m;
    HopcroftKarp match(l, r);
    while (m--)
    {
        int u, v; cin >> u >> v;
        match.addEdge(u, v);
    }
    cout << match.matching() << '\n';
    for (int u = 0; u < l; u++) if (match.leftMatch()[u] != -1)
        cout << u << ' ' << match.leftMatch()[u] << '\n';
}
'''),
    ("other", "two_sat"): (
        "5_4_001.hpp", "2 Sat", "tests/test_ch05_two_sat.cpp", r'''
signed main()
{
    string p, cnf;
    int n, m; cin >> p >> cnf >> n >> m;
    TarjanTwoSAT sat(n);
    while (m--)
    {
        int x, y, zero; cin >> x >> y >> zero;
        sat.addOr(abs(x) - 1, x > 0, abs(y) - 1, y > 0);
    }
    auto ans = sat.solve();
    if (!ans)
    {
        cout << "s UNSATISFIABLE\n";
        return 0;
    }
    cout << "s SATISFIABLE\nv";
    for (int i = 0; i < n; i++) cout << ' ' << ((*ans)[i] ? i + 1 : -i - 1);
    cout << " 0\n";
}
'''),
    ("string", "prefix_substring_lcs"): (
        "4_5_007.hpp", "Prefix-Substring LCS", "tests/test_lc_gap_string_number.cpp", r'''
signed main()
{
    int q; string s, t;
    cin >> q >> s >> t;
    vector<array<int, 3>> ask(q);
    for (auto &x : ask) cin >> x[0] >> x[1] >> x[2];
    for (int x : PrefixSubstringLCS::solve(s, t, ask)) cout << x << '\n';
}
'''),
    ("number_theory", "gcd_of_gaussian_integers"): (
        "7_1_017.hpp", "Gcd of Gaussian Integers", "tests/test_lc_gap_gaussian.cpp", r'''
signed main()
{
    int q; cin >> q;
    while (q--)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        auto g = GaussianInt::gcd({a, b}, {c, d});
        cout << g.x << ' ' << g.y << '\n';
    }
}
'''),
    ("graph", "three_edge_connected_components"): (
        "5_3_013.hpp", "Three-Edge-Connected Components", "tests/test_lc_gap_graph_tree.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto a = threeEdgeConnectedComponents(g);
    cout << a.size() << '\n';
    for (auto &x : a)
    {
        cout << x.size();
        for (int v : x) cout << ' ' << v;
        cout << '\n';
    }
}
'''),
    ("graph", "global_minimum_cut_of_dynamic_star_augmented_graph"): (
        "5_8_012.hpp", "Global Minimum Cut of Dynamic Star Augmented Graph", "tests/test_lc_gap_graph_tree.cpp", r'''
signed main()
{
    int n, m, q; cin >> n >> m >> q;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    vector<array<int, 3>> e(m);
    for (auto &x : e) cin >> x[0] >> x[1] >> x[2];
    DynamicStarMinCut cut(n, e, a);
    while (q--)
    {
        int u, x; cin >> u >> x;
        cout << cut.update(u, x) << '\n';
    }
}
'''),
    ("tree", "common_interval_decomposition_tree"): (
        "6_1_011.hpp", "Common Interval Decomposition Tree", "tests/test_lc_gap_graph_tree.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<int> p(n);
    for (int &x : p) cin >> x;
    auto a = commonIntervalTree(p);
    cout << a.size() << '\n';
    for (auto x : a)
    {
        cout << x.parent << ' ' << x.l << ' ' << x.r << ' ' << (x.linear ? "linear" : "prime") << '\n';
    }
}
'''),
    ("number_theory", "kth_root_integer"): (
        "7_1_014.hpp", "Kth Root (Integer)", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        unsigned long long a; int k; cin >> a >> k;
        cout << kthRoot(a, k) << '\n';
    }
}
'''),
    ("number_theory", "primality_test"): (
        "7_2_003.hpp", "Primality Test", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int q; cin >> q;
    while (q--)
    {
        unsigned long long n; cin >> n;
        cout << (isPrime(n) ? "Yes" : "No") << '\n';
    }
}
'''),
    ("number_theory", "enumerate_primes"): (
        "7_2_001.hpp", "Enumerate Primes", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int n, a, b; cin >> n >> a >> b;
    auto primes = enumeratePrimesCompact(n);
    cout << primes.size() << ' ' << (primes.size() + a - 1 - b) / a << '\n';
    bool first = true;
    for (int i = b; i < (int)primes.size(); i += a)
    {
        if (!first) cout << ' ';
        first = false;
        cout << primes[i];
    }
    cout << '\n';
}
'''),
    ("number_theory", "factorize"): (
        "7_2_004.hpp", "Factorize", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int q; cin >> q;
    while (q--)
    {
        unsigned long long n; cin >> n;
        auto a = factor64(n);
        cout << a.size();
        for (auto p : a) cout << ' ' << p;
        cout << '\n';
    }
}
'''),
    ("number_theory", "sum_of_totient_function"): (
        "7_3_006.hpp", "Sum of Totient Function", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    long long n; cin >> n;
    DuSieve s(n);
    cout << (long long)(s.sumPhi(n) % 998244353) << '\n';
}
'''),
    ("number_theory", "counting_primes"): (
        "7_3_008.hpp", "Counting Primes", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    long long n; cin >> n;
    LehmerPi pi;
    cout << pi(n) << '\n';
}
'''),
    ("number_theory", "enumerate_quotients"): (
        "7_3_009.hpp", "Enumerate Quotients", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    long long n; cin >> n;
    auto a = quotients(n);
    cout << a.size() << '\n';
    for (int i = 0; i < (int)a.size(); i++) cout << a[i] << " \n"[i + 1 == (int)a.size()];
}
'''),
    ("number_theory", "counting_squarefrees"): (
        "7_3_013.hpp", "Counting Square-free Integers", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    long long n; cin >> n;
    cout << countSquarefree(n) << '\n';
}
'''),
    ("number_theory", "sum_of_multiplicative_function"): (
        "7_3_007.hpp", "Sum of Multiplicative Function", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n, a, b; cin >> n >> a >> b;
        Min25T<MInt<469762049>> q(n);
        cout << q.linearPrimePower(a, b).val() << '\n';
    }
}
'''),
    ("number_theory", "sum_of_multiplicative_function_large"): (
        "7_3_007.hpp", "Sum of Multiplicative Function(Large)", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n, a, b; cin >> n >> a >> b;
        Min25T<MInt<469762049>> q(n);
        cout << q.linearPrimePower(a, b).val() << '\n';
    }
}
'''),
    ("number_theory", "dirichlet_convolution_and_prefix_sums"): (
        "7_3_012.hpp", "Dirichlet Convolution and Prefix Sums", "tests/test_ch07_quotient_dirichlet.cpp", r'''
signed main()
{
    using Mint = StaticModInt<998244353>;
    int t; cin >> t;
    while (t--)
    {
        unsigned long long n; cin >> n;
        QuotientDirichlet q(n);
        vector<Mint> f(q.n), g(q.n);
        for (int i = 1; i < q.n; i++) cin >> f[i].x;
        for (int i = 1; i < q.n; i++) cin >> g[i].x;
        auto h = q.conv(f, g);
        for (int i = 1; i < q.n; i++) cout << h[i].x << " \n"[i + 1 == q.n];
    }
}
'''),
    ("number_theory", "dirichlet_inverse_and_prefix_sums"): (
        "7_3_012.hpp", "Dirichlet Inverse and Prefix Sums", "tests/test_ch07_quotient_dirichlet.cpp", r'''
signed main()
{
    using Mint = StaticModInt<998244353>;
    int t; cin >> t;
    while (t--)
    {
        unsigned long long n; cin >> n;
        QuotientDirichlet q(n);
        vector<Mint> f(q.n), one(q.n, Mint(1));
        one[0] = 0;
        for (int i = 1; i < q.n; i++) cin >> f[i].x;
        auto g = q.divide(one, f);
        for (int i = 1; i < q.n; i++) cout << g[i].x << " \n"[i + 1 == q.n];
    }
}
'''),
    ("number_theory", "discrete_logarithm_mod"): (
        "7_4_001.hpp", "Discrete Logarithm", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long x, y, m; cin >> x >> y >> m;
        auto ans = exbsgs(x, y, m);
        cout << (ans ? *ans : -1) << '\n';
    }
}
'''),
    ("number_theory", "primitive_root"): (
        "7_4_003.hpp", "Primitive Root", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int q; cin >> q;
    while (q--)
    {
        long long p; cin >> p;
        cout << primitiveRoot(p) << '\n';
    }
}
'''),
    ("number_theory", "sqrt_mod"): (
        "7_4_004.hpp", "Sqrt Mod", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long y, p; cin >> y >> p;
        auto ans = sqrtMod(y, p);
        cout << (ans ? *ans : -1) << '\n';
    }
}
'''),
    ("number_theory", "kth_root_mod"): (
        "7_4_005.hpp", "Kth Root (Mod)", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int k, y, p; cin >> k >> y >> p;
        auto ans = kthResidue(y, k, p);
        cout << (ans ? *ans : -1) << '\n';
    }
}
'''),
    ("number_theory", "tetration_mod"): (
        "7_4_006.hpp", "Tetration Mod", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long a, b, m; cin >> a >> b >> m;
        cout << PowerTower::eval(a, b, m) << '\n';
    }
}
'''),
    ("number_theory", "discrete_logarithm_fixed_mod"): (
        "7_4_008.hpp", "Discrete Logarithm (Fixed Mod)", "tests/test_ch07_batch_dlog.cpp", r'''
signed main()
{
    int p, g, n; cin >> p >> g >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto ans = batchDiscreteLog(p, g, a);
    for (int x : ans) cout << x << '\n';
}
'''),
    ("number_theory", "sum_of_floor_of_linear"): (
        "7_5_001.hpp", "Sum of Floor of Linear", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n, m, a, b; cin >> n >> m >> a >> b;
        cout << (long long)floorSum(n, m, a, b) << '\n';
    }
}
'''),
    ("number_theory", "min_of_mod_of_linear"): (
        "7_5_004.hpp", "Min of Mod of Linear", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n, m, a, b; cin >> n >> m >> a >> b;
        cout << minModLinear(n, m, a, b) << '\n';
    }
}
'''),
    ("number_theory", "nim_product_64"): (
        "13_1_009.hpp", "Nim Product ($\\mathbb{F}_{2^{64}}$)", "tests/test_ch13_nim_product.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        unsigned long long a, b; cin >> a >> b;
        cout << nimProduct(a, b) << '\n';
    }
}
'''),
    ("number_theory", "rational_approximation"): (
        "7_5_005.hpp", "Rational Approximation", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n, x, y; cin >> n >> x >> y;
        auto [lo, hi] = rationalApproximation(n, x, y);
        cout << lo.first << ' ' << lo.second << ' ' << hi.first << ' ' << hi.second << '\n';
    }
}
'''),
    ("number_theory", "stern_brocot_tree"): (
        "7_5_005.hpp", "Stern–Brocot Tree", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string op; cin >> op;
        if (op == "END") break;
        if (op == "ENCODE_PATH")
        {
            long long p, q; cin >> p >> q;
            auto path = sternPath(p, q);
            cout << path.size();
            for (auto [c, k] : path) cout << ' ' << c << ' ' << k;
            cout << '\n';
        }
        else if (op == "DECODE_PATH")
        {
            int n; cin >> n;
            vector<pair<char, long long>> path(n);
            for (auto &[c, k] : path) cin >> c >> k;
            auto [p, q] = sternValue(path);
            cout << p << ' ' << q << '\n';
        }
        else if (op == "LCA")
        {
            long long a, b, c, d; cin >> a >> b >> c >> d;
            auto [p, q] = sternLca(a, b, c, d);
            cout << p << ' ' << q << '\n';
        }
        else if (op == "CHILD")
        {
            long long p, q; cin >> p >> q;
            auto [l, r] = sternChildren(p, q);
            cout << l.first << ' ' << l.second << ' ' << r.first << ' ' << r.second << '\n';
        }
        else if (op == "ANCESTOR")
        {
            long long k, p, q; cin >> k >> p >> q;
            auto ans = sternAncestor(k, p, q);
            if (!ans) cout << -1 << '\n';
            else cout << ans->first << ' ' << ans->second << '\n';
        }
        else
        {
            long long p, q; cin >> p >> q;
            auto [l, r] = sternRange(p, q);
            cout << l.first << ' ' << l.second << ' ' << r.first << ' ' << r.second << '\n';
        }
    }
}
'''),
    ("number_theory", "two_square_sum"): (
        "7_5_007.hpp", "Represent A Number As Two Square Sum", "tests/test_ch07_revised.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n; cin >> n;
        auto ans = twoSquareRepresentations(n);
        cout << ans.size() << '\n';
        for (auto [x, y] : ans) cout << x << ' ' << y << '\n';
    }
}
'''),
    ("number_theory", "bernoulli_number"): (
        "8_2_007.hpp", "Bernoulli Number", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n; cin >> n;
    auto b = bernoulliFast(n);
    for (int i = 0; i <= n; i++) cout << b[i].val() << " \n"[i == n];
}
'''),
    ("enumerative_combinatorics", "factorial"): (
        "10_2_010.hpp", "Factorial", "tests/test_ch10_poly_misc.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n; cin >> n;
        cout << factorialFast(n).val() << '\n';
    }
}
'''),
    ("enumerative_combinatorics", "binomial_coefficient"): (
        "8_1_004.hpp", "Binomial Coefficient", "tests/test_ch08_combinations.cpp", r'''
signed main()
{
    int t, m; cin >> t >> m;
    BinomMod comb(m);
    while (t--)
    {
        int n, k; cin >> n >> k;
        cout << comb.C(n, k) << '\n';
    }
}
'''),
    ("enumerative_combinatorics", "binomial_coefficient_prime_mod"): (
        "8_1_003.hpp", "Binomial Coefficient (Prime Mod)", "tests/test_ch08_combinations.cpp", r'''
signed main()
{
    int t, p; cin >> t >> p;
    vector<pair<int, int>> q(t);
    int mx = 0;
    for (auto &[n, k] : q)
    {
        cin >> n >> k;
        mx = max(mx, n);
    }
    PrimeBinom comb(p, mx);
    for (auto [n, k] : q) cout << comb.C(n, k) << '\n';
}
'''),
    ("enumerative_combinatorics", "q_binomial_coefficient_prime_mod"): (
        "8_1_011.hpp", "$q$-Binomial Coefficient (Prime Mod)", "tests/test_ch08_combinations.cpp", r'''
signed main()
{
    int t, p, q; cin >> t >> p >> q;
    vector<pair<int, int>> a(t);
    int mx = 0;
    for (auto &[n, k] : a)
    {
        cin >> n >> k;
        mx = max(mx, n);
    }
    QBinomPrime comb(p, q, mx);
    for (auto [n, k] : a) cout << comb.C(n, k) << '\n';
}
'''),
    ("enumerative_combinatorics", "bell_number"): (
        "8_2_004.hpp", "Bell Number", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n; cin >> n;
    for (Z x : bellFast(n)) cout << x.val() << ' ';
    cout << '\n';
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_first_kind"): (
        "8_2_005.hpp", "Stirling Number of the First Kind", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n; cin >> n;
    for (Z x : stirling1Fast(n)) cout << x.val() << ' ';
    cout << '\n';
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_second_kind"): (
        "8_2_005.hpp", "Stirling Number of the Second Kind", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n; cin >> n;
    for (Z x : stirling2Fast(n)) cout << x.val() << ' ';
    cout << '\n';
}
'''),
    ("enumerative_combinatorics", "montmort_number_mod"): (
        "8_2_008.hpp", "Montmort Number", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    auto d = derangementsMod(n, m);
    for (int i = 1; i <= n; i++) cout << d[i] << " \n"[i == n];
}
'''),
    ("enumerative_combinatorics", "partition_function"): (
        "8_2_009.hpp", "Partition Function", "tests/test_ch08_sequences.cpp", r'''
signed main()
{
    int n; cin >> n;
    for (Z x : partitionsFast(n)) cout << x.val() << ' ';
    cout << '\n';
}
'''),
    ("enumerative_combinatorics", "sharp_p_subset_sum"): (
        "10_1_013.hpp", "$\#_p$ Subset Sum", "tests/test_ch10_poly_misc.cpp", r'''
signed main()
{
    int n, t; cin >> n >> t;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto ans = sharpSubsetSum(a, t);
    for (int i = 1; i <= t; i++) cout << ans[i].val() << " \n"[i == t];
}
'''),
    ("enumerative_combinatorics", "many_factorials"): (
        "10_2_010.hpp", "Many Factorials", "tests/test_ch10_poly_misc.cpp", r'''
signed main()
{
    int t; cin >> t;
    vector<int> q(t);
    for (int &n : q) cin >> n;
    for (Z x : factorialBatch(q)) cout << x.val() << '\n';
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_first_kind_fixed_k"): (
        "8_2_014.hpp", "Stirling Number of the First Kind (Fixed K)", "tests/test_ch08_revised.cpp", r'''
signed main()
{
    int n, k; cin >> n >> k;
    auto a = stirling1FixedK(n, k);
    for (int i = 0; i < (int)a.size(); i++) cout << a[i].val() << " \n"[i + 1 == (int)a.size()];
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_second_kind_fixed_k"): (
        "8_2_015.hpp", "Stirling Number of the Second Kind (Fixed K)", "tests/test_ch08_revised.cpp", r'''
signed main()
{
    int n, k; cin >> n >> k;
    auto a = stirling2FixedK(n, k);
    for (int i = 0; i < (int)a.size(); i++) cout << a[i].val() << " \n"[i + 1 == (int)a.size()];
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_first_kind_small_p_large_n"): (
        "8_2_016.hpp", "Stirling Number of the First Kind (Small p, Large n)", "tests/test_ch08_revised.cpp", r'''
signed main()
{
    int t, p; cin >> t >> p;
    Stirling1Prime s(p);
    while (t--)
    {
        long long n, k; cin >> n >> k;
        cout << s.get(n, k) << '\n';
    }
}
'''),
    ("enumerative_combinatorics", "stirling_number_of_the_second_kind_small_p_large_n"): (
        "8_2_017.hpp", "Stirling Number of the Second Kind (Small p, Large n)", "tests/test_ch08_revised.cpp", r'''
signed main()
{
    int t, p; cin >> t >> p;
    Stirling2Prime s(p);
    while (t--)
    {
        long long n, k; cin >> n >> k;
        cout << s.get(n, k) << '\n';
    }
}
'''),
    ("enumerative_combinatorics", "number_of_increasing_sequences_between_two_sequences"): (
        "10_3_006.hpp", "Number of Increasing Sequences Between Two Sequences", "tests/test_ch10_increasing_sequences.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<int> a(n), b(n);
    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;
    cout << countIncSeq(a, b).val() << '\n';
}
'''),
    ("enumerative_combinatorics", "number_of_subsequences"): (
        "12_2_010.hpp", "Number of Subsequences ", "tests/test_ch12_dp.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    cout << distinctSub(a) << '\n';
}
'''),
    ("tree", "cartesian_tree"): (
        "3_4_004.hpp", "Cartesian Tree", "tests/test_ch03_cartesian.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto c = cartesian(a);
    c.parent[c.root] = c.root;
    for (int i = 0; i < n; i++) cout << c.parent[i] << " \n"[i + 1 == n];
}
'''),
    ("tree", "tree_diameter"): (
        "6_1_001.hpp", "Tree Diameter", "tests/test_ch06_tree_pack.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<vector<pair<int, long long>>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; long long w; cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    auto ans = treeDiameter(g);
    cout << ans.dist << ' ' << ans.path.size() << '\n';
    for (int i = 0; i < (int)ans.path.size(); i++) cout << ans.path[i] << " \n"[i + 1 == (int)ans.path.size()];
}
'''),
    ("tree", "lca"): (
        "6_1_002.hpp", "Lowest Common Ancestor", "tests/test_ch06_tree_pack.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<vector<int>> g(n);
    for (int v = 1; v < n; v++)
    {
        int p; cin >> p;
        g[p].push_back(v);
        g[v].push_back(p);
    }
    LCA a(g);
    while (q--)
    {
        int u, v; cin >> u >> v;
        cout << a.lca(u, v) << '\n';
    }
}
'''),
    ("tree", "jump_on_tree"): (
        "6_1_002.hpp", "Jump on Tree", "tests/test_ch06_tree_pack.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    LCA a(g);
    while (q--)
    {
        int s, t, k; cin >> s >> t >> k;
        cout << a.pathKth(s, t, k) << '\n';
    }
}
'''),
    ("tree", "rooted_tree_isomorphism_classification"): (
        "6_1_008.hpp", "Rooted Tree Isomorphism Classification", "tests/test_ch06_tree_pack.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<vector<int>> g(n);
    for (int v = 1; v < n; v++)
    {
        int p; cin >> p;
        g[p].push_back(v);
        g[v].push_back(p);
    }
    TreeIso iso;
    auto id = iso.ids(g, 0);
    cout << iso.mp.size() << '\n';
    for (int i = 0; i < n; i++) cout << id[i] - 1 << " \n"[i + 1 == n];
}
'''),
    ("tree", "rooted_tree_topological_order_with_minimum_inversions"): (
        "6_5_005.hpp", "Rooted Tree Topological Order with Minimum Inversions", "tests/test_ch06_min_inversion_order.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<int> p(n, -1);
    for (int i = 1; i < n; i++) cin >> p[i];
    vector<long long> c(n), d(n);
    for (long long &x : c) cin >> x;
    for (long long &x : d) cin >> x;
    auto order = minInversionTreeOrder(p, c, d);
    long long ans = 0, sum = 0;
    for (int u : order)
    {
        ans += sum * c[u];
        sum += d[u];
    }
    cout << ans << '\n';
    for (int i = 0; i < n; i++) cout << order[i] << " \n"[i + 1 == n];
}
'''),
    ("tree", "frequency_table_of_tree_distance"): (
        "6_2_001_frequency.hpp", "Frequency Table of Tree Distance", "tests/test_ch06_distance_frequency.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto answer = treeDistanceFrequency(g);
    for (int i = 0; i + 1 < n; i++) cout << answer[i] << " \n"[i + 2 == n];
}
'''),
    ("tree", "vertex_add_range_contour_sum_on_tree"): (
        "6_2_002_contour.hpp", "Vertex Add Range Contour Sum on Tree", "tests/test_ch06_contour.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    PointSetRangeContourSum<long long> tree(g, a);
    while (q--)
    {
        int op, p; cin >> op >> p;
        if (op == 0)
        {
            long long x; cin >> x;
            tree.set(p, tree.get(p) + x);
        }
        else
        {
            int l, r; cin >> l >> r;
            cout << tree.prod(p, l, r) << '\n';
        }
    }
}
'''),
    ("tree", "vertex_get_range_contour_add_on_tree"): (
        "6_2_002_contour.hpp", "Vertex Get Range Contour Add on Tree", "tests/test_ch06_contour.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    PointGetRangeContourAdd<long long> tree(g, a);
    while (q--)
    {
        int op, p; cin >> op >> p;
        if (op == 0)
        {
            int l, r; long long x; cin >> l >> r >> x;
            tree.apply(p, l, r, x);
        }
        else cout << tree.get(p) << '\n';
    }
}
'''),
    ("tree", "vertex_add_path_sum"): (
        "6_3_004.hpp", "Vertex Add Path Sum", "tests/test_dynamic_tree.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    TreePathSum sum(g, a);
    while (q--)
    {
        int op, u; cin >> op >> u;
        if (op == 0)
        {
            long long x; cin >> x;
            a[u] += x;
            sum.set(u, a[u]);
        }
        else
        {
            int v; cin >> v;
            cout << sum.pathSum(u, v) << '\n';
        }
    }
}
'''),
    ("tree", "vertex_add_subtree_sum"): (
        "6_3_004.hpp", "Vertex Add Subtree Sum", "tests/test_dynamic_tree.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int v = 1; v < n; v++)
    {
        int p; cin >> p;
        g[p].push_back(v);
        g[v].push_back(p);
    }
    TreePathSum sum(g, a);
    while (q--)
    {
        int op, u; cin >> op >> u;
        if (op == 0)
        {
            long long x; cin >> x;
            a[u] += x;
            sum.set(u, a[u]);
        }
        else cout << sum.subtreeSum(u) << '\n';
    }
}
'''),
    ("tree", "vertex_set_path_composite"): (
        "6_3_004.hpp", "Vertex Set Path Composite", "tests/test_ch06_path_composite.cpp", r'''
signed main()
{
    constexpr long long P = 998244353;
    int n, q; cin >> n >> q;
    vector<TreePathComposite<P>::Func> f(n);
    for (auto &[a, b] : f) cin >> a >> b;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    TreePathComposite<P> tr(g, f);
    while (q--)
    {
        int op; cin >> op;
        if (op == 0)
        {
            int p, a, b; cin >> p >> a >> b;
            tr.set(p, {a, b});
        }
        else
        {
            int u, v, x; cin >> u >> v >> x;
            cout << tr.apply(u, v, x) << '\n';
        }
    }
}
'''),
    ("tree", "tree_path_composite_sum"): (
        "6_3_004_affine_sum.hpp", "Tree Path Composite Sum", "tests/test_ch06_affine_sum.cpp", r'''
signed main()
{
    constexpr long long P = 998244353;
    int n; cin >> n;
    vector<long long> value(n);
    for (auto &x : value) cin >> x;
    vector<TreeAffineEdge<P>> edges(n - 1);
    for (auto &[u, v, a, b] : edges) cin >> u >> v >> a >> b;
    auto answer = treeAffineAllRoots<P>(value, edges);
    for (int i = 0; i < n; i++) cout << answer[i] << " \n"[i + 1 == n];
}
'''),
    ("tree", "point_set_tree_path_composite_sum_fixed_root"): (
        "6_3_004_affine_sum.hpp", "Point Set Tree Path Composite Sum (Fixed Root)", "tests/test_ch06_affine_sum.cpp", r'''
signed main()
{
    constexpr long long P = 998244353;
    int n, q; cin >> n >> q;
    vector<DynamicTreeAffineSum<P>::Info> info(2 * n - 1);
    for (int i = 0; i < n; i++) cin >> info[i].a;
    vector<int> u(n - 1), v(n - 1);
    for (int i = 0; i < n - 1; i++)
    {
        cin >> u[i] >> v[i] >> info[n + i].a >> info[n + i].b;
        info[n + i].vertex = false;
    }
    DynamicTreeAffineSum<P> tree(info);
    for (int i = 0; i < n - 1; i++) tree.link(n + i, u[i]), tree.link(n + i, v[i]);
    while (q--)
    {
        int op, i, x; cin >> op >> i >> x;
        if (op == 0) tree.setVertex(i, x);
        else
        {
            int y; cin >> y;
            tree.setEdge(n + i, x, y);
        }
        cout << tree.query(0) << '\n';
    }
}
'''),
    ("tree", "point_set_tree_path_composite_sum"): (
        "6_3_004_affine_sum.hpp", "Point Set Tree Path Composite Sum", "tests/test_ch06_affine_sum.cpp", r'''
signed main()
{
    constexpr long long P = 998244353;
    int n, q; cin >> n >> q;
    vector<DynamicTreeAffineSum<P>::Info> info(2 * n - 1);
    for (int i = 0; i < n; i++) cin >> info[i].a;
    vector<int> u(n - 1), v(n - 1);
    for (int i = 0; i < n - 1; i++)
    {
        cin >> u[i] >> v[i] >> info[n + i].a >> info[n + i].b;
        info[n + i].vertex = false;
    }
    DynamicTreeAffineSum<P> tree(info);
    for (int i = 0; i < n - 1; i++) tree.link(n + i, u[i]), tree.link(n + i, v[i]);
    while (q--)
    {
        int op, i, x; cin >> op >> i >> x;
        if (op == 0) tree.setVertex(i, x);
        else
        {
            int y; cin >> y;
            tree.setEdge(n + i, x, y);
        }
        int root; cin >> root;
        cout << tree.query(root) << '\n';
    }
}
'''),
    ("tree", "dynamic_tree_vertex_add_path_sum"): (
        "6_4_001.hpp", "Dynamic Tree Vertex Add Path Sum", "tests/test_dynamic_tree.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    LinkCutTree lct(a);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        lct.link(u, v);
    }
    while (q--)
    {
        int op, u, v; cin >> op >> u >> v;
        if (op == 0)
        {
            int x, y; cin >> x >> y;
            lct.cut(u, v);
            lct.link(x, y);
        }
        else if (op == 1)
        {
            a[u] += v;
            lct.set(u, a[u]);
        }
        else cout << lct.pathSum(u, v) << '\n';
    }
}
'''),
    ("tree", "dynamic_tree_vertex_set_path_composite"): (
        "6_4_001_composite.hpp", "Dynamic Tree Vertex Set Path Composite", "tests/test_ch06_dynamic_composite.cpp", r'''
signed main()
{
    constexpr long long P = 998244353;
    int n, q; cin >> n >> q;
    vector<DynamicPathComposite<P>::Func> f(n);
    for (auto &[a, b] : f) cin >> a >> b;
    DynamicPathComposite<P> tr(f);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        tr.link(u, v);
    }
    while (q--)
    {
        int op; cin >> op;
        if (op == 0)
        {
            int u, v, w, x; cin >> u >> v >> w >> x;
            tr.cut(u, v); tr.link(w, x);
        }
        else if (op == 1)
        {
            int p, a, b; cin >> p >> a >> b;
            tr.set(p, {a, b});
        }
        else
        {
            int u, v, x; cin >> u >> v >> x;
            cout << tr.apply(u, v, x) << '\n';
        }
    }
}
'''),
    ("tree", "dynamic_tree_vertex_add_subtree_sum"): (
        "6_4_001.hpp", "Dynamic Tree Vertex Add Subtree Sum", "tests/test_dynamic_tree.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    SubtreeLCT lct(a);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        lct.link(u, v);
    }
    while (q--)
    {
        int op, u, v; cin >> op >> u >> v;
        if (op == 0)
        {
            int x, y; cin >> x >> y;
            lct.cut(u, v);
            lct.link(x, y);
        }
        else if (op == 1)
        {
            a[u] += v;
            lct.set(u, a[u]);
        }
        else
        {
            lct.cut(u, v);
            cout << lct.subtreeSum(u, u) << '\n';
            lct.link(u, v);
        }
    }
}
'''),
    ("tree", "dynamic_tree_subtree_add_subtree_sum"): (
        "6_4_002_subtree_add.hpp", "Dynamic Tree Subtree Add Subtree Sum", "tests/test_ch06_dynamic_subtree_add.cpp", r'''
signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    DynamicSubtreeAddSumExact<long long> tree(a);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        tree.link(u, v);
    }
    while (q--)
    {
        int type; cin >> type;
        if (type == 0)
        {
            int u, v, w, x; cin >> u >> v >> w >> x;
            tree.cut(u, v);
            tree.link(w, x);
        }
        else
        {
            int v, p; cin >> v >> p;
            tree.cut(v, p);
            if (type == 1)
            {
                long long x; cin >> x;
                tree.add(v, x);
            }
            else cout << tree.componentSum(v) << '\n';
            tree.link(v, p);
        }
    }
}
'''),
    ("convolution", "mul_modp_convolution"): (
        "10_1_017.hpp", "Convolution on the Multiplicative Monoid of $\\mathbb{Z} / P\\mathbb{Z}$", "tests/test_lc_gap_convolution.cpp", r'''
signed main()
{
    int p; cin >> p;
    vector<Z> a(p), b(p);
    for (auto &x : a) { int v; cin >> v; x = v; }
    for (auto &x : b) { int v; cin >> v; x = v; }
    auto c = mulModPrimeConvolution(p, a, b);
    for (int i = 0; i < p; i++) cout << (i ? " " : "") << c[i].val();
    cout << '\n';
}
'''),
    ("convolution", "convolution_mod_2_64"): (
        "10_1_018.hpp", "Convolution (Mod 2^64)", "tests/test_lc_gap_convolution.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m;
    vector<unsigned long long> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    auto c = convU64(a, b);
    for (int i = 0; i < (int)c.size(); i++) cout << (i ? " " : "") << c[i];
    cout << '\n';
}
'''),
    ("linear_algebra", "matrix_product"): (
        "9_1_001.hpp", "Matrix Product", "tests/test_ch09_revised.cpp", r'''
using Mint = LinearMint<998244353>;
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    Mat<Mint> a(n, m), b(m, k);
    for (auto &r : a.a) for (auto &x : r) cin >> x;
    for (auto &r : b.a) for (auto &x : r) cin >> x;
    auto c = a * b;
    for (int i = 0; i < n; i++) for (int j = 0; j < k; j++) cout << c[i][j] << " \n"[j + 1 == k];
}
'''),
    ("linear_algebra", "matrix_product_mod_2"): (
        "9_1_001.hpp", "Matrix Product (Mod 2)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<F2Vector> a(n, F2Vector(m)), b(m, F2Vector(k));
    string s;
    for (int i = 0; i < n; i++) { cin >> s; for (int j = 0; j < m; j++) a[i].set(j, s[j] == '1'); }
    for (int i = 0; i < m; i++) { cin >> s; for (int j = 0; j < k; j++) b[i].set(j, s[j] == '1'); }
    for (auto &r : matMulF2(a, b, k)) cout << r.str() << '\n';
}
'''),
    ("linear_algebra", "pow_of_matrix"): (
        "9_1_002.hpp", "Pow of Matrix", "tests/test_ch09_revised.cpp", r'''
using Mint = LinearMint<998244353>;
signed main()
{
    int n; unsigned long long k; cin >> n >> k;
    vector a(n, vector<Mint>(n));
    for (auto &r : a) for (auto &x : r) cin >> x;
    a = matPow(a, k);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cout << a[i][j] << " \n"[j + 1 == n];
}
'''),
    ("linear_algebra", "inverse_matrix"): (
        "9_1_005.hpp", "Inverse Matrix", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n;
    vector a(n, vector<long long>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    auto b = matInvPrimePower(a, 998244353, 998244353);
    if (!b) return cout << -1 << '\n', 0;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cout << (*b)[i][j] << " \n"[j + 1 == n];
}
'''),
    ("linear_algebra", "inverse_matrix_mod_2"): (
        "9_1_005.hpp", "Inverse Matrix (Mod 2)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n; vector<F2Vector> a(n, F2Vector(n)); string s;
    for (int i = 0; i < n; i++) { cin >> s; for (int j = 0; j < n; j++) a[i].set(j, s[j] == '1'); }
    auto b = matInvF2(a); if (!b) return cout << -1 << '\n', 0;
    for (auto &r : *b) cout << r.str() << '\n';
}
'''),
    ("linear_algebra", "matrix_det"): (
        "9_1_006.hpp", "Determinant of Matrix", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n; vector a(n, vector<long long>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    cout << detPrime(a, 998244353) << '\n';
}
'''),
    ("linear_algebra", "matrix_det_arbitrary_mod"): (
        "9_1_006.hpp", "Determinant of Matrix (Arbitrary Mod)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; long long mod; cin >> n >> mod; vector a(n, vector<long long>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    cout << detAny(a, mod) << '\n';
}
'''),
    ("linear_algebra", "matrix_det_mod_2"): (
        "9_1_006.hpp", "Determinant of Matrix (Mod 2)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n; vector<F2Vector> a(n, F2Vector(n)); string s;
    for (int i = 0; i < n; i++) { cin >> s; for (int j = 0; j < n; j++) a[i].set(j, s[j] == '1'); }
    cout << (rankF2(a) == n) << '\n';
}
'''),
    ("linear_algebra", "characteristic_polynomial"): (
        "9_1_007.hpp", "Characteristic Polynomial", "tests/test_ch09_revised.cpp", r'''
using Mint = LinearMint<998244353>;
signed main()
{
    int n; cin >> n; vector a(n, vector<Mint>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    auto p = charPoly(a); for (int i = 0; i <= n; i++) cout << p[i] << " \n"[i == n];
}
'''),
    ("linear_algebra", "sparse_matrix_det"): (
        "9_1_008.hpp", "Determinant of Sparse Matrix", "tests/test_ch09_revised.cpp", r'''
using Mint = LinearMint<998244353>;
signed main()
{
    int n, k; cin >> n >> k; SparseMat<Mint> a(n, n);
    while (k--) { int i, j, x; cin >> i >> j >> x; a.add(i, j, Mint(x)); }
    cout << sparseDeterminant(a) << '\n';
}
'''),
    ("linear_algebra", "adjugate_matrix"): (
        "9_1_011.hpp", "Adjugate Matrix", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n; vector a(n, vector<long long>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    auto b = adjugatePrime(a, 998244353);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cout << b[i][j] << " \n"[j + 1 == n];
}
'''),
    ("linear_algebra", "hafnian_of_matrix"): (
        "9_1_012.hpp", "Hafnian of Matrix", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n; cin >> n; vector a(n, vector<Z>(n)); for (auto &r : a) for (auto &x : r) { int v; cin >> v; x = v; }
    cout << hafnian(a).val() << '\n';
}
'''),
    ("linear_algebra", "matrix_rank"): (
        "9_2_001.hpp", "Rank of Matrix", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m; vector a(n, vector<long long>(m)); for (auto &r : a) for (auto &x : r) cin >> x;
    cout << rankMod(a, m, 998244353) << '\n';
}
'''),
    ("linear_algebra", "matrix_rank_mod_2"): (
        "9_2_001.hpp", "Rank of Matrix (Mod 2)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m; if (n == 0 || m == 0) return cout << 0 << '\n', 0;
    bool transposed = n > m; int rows = min(n, m), cols = max(n, m);
    vector<F2Vector> a(rows, F2Vector(cols)); string s;
    for (int i = 0; i < n; i++)
    {
        cin >> s;
        for (int j = 0; j < m; j++) if (s[j] == '1')
        {
            if (transposed) a[j].set(i); else a[i].set(j);
        }
    }
    cout << rankF2(a) << '\n';
}
'''),
    ("linear_algebra", "system_of_linear_equations"): (
        "9_2_001.hpp", "System of Linear Equations", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m; vector a(n, vector<long long>(m + 1));
    for (auto &r : a) for (int j = 0; j < m; j++) cin >> r[j]; for (auto &r : a) cin >> r[m];
    auto s = gaussMod(a, m, 998244353); if (!s) return cout << -1 << '\n', 0;
    cout << s->basis.size() << '\n';
    for (int i = 0; i < m; i++) cout << s->particular[i] << " \n"[i + 1 == m];
    for (auto &r : s->basis) for (int i = 0; i < m; i++) cout << r[i] << " \n"[i + 1 == m];
}
'''),
    ("linear_algebra", "system_of_linear_equations_mod_2"): (
        "9_2_001.hpp", "System of Linear Equations (Mod 2)", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int n, m; cin >> n >> m; vector<F2Vector> a(n, F2Vector(m)); string s;
    for (int i = 0; i < n; i++) { cin >> s; for (int j = 0; j < m; j++) a[i].set(j, s[j] == '1'); }
    cin >> s; F2Vector b(n); for (int i = 0; i < n; i++) b.set(i, s[i] == '1');
    auto ans = gaussF2(a, b); if (!ans) return cout << -1 << '\n', 0;
    cout << ans->basis.size() << '\n' << ans->particular.str() << '\n'; for (auto &r : ans->basis) cout << r.str() << '\n';
}
'''),
    ("linear_algebra", "intersection_of_f2_vector_spaces"): (
        "9_2_004.hpp", "Intersection of $\\mathbb{F}_{2}$ vector spaces", "tests/test_ch09_revised.cpp", r'''
signed main()
{
    int t; cin >> t; while (t--)
    {
        int n, m; cin >> n; vector<int> a(n); for (int &x : a) cin >> x; cin >> m; vector<int> b(m); for (int &x : b) cin >> x;
        auto c = xorSpaceIntersection(a, b); cout << c.size(); for (int x : c) cout << ' ' << x; cout << '\n';
    }
}
'''),
    ("linear_algebra", "pfaffian_of_matrix"): (
        "9_2_008.hpp", "Pfaffian of Matrix", "tests/test_ch09_revised.cpp", r'''
using Mint = LinearMint<998244353>;
signed main()
{
    int n; cin >> n; n *= 2; vector a(n, vector<Mint>(n)); for (auto &r : a) for (auto &x : r) cin >> x;
    cout << pfaffian(a) << '\n';
}
'''),
    ("convolution", "convolution_mod"): (
        "10_1_003.hpp", "Convolution", "tests/test_polynomial.cpp", r'''
signed main(){ int n,m; cin>>n>>m; vector<Z>a(n),b(m); for(auto&x:a){int v;cin>>v;x=v;} for(auto&x:b){int v;cin>>v;x=v;} auto c=conv(a,b); for(int i=0;i<(int)c.size();i++) cout<<c[i].val()<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "convolution_mod_large"): (
        "10_1_003.hpp", "Convolution (Large)", "tests/test_polynomial.cpp", r'''
signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr); int n,m; cin>>n>>m; vector<Z>a(n),b(m); for(auto&x:a){int v;cin>>v;x=v;} for(auto&x:b){int v;cin>>v;x=v;} auto c=convLarge(a,b); for(int i=0;i<(int)c.size();i++) cout<<c[i].val()<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "convolution_mod_1000000007"): (
        "10_1_004.hpp", "Convolution (Mod 1,000,000,007)", "tests/test_ch10_convolution.cpp", r'''
signed main(){ int n,m; cin>>n>>m; vector<long long>a(n),b(m); for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=convMod(a,b,1000000007);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "bitwise_and_convolution"): (
        "10_1_006.hpp", "Bitwise And Convolution", "tests/test_ch10_fwt.cpp", r'''
signed main(){ int k;cin>>k;int n=1LL<<k;vector<Z>a(n),b(n);for(auto&x:a){int v;cin>>v;x=v;}for(auto&x:b){int v;cin>>v;x=v;}auto c=bitConv(a,b,BitOp::And);for(int i=0;i<n;i++)cout<<c[i].val()<<" \n"[i+1==n]; }
'''),
    ("convolution", "bitwise_xor_convolution"): (
        "10_1_006.hpp", "Bitwise Xor Convolution", "tests/test_ch10_fwt.cpp", r'''
signed main(){ int k;cin>>k;int n=1LL<<k;vector<Z>a(n),b(n);for(auto&x:a){int v;cin>>v;x=v;}for(auto&x:b){int v;cin>>v;x=v;}auto c=bitConv(a,b,BitOp::Xor);for(int i=0;i<n;i++)cout<<c[i].val()<<" \n"[i+1==n]; }
'''),
    ("convolution", "gcd_convolution"): (
        "10_1_010.hpp", "Gcd Convolution", "tests/test_ch10_divisor_transform.cpp", r'''
signed main(){ int n;cin>>n;vector<Z>a(n+1),b(n+1);for(int i=1;i<=n;i++){int v;cin>>v;a[i]=v;}for(int i=1;i<=n;i++){int v;cin>>v;b[i]=v;}multipleZeta(a);multipleZeta(b);for(int i=1;i<=n;i++)a[i]*=b[i];mulMob(a);for(int i=1;i<=n;i++)cout<<a[i].val()<<" \n"[i==n]; }
'''),
    ("convolution", "lcm_convolution"): (
        "10_1_010.hpp", "Lcm Convolution", "tests/test_ch10_divisor_transform.cpp", r'''
signed main(){ int n;cin>>n;vector<Z>a(n+1),b(n+1);for(int i=1;i<=n;i++){int v;cin>>v;a[i]=v;}for(int i=1;i<=n;i++){int v;cin>>v;b[i]=v;}divisorZeta(a);divisorZeta(b);for(int i=1;i<=n;i++)a[i]*=b[i];divMob(a);for(int i=1;i<=n;i++)cout<<a[i].val()<<" \n"[i==n]; }
'''),
    ("convolution", "min_plus_convolution_convex_convex"): (
        "10_1_012.hpp", "Min Plus Convolution (Convex and Convex)", "tests/test_ch10_minplus.cpp", r'''
signed main(){ int n,m;cin>>n>>m;vector<long long>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=minPlusCC(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "min_plus_convolution_convex_arbitrary"): (
        "10_1_012.hpp", "Min Plus Convolution (Convex and Arbitrary)", "tests/test_ch10_minplus.cpp", r'''
signed main(){ int n,m;cin>>n>>m;vector<long long>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=minPlusCA(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "min_plus_convolution_concave_arbitrary"): (
        "10_1_012.hpp", "Min Plus Convolution (Concave and Arbitrary)", "tests/test_ch10_minplus.cpp", r'''
signed main(){ int n,m;cin>>n>>m;vector<long long>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=minPlusConcaveArbitrary(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
'''),
    ("convolution", "multivariate_convolution"): (
        "10_1_014.hpp", "Multidimensional Convolution (Truncated)", "tests/test_ch10_missing_convolution.cpp", r'''
signed main(){ int k;cin>>k;vector<int>b(k);int n=1;for(int&x:b){cin>>x;n*=x;}vector<Z>f(n),g(n);for(auto&x:f){int v;cin>>v;x=v;}for(auto&x:g){int v;cin>>v;x=v;}auto c=multiConv(b,f,g);for(int i=0;i<n;i++)cout<<c[i].val()<<" \n"[i+1==n]; }
'''),
    ("convolution", "multivariate_convolution_cyclic"): (
        "10_1_014.hpp", "Multidimensional Convolution (Circular)", "tests/test_ch10_missing_convolution.cpp", r'''
signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr);long long p;int k;cin>>p>>k;vector<int>b(k);int n=1;for(int&x:b){cin>>x;n*=x;}vector<long long>f(n),g(n);for(auto&x:f)cin>>x;for(auto&x:g)cin>>x;auto c=multiCyclicConv(p,b,f,g);for(int i=0;i<n;i++)cout<<c[i]<<" \n"[i+1==n]; }
'''),
    ("convolution", "mul_mod2n_convolution"): (
        "10_1_015.hpp", "Convolution on the Multiplicative Monoid of $\\mathbb{Z} / 2^N\\mathbb{Z}$", "tests/test_ch10_missing_convolution.cpp", r'''
signed main(){ int k;cin>>k;int n=1LL<<k;vector<Z>a(n),b(n);for(auto&x:a){int v;cin>>v;x=v;}for(auto&x:b){int v;cin>>v;x=v;}auto c=mulMod2n(a,b);for(int i=0;i<n;i++)cout<<c[i].val()<<" \n"[i+1==n]; }
'''),
    ("convolution", "convolution_F_2_64"): (
        "10_1_016.hpp", "Convolution ($\\mathbb{F}_{2^{64}}$)", "tests/test_ch10_gf64_conv.cpp", r'''
signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<uint64_t>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=convGF64(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
'''),
    ("geo", "manhattanmst"): (
        "5_5_006.hpp", "Manhattan MST", "tests/test_ch05_foundations.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<CP>p(n),q(n);for(int i=0;i<n;i++){cin>>p[i][0]>>p[i][1];q[i]={p[i][0]+p[i][1],p[i][0]-p[i][1]};}auto r=chebyshevMstEdges(q);cout<<(long long)r.weight<<'\n';for(auto[u,v]:r.edge)cout<<u<<' '<<v<<'\n';}
'''),
    ("geo", "euclidean_mst"): (
        "5_5_007.hpp", "Euclidean MST", "tests/test_ch05_euclidean_mst.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<array<long long,2>>p(n);for(auto&x:p)cin>>x[0]>>x[1];for(auto[u,v]:euclideanMSTEdges(p))cout<<u<<' '<<v<<'\n';}
'''),
    ("geo", "sort_points_by_argument"): (
        "11_1_004.hpp", "Sort Points by Argument", "tests/test_ch11_basic.cpp", r'''
signed main(){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;polarSort(p);auto it=find_if(p.begin(),p.end(),[](auto x){return x.y<0;});rotate(p.begin(),it,p.end());for(auto x:p)cout<<x.x<<' '<<x.y<<'\n';}
'''),
    ("geo", "count_points_in_triangle"): (
        "11_2_010.hpp", "Count Points in Triangles", "tests/test_ch11_triangle_counter.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<Point<long long>>a(n);for(auto&x:a)cin>>x.x>>x.y;int m;cin>>m;vector<Point<long long>>b(m);for(auto&x:b)cin>>x.x>>x.y;TrianglePointCounter c(a,b);int q;cin>>q;while(q--){int x,y,z;cin>>x>>y>>z;cout<<c.count(x,y,z)<<'\n';}}
'''),
    ("geo", "static_convex_hull"): (
        "11_3_001.hpp", "Static Convex Hull", "tests/test_ch11_convex.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int t;cin>>t;while(t--){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto h=hull(p);cout<<h.size()<<'\n';for(auto x:h)cout<<x.x<<' '<<x.y<<'\n';}}
'''),
    ("geo", "furthest_pair"): (
        "11_3_006.hpp", "Furthest Pair of Points", "tests/test_ch11_convex.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int t;cin>>t;while(t--){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto[a,b]=farthestPair(p);cout<<a<<' '<<b<<'\n';}}
'''),
    ("geo", "convex_layers"): (
        "11_3_013.hpp", "Convex Layers", "tests/test_ch11_convex_layers.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto layer=convexLayers(p);vector<int>ans(n);for(int i=0;i<(int)layer.size();i++)for(int x:layer[i])ans[x]=i+1;for(int x:ans)cout<<x<<'\n';}
'''),
    ("geo", "minimum_enclosing_circle"): (
        "11_4_006.hpp", "Minimum Enclosing Circle", "tests/test_ch11_circle.cpp", r'''
signed main(){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;cout<<minimumEnclosingCircleBoundary(p)<<'\n';}
'''),
    ("geo", "closest_pair"): (
        "11_5_001.hpp", "Closest Pair of Points", "tests/test_ch11_distance.cpp", r'''
signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int t;cin>>t;while(t--){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto r=ClosestPair<long long>().work(p);cout<<r->second.first<<' '<<r->second.second<<'\n';}}
'''),
    ("other", "find_linear_recurrence"): (
        "10_4_001.hpp", "Find Linear Recurrence", "tests/test_ch10_recurrence.cpp", r'''
signed main(){ int n;cin>>n;vector<Z>a(n);for(auto&x:a){int v;cin>>v;x=v;}auto c=berlekampMassey(a);cout<<c.size()<<'\n';for(int i=0;i<(int)c.size();i++)cout<<c[i].val()<<" \n"[i+1==(int)c.size()];if(c.empty())cout<<'\n'; }
'''),
    ("other", "kth_term_of_linearly_recurrent_sequence"): (
        "10_4_005.hpp", "Kth term of Linearly Recurrent Sequence", "tests/test_ch10_recurrence.cpp", r'''
signed main(){ int d;unsigned long long k;cin>>d>>k;vector<Z>a(d),c(d);for(auto&x:a){int v;cin>>v;x=v;}for(auto&x:c){int v;cin>>v;x=v;}auto [p,q]=recurrenceFraction(a,c);cout<<bostanMori(p,q,k).val()<<'\n'; }
'''),
    ("other", "sum_of_exponential_times_polynomial"): (
        "9_3_006.hpp", "$\\sum_{i=0}^{n-1} r^i i^d$", "tests/test_ch09_revised.cpp", r'''
signed main(){ int r,d;long long n;cin>>r>>d>>n;cout<<sumExpMonomial(Z(r),d,n).val()<<'\n'; }
'''),
    ("other", "sum_of_exponential_times_polynomial_limit"): (
        "9_3_007.hpp", "$\\sum_{i=0}^{\\infty} r^i i^d$", "tests/test_ch09_revised.cpp", r'''
signed main(){ int r,d;cin>>r>>d;cout<<sumExpMonomialLimit(Z(r),d).val()<<'\n'; }
'''),
    ("big_integer", "addition_of_big_integers"): (
        "14_1_001.hpp", "Addition of Big Integers", "tests/test_ch14_core.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (BigInt(a) + BigInt(b)).str() << '\n';
    }
}
'''),
    ("big_integer", "division_of_big_integers"): (
        "14_1_019.hpp", "Division of Big Integers", "tests/test_ch14_hex_bigint.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        DecimalBigInt x(a), y(b);
        auto [q, r] = divmod(x, y);
        cout << q.toDecimal() << ' ' << r.toDecimal() << '\n';
    }
}
'''),
    ("big_integer", "multiplication_of_big_integers"): (
        "14_1_019.hpp", "Multiplication of Big Integers", "tests/test_ch14_hex_bigint.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (DecimalBigInt(a) * DecimalBigInt(b)).toDecimal() << '\n';
    }
}
'''),
    ("big_integer", "addition_of_hex_big_integers"): (
        "14_1_019.hpp", "Addition of Hex Big Integers", "tests/test_ch14_hex_bigint.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (HexBigInt(a) + HexBigInt(b)).toHex() << '\n';
    }
}
'''),
    ("big_integer", "division_of_hex_big_integers"): (
        "14_1_019.hpp", "Division of Hex Big Integers", "tests/test_ch14_hex_bigint.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        auto [q, r] = divmod(HexBigInt(a), HexBigInt(b));
        cout << q.toHex() << ' ' << r.toHex() << '\n';
    }
}
'''),
    ("big_integer", "multiplication_of_hex_big_integers"): (
        "14_1_019.hpp", "Multiplication of Hex Big Integers", "tests/test_ch14_hex_bigint.cpp", r'''
signed main()
{
    int t; cin >> t;
    while (t--)
    {
        string a, b; cin >> a >> b;
        cout << (HexBigInt(a) * HexBigInt(b)).toHex() << '\n';
    }
}
'''),
}


def expand(path: Path, seen: set[Path], include_ch14_only: bool = False) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    out = []
    skip = False
    for line in path.read_text(encoding="utf-8").replace("\r\n", "\n").splitlines(True):
        if "LC_GENERATOR_CH14_ONLY_BEGIN" in line:
            skip = not include_ch14_only
            if skip:
                out.append("\n")
                continue
        if "LC_GENERATOR_CH14_ONLY_END" in line:
            if skip:
                skip = False
                continue
        if skip:
            continue
        if line.strip() == "#pragma once":
            continue
        match = INC.match(line.rstrip("\n"))
        out.append(expand(path.parent / match.group(1), seen, include_ch14_only) if match else line)
    return "".join(out)


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def merge_history(meta: dict, old: dict, source_sha256: str) -> dict:
    """Preserve append-only submission history and same-source validation."""
    old_attempts = old.get("submission_attempts")
    if isinstance(old_attempts, list):
        meta["submission_attempts"] = old_attempts
    old_validation = old.get("validation")
    if (
        isinstance(old_validation, dict)
        and old_validation.get("current_source_sha256") == source_sha256
    ):
        meta["validation"] = old_validation
    return meta


def main() -> None:
    parser = argparse.ArgumentParser(description="生成或核验模板派生的 Library Checker 独立源码")
    parser.add_argument("--only", help="逗号分隔的题目名")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--write", action="store_true", help="刷新源码、元数据和 manifest")
    mode.add_argument("--check", action="store_true", help="只核验源码和 manifest，不写文件")
    args = parser.parse_args()
    selected = set(args.only.split(",")) if args.only else None
    if selected is not None:
        known = {name for _, name in SPECS}
        unknown = selected - known
        if unknown:
            raise SystemExit("UNKNOWN_PROBLEM: " + ",".join(sorted(unknown)))

    manifest_path = OUT / "manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    entries = {(x["category"], x["name"]): x for x in manifest["problems"]}
    for (category, name), (header, title, test, adapter) in SPECS.items():
        if selected is not None and name not in selected:
            continue
        include_ch14_only = header == "14_1_019.hpp" or name == "convolution_mod_2_64"
        data = (expand(TEMPLATE / "include" / "full" / header, set(), include_ch14_only).rstrip() + "\n\n" + adapter.strip() + "\n").encode()
        target = OUT / category / name / "main.cpp"
        source = target.relative_to(OUT).as_posix()
        sha = hashlib.sha256(data).hexdigest()
        if args.check:
            if not target.is_file() or target.read_bytes() != data:
                raise SystemExit(f"SOURCE_OUT_OF_DATE: {category}/{name}")
            entry = entries.get((category, name))
            if not entry or entry.get("source") != source or entry.get("source_sha256") != sha:
                raise SystemExit(f"MANIFEST_OUT_OF_DATE: {category}/{name}")
            continue
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
        meta = {
            "schema_version": 3,
            "name": name,
            "title": title,
            "category": category,
            "provenance": {"kind": "template_solution", "source": source, "source_sha256": sha},
            "official_reference": {"commit": COMMIT, "category": category, "problem": name},
            "validation": {"status": "local_only", "current_source_sha256": sha,
                           "checks": ["source_cxx20_compile", "hushuqi算法竞赛模板/" + test]},
            "submission_attempts": [],
        }
        if name in BOUNDARY_INPUTS:
            meta["validation"]["boundary_input"] = BOUNDARY_INPUTS[name]
        meta_path = target.parent / "meta.json"
        if meta_path.is_file():
            old = json.loads(meta_path.read_text(encoding="utf-8"))
            meta = merge_history(meta, old, sha)
        meta["validation"]["checks"] = [
            "source_cxx20_compile",
            "hushuqi算法竞赛模板/" + test,
        ]
        if name in BOUNDARY_INPUTS:
            meta["validation"]["boundary_input"] = BOUNDARY_INPUTS[name]
        meta_path.write_text(json.dumps(meta, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        entries[(category, name)] = {"name": name, "title": title, "category": category,
                                     "source": source, "source_sha256": sha,
                                     "meta": (target.parent / "meta.json").relative_to(OUT).as_posix(),
                                     "state": "template_solution"}
    if args.check:
        print(f"GENERATION_CURRENT: {len(SPECS) if selected is None else len(selected)}")
        return
    manifest["problems"] = [entries[key] for key in sorted(entries)]
    manifest["problem_count"] = len(manifest["problems"])
    manifest["counts"] = {"template_solution": len(manifest["problems"])}
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
