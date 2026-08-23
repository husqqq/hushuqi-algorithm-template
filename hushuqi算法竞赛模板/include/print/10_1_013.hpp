#pragma once

#include "10_1_011.hpp"
#include "10_2_005.hpp"

namespace setfps
{
template <class T> vector<T> compEgf(const vector<T> &c, const vector<T> &f)
{
    // c[k] 是外层 EGF 的第 k 个分子系数，f 是常数项为 0 的集合幂级数；返回 sum c[k]*f^k/k!。
    assert(!c.empty() && !f.empty() && has_single_bit(f.size()) && f[0] == T(0)); // 调试检查，可删。
    int n = c.size() - 1;
    assert(n <= 30 && (int)f.size() == (1LL << n)); // 调试检查，可删。
    vector<vector<vector<T>>> a(n + 1);
    for (int i = 0; i <= n; i++)
    {
        a[i].assign(i + 1, vector<T>(1LL << i));
        a[i][0][0] = c[n - i];
    }
    for (int v = 0; v < n; v++)
    {
        int z = 1LL << v;
        vector h(v + 1, vector<T>(z));
        for (int s = 0; s < z; s++)
        {
            h[popcount((unsigned long long)s)][s] = f[z + s];
        }
        for (auto &x : h)
        {
            fwt(x, BitOp::Or);
        }
        for (int i = v + 1; i <= n; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                copy_n(a[i][j].begin(), z, a[i][j].begin() + z);
            }
        }
        for (int i = v; i < n; i++)
        {
            for (int j = 0; j <= v; j++)
            {
                for (int k = 0; j + k <= i; k++)
                {
                    for (int s = 0; s < z; s++)
                    {
                        a[i + 1][j + k + 1][z + s] += h[j][s] * a[i][k][s];
                    }
                }
            }
        }
    }
    for (auto &x : a[n])
    {
        fwt(x, BitOp::Or, true);
    }
    vector<T> ans(f.size());
    for (int s = 0; s < (int)f.size(); s++)
    {
        ans[s] = a[n][popcount((unsigned long long)s)][s];
    }
    return ans;
}

template <class T> vector<T> transConv(const vector<T> &f, vector<T> x)
{
    // f 是固定右因子，x 是输出线性泛函；返回子集卷积算子关于 f 的转置作用结果。
    assert(!f.empty() && f.size() == x.size() && has_single_bit(f.size())); // 调试检查，可删。
    reverse(x.begin(), x.end());
    x = subsetConv(x, f);
    reverse(x.begin(), x.end());
    return x;
}

template <class T> vector<T> projEgf(const vector<T> &f, const vector<T> &w)
{
    // f 是常数项为 0 的集合幂级数，w 是线性泛函权重；返回 <w,f^k/k!>（0<=k<=n）。
    assert(!f.empty() && f.size() == w.size() && has_single_bit(f.size()) && f[0] == T(0)); // 调试检查，可删。
    int n = countr_zero((unsigned long long)f.size());
    vector<vector<T>> block(n);
    for (int j = 0; j < n; j++)
    {
        block[j] = vector<T>(f.begin() + (1LL << j), f.begin() + (2LL << j));
    }
    vector<T> dp = w;
    vector<T> ans(n + 1);
    ans[0] = w[0];
    for (int i = 0; i < n; i++)
    {
        vector<T> ndp(1LL << (n - i - 1));
        for (int j = 0; j < n - i; j++)
        {
            vector<T> x(dp.begin() + (1LL << j), dp.begin() + (2LL << j));
            x = transConv(block[j], move(x));
            for (int k = 0; k < (int)x.size(); k++)
            {
                ndp[k] += x[k];
            }
        }
        dp = move(ndp);
        ans[i + 1] = dp[0];
    }
    return ans;
}
}

template <class T> vector<T> setExp(const vector<T> &f)
{
    // f[0] 必须为 0；返回子集卷积意义下的 exp(f)。
    assert(!f.empty() && has_single_bit(f.size()) && f[0] == T(0)); // 调试检查，可删。
    int n = countr_zero((unsigned long long)f.size());
    vector<T> g(f.size());
    g[0] = T(1);
    for (int i = 0; i < n; i++)
    {
        int m = 1LL << i;
        vector<T> a(f.begin() + m, f.begin() + 2 * m);
        vector<T> b(g.begin(), g.begin() + m);
        a = subsetConv(a, b);
        copy(a.begin(), a.end(), g.begin() + m);
    }
    return g;
}

