#pragma once

#include "12_1_001.hpp"

vector<int> groupPack(int m, const vector<vector<pair<int, int>>> &a)
{
    // m 是容量，a[i] 是第 i 组全部“体积、价值”物品；每组至多选一件，返回各恰好容量最大价值。
    assert(m >= 0); // 调试检查，可删。
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto &g : a)
    {
        auto h = f;
        for (auto [w, v] : g)
        {
            assert(w > 0); // 调试检查，可删。
            for (int j = w; j <= m; j++)
            {
                if (f[j - w] != -INF)
                {
                    chmax(h[j], f[j - w] + v);
                }
            }
        }
        f.swap(h);
    }
    return f;
}
