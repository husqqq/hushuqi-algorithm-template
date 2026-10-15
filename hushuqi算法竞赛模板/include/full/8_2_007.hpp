#pragma once

#include <bits/stdc++.h>
#include "10_2_003.hpp"
using namespace std;
#define int long long

template <class T> vector<T> bernoulli(int n)
{
    // n 是要计算的最大下标；返回 B_0 到 B_n，采用 B_1=+1/2 的约定。
    assert(n >= 0); // 调试检查，可删。
    vector<T> a(n + 1), b(n + 1);
    for (int m = 0; m <= n; m++)
    {
        a[m] = T(1) / T(m + 1);
        for (int j = m; j; j--)
        {
            a[j - 1] = T(j) * (a[j - 1] - a[j]);
        }
        b[m] = a[0];
    }
    return b;
}

vector<Z> fastBern(int n)
{
    // 返回生成函数 x/(exp(x)-1) 定义的 B_0..B_n，采用 B_1=-1/2。
    assert(0 <= n && n < mod); // 调试检查，可删。
    const auto &iv = invTable<mod>(n + 1);
    Poly den(n + 1);
    Z ifact = 1;
    for (int i = 0; i <= n; i++)
    {
        ifact *= iv[i + 1];
        den[i] = ifact;
    }
    Poly series = fpsInvFast(den, n + 1);
    Z fact = 1;
    for (int i = 0; i <= n; i++)
    {
        if (i) fact *= i;
        series[i] *= fact;
    }
    return series;
}
