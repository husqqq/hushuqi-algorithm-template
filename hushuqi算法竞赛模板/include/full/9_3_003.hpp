#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class F, class G>
optional<long double> newton(F f, G df, long double x, int it = 80)
{
    // f、df 是函数及导数，x 是初值，it 是迭代上限；收敛时返回一个根，失败时返回空。
    constexpr long double eps = 1E-12L;
    // 调试检查，可删。
    assert(it >= 0);
    for (int i = 0; i < it; i++)
    {
        long double d = df(x);
        if (d == 0 || !isfinite(d))
        {
            return nullopt;
        }
        long double y = x - f(x) / d;
        if (!isfinite(y))
        {
            return nullopt;
        }
        if (fabsl(y - x) <= eps * max(1.L, fabsl(y)))
        {
            return y;
        }
        x = y;
    }
    return nullopt;
}
