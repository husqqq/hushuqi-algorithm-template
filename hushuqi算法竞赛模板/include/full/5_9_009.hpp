#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct FunctionalGraph
{
    int n;
    vector<int> to, comp, pos, entry, depth, len;
    vector<vector<int>> cycles, up;
    vector<vector<__int128>> sum;
    vector<vector<long long>> mn;
    // 路径聚合按离开点取 w[v]；默认每个点权为 0。
    FunctionalGraph(vector<int> f, vector<long long> w = {}) : n(f.size()), to(move(f)),
        comp(n, -1), pos(n, -1), entry(n), depth(n), len(n), up(64, vector<int>(n))
    {
        if (w.empty()) w.assign(n, 0);
        if (w.size() != to.size()) throw invalid_argument("weight size mismatch");
        vector<int> deg(n), order;
        for (int v : to) { if (v < 0 || v >= n) throw invalid_argument("invalid successor"); ++deg[v]; }
        queue<int> q; for (int i = 0; i < n; ++i) if (!deg[i]) q.push(i);
        while (!q.empty()) { int u = q.front(); q.pop(); order.push_back(u); if (!--deg[to[u]]) q.push(to[u]); }
        for (int s = 0; s < n; ++s) if (deg[s] && comp[s] < 0)
        {
            int id = cycles.size(), u = s; vector<int> c;
            do { comp[u] = id; pos[u] = c.size(); entry[u] = u; c.push_back(u); u = to[u]; } while (u != s);
            for (int v : c) len[v] = c.size(); cycles.push_back(move(c));
        }
        reverse(order.begin(), order.end());
        for (int u : order) { int v = to[u]; comp[u] = comp[v]; entry[u] = entry[v]; depth[u] = depth[v] + 1; len[u] = len[v]; }
        sum.assign(64, vector<__int128>(n)); mn.assign(64, vector<long long>(n));
        for (int i = 0; i < n; ++i) up[0][i] = to[i], sum[0][i] = w[i], mn[0][i] = w[i];
        for (int j = 1; j < 64; ++j) for (int i = 0; i < n; ++i)
        {
            int v = up[j - 1][i]; up[j][i] = up[j - 1][v];
            sum[j][i] = sum[j - 1][i] + sum[j - 1][v]; mn[j][i] = min(mn[j - 1][i], mn[j - 1][v]);
        }
    }
    struct Walk { int end; __int128 total; optional<long long> minimum; };
    Walk walk(int v, unsigned long long k) const
    {
        if (v < 0 || v >= n) throw out_of_range("invalid vertex");
        Walk r{v, 0, nullopt};
        for (int j = 0; j < 64; ++j) if (k >> j & 1)
        {
            r.total += sum[j][r.end];
            r.minimum = r.minimum ? min(*r.minimum, mn[j][r.end]) : mn[j][r.end];
            r.end = up[j][r.end];
        }
        return r;
    }
    int jump(int v, unsigned long long k) const { return walk(v, k).end; }
    // 同时每步走一条边，返回首次相遇的步数；永不相遇返回空。
    optional<unsigned long long> meet(int u, int v) const
    {
        if (comp.at(u) != comp.at(v)) return nullopt;
        unsigned long long bound = max(depth[u], depth[v]);
        if (jump(u, bound) != jump(v, bound)) return nullopt;
        unsigned long long l = 0, r = bound;
        while (l < r) { auto m = l + (r - l) / 2; if (jump(u, m) == jump(v, m)) r = m; else l = m + 1; }
        return l;
    }
};
