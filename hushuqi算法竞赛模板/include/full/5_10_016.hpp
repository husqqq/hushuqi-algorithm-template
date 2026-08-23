#pragma once

#include "5_4_001.hpp"

optional<vector<bool>> outerChordOK(int n, const vector<pair<int, int>> &chord)
{
    // n 是环上顶点数；每条弦选择环内或环外，返回一组可行侧别。
    assert(n >= 3);
    TarjanTwoSAT sat(chord.size());
    vector<pair<int, int>> a = chord;
    for (auto &[u, v] : a)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        if (u > v)
        {
            swap(u, v);
        }
    }
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = i + 1; j < (int)a.size(); j++)
        {
            auto [u, v] = a[i];
            auto [x, y] = a[j];
            if ((u < x && x < v && v < y) || (x < u && u < y && y < v))
            {
                sat.addOr(i, false, j, false);
                sat.addOr(i, true, j, true);
            }
        }
    }
    return sat.solve();
}
