#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// 知识点 14.1.009：障碍点网格最短路径计数。
namespace grid_count
{
template <class T, class C> T gridPaths(int h, int w, vector<array<int, 2>> ban, C &comb)
{
    // h、w 是正网格尺寸；ban[i][0]、[1] 为障碍行、列；comb.C(n,k) 返回类型 T 的组合数。
    // 坐标从 0 开始，要求 h+w-2<T::mod()；返回从左上到右下只向下、向右的路径数。
    assert(h > 0 && w > 0);                                 // 调试检查，可删。
    assert(h - 1 < T::mod() && w - 1 < T::mod() - (h - 1)); // 调试检查，可删。
    for (auto p : ban)
    {
        assert(0 <= p[0] && p[0] < h && 0 <= p[1] && p[1] < w); // 调试检查，可删。
    }
    sort(ban.begin(), ban.end());
    ban.erase(unique(ban.begin(), ban.end()), ban.end());
    array<int, 2> goal{h - 1, w - 1};
    if (binary_search(ban.begin(), ban.end(), goal))
    {
        return T(0);
    }
    ban.push_back(goal); // 终点不小于任意合法障碍，追加后仍保持坐标序。
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
    return dp.back();
}
} // 命名空间 grid_count
