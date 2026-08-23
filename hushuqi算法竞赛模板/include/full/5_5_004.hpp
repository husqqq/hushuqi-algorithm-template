#pragma once

#include "5_5_003.hpp"

struct DynamicMST
{
    // n 是图的点数。
    int n;
    // e 是当前无向边集。
    vector<MEdge> e;

    DynamicMST(int n, vector<MEdge> e) : n(n), e(move(e))
    {
        // n 是点数，e 是初始边集；保存当前图。
    }

    void set(int id, int w)
    {
        // id 是边编号，w 是新边权；修改该边。
        e[id].w = w;
    }

    optional<int> query() const
    {
        // 无参数；返回当前 MST 总权，图不连通时返回空。
        return minKForest(n, 1, e);
    }
};
