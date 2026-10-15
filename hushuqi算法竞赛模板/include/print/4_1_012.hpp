#pragma once
#include "10_1_002.hpp"

vector<bool> wildcardMC(const string &s, string t, unsigned long long seed, char any = '*')
{
    // s 是文本串，t 是模式串，seed 是本轮随机种子；返回每个合法起点是否匹配。
    int n = s.size();
    int m = t.size();
    if (!m)
    {
        return vector<bool>(n + 1, true);
    }
    if (m > n)
    {
        return {};
    }
    mt19937_64 rng(seed);
    constexpr unsigned long long mask = (1ULL << 48) - 1;
    constexpr long double scale = 1ULL << 48;
    array<long double, 256> w;
    for (long double &x : w)
    {
        x = 1 + ((rng() & mask) + 0.5L) / scale;
    }
    vector<long double> a(n), b(m);
    for (int i = 0; i < n; i++)
    {
        a[i] = w[(unsigned char)s[i]];
    }
    int fixed = 0;
    for (int i = 0; i < m; i++)
    {
        char c = t[m - 1 - i];
        if (c != any)
        {
            b[i] = 1 / w[(unsigned char)c];
            fixed++;
        }
    }
    auto c = convolution(a, b);
    long double tol = 1E-9L * max<int>(1, fixed);
    vector<bool> ans(n - m + 1);
    for (int i = 0; i + m <= n; i++)
    {
        ans[i] = fabsl(c[i + m - 1] - fixed) <= tol;
    }
    return ans;
}
