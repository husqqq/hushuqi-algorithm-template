#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class F>
long double simpson(F f, long double l, long double r,
                            long double err = 1E-12L, int dep = 24)
{
    // f 是被积函数，[l,r] 是区间，err 是绝对误差目标，dep 是递归上限；返回定积分近似值。
    // 调试检查，可删。
    assert(err > 0 && dep >= 0);
    auto simpson = [](long double ql, long double qr, long double vl,
                      long double vm, long double vr)
    {
        // ql、qr 是端点，vl、vm、vr 是左端、中点、右端函数值；返回一次 Simpson 近似。
        return (vl + 4 * vm + vr) * (qr - ql) / 6;
    };
    long double m = (l + r) / 2;
    long double fl = f(l);
    long double fm = f(m);
    long double fr = f(r);
    auto dfs = [&](auto &&self, long double ql, long double qr, long double vl,
                   long double vm, long double vr, long double whole,
                   long double tol, int depth) -> long double
    {
        // self 是递归入口，whole 是当前段近似，tol 是误差预算，depth 是剩余层数；返回当前段积分值。
        long double mid = (ql + qr) / 2;
        long double x = (ql + mid) / 2;
        long double y = (mid + qr) / 2;
        long double fx = f(x);
        long double fy = f(y);
        long double a = simpson(ql, mid, vl, fx, vm);
        long double b = simpson(mid, qr, vm, fy, vr);
        if (!depth || fabsl(a + b - whole) <= 15 * tol)
        {
            return a + b + (a + b - whole) / 15;
        }
        return self(self, ql, mid, vl, fx, vm, a, tol / 2, depth - 1) +
               self(self, mid, qr, vm, fy, vr, b, tol / 2, depth - 1);
    };
    return dfs(dfs, l, r, fl, fm, fr, simpson(l, r, fl, fm, fr), err, dep);
}
