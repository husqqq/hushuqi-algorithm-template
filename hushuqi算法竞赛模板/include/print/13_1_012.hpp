#pragma once

#include "5_6_003.hpp"
#include "5_6_007.hpp"

pair<vector<int>, vector<int>> bipGame(const vector<vector<int>> &g, int nr)
{
    // g[u] 是左点 u 相邻的右点编号，nr 是右部点数；左右部均从 0 编号。
    // 每步走到邻点并删原点，无法行动者负；返回全部必胜起点的 {左点表,右点表}，各自递增。
    assert(nr >= 0);
    int nl = g.size();
    HopcroftKarp hk(nl, nr);
    vector<vector<int>> rev(nr);
    for (int u = 0; u < nl; u++)
    {
        for (int v : g[u])
        {
            hk.addEdge(u, v);
            rev[v].push_back(u);
        }
    }
    hk.matching();
    auto ml = hk.leftMatch(), mr = hk.matchR();
    // 点覆盖构造的左部结果恰为本侧交替搜索不可达点，即所有最大匹配均覆盖的点。
    auto l = minVtxCover(g, ml, mr).first;
    auto r = minVtxCover(rev, mr, ml).first;
    return {move(l), move(r)};
}
