#pragma once

#include "12_1_001.hpp"

vector<int> boundedPack(int m, const vector<array<int, 3>> &a)
{
    // m 是容量；a[i][0..2] 依次是体积、价值和最多可选件数；返回各恰好容量的最大价值。
    assert(m >= 0); // 调试检查，可删。
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v, c] : a)
    {
        assert(w > 0 && c >= 0); // 调试检查，可删。
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
