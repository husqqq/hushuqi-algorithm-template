#pragma once

#include "12_1_001.hpp"

vector<int> completePack(int m, const vector<pair<int, int>> &a)
{
    assert(m >= 0);
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v] : a)
    {
        assert(w > 0);
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
