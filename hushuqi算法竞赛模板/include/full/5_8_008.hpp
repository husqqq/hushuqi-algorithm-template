#pragma once

#include "5_8_001.hpp"

template <class Cap = long long>
vector<tuple<int, int, Cap>>
gomoryHu(int n, const vector<tuple<int, int, long long>> &edges)
{
    // edges 是非负权无向边；返回 Gomory-Hu 树边及割值。
    vector<int> p(n);
    vector<Cap> w(n);
    for (int s = 1; s < n; s++)
    {
        int t = p[s];
        Flow<Cap> f(n);
        for (auto [u, v, c] : edges)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && c >= 0); // 调试检查，可删。
            f.add(u, v, (Cap)c);
            f.add(v, u, (Cap)c);
        }
        Cap z = f.flow(s, t);
        auto cut = f.cut(s);
        for (int i = s + 1; i < n; i++)
        {
            if (p[i] == t && cut[i])
            {
                p[i] = s;
            }
        }
        if (t && cut[p[t]])
        {
            p[s] = p[t];
            p[t] = s;
            w[s] = w[t];
            w[t] = z;
        }
        else
        {
            w[s] = z;
        }
    }
    vector<tuple<int, int, Cap>> ans;
    for (int i = 1; i < n; i++)
    {
        ans.push_back({i, p[i], w[i]});
    }
    return ans;
}
