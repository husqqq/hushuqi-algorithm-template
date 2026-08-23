#pragma once

#include "12_1_001.hpp"

vector<int> groupPack(int m, const vector<vector<pair<int, int>>> &a)
{
    assert(m >= 0);
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto &g : a)
    {
        auto h = f;
        for (auto [w, v] : g)
        {
            assert(w > 0);
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
