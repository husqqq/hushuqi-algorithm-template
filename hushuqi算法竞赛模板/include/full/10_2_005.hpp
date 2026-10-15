#pragma once

#include "7_4_004.hpp"
#include "10_2_003.hpp"
#include "10_2_004.hpp"

Poly fpsLog(const Poly &a, int n)
{
    // a 的常数项必须为 1，n 是不超过 2^22 的截断长度；返回 ln(a) mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(!a.empty() && a[0] == Z(1)); // 调试检查，可删。
    Poly f = a.cut(n);
    return integr((deriv(f) * fpsInvFast(f, n)).cut(n - 1)).cut(n);
}

Poly fpsExpNewton(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 是 [0,2^22] 内的截断长度；用 Newton 迭代返回 exp(a) mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(a.empty() || a[0] == Z(0)); // 调试检查，可删。
    Poly g{1};
    for (int k = 2; k < 2 * n; k <<= 1)
    {
        Poly q = a.cut(k) - fpsLog(g, k);
        q[0] += 1;
        g = (g * q).cut(k);
    }
    return g.cut(n);
}

namespace rexp
{
constexpr int B = 16;
constexpr int S = 64;
constexpr int LIM = 1 << 20;

struct RExp
{
    int n; // 实际截断长度。
    int sz; // 补齐后的二次幂长度。
    NTT32 nt; // 预分配切片使用的公共二进制 NTT。
    vector<uint32_t> dg; // dg[i]=i*g[i]，是输入指数多项式的导数系数。
    vector<uint32_t> f; // 已经求出的 exp(g) 系数。
    vector<uint32_t> iv; // iv[i] 是 i 的模逆元，iv[0]=iv[1]=1。
    vector<uint32_t> t; // 当前层逆变换的临时数组。
    vector<uint32_t> wf; // 各层答案块的预分配频域工作区。
    vector<uint32_t> wg; // 各层导数块的预分配频域工作区。
    vector<uint64_t> sum; // 当前频点的延迟取模乘积和。

    static size_t wsize(int n)
    {
        // n 是当前 16 叉分治区间长度；返回各递归层频域块所需的 uint32_t 元素总数。
        size_t ans = 0;
        while (n > S)
        {
            ans += 2 * n;
            n /= B;
        }
        return ans;
    }

    RExp(const Poly &g, int m)
        : n(m), sz(bit_ceil((unsigned long long)max<int>(1, m))), nt(sz > S ? sz / 8 : 0),
          dg(sz), f(sz), iv(m, 1),
          t(sz > S ? sz / 8 : 0), wf(wsize(sz)), wg(wsize(sz)), sum(t.size())
    {
        // g 是常数项为 0 的输入 FPS，m 是实际截断长度；初始化在线卷积所需全部状态。
        // dg[i]=i*g[i]，使 exp(g)'=g'*exp(g) 成为在线卷积递推。
        for (int i = 1; i < min(m, (int)g.size()); i++)
        {
            dg[i] = (uint64_t)g[i].x * i % mod;
        }
        f[0] = 1;
        const auto &ziv = invTable<mod>(m - 1);
        for (int i = 1; i < m; i++)
        {
            iv[i] = ziv[i].val();
        }
    }

    void dfs(int l, int w, uint32_t *pf, uint32_t *pg)
    {
        // l、w 确定当前区间，pf、pg 是本层答案块与导数块的频域工作区；补全该区间答案，无返回值。
        if (w <= S)
        {
            int r = min(n, l + w);
            for (int i = l; i < r; i++)
            {
                uint64_t v = 0;
                for (int j = l; j < i; j++)
                {
                    v += (uint64_t)f[j] * dg[i - j];
                    if (((j - l) & 15) == 15)
                    {
                        v %= mod;
                    }
                }
                uint32_t x = f[i] + v % mod;
                if (x >= mod)
                {
                    x -= mod;
                }
                f[i] = (uint64_t)x * iv[i] % mod;
            }
            return;
        }

        int b = w / B;
        int len = 2 * b;
        array<uint32_t *, B - 1> tf, tg;
        for (int i = 0; i < B - 1; i++)
        {
            tf[i] = pf + i * len;
            tg[i] = pg + i * len;
        }

        // 卷积核只与下标差有关，同一层的所有结点复用这些频域块。
        if (l == 0)
        {
            for (int i = 0; i < B - 1 && (i + 1) * b < this->n; i++)
            {
                copy_n(dg.data() + i * b, len, tg[i]);
                nt.dft(tg[i], len);
            }
        }

        for (int i = 0; i < B && l + i * b < this->n; i++)
        {
            if (i)
            {
                fill(sum.begin(), sum.begin() + len, uint64_t(0));
                for (int j = 0; j < i; j++)
                {
                    for (int k = 0; k < len; k++)
                    {
                        sum[k] += (uint64_t)tf[j][k] * tg[i - j - 1][k];
                    }
                }
                for (int k = 0; k < len; k++)
                {
                    t[k] = sum[k] % mod;
                }
                nt.idft(t.data(), len);
                for (int k = 0; k < b && l + i * b + k < this->n; k++)
                {
                    uint32_t &x = f[l + i * b + k];
                    x += t[k + b];
                    if (x >= mod)
                    {
                        x -= mod;
                    }
                }
            }

            dfs(l + i * b, b, pf + 2 * w, pg + 2 * w);
            if (i + 1 < B && l + (i + 1) * b < this->n)
            {
                fill(tf[i], tf[i] + len, uint32_t(0));
                copy_n(f.data() + l + i * b, b, tf[i]);
                nt.dft(tf[i], len);
            }
        }
        fill(pf, pf + (B - 1) * len, uint32_t(0));
    }

