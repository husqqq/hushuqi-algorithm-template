#pragma once

#include "5_5_001.hpp"

optional<pair<int, vector<MEdge>>> boruvka(int n, const vector<MEdge> &e)
{
    // n 是点数，e 是无向边集；返回 MST 总权与所选边，不连通时返回空。
    assert(n >= 0); // 调试检查，可删
    DSU d(n);
    int cnt = n, ans = 0;
    vector<MEdge> use;
    while (cnt > 1)
    {
        vector<int> best(n, -1);
        for (int i = 0; i < (int)e.size(); i++)
        {
            int x = d.find(e[i].x), y = d.find(e[i].y);
            if (x == y)
            {
                continue;
            }
            // 求最大生成树时，下面两处 < 要同时改成 >，其余逻辑不变。
            if (best[x] == -1 || e[i].w < e[best[x]].w)
            {
                best[x] = i;
            }
            if (best[y] == -1 || e[i].w < e[best[y]].w)
            {
                best[y] = i;
            }
        }
        bool upd = false;
        for (int x = 0; x < n; x++)
        {
            if (d.find(x) != x || best[x] == -1)
            {
                continue;
            }
            auto v = e[best[x]];
            if (d.merge(v.x, v.y))
            {
                ans += v.w;
                use.push_back(v);
                cnt--;
                upd = true;
            }
        }
        if (!upd)
        {
            return nullopt;
        }
    }
    return pair{ans, use};
}
