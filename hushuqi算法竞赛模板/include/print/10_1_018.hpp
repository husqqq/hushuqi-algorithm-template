#pragma once

#include "10_1_004.hpp"

vector<unsigned long long> convU64(const vector<unsigned long long> &a,
                                   const vector<unsigned long long> &b)
{
    // a、b 是普通整数系数；返回整数卷积对 2^64 取模的无符号 64 位结果。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int need = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)need);
    assert(n <= (1 << 20)); // 当前五个 NTT 素数均支持这一长度。
    constexpr array<int, 5> mod{998244353, 985661441, 943718401, 935329793, 918552577};
    constexpr array<int, 5> root{3, 3, 7, 3, 5};
    array<vector<int>, 5> rem;
    for (int k = 0; k < 5; k++)
    {
        rem[k] = arbModConv::NTT{mod[k], root[k]}.conv(a, b);
    }
    vector<unsigned long long> ans(need);
    for (int i = 0; i < need; i++)
    {
        array<int, 5> coef{}; // coef[k] 是第 k 个 Garner 系数。
        unsigned long long cur = 0, mul64 = 1;
        for (int k = 0; k < 5; k++)
        {
            unsigned long long seen = 0, mul = 1;
            for (int j = 0; j < k; j++)
            {
                seen = (seen + mul * coef[j]) % mod[k];
                mul = mul * mod[j] % mod[k];
            }
            int delta = (rem[k][i] + mod[k] - seen % mod[k]) % mod[k];
            coef[k] = (unsigned long long)delta * arbModConv::inv((int)mul, mod[k]) % mod[k];
            cur += mul64 * coef[k];
            mul64 *= mod[k];
        }
        ans[i] = cur;
    }
    return ans;
}
