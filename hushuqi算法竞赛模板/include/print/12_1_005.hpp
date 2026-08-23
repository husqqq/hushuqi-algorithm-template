#pragma once

#include "12_1_001.hpp"

int knapsack2(int cap1, int cap2, const vector<array<int, 3>> &a)
{
    assert(cap1 >= 0 && cap2 >= 0);
    vector f(cap1 + 1, vector<int>(cap2 + 1, -INF));
    f[0][0] = 0;
    for (auto [x, y, v] : a)
    {
        assert(x >= 0 && y >= 0 && (x > 0 || y > 0));
        for (int i = cap1; i >= x; i--)
        {
            for (int j = cap2; j >= y; j--)
            {
                if (f[i - x][j - y] != -INF)
                {
                    chmax(f[i][j], f[i - x][j - y] + v);
                }
            }
        }
    }
    int ans = 0;
    for (const auto &row : f)
    {
        for (int x : row)
        {
            chmax(ans, x);
        }
    }
    return ans;
}
