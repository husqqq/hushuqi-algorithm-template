#pragma once

#include "10_2_001.hpp"

Poly deriv(const Poly &a)
{
    // a 是输入多项式；返回形式导数。
    if (a.empty())
    {
        return {};
    }
    Poly b(a.size() - 1);
    for (int i = 1; i < (int)a.size(); i++)
    {
        b[i - 1] = a[i] * i;
    }
    return b;
}

Poly integr(const Poly &a)
{
    // a 是输入多项式；返回常数项为 0 的形式积分，次数必须小于 mod-1。
    assert(a.size() < mod); // 调试检查，可删。
    Poly b(a.size() + 1);
    const auto &iv = invTable<mod>(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        b[i + 1] = a[i] * iv[i + 1];
    }
    return b;
}

vector<Z> batchInv(const vector<Z> &a)
{
    // a 是允许含零的数组；返回逐项逆元，零位置仍为零。
    vector<Z> pre(a.size() + 1, 1);
    vector<Z> ans(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        pre[i + 1] = pre[i] * (a[i] == Z(0) ? Z(1) : a[i]);
    }
    Z suf = pre.back().inv();
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        if (a[i] != Z(0))
        {
            ans[i] = suf * pre[i];
            suf *= a[i];
        }
    }
    return ans;
}
