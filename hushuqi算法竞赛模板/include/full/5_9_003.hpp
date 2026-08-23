#pragma once

#include "8_2_011.hpp"

Z bestTheorem(int n, int root, const vector<pair<int, int>> &e, bool fixFirst = false)
{
    // n 是点数，root 是起点，e 是有编号有向边；fixFirst 表示是否固定首边。
    if (n == 0)
    {
        return e.empty() ? Z(1) : Z(0);
    }
    assert(0 <= root && root < n); // 调试检查，可删。
    vector<int> out(n), in(n);
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        out[u]++;
        in[v]++;
    }
    for (int u = 0; u < n; u++)
    {
        if (out[u] != in[u])
        {
            return 0;
        }
    }
    if (e.empty())
    {
        return 1;
    }
    if (out[root] == 0)
    {
        return 0;
    }
    vector<int> id(n, -1), active;
    for (int u = 0; u < n; u++)
    {
        if (out[u] > 0)
        {
            id[u] = active.size();
            active.push_back(u);
        }
    }
    vector<pair<int, int>> rev;
    for (auto [u, v] : e)
    {
        rev.push_back({id[v], id[u]});
    }
    Z ans = arboCount(active.size(), id[root], rev);
    for (int u : active)
    {
        for (int k = 1; k < out[u]; k++)
        {
            ans *= k;
        }
    }
    if (!fixFirst)
    {
        ans *= out[root];
    }
    return ans;
}
