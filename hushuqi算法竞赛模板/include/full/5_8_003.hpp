#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T = long long, class Cost = long long>
struct SpfaCostFlow
{
    struct E
    {
        int to, rev;  // 终点与反向边下标。
        T cap;        // 当前残量容量。
        Cost cost;     // 单位流量费用。
    };

    int n;               // 残量网络点数。
    vector<vector<E>> e; // 每个点的残量边。
    // d 是本轮从源点出发的最短距离，空值表示不可达。
    vector<optional<Cost>> d;
    // in 标记 SPFA 队列中的点，cur 是紧边 DFS 的当前弧，vis 标记递归栈中的点。
    vector<int> in, cur, vis;

    SpfaCostFlow(int n) : n(n), e(n), d(n), in(n), cur(n), vis(n)
    {
        // n 是点数；构造空残量网络，无返回值。
    }

    void add(int u, int v, T cap, Cost cost)
    {
        // u、v 是端点，cap 是容量，cost 是单位费用；加入正向边和零容量反向边，无返回值。
        assert(0 <= u && u < n && 0 <= v && v < n && cap >= 0); // 调试检查，可删。
        assert(cost != numeric_limits<Cost>::min()); // 调试检查，可删。
        int id = e[u].size();
        int rev = e[v].size() + (u == v);
        e[u].push_back({v, rev, cap, cost});
        e[v].push_back({u, id, 0, -cost});
    }

    bool shortest(int s, int t)
    {
        // s、t 是源汇点；用 SPFA 建立残量最短距离，返回 t 是否可达。
        fill(d.begin(), d.end(), nullopt);
        fill(in.begin(), in.end(), 0);
        queue<int> q;
        d[s] = 0;
        q.push(s);
        in[s] = 1;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            in[u] = 0;
            for (const auto &a : e[u])
            {
                if (a.cap == 0)
                {
                    continue;
                }
                Cost nd = *d[u] + a.cost;
                if (!d[a.to].has_value() || nd < *d[a.to])
                {
                    d[a.to] = nd;
                    if (!in[a.to])
                    {
                        q.push(a.to);
                        in[a.to] = 1;
                    }
                }
            }
        }
        return d[t].has_value();
    }

    T dfs(int u, int t, T lim)
    {
        // u、t 是当前点与汇点，lim 是流量上限；沿本轮紧边尽量增广并返回实际流量。
        if (u == t)
        {
            return lim;
        }
        vis[u] = 1;
        T f = 0;
        for (int &i = cur[u]; i < (int)e[u].size() && f < lim; i++)
        {
            auto &a = e[u][i];
            if (a.cap == 0 || vis[a.to] || !d[a.to].has_value() ||
                *d[u] + a.cost != *d[a.to])
            {
                continue;
            }
            T x = dfs(a.to, t, min(lim - f, a.cap));
            if (x == 0)
            {
                continue;
            }
            a.cap -= x;
            e[a.to][a.rev].cap += x;
            f += x;
        }
        vis[u] = 0;
        return f;
    }

    pair<T, Cost> flow(int s, int t, T lim = numeric_limits<T>::max())
    {
        // s、t 是源汇点，lim 是最多发送的流量；返回实际流量与最小费用。
        if (s == t)
        {
            return {0, 0};
        }
        assert(lim >= 0); // 调试检查，可删。
        T f = 0;
        Cost cost = 0;
        while (f < lim && shortest(s, t))
        {
            fill(cur.begin(), cur.end(), 0);
            fill(vis.begin(), vis.end(), 0);
            T x = dfs(s, t, lim - f);
            assert(x > 0); // 调试检查，可删：最短路存在时紧边 DFS 必须能增广。
            if (x == 0) break;
            f += x;
            cost += (Cost)x * *d[t];
        }
        return {f, cost};
    }
};
