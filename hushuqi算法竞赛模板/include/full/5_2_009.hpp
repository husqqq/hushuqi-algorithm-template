#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template<class Dist = long long>
struct ShortestCnt
{
    // d[v][0..1] 是最短、严格次短距离，空值表示不存在。
    vector<array<optional<Dist>, 2>> d;
    // cnt[v][0..1] 是对应游走条数模 mod。
    vector<array<int, 2>> cnt;
};

template<class Dist = long long>
optional<ShortestCnt<Dist>> shortestCnt(
    const vector<vector<pair<int, int>>> &g, int s, int mod)
{
    // g 是非负权邻接表；零权环导致计数无穷时返回空。
    int n = g.size();
    assert(0 <= s && s < n); // 调试检查，可删。
    assert(mod > 0); // 调试检查，可删。
    ShortestCnt<Dist> z{vector<array<optional<Dist>, 2>>(n), vector<array<int, 2>>(n)};
    using Q = pair<Dist, int>;
    priority_queue<Q, vector<Q>, greater<>> q;
    z.d[s][0] = 0;
    q.push({0, s});
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if ((!z.d[u][0].has_value() || du != *z.d[u][0]) &&
            (!z.d[u][1].has_value() || du != *z.d[u][1]))
        {
            continue;
        }
        for (auto [v, w] : g[u])
        {
            assert(w >= 0); // 调试检查，可删。
            Dist ew = (Dist)w;
            Dist nd = du + ew;
            if (!z.d[v][0].has_value() || nd < *z.d[v][0])
            {
                z.d[v][1] = z.d[v][0];
                z.d[v][0] = nd;
                q.push({nd, v});
            }
            else if (nd > *z.d[v][0] && (!z.d[v][1].has_value() || nd < *z.d[v][1]))
            {
                z.d[v][1] = nd;
                q.push({nd, v});
            }
        }
    }
    vector<vector<int>> e(2 * n);
    vector<int> deg(2 * n), use(2 * n);
    for (int u = 0; u < n; u++)
    {
        for (int k = 0; k < 2; k++)
        {
            if (!z.d[u][k].has_value())
            {
                continue;
            }
            int x = 2 * u + k;
            use[x] = 1;
            for (auto [v, w] : g[u])
            {
                Dist ew = (Dist)w;
                Dist nd = *z.d[u][k] + ew;
                for (int p = 0; p < 2; p++)
                {
                    if (z.d[v][p].has_value() && nd == *z.d[v][p])
                    {
                        e[x].push_back(2 * v + p);
                        deg[2 * v + p]++;
                    }
                }
            }
        }
    }
    queue<int> que;
    int states = 0, done = 0;
    for (int x = 0; x < 2 * n; x++)
    {
        if (use[x])
        {
            states++;
            if (deg[x] == 0)
            {
                que.push(x);
            }
        }
    }
    z.cnt[s][0] = 1 % mod;
    while (!que.empty())
    {
        int x = que.front();
        que.pop();
        done++;
        for (int y : e[x])
        {
            int v = y / 2, k = y % 2;
            int add = z.cnt[x / 2][x % 2];
            if (z.cnt[v][k] >= mod - add)
            {
                z.cnt[v][k] -= mod - add;
            }
            else
            {
                z.cnt[v][k] += add;
            }
            if (--deg[y] == 0)
            {
                que.push(y);
            }
        }
    }
    if (done != states)
    {
        return nullopt;
    }
    return z;
}
