#pragma once

#include "5_5_001.hpp"
#include "11_3_011.hpp"

long double euclideanMST(const vector<array<long double, 2>> &p)
{
    // p[i][0..1] 依次是第 i 个点的 x、y 坐标；返回完全图欧氏距离最小生成树总长。
    int n = p.size();
    if (n == 0)
    {
        return 0;
    }
    vector<long double> dis(n, numeric_limits<long double>::infinity());
    vector<bool> vis(n);
    dis[0] = 0;
    long double ans = 0;
    for (int t = 0; t < n; t++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!vis[i] && (u == -1 || dis[i] < dis[u]))
            {
                u = i;
            }
        }
        vis[u] = true;
        ans += sqrtl(dis[u]);
        for (int v = 0; v < n; v++)
        {
            if (vis[v])
            {
                continue;
            }
            long double dx = p[u][0] - p[v][0], dy = p[u][1] - p[v][1];
            dis[v] = min(dis[v], dx * dx + dy * dy);
        }
    }
    return ans;
}

vector<pair<int, int>> euclidMST(const vector<array<long long, 2>> &p)
{
    // p[i][0..1] 是整数点坐标；返回完全图欧氏距离 MST 的原点编号边集。
    int n = p.size();
    if (n <= 1)
    {
        return {};
    }
    map<pair<long long, long long>, int> first;
    vector<IP> unique;
    vector<int> rep;
    vector<MEdge> edge;
    for (int i = 0; i < n; i++)
    {
        pair key{p[i][0], p[i][1]};
        auto [it, fresh] = first.emplace(key, i);
        if (!fresh)
        {
            edge.push_back({it->second, i, 0});
        }
        else
        {
            unique.push_back({p[i][0], p[i][1]});
            rep.push_back(i);
        }
    }
    auto tri = delaunay(unique);
    for (auto [x, y] : tri.edge)
    {
        long long dx = unique[x].x - unique[y].x;
        long long dy = unique[x].y - unique[y].y;
        edge.push_back({rep[x], rep[y], dx * dx + dy * dy});
    }
    auto mst = kruskal(n, move(edge));
    assert(mst.has_value()); // 调试检查，可删；Delaunay 边与重合点零边必然连通。
    vector<pair<int, int>> ans;
    for (auto [x, y, w] : mst->second)
    {
        ans.push_back({x, y});
    }
    return ans;
}
