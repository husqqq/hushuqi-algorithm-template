#pragma once

#include "10_2_005.hpp"
#include "10_2_011.hpp"

namespace lagrange
{
int lagrangeCoef(vector<int> phi, int n)
{
    // phi 是 phi(t) 的低次系数，n 为正；返回方程 w=x*phi(w) 中 [x^n]w。
    Poly f(n);
    for (int i = 0; i < min(n, (int)phi.size()); i++)
    {
        f[i] = phi[i];
    }
    return (fpsPow(f, n, n)[n - 1] / Z(n)).val();
}
}

namespace cinv
{
Poly powUnit(const Poly &f, Z k, int n)
{
    // f[0]=1，k 是模意义指数，n 是截断长度；返回 f^k mod x^n。
    if (n == 0)
    {
        return {};
    }
    return fpsExp(fpsLog(f, n) * k, n);
}
}

Poly compInverse(const Poly &f, int n)
{
    // f[0]=0、f[1]!=0，n 是截断项数；返回满足 f(g(x))=x mod x^n 的 g。
    if (n == 0)
    {
        return {};
    }
    if (n == 1)
    {
        return Poly(1);
    }

    Z a = f[1], ia = a.inv();
    Poly h = f.cut(n) * ia;
    int d = n - 1;
    vector<Z> w(d + 1);
    w[d] = 1;
    auto c = powerProj(h, w, d + 1);

    Poly q(d);
    for (int i = 1; i <= d; i++)
    {
        q[d - i] = Z(d) * c[i] / Z(i);
    }
    q = cinv::powUnit(q, -Z(d).inv(), d);

    Poly ans(n);
    Z pw = ia;
    for (int i = 1; i < n; i++)
    {
        ans[i] = q[i - 1] * pw;
        pw *= ia;
    }
    return ans;
}
