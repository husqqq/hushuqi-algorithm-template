#pragma once

#include "10_1_003.hpp"

vector<Z> fallMul(const vector<Z> &a, const vector<Z> &b)
{
    // a、b 是下降幂基系数；返回二者乘积的同基系数。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int n = a.size() + b.size() - 1;
    assert(n <= mod && convFit(n, n)); // 调试检查，可删。
    vector<Z> fac(n);
    vector<Z> ifac(n);
    vector<Z> e(n);
    fac[0] = 1;
    for (int i = 1; i < n; i++)
    {
        fac[i] = fac[i - 1] * i;
    }
    ifac[n - 1] = fac[n - 1].inv();
    for (int i = n - 1; i; i--)
    {
        ifac[i - 1] = ifac[i] * i;
    }
    vector<Z> x = conv(a, ifac);
    vector<Z> y = conv(b, ifac);
    x.resize(n);
    y.resize(n);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i] * fac[i];
    }
    for (int i = 0; i < n; i++)
    {
        e[i] = i & 1 ? -ifac[i] : ifac[i];
    }
    x = conv(x, e);
    x.resize(n);
    return x;
}
