#pragma once

#include "10_2_001.hpp"

Poly fpsInv(const Poly &a, int n)
{
    // a 是输入 FPS，n 是 [0,2^22] 内的截断长度；n>0 时 a[0] 必须非零，返回满足 a*g=1 mod x^n 的 g。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] != Z(0)); // 调试检查，可删。
    Poly g{a[0].inv()};
    for (int k = 2; k < 2 * n; k <<= 1)
    {
        g = (g * (Poly{2} - a.cut(k) * g)).cut(k);
    }
    return g.cut(n);
}

Poly fpsInvFast(const Poly &a, int n)
{
    // a 是输入 FPS，n 是 [0,2^23] 内的截断长度；n>0 时 a[0] 必须非零，返回满足 a*g=1 mod x^n 的快速逆元。
    assert(n >= 0 && n <= (1 << 23)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] != Z(0)); // 调试检查，可删。

    int m = min<int>(n, 64); // m 是当前已经求出的逆元项数。
    Poly g(m);
    g[0] = a[0].inv();
    for (int i = 1; i < m; i++)
    {
        Z sum = 0;
        for (int j = 1; j <= i && j < (int)a.size(); j++)
        {
            sum += a[j] * g[i - j];
        }
        g[i] = -sum * g[0];
    }

    vector<Z> fg; // fg 保存当前 g 的频域值，供本轮两次乘法复用。
    vector<Z> err; // err 依次保存 a、ag 的高半误差和最终校正乘积。
    int cap = bit_ceil((unsigned long long)n);
    fg.reserve(cap);
    err.reserve(cap);
    while (m < n)
    {
        int k = min(2 * m, n); // k 是本轮要求的逆元项数。
        int z = 2 * m; // z 是本轮循环卷积的 NTT 长度。
        fg.assign(z, Z(0));
        err.assign(z, Z(0));
        copy(g.begin(), g.end(), fg.begin());
        copy(a.begin(), a.begin() + min(k, (int)a.size()), err.begin());

        dft(fg);
        dft(err);
        for (int i = 0; i < z; i++)
        {
            err[i] *= fg[i];
        }
        idft(err);
        fill(err.begin(), err.begin() + m, Z(0));
        fill(err.begin() + k, err.end(), Z(0));

        dft(err);
        for (int i = 0; i < z; i++)
        {
            err[i] *= fg[i];
        }
        idft(err);

        g.resize(k);
        for (int i = m; i < k; i++)
        {
            g[i] = -err[i];
        }
        m = k;
    }
    return g;
}
