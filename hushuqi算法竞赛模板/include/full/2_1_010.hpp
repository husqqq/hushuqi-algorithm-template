#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace iter_search
{
optional<vector<int>> iddfs(const vector<vector<int>> &g, int s, int t, int lim)
{
    // g 是有向图邻接表，s、t 是起点和终点，lim 是允许的最多边数；
    // 返回边数最少的简单路径（含两端点的顶点序列），lim 内不可达时返回空。
    vector<char> used(g.size());
    vector<int> path;
    auto dfs = [&](auto &&self, int u, int d) -> bool
    {
        // u 是当前点，d 是还能走的边数；找到 t 时保留 path 并返回 true。
        path.push_back(u);
        used[u] = true;
        if (u == t)
        {
            return true;
        }
        if (d > 0)
        {
            for (int v : g[u])
            {
                if (!used[v] && self(self, v, d - 1))
                {
                    return true;
                }
            }
        }
        used[u] = false;
        path.pop_back();
        return false;
    };
    for (int k = 0; k <= min(lim, (int)g.size() - 1); k++)
    {
        if (dfs(dfs, s, k))
        {
            return path;
        }
    }
    return nullopt;
}

struct Path
{
    __int128 cost;        // cost 是路径边权和。
    vector<int> vertices; // vertices 是从起点到终点的顶点序列。
};

optional<Path> ida(const vector<vector<pair<int, int>>> &g, int s, int t, const vector<int> &h)
{
    // g[u] 存 {v,w}，w 为非负边权；h[u] 是 u 到 t 的可采纳下界，且 h[t]=0。
    // 返回 s 到 t 的一条最短简单路径，不可达时返回空。
    assert(h.size() == g.size() && h[t] == 0); // 调试检查，可删。
    const __int128 INF = (__int128)1 << 120;
    __int128 lim = h[s], next = INF, found = 0;
    vector<int> path;
    vector<char> used(g.size());
    auto dfs = [&](auto &&self, int u, __int128 cost) -> bool
    {
        // u 是当前点，cost 是已走代价；f=cost+h[u] 超过阈值时记录下一轮阈值并剪枝。
        __int128 f = cost + h[u];
        if (f > lim)
        {
            next = min(next, f);
            return false;
        }
        path.push_back(u);
        used[u] = true;
        if (u == t)
        {
            found = cost;
            return true;
        }
        for (auto [v, w] : g[u])
        {
            if (!used[v] && self(self, v, cost + w))
            {
                return true;
            }
        }
        used[u] = false;
        path.pop_back();
        return false;
    };
    while (true)
    {
        next = INF;
        if (dfs(dfs, s, 0))
        {
            return Path{found, path};
        }
        if (next == INF)
        {
            return nullopt;
        }
        lim = next;
    }
}

optional<vector<int>> biBfs(const vector<vector<int>> &g, int s, int t)
{
    // g 是有向无权图邻接表；返回 s 到 t 的一条最短路径顶点序列，不可达时返回空。
    int n = g.size();
    vector<vector<int>> rev(n);
    for (int u = 0; u < n; u++)
    {
        for (int v : g[u])
        {
            rev[v].push_back(u);
        }
    }
    // da、db 是正向、反向 BFS 距离，pa、pb 是对应的前驱，a、b 是两侧当前层。
    vector<int> da(n, -1), db(n, -1), pa(n, -1), pb(n, -1), a{s}, b{t};
    da[s] = db[t] = 0;
    int best = n + 1, meet = -1;
    auto relax = [&](int v)
    {
        // v 是刚被访问的点；若两侧都到过 v，就用 v 更新当前最短拼接路径。
        if (da[v] >= 0 && db[v] >= 0 && da[v] + db[v] < best)
        {
            best = da[v] + db[v];
            meet = v;
        }
    };
    relax(s);
    while (!a.empty() && !b.empty() && da[a[0]] + db[b[0]] < best)
    {
        // 每次扩展较小的一侧整层，保证两侧层数之和单调增加。
        bool fw = a.size() <= b.size();
        auto &cur = fw ? a : b;
        auto &d = fw ? da : db;
        auto &p = fw ? pa : pb;
        const auto &adj = fw ? g : rev;
        vector<int> nxt;
        for (int u : cur)
        {
            for (int v : adj[u])
            {
                if (d[v] < 0)
                {
                    d[v] = d[u] + 1;
                    p[v] = u;
                    nxt.push_back(v);
                }
                relax(v);
            }
        }
        cur.swap(nxt);
    }
    if (meet < 0)
    {
        return nullopt;
    }
    vector<int> path;
    for (int u = meet; u >= 0; u = pa[u])
    {
        path.push_back(u);
    }
    reverse(path.begin(), path.end());
    for (int u = pb[meet]; u >= 0; u = pb[u])
    {
        path.push_back(u);
    }
    return path;
}
}
