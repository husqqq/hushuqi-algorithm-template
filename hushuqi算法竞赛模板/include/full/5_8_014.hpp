#pragma once
#include "5_8_001.hpp"

pair<__int128, vector<bool>> dirMinCut(int n, const vector<array<int, 3>> &e)
{
    // n>=2 是点数，e[i][0..2] 依次是有向边起点、终点和非负容量；
    // 返回有向图全局最小割的容量，以及一组割的源侧点集 side（side[v] 为真表示 v 在源侧）。
    assert(n >= 2); // 调试检查，可删。
    __int128 best = (__int128)1 << 120;
    vector<bool> side;
    // 任一非平凡割都把点 0 与某点 v 分开：枚举 v 和两个方向各求一次最大流即可覆盖。
    for (int v = 1; v < n; v++)
    {
        for (int dir = 0; dir < 2; dir++)
        {
            Flow<__int128> f(n);
            for (auto [x, y, c] : e)
            {
                f.add(x, y, c);
            }
            int s = dir ? v : 0, t = dir ? 0 : v;
            __int128 cur = f.flow(s, t);
            if (cur < best)
            {
                best = cur;
                side = f.cut(s);
            }
        }
    }
    return {best, side};
}
