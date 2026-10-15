#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template<class Dist = long long>
class IntGraph
{
    // n 是原始点数，g 是含辅助点的带权有向图。
    int n;
    vector<vector<array<int, 2>>> g;
    // out、in 是线段树节点对应的向外树、向内树图节点。
    vector<int> out, in;

    int node()
    {
        // 无参数；新增空图节点并返回编号。
        g.push_back({});
        return g.size() - 1;
    }

    void edge(int u, int v, int w)
    {
        // u、v 是图节点，w 是非负边权；加入有向边。
        g[u].push_back({v, w});
    }

    void build(int p, int l, int r)
    {
        // p、[l,r) 是线段树状态；建立两棵方向相反的零权树。
        out[p] = node();
        in[p] = node();
        if (r - l == 1)
        {
            edge(out[p], l, 0);
            edge(l, in[p], 0);
            return;
        }
        int m = (l + r) / 2;
        build(p << 1, l, m);
        build(p << 1 | 1, m, r);
        edge(out[p], out[p << 1], 0);
        edge(out[p], out[p << 1 | 1], 0);
        edge(in[p << 1], in[p], 0);
        edge(in[p << 1 | 1], in[p], 0);
    }

    void pointRange(int p, int l, int r, int u, int ql, int qr, int w)
    {
        // p、[l,r) 是树状态，u 到 [ql,qr) 连权 w 的压缩边。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            edge(u, out[p], w);
            return;
        }
        int m = (l + r) / 2;
        pointRange(p << 1, l, m, u, ql, qr, w);
        pointRange(p << 1 | 1, m, r, u, ql, qr, w);
    }

    void rangePoint(int p, int l, int r, int ql, int qr, int v, int w)
    {
        // p、[l,r) 是树状态，[ql,qr) 到 v 连权 w 的压缩边。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            edge(in[p], v, w);
            return;
        }
        int m = (l + r) / 2;
        rangePoint(p << 1, l, m, ql, qr, v, w);
        rangePoint(p << 1 | 1, m, r, ql, qr, v, w);
    }

  public:
    IntGraph(int n) : n(n), g(n), out(4 * n), in(4 * n)
    {
        // n 是正的原始点数；建立区间图辅助节点。
        assert(n > 0); // 调试检查，可删。
        build(1, 0, n);
    }

    void addEdge(int u, int v, int w)
    {
        // u、v 是原始点，w 是非负边权；加入单点边。
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        edge(u, v, w);
    }

    void addPointRng(int u, int l, int r, int w)
    {
        // u 到 [l,r) 加权 w 的逻辑边。
        assert(0 <= u && u < n && 0 <= l && l <= r && r <= n && w >= 0); // 调试检查，可删。
        pointRange(1, 0, n, u, l, r, w);
    }

    void addRngPoint(int l, int r, int v, int w)
    {
        // [l,r) 到 v 加权 w 的逻辑边。
        assert(0 <= l && l <= r && r <= n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        rangePoint(1, 0, n, l, r, v, w);
    }

    vector<optional<Dist>> dijkstra(int s) const
    {
        // s 是原始源点；返回到全部原始点的最短路，不可达点为空。
        assert(0 <= s && s < n); // 调试检查，可删。
        vector<optional<Dist>> d(g.size());
        priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
        d[s] = 0;
        q.push({0, s});
        while (!q.empty())
        {
            auto [du, u] = q.top();
            q.pop();
            if (!d[u].has_value() || du != *d[u])
            {
                continue;
            }
            for (auto [v, w] : g[u])
            {
                Dist nd = du + (Dist)w;
                if (!d[v].has_value() || nd < *d[v])
                {
                    d[v] = nd;
                    q.push({nd, v});
                }
            }
        }
        d.resize(n);
        return d;
    }
};
