#pragma once
#include "7_1_014.hpp"

vector<long long> segPrimes(long long l, long long r)
{
    // 返回半开区间 [l,r) 内的全部素数并按数值升序排列。
    l = max(l, 2LL);
    if (l >= r)
    {
        return {};
    }
    int n = kthRoot((unsigned long long)(r - 1), 2);
    vector<bool> a(n + 1, true);
    vector<bool> b((size_t)(r - l), true);
    vector<int> p;
    for (int i = 2; i <= n; i++)
    {
        if (!a[i])
        {
            continue;
        }
        p.push_back(i);
        if (i <= n / i)
        {
            for (int j = i * i; j <= n; j += i)
            {
                a[j] = false;
            }
        }
    }
    for (long long x : p)
    {
        long long q = l / x + (l % x != 0);
        long long first = q > (r - 1) / x ? r : q * x;
        long long square = x > (r - 1) / x ? r : x * x;
        long long s = max(square, first);
        for (long long y = s; y < r; )
        {
            b[(size_t)(y - l)] = false;
            if (y > r - x)
            {
                break;
            }
            y += x;
        }
    }
    vector<long long> ans;
    for (long long x = l; x < r; x++)
    {
        if (b[x - l])
        {
            ans.push_back(x);
        }
    }
    return ans;
}
