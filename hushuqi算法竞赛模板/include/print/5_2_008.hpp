#pragma once

#include "5_2_001.hpp"

template<class Dist = long long>
pair<vector<optional<Dist>>, vector<int>> shortestTree(
    const vector<vector<WEdge>> &g, int s)
{
    // g 是非负权有向图，s 是源点；返回 {最短距离,父点}，源点和不可达点的父点为 -1。
    int n = g.size();
    assert(0 <= s && s < n); // 调试检查，可删。
    vector<optional<Dist>> d(n);
    vector<int> par(n, -1);
    d[s] = 0;
    dijkstraInit(g, d, [&](int u, const WEdge &edge)
    {
        // u 是前驱，edge 为 {终点,边权}；严格改善时更新父点。
        par[edge[0]] = u;
    });
    return {move(d), move(par)};
}
