#pragma once

#include "14_1_001.hpp"

namespace grid_count
{
template<class T, class C>
T gridPaths(int h, int w, vector<array<int, 2>> ban, C &comb)
{
    // h、w 是网格尺寸，ban[i] 是障碍坐标，comb 提供 C(n,k)；返回模 T::mod() 的路径数。
    // 调试检查，可删。
    assert(h > 0 && w > 0);
    // 调试检查，可删。
    assert(h - 1 < T::mod() && w - 1 < T::mod() - (h - 1));
    for (auto p : ban)
    {
        // 调试检查，可删。
        assert(0 <= p[0] && p[0] < h && 0 <= p[1] && p[1] < w);
    }
    sort(ban.begin(), ban.end());
    ban.erase(unique(ban.begin(), ban.end()), ban.end());
    array<int, 2> goal{h - 1, w - 1};
    if (binary_search(ban.begin(), ban.end(), goal))
    {
        return T(0);
    }
    ban.push_back(goal);
    sort(ban.begin(), ban.end());
    int goalPos = lower_bound(ban.begin(), ban.end(), goal) - ban.begin();
    vector<T> dp(ban.size());
    for (int i = 0; i < (int)ban.size(); i++)
    {
        auto [x, y] = ban[i];
        dp[i] = comb.C(x + y, x);
        for (int j = 0; j < i; j++)
        {
            auto [u, v] = ban[j];
            if (u <= x && v <= y)
            {
                dp[i] -= dp[j] * comb.C(x - u + y - v, x - u);
            }
        }
    }
    return dp[goalPos];
}
}
