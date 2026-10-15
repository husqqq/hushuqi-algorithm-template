#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class Dist = long long>
struct Dia
{
    // dist 是直径长度，path 是一条直径上的完整点列。
    Dist dist;
    vector<int> path;
};

template <class Dist = long long>
Dia<Dist> treeDiameter(const vector<vector<pair<int, long long>>> &g)
{
    // g 是非负边权树；返回直径长度与一条直径路径。
    static_assert(numeric_limits<Dist>::is_integer && numeric_limits<Dist>::is_signed &&
                  sizeof(Dist) >= sizeof(long long));
    int n = g.size();
    if (!n)
    {
        return Dia<Dist>{0, {}};
    }
    auto far = [&](int s)
    {
        vector<int> p(n, -1);
        vector<Dist> d(n, -1);
        d[s] = 0;
        auto dfs = [&](auto &&self, int u) -> void
        {
            for (auto [v, w] : g[u])
            {
                if (v == p[u])
                {
                    continue;
                }
                assert(w >= 0); // 调试检查，可删。
                p[v] = u;
                d[v] = d[u] + (Dist)w;
                self(self, v);
            }
        };
        dfs(dfs, s);
        int u = max_element(d.begin(), d.end()) - d.begin();
        return tuple{u, move(p), move(d)};
    };
    auto first = far(0);
    int s = get<0>(first);
    auto second = far(s);
    auto [t, p, d] = move(second);
    vector<int> path;
    for (int u = t; u != -1; u = p[u])
    {
        path.push_back(u);
    }
    reverse(path.begin(), path.end());
    return Dia<Dist>{d[t], path};
}

vector<int> centroids(const vector<vector<int>> &g)
{
    // g 是无权树；返回一个或两个重心。
    int n = g.size();
    vector<int> sz(n), ans;
    auto dfs = [&](auto &&self, int u, int p) -> void
    {
        sz[u] = 1;
        int mx = 0;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            self(self, v, u);
            sz[u] += sz[v];
            chmax(mx, sz[v]);
        }
        chmax(mx, n - sz[u]);
        if (2 * mx <= n)
        {
            ans.push_back(u);
        }
    };
    if (n)
    {
        dfs(dfs, 0, -1);
    }
    return ans;
}

vector<int> treeCenters(const vector<vector<int>> &g)
{
    // g 是无权树；返回一个或两个中心。
    int n = g.size();
    if (n <= 2)
    {
        vector<int> a(n);
        iota(a.begin(), a.end(), 0);
        return a;
    }
    vector<int> d(n);
    queue<int> q;
    for (int u = 0; u < n; u++)
    {
        d[u] = g[u].size();
        if (d[u] == 1)
        {
            q.push(u);
        }
    }
    int left = n;
    while (left > 2)
    {
        int z = q.size();
        left -= z;
        while (z--)
        {
            int u = q.front();
            q.pop();
            for (int v : g[u])
            {
                if (--d[v] == 1)
                {
                    q.push(v);
                }
            }
        }
    }
    vector<int> ans;
    while (!q.empty())
    {
        ans.push_back(q.front());
        q.pop();
    }
    return ans;
}