template <class T> vector<T> setLog(const vector<T> &g)
{
    // g[0] 必须为 1；返回子集卷积意义下的 log(g)。
    assert(!g.empty() && has_single_bit(g.size()) && g[0] == T(1)); // 调试检查，可删。
    int n = countr_zero((unsigned long long)g.size());
    vector<T> f(g.size());
    for (int i = n - 1; i >= 0; i--)
    {
        int m = 1LL << i;
        vector<T> a(g.begin() + m, g.begin() + 2 * m);
        vector<T> b(g.begin(), g.begin() + m);
        auto x = rankedZeta(a);
        auto y = rankedZeta(b);
        for (int s = 0; s < m; s++)
        {
            for (int d = 0; d <= i; d++)
            {
                for (int k = 0; k < d; k++)
                {
                    x(s, d) -= x(s, k) * y(s, d - k);
                }
            }
        }
        a = rankedMobius(move(x));
        copy(a.begin(), a.end(), f.begin() + m);
    }
    return f;
}

template <class T> vector<T> setCompose(const vector<T> &p, const vector<T> &f)
{
    // p[k] 是普通多项式系数，f 是集合幂级数；返回普通多项式 p(f)。
    assert(!f.empty() && has_single_bit(f.size())); // 调试检查，可删。
    int n = countr_zero((unsigned long long)f.size());
    if (p.empty())
    {
        return vector<T>(f.size());
    }
    vector<T> q = p;
    vector<T> c(n + 1);
    T x = f[0];
    for (int k = 0; k <= n && !q.empty(); k++)
    {
        T v{};
        for (auto it = q.rbegin(); it != q.rend(); ++it)
        {
            v = v * x + *it;
        }
        c[k] = v;
        for (int i = 1; i < (int)q.size(); i++)
        {
            q[i - 1] = q[i] * T(i);
        }
        q.pop_back();
    }
    vector<T> g = f;
    g[0] = T(0);
    return setfps::compEgf(c, g);
}

template <class T> vector<T> setPowerProj(const vector<T> &f, const vector<T> &w, int m)
{
    // f 是集合幂级数，w 是同长线性泛函权重，m 是输出项数；返回 <w,f^k>（0<=k<m）。
    // T 必须是交换域，且 1..m-1 在其中可逆。
    assert(m >= 0 && !f.empty() && f.size() == w.size() && has_single_bit(f.size())); // 调试检查，可删。
    if (m == 0)
    {
        return {};
    }
    T c = f[0];
    vector<T> g = f;
    g[0] = T(0);
    auto q = setfps::projEgf(g, w);
    vector<T> e(m, T(1));
    for (int i = 1; i < m; i++)
    {
        e[i] = e[i - 1] * c / T(i);
    }
    vector<T> ans(m);
    for (int i = 0; i < min(m, (int)q.size()); i++)
    {
        for (int j = 0; i + j < m; j++)
        {
            ans[i + j] += q[i] * e[j];
        }
    }
    T fac = T(1);
    for (int i = 0; i < m; i++)
    {
        if (i)
        {
            fac *= T(i);
        }
        ans[i] *= fac;
    }
    return ans;
}

vector<Z> sharpSubSum(const vector<int> &a, int t)
{
    // a 是全部正元素且每项不超过 t，t 是目标上界；返回各和 0..t 的子集计数。
    assert(t >= 0);
    vector<int> cnt(t + 1);
    for (int x : a)
    {
        assert(1 <= x && x <= t);
        cnt[x]++;
    }
    vector<Z> inv(t + 1), f(t + 1);
    if (t >= 1) inv[1] = 1;
    for (int i = 2; i <= t; i++) inv[i] = -Z(mod / i) * inv[mod % i];
    for (int x = 1; x <= t; x++)
    {
        for (int k = 1; x * k <= t; k++)
        {
            f[x * k] += Z(cnt[x]) * (k & 1 ? inv[k] : -inv[k]);
        }
    }
    return fpsExp(f, t + 1);
}
