#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace planar_graph
{
optional<vector<int>> chordPlanar(int n, vector<array<int, 2>> es)
{
    // n 是按环排列的点数，es[i] 是弦端点；返回内外侧 0/1 方案，无解返回空。
    int m = es.size();
    vector<vector<int>> g(m);
    for (auto &[u, v] : es)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        if (u > v)
        {
            swap(u, v);
        }
    }
    for (int i = 0; i < m; i++)
    {
        auto [a, b] = es[i];
        for (int j = i + 1; j < m; j++)
        {
            auto [c, d] = es[j];
            bool cross = (a < c && c < b && b < d) || (c < a && a < d && d < b);
            if (cross)
            {
                g[i].push_back(j);
                g[j].push_back(i);
            }
        }
    }
    vector<int> col(m, -1);
    for (int s = 0; s < m; s++)
    {
        if (col[s] >= 0)
        {
            continue;
        }
        col[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : g[u])
            {
                if (col[v] < 0)
                {
                    col[v] = col[u] ^ 1;
                    q.push(v);
                }
                else if (col[v] == col[u])
                {
                    return nullopt;
                }
            }
        }
    }
    return col;
}

long long faceCount(long long v, long long e, long long comp = 1)
{
    // v、e、comp 是点数、边数和连通块数；返回平面嵌入的面数。
    if (e >= v)
    {
        return e - v + comp + 1;
    }
    return comp - (v - e) + 1;
}
}
