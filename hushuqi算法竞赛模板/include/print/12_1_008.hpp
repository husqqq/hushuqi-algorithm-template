#pragma once

#include "12_1_001.hpp"

struct PackResult
{
    // val 是不超过容量时的最优总价值。
    long long val;
    // ways 是模 mod 的最优方案数。
    int ways;
    // pick 是恢复出的一组物品下标，按输入顺序排列。
    vector<int> pick;
};

PackResult packPlan(int cap, const vector<pair<int, long long>> &a, int mod = 1000000007)
{
    // cap 是容量上限，a 是各 01 物品的体积与价值，mod 是计数模数；返回最优价值、方案数及一组方案。
    // 所有可达价值与候选加法须大于 -INF 且能由 long long 表示。
    // 改求最小价值：所有物品价值取反后调用，再把 val 取反；ways、pick 和空方案语义不变。
    // 取反前须保证原值、取反值及全部中间和都在上述范围内。
    assert(cap >= 0 && mod > 0); // 调试检查，可删。
    int n = a.size();
    vector<long long> f(cap + 1, -INF);
    vector<int> cnt(cap + 1), at(cap + 1, -1);
    vector<array<int, 2>> path; // 每项依次是物品下标、上一前驱节点编号；-1 表示链首。
    f[0] = 0;
    cnt[0] = 1 % mod;
    for (int i = 0; i < n; i++)
    {
        auto [w, v] = a[i];
        assert(w > 0); // 调试检查，可删。
        for (int j = cap; j >= w; j--)
        {
            if (f[j - w] == -INF)
            {
                continue;
            }
            long long x = f[j - w] + v;
            if (x > f[j])
            {
                f[j] = x;
                cnt[j] = cnt[j - w];
                path.push_back({i, at[j - w]});
                at[j] = path.size() - 1;
            }
            else if (x == f[j] && x > -INF)
            {
                cnt[j] = addMod(cnt[j], cnt[j - w], mod);
            }
        }
    }
    long long val = *max_element(f.begin(), f.end());
    int ways = 0;
    int last = -1;
    for (int j = 0; j <= cap; j++)
    {
        if (f[j] == val)
        {
            ways = addMod(ways, cnt[j], mod);
            last = at[j];
        }
    }
    vector<int> pick;
    while (last >= 0)
    {
        pick.push_back(path[last][0]);
        last = path[last][1];
    }
    reverse(pick.begin(), pick.end());
    return {val, ways, pick};
}
