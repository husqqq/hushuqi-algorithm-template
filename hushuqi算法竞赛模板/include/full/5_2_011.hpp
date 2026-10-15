#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// BinaryEdge[0] 是终点，BinaryEdge[1] 是只能取 0 或 1 的边权。
using BinaryEdge = array<int, 2>;

template <class Dist = long long> vector<optional<Dist>> zeroOneBfs(const vector<vector<BinaryEdge>> &g, int s)
{
    // g 是 0-1 权有向图，s 是源点；返回最短距离，不可达点为空。
    int n = g.size();
    assert(0 <= s && s < n); // 调试检查，可删
    vector<optional<Dist>> d(n);
    vector<bool> done(n);
    deque<int> q;
    d[s] = (Dist)0;
    q.push_front(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        if (done[u])
        {
            continue;
        }
        done[u] = true;
        for (auto [v, w] : g[u])
        {
            assert(0 <= v && v < n && (w == 0 || w == 1)); // 调试检查，可删
            Dist nd = *d[u] + (Dist)w;
            if (!d[v].has_value() || nd < *d[v])
            {
                d[v] = nd;
                if (w == 0)
                {
                    q.push_front(v);
                }
                else
                {
                    q.push_back(v);
                }
            }
        }
    }
    return d;
}
