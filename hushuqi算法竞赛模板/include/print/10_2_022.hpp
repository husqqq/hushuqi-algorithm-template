#pragma once
#include "10_2_005.hpp"

vector<Z> subsetCount(const vector<int> &a, int T)
{
    // a 是非负整数权，0<=T<2^22；返回 f[0..T]，f[s] 是和为 s 的子集个数模 998244353。
    // prod (1+x^v) = exp(sum_v sum_k (-1)^{k+1} x^{kv}/k)，按权值合并后调和级数求 ln，再 exp。
    assert(0 <= T && T < (1LL << 22)); // 调试检查，可删。
    vector<int> cnt(T + 1); // cnt[v] 是权值为 v 的元素个数（只统计 1..T）。
    int zeros = 0;
    for (int x : a)
    {
        assert(x >= 0); // 调试检查，可删。
        if (!x)
        {
            zeros++;
        }
        else if (x <= T)
        {
            cnt[x]++;
        }
    }
    const auto &iv = invTable<mod>(T);
    Poly lg(T + 1);
    for (int v = 1; v <= T; v++)
    {
        if (cnt[v])
        {
            for (int k = 1; k * v <= T; k++)
            {
                lg[k * v] += Z(cnt[v]) * (k & 1 ? iv[k] : -iv[k]);
            }
        }
    }
    // 权为 0 的元素可选可不选，整体乘 2^zeros。
    Poly f = fpsExp(lg, T + 1);
    Z sc = Z(2).pow(zeros);
    for (auto &x : f)
    {
        x *= sc;
    }
    return f;
}
