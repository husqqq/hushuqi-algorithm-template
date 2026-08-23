#pragma once

#include "10_2_004.hpp"
#include "10_2_007.hpp"

Z lagrangeSeq(const vector<Z> &y, long long x)
{
    // y[i]=f(i) 且 deg(f)<y.size()，x 是求值点；返回 f(x)。
    int n = y.size();
    assert(n > 0 && n <= mod); // 调试检查，可删。
    if (0 <= x && x < n)
    {
        return y[x];
    }
    vector<Z> pre(n + 1, 1);
    vector<Z> suf(n + 1, 1);
    vector<Z> fac(n + 1, 1);
    Z p = x;
    for (int i = 0; i < n; i++)
    {
        pre[i + 1] = pre[i] * (p - Z(i));
        fac[i + 1] = fac[i] * Z(i + 1);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        suf[i] = suf[i + 1] * (p - Z(i));
    }
    Z ans = 0;
    for (int i = 0; i < n; i++)
    {
        Z c = pre[i] * suf[i + 1] / fac[i] / fac[n - 1 - i];
        if ((n - 1 - i) & 1)
        {
            c = -c;
        }
        ans += y[i] * c;
    }
    return ans;
}

Poly interpPoints(const vector<Z> &x, const vector<Z> &y)
{
    // x 是互不相同的横坐标，y 是对应点值；返回次数小于 x.size() 的唯一插值多项式。
    assert(x.size() == y.size()); // 调试检查，可删。
    if (x.empty())
    {
        return {};
    }
    PolyTree tr(x);
    auto d = tr.eval(deriv(tr.t[1]));
    auto iv = batchInv(d);
    vector<Z> w(tr.n);
    for (int i = 0; i < tr.n; i++)
    {
        assert(d[i] != Z(0)); // 调试检查，可删。
        w[i] = y[i] * iv[i];
    }
    auto dfs = [&](auto &&self, int p) -> Poly
    {
        if (p >= tr.z)
        {
            int i = p - tr.z;
            return i < tr.n ? Poly{w[i]} : Poly{};
        }
        Poly left = self(self, p << 1);
        Poly right = self(self, p << 1 | 1);
        return left * tr.t[p << 1 | 1] + right * tr.t[p << 1];
    };
    return dfs(dfs, 1).cut(tr.n);
}
