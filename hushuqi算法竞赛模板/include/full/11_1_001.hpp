#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

using Real = long double;
constexpr Real eps = 1E-12L;

template <class T> int sgn(T x)
{
    // x 是整数；返回 -1、0、1 表示负、零、正。
    return (x > 0) - (x < 0);
}

int sgn(Real x)
{
    // x 表示待判断的浮点数；返回 -1、0、1 表示负、近似零、正。
    Real t = eps * max<Real>(1, fabsl(x));
    return (x > t) - (x < -t);
}

template <class T> bool eq(T x, T y)
{
    // x、y 是整数；返回二者是否相等。
    return x == y;
}

bool eq(Real x, Real y)
{
    // x、y 表示两个浮点数；使用相对误差判断它们是否近似相等。
    return fabsl(x - y) <= eps * max<Real>(1, max(fabsl(x), fabsl(y)));
}
