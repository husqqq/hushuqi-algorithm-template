#pragma once

#include "5_8_001.hpp"

template <class Cap = long long>
struct ClosureRes
{
    Cap val;          // 最大闭合子图点权和。
    vector<int> pick; // 一组被选择的原图点编号。
};

template <class Cap = long long>
ClosureRes<Cap> maxClosure(
    const vector<long long> &w, const vector<array<int, 2>> &need)
{
    // w[u] 是点权，need 中的 {u, v} 表示选 u 必须选 v；返回最优闭集。
    int n = w.size(), s = n, t = n + 1;
    Cap sum = 0;
    for (long long x : w)
    {
        if (x > 0)
        {
            sum += (Cap)x;
        }
    }

    Cap inf = sum + 1;
    Flow<Cap> fl(n + 2);
    for (int u = 0; u < n; u++)
    {
        if (w[u] > 0)
        {
            fl.add(s, u, (Cap)w[u]);
        }
        else if (w[u] < 0)
        {
            fl.add(u, t, -(Cap)w[u]);
        }
    }
    for (auto [u, v] : need)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        fl.add(u, v, inf);
    }

    Cap flow = fl.flow(s, t);
    vector<bool> vis = fl.cut(s);
    vector<int> pick;
    for (int u = 0; u < n; u++)
    {
        if (vis[u])
        {
            pick.push_back(u);
        }
    }
    return ClosureRes<Cap>{sum - flow, pick};
}
