#pragma once

#include "5_8_001.hpp"

template <class Cap = long long>
struct BoundResult
{
    // 指定源汇时的流值；只求循环可行流时为 0。
    Cap value;
    // 每条原边的实际流量。
    vector<Cap> edge;
};

template <class Cap = long long>
struct BoundFlow
{
    struct E
    {
        // 端点与流量上下界。
        int u, v;
        Cap l, r;
    };

    // 点数与原图边集。
    int n;
    vector<E> e;

    BoundFlow(int n = 0) : n(n)
    {
        // n 是原图点数；构造空网络。
    }

    void add(int u, int v, Cap l, Cap r)
    {
        // u、v 是端点，l、r 是非负流量下界和上界。
        assert(0 <= u && u < n && 0 <= v && v < n && 0 <= l && l <= r); // 调试检查，可删。
        e.push_back({u, v, l, r});
    }

    optional<BoundResult<Cap>> solve(int s = -1, int t = -1, bool minimum = false) const
    {
        // s=t=-1 求循环可行流；否则 minimum 决定求最小流还是最大流。
        assert((s == -1 && t == -1) ||
               (0 <= s && s < n && 0 <= t && t < n && s != t)); // 调试检查，可删。
        int ss = n, tt = n + 1;
        Flow<Cap> f(n + 2);
        vector<Cap> d(n);
        vector<pair<int, int>> id;
        Cap sumR = 0;
        for (const auto &x : e)
        {
            id.push_back({x.u, f.add(x.u, x.v, x.r - x.l)});
            d[x.u] -= x.l;
            d[x.v] += x.l;
            sumR += x.r;
        }
        int art = -1;
        if (s != -1)
        {
            art = f.add(t, s, sumR + 1);
        }
        Cap need = 0;
        for (int i = 0; i < n; i++)
        {
            if (d[i] > 0)
            {
                f.add(ss, i, d[i]);
                need += d[i];
            }
            else if (d[i] < 0)
            {
                f.add(i, tt, -d[i]);
            }
        }
        if (f.flow(ss, tt) != need)
        {
            return nullopt;
        }
        Cap val = 0;
        if (s != -1)
        {
            auto &a = f.e[t][art];
            val = f.e[s][a.rev].cap;
            a.cap = 0;
            f.e[s][a.rev].cap = 0;
        }
        for (int u : {ss, tt})
        {
            for (auto &a : f.e[u])
            {
                f.e[a.to][a.rev].cap = 0;
                a.cap = 0;
            }
        }
        if (s != -1)
        {
            if (minimum)
            {
                val -= f.flow(t, s, val);
            }
            else
            {
                val += f.flow(s, t);
            }
        }
        vector<Cap> ans(e.size());
        for (int i = 0; i < (int)e.size(); i++)
        {
            auto [u, j] = id[i];
            auto &a = f.e[u][j];
            ans[i] = e[i].l + f.e[a.to][a.rev].cap;
        }
        return BoundResult<Cap>{val, ans};
    }
};
