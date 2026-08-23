#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace gauss_binom
{
vector<int> qBinomial(int n, int k)
{
    // n、k 是高斯二项式参数；返回按 q 次数升序的整数系数。
    if (k < 0 || k > n)
    {
        return {};
    }
    k = min(k, n - k);
    int w = n - k;
    vector<int> a{1};
    for (int i = 1; i <= k; i++)
    {
        vector<int> b(i * w + 1);
        for (int j = 0; j <= i * w; j++)
        {
            if (j < (int)a.size())
            {
                b[j] += a[j];
            }
            if (j >= w + i && j - w - i < (int)a.size())
            {
                b[j] -= a[j - w - i];
            }
            if (j >= i)
            {
                b[j] += b[j - i];
            }
        }
        a = move(b);
    }
    return a;
}
}
