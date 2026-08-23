#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

long long carmichael(long long n)
{
    // n 是正整数；返回 Carmichael 函数 lambda(n)。
    assert(n >= 1); // 调试检查，可删
    long long ans = 1;
    for (long long p = 2; p <= n / p; p++)
    {
        if (n % p)
        {
            continue;
        }
        long long q = 1;
        int k = 0;
        while (n % p == 0)
        {
            n /= p;
            q *= p;
            k++;
        }
        long long z = q / p * (p - 1);
        if (p == 2 && k >= 3)
        {
            z /= 2;
        }
        ans = lcm(ans, z);
    }
    if (n > 1)
    {
        ans = lcm(ans, n - 1);
    }
    return ans;
}
