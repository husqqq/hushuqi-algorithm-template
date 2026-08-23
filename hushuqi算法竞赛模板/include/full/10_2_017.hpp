#pragma once

#include "10_1_005.hpp"

vector<vector<Z>> fpsInv2(const vector<vector<Z>> &f, int n, int m)
{
    // f 是二维 FPS，n、m 是两维截断长度；返回满足 f*g=1 mod (x^n,y^m) 的 g。
    assert(n >= 0 && m >= 0);
    if (n == 0 || m == 0)
    {
        return {};
    }
    assert(!f.empty() && !f[0].empty() && f[0][0] != Z(0));
    assert(((__int128)2 * n - 1) * ((__int128)2 * m - 1) <= NTT_LIM + 2); // 调试检查，可删。
    vector<vector<Z>> g(1, vector<Z>(1, f[0][0].inv()));
    for (int len = 1; len < n + m - 1; len <<= 1)
    {
        int rn = min(n, len * 2);
        int cm = min(m, len * 2);
        vector<vector<Z>> a(rn, vector<Z>(cm));
        for (int i = 0; i < rn && i < (int)f.size(); i++)
        {
            for (int j = 0; j < cm && j < (int)f[i].size(); j++)
            {
                if (i + j < len * 2)
                {
                    a[i][j] = f[i][j];
                }
            }
        }
        auto gg = conv2(g, g);
        gg.resize(rn);
        for (auto &row : gg)
        {
            row.resize(cm);
        }
        auto ggf = conv2(move(gg), move(a));
        g.resize(rn);
        for (auto &row : g)
        {
            row.resize(cm);
        }
        for (int i = 0; i < rn; i++)
        {
            for (int j = 0; j < cm; j++)
            {
                if (len <= i + j && i + j < len * 2)
                {
                    g[i][j] = -ggf[i][j];
                }
            }
        }
    }
    g.resize(n);
    for (auto &row : g)
    {
        row.resize(m);
    }
    return g;
}
