#pragma once

#include "12_1_001.hpp"

vector<int> mixedPack(int m, const vector<array<int, 3>> &a)
{
    assert(m >= 0);
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v, c] : a)
    {
        assert(w > 0 && c >= -1);
        if (c == 0)
        {
            for (int j = w; j <= m; j++)
            {
                if (f[j - w] != -INF)
                {
                    chmax(f[j], f[j - w] + v);
                }
            }
            continue;
        }
        if (c == -1)
        {
            c = 1;
        }
        int k = 1;
        while (c > 0)
        {
            int z = min(k, c);
            c -= z;
            if (z <= m / w)
            {
                int ww = w * z;
                int vv = v * z;
                for (int j = m; j >= ww; j--)
                {
                    if (f[j - ww] != -INF)
                    {
                        chmax(f[j], f[j - ww] + vv);
                    }
                }
            }
            if (c > 0)
            {
                k = k <= c / 2 ? k * 2 : c;
            }
        }
    }
    return f;
}
