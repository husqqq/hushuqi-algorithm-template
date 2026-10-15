#pragma once

#include "10_2_009.hpp"

namespace comp
{
void kl(vector<Z> &p, vector<Z> q, int b, int w)
{
    // p 是待更新分子，q 是当前分母，b、w 是二维块数与块宽；原地完成 Kinoshita-Li 递归消元。
    // b*w 保持不变；公共 dft 的相邻配对与半长递归排列保证 q[2i]q[2i+1] 可直接作为半长频域值。
    assert((int)p.size() == 2 * b * w); // 调试检查，可删。
    assert((int)q.size() == 2 * b * w); // 调试检查，可删。
    if (w == 1)
    {
        return;
    }

    q.resize(4 * b * w);
    q[2 * b * w] = 1;
    dft(q);
    if (w > 2)
    {
        vector<Z> nq(2 * b * w);
        for (int i = 0; i < 4 * b * w; i += 2)
        {
            nq[i / 2] = q[i] * q[i + 1];
        }
        idft(nq);
        assert(nq[0] == Z(1)); // 调试检查，可删。
        nq[0] = 0;
        for (int i = 0; i < 2 * b; i++)
        {
            fill(nq.begin() + i * w + w / 2, nq.begin() + (i + 1) * w, Z(0));
        }
        kl(p, move(nq), 2 * b, w / 2);
    }

    dft(p);
    for (int i = 0; i < 4 * b * w; i += 2)
    {
        Z x = q[i];
        q[i] = p[i / 2] * q[i + 1];
        q[i + 1] = p[i / 2] * x;
    }
    idft(q);
    for (int i = 0; i < b; i++)
    {
        auto dst = p.begin() + 2 * i * w;
        auto src = q.begin() + 2 * (i + b) * w;
        copy_n(src, w, dst);
        fill(dst + w, dst + 2 * w, Z(0));
    }
}

Poly zero(const Poly &f, const Poly &g, int n)
{
    // g[0]=0；用 Kinoshita-Li 算法返回 f(g(x)) mod x^n。
    assert(n > 0 && (g.empty() || g[0] == Z(0))); // 调试检查，可删。
    int s = bit_ceil((unsigned long long)n);
    assert(s <= (1 << 21)); // 调试检查，可删；递归中的最大 NTT 长度为 4*s。
    vector<Z> p(2 * s), q(2 * s);
    for (int i = 0; i < min({n, s, (int)f.size()}); i++)
    {
        p[2 * i] = f[i];
    }
    for (int i = 0; i < min({n, s, (int)g.size()}); i++)
    {
        q[i] = -g[i];
    }
    kl(p, move(q), 1, s);
    Poly ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = p[i];
    }
    return ans;
}
}

Poly composePoly(const Poly &f, const Poly &g, int n)
{
    // f 是外层多项式，g 是内层多项式，n 是不超过 2^21 的截断项数；返回 f(g(x)) mod x^n。
    assert(n >= 0 && n <= (1 << 21)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    if (f.empty())
    {
        return {};
    }
    Z c = g.empty() ? Z(0) : g[0];
    Poly b = g.cut(n);
    b[0] -= c;
    if (all_of(b.begin(), b.end(), [](Z x) { return x == Z(0); }))
    {
        Z v = 0;
        for (auto it = f.rbegin(); it != f.rend(); ++it)
        {
            v = v * c + *it;
        }
        Poly ans{v};
        ans.norm();
        return ans;
    }
    if (c != Z(0))
    {
        assert((int)f.size() <= mod && convFit(f.size(), f.size())); // 调试检查，可删。
    }
    Poly a = c == Z(0) ? f : taylorShift(f, c);
    a.resize(min(n, (int)a.size()));
    if (n <= 32)
    {
        Poly ans(n), p{1};
        for (int k = 0; k < (int)a.size(); k++)
        {
            for (int i = 0; i < (int)p.size(); i++)
            {
                ans[i] += a[k] * p[i];
            }
            p = (p * b).cut(n);
        }
        ans.norm();
        return ans;
    }
    Poly ans = comp::zero(a, b, n);
    ans.norm();
    return ans;
}
