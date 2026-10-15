#pragma once
#include "5_8_001.hpp"

// 有向全局割：固定根 r，枚举 r->v 与 v->r 即覆盖任意非平凡割。
inline pair<__int128, vector<bool>> directedGlobalCut(
    int n, const vector<tuple<int, int, long long>> &edges)
{
    if (n < 2) throw invalid_argument("global cut needs n>=2");
    __int128 best = (__int128)1 << 120; vector<bool> side;
    for (int v = 1; v < n; ++v) for (int dir = 0; dir < 2; ++dir)
    {
        Flow<__int128> f(n);
        for (auto [u, w, c] : edges)
        {
            if (u < 0 || u >= n || w < 0 || w >= n || c < 0) throw invalid_argument("invalid arc");
            f.add(u, w, c);
        }
        int s = dir ? v : 0, t = dir ? 0 : v;
        auto x = f.flow(s, t); if (x < best) best = x, side = f.cut(s);
    }
    return {best, side};
}
