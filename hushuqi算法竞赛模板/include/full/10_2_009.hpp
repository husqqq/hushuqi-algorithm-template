#pragma once

#include "10_2_004.hpp"

Poly taylorShift(const Poly &a, Z c)
{
    // a 保存 f(x) 的系数，c 是平移量；返回 f(x+c) 的系数。
    int n = a.size();
    if (n == 0)
    {
        return {};
    }
    assert(n <= mod && convFit(n, n)); // 调试检查，可删。
    vector<Z> fac(n, 1);
    vector<Z> ifac(n, 1);
    for (int i = 1; i < n; i++)
    {
        fac[i] = fac[i - 1] * i;
    }
    ifac[n - 1] = fac[n - 1].inv();
    for (int i = n - 1; i; i--)
    {
        ifac[i - 1] = ifac[i] * i;
    }
    Poly x(n);
    Poly y(n);
    Z pw = 1;
    for (int i = 0; i < n; i++)
    {
        x[n - 1 - i] = a[i] * fac[i];
        y[i] = pw * ifac[i];
        pw *= c;
    }
    Poly z = x * y;
    Poly ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = z[n - 1 - i] * ifac[i];
    }
    return ans;
}

vector<Z> shiftSamples(const vector<Z> &y, int c, int m)
{
    // y[i]=f(i)，c 是首个整数点，m 是输出长度；返回模 mod 下 f(c)..f(c+m-1)。
    int n = y.size();
    assert(n > 0 && n < mod && 0 <= m && m <= mod); // 调试检查，可删。
    if (m == 0)
    {
        return {};
    }
    assert(convFit(n, n + m - 1)); // 调试检查，可删。
    vector<Z> fac(n + 1, 1);
    vector<Z> ifac(n + 1, 1);
    for (int i = 1; i <= n; i++)
    {
        fac[i] = fac[i - 1] * i;
    }
    ifac[n] = fac[n].inv();
    for (int i = n; i; i--)
    {
        ifac[i - 1] = ifac[i] * i;
    }
    vector<Z> a(n);
    vector<Z> den(n + m - 1);
    vector<int> xs(m);
    for (int i = 0; i < n; i++)
    {
        a[i] = y[i] * ifac[i] * ifac[n - 1 - i];
        if ((n - 1 - i) & 1)
        {
            a[i] = -a[i];
        }
    }
    Z cur = Z(c) - Z(n - 1);
    for (auto &x : den)
    {
        x = cur;
        cur += 1;
    }
    cur = c;
    for (auto &x : xs)
    {
        x = cur.val();
        cur += 1;
    }
    auto iv = batchInv(den);
    auto sum = conv(a, iv);
    vector<Z> step(m);
    for (int i = 0; i < m; i++)
    {
        int x = xs[i];
        if (x >= n)
        {
            step[i] = x - n;
        }
    }
    auto istep = batchInv(step);
    vector<Z> ans(m);
    Z fall = 0;
    int pre = -2;
    for (int i = 0; i < m; i++)
    {
        int x = xs[i];
        if (x < n)
        {
            ans[i] = y[x];
            pre = x;
            continue;
        }
        if (pre >= n && x == pre + 1)
        {
            fall *= Z(x) * istep[i];
        }
        else
        {
            fall = 1;
            for (int j = 0; j < n; j++)
            {
                fall *= x - j;
            }
        }
        ans[i] = fall * sum[i + n - 1];
        pre = x;
    }
    return ans;
}
