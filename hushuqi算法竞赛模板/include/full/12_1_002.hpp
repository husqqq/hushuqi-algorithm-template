#pragma once

#include "12_1_001.hpp"

vector<int> completePack(int m, const vector<pair<int, int>> &a)
{
    // m 是容量，a 的每项是物品体积和价值且每种可取任意次；返回各恰好容量的最大价值。
    assert(m >= 0); // 调试检查，可删。
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v] : a)
    {
        assert(w > 0); // 调试检查，可删。
        for (int j = w; j <= m; j++)
        {
            if (f[j - w] != -INF)
            {
                chmax(f[j], f[j - w] + v);
            }
        }
    }
    return f;
}
