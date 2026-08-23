#pragma once

#include "5_3_001.hpp"

vector<int> minPtBasis(const vector<vector<int>> &g)
{
    // g 是有向邻接表；返回升序最小点基，每个源 SCC 取最小编号点。
    if (g.empty())
    {
        return {};
    }
    int n = g.size();
    auto id = tarjanSCC(g);
    int m = *max_element(id.begin(), id.end()) + 1;
    vector<int> in(m), mn(m, n);
    for (int u = 0; u < n; u++)
    {
        mn[id[u]] = min(mn[id[u]], u);
        for (int v : g[u])
        {
            if (id[u] != id[v])
            {
                in[id[v]]++;
            }
        }
    }
    vector<int> ans;
    for (int i = 0; i < m; i++)
    {
        if (!in[i])
        {
            ans.push_back(mn[i]);
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}
