#pragma once

#include "5_6_003.hpp"

vector<vector<int>> dagPathCover(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是 DAG 边；返回一组最小点不相交路径覆盖。
    HopcroftKarp hk(n, n);
    for (auto [x, y] : edges)
    {
        hk.addEdge(x, y);
    }
    hk.matching();
    const auto &ml = hk.leftMatch();
    const auto &mr = hk.matchR();
    vector<vector<int>> ans;
    for (int s = 0; s < n; s++)
    {
        if (mr[s] != -1)
        {
            continue;
        }
        vector<int> a;
        for (int x = s; x != -1; x = ml[x])
        {
            a.push_back(x);
        }
        ans.push_back(move(a));
    }
    return ans;
}
