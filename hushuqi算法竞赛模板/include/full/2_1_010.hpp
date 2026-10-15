#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace iter_search
{
// 显式无权图的 IDDFS，返回最少边数的简单路径；maxDepth 是包含在内的深度上界。
inline optional<vector<int>> iddfs(const vector<vector<int>> &g, int s, int t, int maxDepth)
{
    vector<char> used(g.size()); vector<int> path;
    for (int lim = 0; lim <= min(maxDepth, (int)g.size() - 1); ++lim)
    {
        auto dfs = [&](auto &&self, int u, int d) -> bool
        {
            path.push_back(u); used[u] = true;
            if (u == t) return true;
            if (d) for (int v : g[u]) if (!used[v] && self(self, v, d - 1)) return true;
            used[u] = false; path.pop_back(); return false;
        };
        if (dfs(dfs, s, lim)) return path;
    }
    return nullopt;
}
struct Path { __int128 cost; vector<int> vertices; };
// 非负权；h 必须可采纳、非负且 h[t]=0，路径内去环。无全局 closed 集。
inline optional<Path> ida(const vector<vector<pair<int, long long>>> &g,
                           int s, int t, const vector<long long> &h)
{
    if (h.size() != g.size() || h.at(t) != 0) throw invalid_argument("invalid heuristic");
    for (auto x : h) if (x < 0) throw invalid_argument("negative heuristic");
    for (const auto &row : g) for (auto [v, w] : row)
        if (v < 0 || v >= (int)g.size() || w < 0) throw invalid_argument("invalid edge");
    __int128 lim = h.at(s), next, found = 0;
    const __int128 INF = (__int128)1 << 120;
    vector<int> path; vector<char> used(g.size());
    while (true)
    {
        next = INF;
        auto dfs = [&](auto &&self, int u, __int128 cost) -> bool
        {
            __int128 f = cost + h[u];
            if (f > lim) { next = min(next, f); return false; }
            path.push_back(u); used[u] = true;
            if (u == t) { found = cost; return true; }
            for (auto [v, w] : g[u]) if (!used[v] && self(self, v, cost + w)) return true;
            used[u] = false; path.pop_back(); return false;
        };
        if (dfs(dfs, s, 0)) return Path{found, path};
        if (next == INF) return nullopt;
        lim = next;
    }
}
// 有向无权图：后向搜索用反图；每次扩展完整的一层。
inline optional<vector<int>> biBfs(const vector<vector<int>> &g, int s, int t)
{
    int n = g.size(); vector<vector<int>> rev(n);
    for (int u = 0; u < n; ++u) for (int v : g[u]) rev[v].push_back(u);
    vector<int> da(n, -1), db(n, -1), pa(n, -1), pb(n, -1), a{s}, b{t};
    da[s] = db[t] = 0; int best = n + 1, meet = -1;
    auto relax = [&](int v) { if (da[v] >= 0 && db[v] >= 0 && da[v] + db[v] < best) best = da[v] + db[v], meet = v; };
    relax(s);
    while (!a.empty() && !b.empty())
    {
        if (da[a[0]] + db[b[0]] >= best) break;
        bool forward = a.size() <= b.size();
        auto &front = forward ? a : b; auto &d = forward ? da : db; auto &p = forward ? pa : pb;
        const auto &adj = forward ? g : rev;
        vector<int> next;
        for (int u : front) for (int v : adj[u])
        {
            if (d[v] < 0) { d[v] = d[u] + 1; p[v] = u; next.push_back(v); }
            relax(v);
        }
        front.swap(next);
    }
    if (meet < 0) return nullopt;
    vector<int> path;
    for (int u = meet; u >= 0; u = pa[u]) path.push_back(u);
    reverse(path.begin(), path.end());
    for (int u = pb[meet]; u >= 0; u = pb[u]) path.push_back(u);
    return path;
}
}
