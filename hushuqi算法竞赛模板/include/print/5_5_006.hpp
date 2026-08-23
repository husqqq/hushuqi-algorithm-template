#pragma once

#include "5_5_001.hpp"

// CP[0..1] 依次是原点的 x、y 坐标。
using CP = array<int, 2>;

struct GeometricMST
{
    __int128 weight = 0; // 最小生成树总权。
    vector<pair<int, int>> edge; // 所选边的原点编号。
};

GeometricMST chebyMST(const vector<CP> &input)
{
    // input[i][0..1] 是原点坐标；返回切比雪夫 MST 总权与所选边。
    struct Point
    {
        // x、y 是变换后的坐标，id 是原点编号。
        __int128 x, y;
        int id;
    };
    struct Edge
    {
        // w 是候选边权，u、v 是原点编号。
        __int128 w;
        int u, v;
    };
    int n = input.size();
    vector<Point> p;
    for (int i = 0; i < n; ++i)
    {
        p.push_back({(__int128)input[i][0] + input[i][1],
                     (__int128)input[i][0] - input[i][1], i});
    }
    vector<Edge> e;
    for (int dir = 0; dir < 4; ++dir)
    {
        ranges::sort(p,
                     [](const Point &a, const Point &b)
                     {
                         return a.x + a.y != b.x + b.y ? a.x + a.y < b.x + b.y : a.x < b.x;
                     });
        map<__int128, int> sweep;
        for (int i = 0; i < n; ++i)
        {
            for (auto it = sweep.lower_bound(-p[i].y); it != sweep.end();)
            {
                int j = it->second;
                if (p[i].x - p[j].x < p[i].y - p[j].y)
                {
                    break;
                }
                __int128 dx = p[i].x - p[j].x;
                __int128 dy = p[i].y - p[j].y;
                if (dx < 0)
                {
                    dx = -dx;
                }
                if (dy < 0)
                {
                    dy = -dy;
                }
                e.push_back({(dx + dy) / 2, p[i].id, p[j].id});
                it = sweep.erase(it);
            }
            sweep[-p[i].y] = i;
        }
        for (auto &v : p)
        {
            if (dir & 1)
            {
                v.x = -v.x;
            }
            else
            {
                swap(v.x, v.y);
            }
        }
    }
    ranges::sort(e,
                 {},
                 [](const Edge &a)
                 {
                     return a.w;
                 });
    DSU d(n);
    GeometricMST ans;
    for (auto [w, u, v] : e)
    {
        if (!d.merge(u, v))
        {
            continue;
        }
        ans.weight += w;
        ans.edge.push_back({u, v});
    }
    return ans;
}

__int128 chebyshevMst(const vector<CP> &input)
{
    // input[i][0..1] 是原点坐标；返回切比雪夫 MST 总权。
    return chebyMST(input).weight;
}
