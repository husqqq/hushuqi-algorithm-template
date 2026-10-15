#pragma once

#include "10_2_009.hpp"

vector<Z> triPow(Z r, int n)
{
    // r 是公比，n 是非负长度；返回 a[i]=r^(i(i-1)/2) 的前 n 项。
    assert(n >= 0); // 调试检查，可删。
    vector<Z> a(n);
    if (!n)
    {
        return a;
    }
    a[0] = 1;
    Z pw = 1;
    for (int i = 0; i + 1 < n; i++)
    {
        a[i + 1] = a[i] * pw;
        pw *= r;
    }
    return a;
}

vector<Z> geomEval(vector<Z> f, Z a, Z r, int m)
{
    // f 是低次在前的多项式，a、r 给出点 a*r^i，m 是点数；返回依次对应的 m 个点值。
    assert(m >= 0);
    if (m == 0)
    {
        return {};
    }
    int n = f.size();
    if (n == 0)
    {
        return vector<Z>(m);
    }
    if (r == Z(0))
    {
        vector<Z> ans(m, f[0]);
        Z x = 1, y = 0;
        for (auto c : f)
        {
            y += c * x;
            x *= a;
        }
        ans[0] = y;
        return ans;
    }
    assert(convFit(n, n + m - 1)); // 调试检查，可删。
    Z pw = 1;
    for (auto &x : f)
    {
        x *= pw;
        pw *= a;
    }
    auto p = triPow(r, n + m - 1);
    auto ip = triPow(r.inv(), max(n, m));
    for (int i = 0; i < n; i++)
    {
        f[i] *= ip[i];
    }
    reverse(f.begin(), f.end());
    auto z = conv(f, p);
    vector<Z> ans(m);
    for (int i = 0; i < m; i++)
    {
        ans[i] = z[i + n - 1] * ip[i];
    }
    return ans;
}

vector<Z> geomInterp(vector<Z> y, Z a, Z r)
{
    // y[i]=f(a*r^i)，a、r 非零且 r^i!=1(1<=i<y.size())；返回次数小于 y.size() 的 f。
    int n = y.size();
    if (n <= 1)
    {
        return y;
    }
    assert(a != Z(0) && r != Z(0));
    assert(nttFit(n, n)); // 调试检查，可删。
    vector<Z> pw(2 * n - 1);
    pw[0] = 1;
    Z ir = r.inv();
    for (int i = 1; i < 2 * n - 1; i++)
    {
        pw[i] = pw[i - 1] * r;
    }
    auto tpw = triPow(r, 2 * n - 1);
    auto itpw = triPow(ir, 2 * n - 1);
    vector<Z> s(n, 1);
    for (int i = 1; i < n; i++)
    {
        assert(pw[i] != Z(1));
        s[i] = s[i - 1] * (Z(1) - pw[i]);
    }
    auto is = batchInv(s);
    Z sn = s[n - 1] * (Z(1) - pw[n]);
    for (int i = 0; i < n; i++)
    {
        y[i] *= tpw[n - 1 - i] * itpw[n - 1] * is[i] * is[n - 1 - i];
        if (i & 1)
        {
            y[i] = -y[i];
        }
        y[i] *= itpw[i];
    }
    auto f = midProd(Poly(tpw), Poly(y));
    for (int i = 0; i < n; i++)
    {
        f[i] *= itpw[i];
    }
    vector<Z> g(n);
    g[0] = 1;
    for (int i = 1; i < n; i++)
    {
        g[i] = tpw[i] * sn * is[i] * is[n - i];
        if (i & 1)
        {
            g[i] = -g[i];
        }
    }
    f = conv(f, g);
    f.resize(n);
    reverse(f.begin(), f.end());
    Z ia = a.inv();
    Z ap = 1;
    for (auto &x : f)
    {
        x *= ap;
        ap *= ia;
    }
    return f;
}
