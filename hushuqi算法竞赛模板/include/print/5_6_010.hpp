#pragma once

#include "5_6_003.hpp"
#include "5_6_007.hpp"

vector<int> dagAntichain(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是 DAG 边；返回一组最大互不可达点集。
    vector reach(n, vector<bool>(n));
    for (auto [u, v] : edges)
    {
        reach[u][v] = true;
    }
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            if (!reach[i][k])
            {
                continue;
            }
            for (int j = 0; j < n; j++)
            {
                reach[i][j] = reach[i][j] || reach[k][j];
            }
        }
    }

    vector<vector<int>> g(n);
    HopcroftKarp hk(n, n);
    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            if (reach[u][v])
            {
                g[u].push_back(v);
                hk.addEdge(u, v);
            }
        }
    }
    hk.matching();
    auto [cl, cr] = minVtxCover(g, hk.leftMatch(), hk.matchR());
    vector<bool> inL(n), inR(n);
    for (int u : cl)
    {
        inL[u] = true;
    }
    for (int v : cr)
    {
        inR[v] = true;
    }
    vector<int> ans;
    for (int u = 0; u < n; u++)
    {
        if (!inL[u] && !inR[u])
        {
            ans.push_back(u);
        }
    }
    return ans;
}
