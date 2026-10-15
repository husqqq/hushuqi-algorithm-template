#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace minmax_inv
{
int kthMaxMinSum(const vector<int> &mn, int k)
{
    // mn[j] 是所有大小为 j 的子集最小值之和且 mn[0] 不用，k 从 1 开始；返回第 k 大元素。
    int n = (int)mn.size() - 1;
    assert(1 <= k && k <= n); // 调试检查，可删。
    constexpr array<long long, 3> ps{998244353, 1000000007, 1000000009};
    assert(n < ps[0]); // 调试检查，可删。
    array<long long, 3> rem{};
    for (int z = 0; z < 3; z++)
    {
        long long mod = ps[z];
        vector<long long> inv(n - k + 1);
        if (n > k)
        {
            inv[1] = 1;
        }
        for (int i = 2; i <= n - k; i++)
        {
            inv[i] = mod - (mod / i) * inv[mod % i] % mod;
        }
        long long c = 1;
        long long ans = 0;
        for (int j = k; j <= n; j++)
        {
            if (j > k)
            {
                c = c * (j - 1) % mod * inv[j - k] % mod;
            }
            long long x = mn[j] % mod;
            if (x < 0)
            {
                x += mod;
            }
            long long v = c * x % mod;
            if ((j - k) & 1)
            {
                ans -= v;
                if (ans < 0)
                {
                    ans += mod;
                }
            }
            else
            {
                ans += v;
                if (ans >= mod)
                {
                    ans -= mod;
                }
            }
        }
        rem[z] = ans;
    }
    __int128 ans = rem[0];
    __int128 m = ps[0];
    for (int z = 1; z < 3; z++)
    {
        long long mod = ps[z];
        long long a = rem[z] - (long long)(ans % mod);
        if (a < 0)
        {
            a += mod;
        }
        long long b = m % mod;
        long long e = mod - 2;
        long long iv = 1;
        while (e)
        {
            if (e & 1)
            {
                iv = iv * b % mod;
            }
            b = b * b % mod;
            e >>= 1;
        }
        ans += m * (a * iv % mod);
        m *= mod;
    }
    if (ans > m / 2)
    {
        ans -= m;
    }
    assert(numeric_limits<int>::min() <= ans && ans <= numeric_limits<int>::max()); // 调试检查，可删。
    return (int)ans;
}
}
