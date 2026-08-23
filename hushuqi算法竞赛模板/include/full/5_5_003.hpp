#pragma once

#include "5_5_001.hpp"

optional<int> minKForest(int n, int k, vector<MEdge> e)
{
    // n 是点数，k 是目标连通块数，e 是无向边集；返回最小森林权值。
    assert(n >= 1 && 1 <= k && k <= n); // 调试检查，可删。
    if (k == n)
    {
        return 0;
    }
    sort(e.begin(),
         e.end(),
         [](auto a, auto b)
         {
             return a.w < b.w;
         });
    DSU d(n);
    int cc = n;
    int ans = 0;
    for (auto [u, v, w] : e)
    {
        if (!d.merge(u, v))
        {
            continue;
        }
        ans += w;
        if (--cc == k)
        {
            return ans;
        }
    }
    return cc == k ? optional<int>(ans) : nullopt;
}
