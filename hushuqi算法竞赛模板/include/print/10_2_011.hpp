#pragma once

#include "10_1_005.hpp"
#include "10_2_003.hpp"

vector<Z> powerProj(const Poly &f, const vector<Z> &w, int m)
{
    // f 是输入多项式，w[i] 是线性泛函在 x^i 上的值，m 是输出项数；返回 L(f^k)（0<=k<m）。
    assert(m >= 0 && m <= (1 << 22) && w.size() <= (1 << 20)); // 调试检查，可删。
    if (m == 0)
    {
        return {};
    }
    int n = w.size();
    vector<Z> ans(m);
    if (n == 0)
    {
        return ans;
    }
    if (m == 1)
    {
        return {w[0]};
    }
    if (n <= 32 && m <= 32)
    {
        Poly p{1};
        Poly ff = f.cut(n);
        for (int k = 0; k < m; k++)
        {
            for (int i = 0; i < min(n, (int)p.size()); i++)
            {
                ans[k] += w[i] * p[i];
            }
            p = (p * ff).cut(n);
        }
        return ans;
    }

    // 将 L(h)=sum w[i]h[i] 改写为 [x^(n-1)] reverse(w)*h，
    // 再用 Bostan-Mori 递归同时求出多个 [x^(n-1)] f^k。
    int k = n - 1;
    Poly ff = f.cut(n);
    poly2::Mat p(n, 2), q(n, 2);
    q(0, 0) = 1;
    for (int i = 0; i < n; i++)
    {
        p(i, 0) = w[n - 1 - i];
        q(i, 1) = -ff[i];
    }
    while (k > 0)
    {
        auto r = q;
        for (int i = 1; i < r.n; i += 2)
        {
            for (int j = 0; j < r.m; j++)
            {
                r(i, j) = -r(i, j);
            }
        }
        auto z = poly2::conv(p, q, r);
        int rn = k / 2 + 1;
        int cm = min(m, z[0].m);
        poly2::Mat np(rn, cm), nq(rn, cm);
        int o = k & 1;
        for (int i = 0; i < rn; i++)
        {
            int pi = 2 * i + o;
            int qi = 2 * i;
            for (int j = 0; j < cm; j++)
            {
                np(i, j) = z[0](pi, j);
                nq(i, j) = z[1](qi, j);
            }
        }
        p = move(np);
        q = move(nq);
        k /= 2;
    }
    Poly a(m), b(m);
    for (int i = 0; i < min(m, p.m); i++)
    {
        a[i] = p(0, i);
        b[i] = q(0, i);
    }
    return (a * fpsInvFast(b, m)).cut(m);
}
