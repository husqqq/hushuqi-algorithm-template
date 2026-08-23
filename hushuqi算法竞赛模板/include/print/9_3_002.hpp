#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class F> long double romberg(F f, long double l, long double r, int levels = 15)
{
    // 调试检查，可删。
    assert(1 <= levels && levels <= 25);
    vector<long double> a(levels);
    a[0] = (f(l) + f(r)) * (r - l) / 2;
    for (int i = 1; i < levels; i++)
    {
        unsigned long long cnt = 1ULL << i;
        long double h = (r - l) / cnt;
        long double sum = 0;
        for (unsigned long long j = 1; j < cnt; j += 2)
        {
            sum += f(l + j * h);
        }
        a[i] = a[i - 1] / 2 + h * sum;
        long double p = 4;
        for (int j = i - 1; j >= 0; j--)
        {
            a[j] = (p * a[j + 1] - a[j]) / (p - 1);
            p *= 4;
        }
    }
    return a[0];
}
