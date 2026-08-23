#pragma once

#include "5_8_001.hpp"

template <class Cap = long long>
struct BinaryCut
{
    // 变量数与内部源汇编号。
    int n, s, t;
    // 有限容量边、硬约束边及全部有限容量之和。
    vector<tuple<int, int, Cap>> edge;
    vector<pair<int, int>> hard;
    Cap sum = 0;
    BinaryCut(int n) : n(n), s(n), t(n + 1)
    {
        // n 是二元变量数；构造空模型。
    }

    void addCost(int u, Cap c0, Cap c1)
    {
        // c0、c1 是 u 取 0、1 的非负代价。
        assert(0 <= u && u < n && c0 >= 0 && c1 >= 0); // 调试检查，可删。
        edge.push_back({s, u, c0});
        edge.push_back({u, t, c1});
        sum += c0 + c1;
    }

    void imply(int u, int v)
    {
        // 加入 u=1 推出 v=1 的硬约束。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        hard.push_back({u, v});
    }

    void addDiffCost(int u, int v, Cap c)
    {
        // 二者取值不同时付非负代价 c。
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0); // 调试检查，可删。
        edge.push_back({u, v, c});
        edge.push_back({v, u, c});
        sum += c + c;
    }

    pair<Cap, vector<int>> solve() const
    {
        // 返回最小代价与一组取值；源点侧变量取 1。
        Cap inf = sum + 1;
        Flow<Cap> fl(n + 2);
        for (auto [u, v, c] : edge)
        {
            fl.add(u, v, c);
        }
        for (auto [u, v] : hard)
        {
            fl.add(u, v, inf);
        }
        Cap ans = fl.flow(s, t);
        vector<bool> side = fl.cut(s);
        vector<int> val(n);
        for (int u = 0; u < n; u++)
        {
            val[u] = side[u];
        }
        return pair<Cap, vector<int>>{ans, val};
    }
};
