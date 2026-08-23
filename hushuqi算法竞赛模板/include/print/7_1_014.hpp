#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

unsigned long long kthRoot(unsigned long long n, int k)
{
    // n 是被开方的非负整数，k 是正整数次数；返回 floor(n^(1/k))。
    assert(k > 0); // 调试检查，可删
    if (k == 1 || n <= 1)
    {
        return n;
    }
    if (k >= 64)
    {
        return 1;
    }
    auto ok = [&](unsigned long long x)
    {
        unsigned __int128 y = 1;
        for (int i = 0; i < k; i++)
        {
            y *= x;
            if (y > n)
            {
                return false;
            }
        }
        return true;
    };
    unsigned long long l = 1;
    unsigned long long r = min(n, 1ULL << (64 / k + 1));
    while (l < r)
    {
        unsigned long long m = l + (r - l + 1) / 2;
        if (ok(m))
        {
            l = m;
        }
        else
        {
            r = m - 1;
        }
    }
    return l;
}