    Poly run()
    {
        // 无参数；执行在线分治并返回输入 FPS 指数的前 n 项。
        dfs(0, sz, wf.data(), wg.data());
        Poly ans(n);
        for (int i = 0; i < n; i++)
        {
            ans[i].x = f[i];
        }
        return ans;
    }
};
}

Poly fpsExpRelax(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 不超过 2^20；用 16 叉在线卷积返回 exp(a) mod x^n。
    assert(n >= 0 && n <= rexp::LIM); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    assert(a.empty() || a[0] == Z(0)); // 调试检查，可删。
    return rexp::RExp(a, n).run();
}

Poly fpsExp(const Poly &a, int n)
{
    // a 的常数项必须为 0，n 是截断长度；默认使用在线分块算法，超长时回退 Newton 迭代。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n <= rexp::LIM)
    {
        return fpsExpRelax(a, n);
    }
    return fpsExpNewton(a, n);
}

optional<Poly> fpsSqrtAt(const Poly &a, int n, int p, Z s)
{
    // a[p] 是前 n 项中的首个非零系数，s 是它的一组平方根；返回对应 FPS 平方根，无解时返回空。
    assert(0 <= p && p < min(n, (int)a.size()) && a[p] != Z(0)); // 调试检查，可删。
    if ((p & 1) || s * s != a[p])
    {
        return nullopt;
    }
    int m = n - p;
    Poly b(a.begin() + p, a.begin() + min((int)a.size(), p + m));
    b.resize(m);
    Poly g{s};
    Z iv2 = Z(2).inv();
    for (int k = 2; k < 2 * m; k <<= 1)
    {
        g = ((g + (b.cut(k) * fpsInvFast(g, k)).cut(k)) * iv2).cut(k);
    }
    Poly ans(p / 2);
    g = g.cut(n - p / 2);
    ans.insert(ans.end(), g.begin(), g.end());
    return ans.cut(n);
}

optional<Poly> fpsSqrt(const Poly &a, int n, Z s)
{
    // a 是输入 FPS，n 是不超过 2^22 的截断长度，s 是首个非零系数的一组平方根；无平方根时返回空。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return Poly{};
    }
    int p = 0;
    while (p < min(n, (int)a.size()) && a[p] == Z(0))
    {
        p++;
    }
    if (p == min(n, (int)a.size()))
    {
        return Poly(n);
    }
    return fpsSqrtAt(a, n, p, s);
}

optional<Poly> fpsSqrt(const Poly &a, int n)
{
    // a 是输入 FPS，n 是不超过 2^22 的截断长度；自动求首个非零系数平方根，返回一组平方根或空。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return Poly{};
    }
    int p = 0;
    while (p < min(n, (int)a.size()) && a[p] == Z(0))
    {
        p++;
    }
    if (p == min(n, (int)a.size()))
    {
        return Poly(n);
    }
    if (p & 1)
    {
        return nullopt;
    }
    auto s = sqrtMod(a[p].val(), mod);
    if (!s)
    {
        return nullopt;
    }
    return fpsSqrtAt(a, n, p, Z(*s));
}

Poly fpsPow(const Poly &a, unsigned long long k, int n)
{
    // a 是输入 FPS，k 是非负指数，n 是不超过 2^22 的截断长度；返回 a^k mod x^n。
    assert(n >= 0 && n <= (1 << 22)); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    if (k == 0)
    {
        Poly ans(n);
        ans[0] = 1;
        return ans;
    }
    int lim = min(n, (int)a.size());
    int p = 0;
    while (p < lim && a[p] == Z(0))
    {
        p++;
    }
    if (p == lim || (p && k > (unsigned long long)(n - 1) / p))
    {
        return Poly(n);
    }
    Z c = a[p];
    int sh = p * k;
    int m = n - sh;
    Poly b(a.begin() + p, a.begin() + min((int)a.size(), p + m));
    b.resize(m);
    b *= c.inv();
    b = fpsExp(fpsLog(b, m) * Z(k % mod), m) * c.pow(k);
    Poly ans(sh);
    ans.insert(ans.end(), b.begin(), b.end());
    return ans.cut(n);
}
