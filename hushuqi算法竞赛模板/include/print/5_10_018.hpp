#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

// 邻接矩阵允许有向边、自环、多重边；精确回溯，最坏阶乘时间。
inline optional<vector<int>> graphIso(const vector<vector<int>> &a,
                                      const vector<vector<int>> &b)
{
    int n = a.size(); if ((int)b.size() != n) return nullopt;
    for (const auto &row : a) if ((int)row.size() != n) throw invalid_argument("square adjacency required");
    for (const auto &row : b) if ((int)row.size() != n) throw invalid_argument("square adjacency required");
    vector<pair<__int128, __int128>> da(n), db(n); vector<int> p(n, -1), order(n); vector<char> used(n);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) { da[i].first += a[i][j]; da[i].second += a[j][i]; db[i].first += b[i][j]; db[i].second += b[j][i]; }
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y) { return da[x] > da[y]; });
    auto dfs = [&](auto &&self, int k) -> bool
    {
        if (k == n) return true; int u = order[k];
        for (int v = 0; v < n; ++v) if (!used[v] && da[u] == db[v] && a[u][u] == b[v][v])
        {
            bool ok = true;
            for (int j = 0; j < k; ++j) { int x = order[j]; if (a[u][x] != b[v][p[x]] || a[x][u] != b[p[x]][v]) { ok = false; break; } }
            if (!ok) continue; p[u] = v; used[v] = true; if (self(self, k + 1)) return true; used[v] = false; p[u] = -1;
        }
        return false;
    };
    if (dfs(dfs, 0)) return p; return nullopt;
}

// 极小图的规范标号基线：枚举全部置换，不是 nauty/Traces 的性能替代品。
inline pair<vector<int>, vector<int>> graphCanon(const vector<vector<int>> &a)
{
    int n = a.size(); if (n > 10) throw invalid_argument("canonical baseline needs n<=10");
    for (const auto &r : a) if ((int)r.size() != n) throw invalid_argument("square adjacency required");
    vector<int> p(n), best, order; iota(p.begin(), p.end(), 0); bool first = true;
    do
    {
        vector<int> code; code.reserve(n * n);
        for (int u : p) for (int v : p) code.push_back(a[u][v]);
        if (first || code < best) best = move(code), order = p, first = false;
    } while (next_permutation(p.begin(), p.end()));
    return {best, order};
}
