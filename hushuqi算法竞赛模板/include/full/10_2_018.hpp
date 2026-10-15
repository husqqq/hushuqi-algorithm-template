#pragma once

#include "10_2_001.hpp"

Poly productPolys(vector<Poly> a)
{
    // a 是待相乘的多项式序列；返回保持卷积长度的总乘积，空序列返回常数 1。
    Z c = 1;
    vector<Poly> f;
    f.reserve(a.size());
    for (auto &x : a)
    {
        if (x.empty())
        {
            return {};
        }
        if (x.size() == 1)
        {
            c *= x[0];
        }
        else
        {
            f.push_back(move(x));
        }
    }
    if (f.empty())
    {
        return {c};
    }
    vector<Poly> pool;
    pool.reserve(2 * f.size());
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    for (auto &x : f)
    {
        int p = pool.size();
        pool.push_back(move(x));
        q.push({pool[p].size(), p});
    }
    while (q.size() > 1)
    {
        int x = q.top().second;
        q.pop();
        int y = q.top().second;
        q.pop();
        assert(convFit(pool[x].size(), pool[y].size())); // 调试检查，可删。
        vector<Z> z = conv(move(pool[x]), move(pool[y]));
        int p = pool.size();
        pool.emplace_back(move(z));
        q.push({pool[p].size(), p});
    }
    Poly ans = move(pool[q.top().second]);
    ans *= c;
    return ans;
}
