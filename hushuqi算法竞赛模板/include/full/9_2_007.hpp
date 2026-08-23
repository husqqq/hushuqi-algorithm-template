#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T>
optional<vector<T>> tridiagonal(vector<T> a, vector<T> b, vector<T> c, vector<T> d)
{
    int n = b.size();
    // 调试检查，可删。
    assert((int)a.size() == n && (int)c.size() == n && (int)d.size() == n);
    if (n == 0)
    {
        return vector<T>{};
    }
    for (int i = 1; i < n; i++)
    {
        if (b[i - 1] == T(0))
        {
            return nullopt;
        }
        T q = a[i] / b[i - 1];
        b[i] -= q * c[i - 1];
        d[i] -= q * d[i - 1];
    }
    if (b[n - 1] == T(0))
    {
        return nullopt;
    }
    vector<T> x(n);
    x[n - 1] = d[n - 1] / b[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        if (b[i] == T(0))
        {
            return nullopt;
        }
        x[i] = (d[i] - c[i] * x[i + 1]) / b[i];
    }
    return x;
}

template <class T>
optional<vector<T>> cyclicTri(vector<T> a, vector<T> b, vector<T> c,
                                       T tr, T bl, const vector<T> &d)
{
    int n = b.size();
    // 调试检查，可删。
    assert(n >= 3);
    // 调试检查，可删。
    assert((int)a.size() == n && (int)c.size() == n && (int)d.size() == n);
    T g = b[0] == T(0) ? T(1) : -b[0];
    b[0] -= g;
    b[n - 1] -= tr * bl / g;
    vector<T> u(n);
    u[0] = g;
    u[n - 1] = bl;
    auto x = tridiagonal(a, b, c, d);
    auto z = tridiagonal(a, b, c, u);
    if (!x || !z)
    {
        return nullopt;
    }
    T den = T(1) + (*z)[0] + tr * (*z)[n - 1] / g;
    if (den == T(0))
    {
        return nullopt;
    }
    T q = ((*x)[0] + tr * (*x)[n - 1] / g) / den;
    for (int i = 0; i < n; i++)
    {
        (*x)[i] -= q * (*z)[i];
    }
    return x;
}
