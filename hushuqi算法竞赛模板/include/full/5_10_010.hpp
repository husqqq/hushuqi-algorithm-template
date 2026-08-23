#pragma once

#include "8_2_011.hpp"

vector<array<Z, 21>> rankedZeta(vector<Z> f, int n)
{
    // f 是大小为 2^n 的子集函数；返回按子集大小分层后的子集 Zeta 变换。
    vector<array<Z, 21>> a(f.size());
    for (int s = 0; s < (int)f.size(); s++) a[s][popcount((unsigned)s)] = f[s];
    for (int i = 0; i < n; i++)
    {
        for (int s = 0; s < (1 << n); s++)
        {
            if (s >> i & 1)
            {
                for (int d = 0; d <= n; d++) a[s][d] += a[s ^ (1 << i)][d];
            }
        }
    }
    return a;
}

vector<Z> rankedMobius(vector<array<Z, 21>> a, int n)
{
    // a 是按子集大小分层后的 Zeta 值；返回对应的子集函数。
    for (int i = 0; i < n; i++)
    {
        for (int s = 0; s < (1 << n); s++)
        {
            if (s >> i & 1)
            {
                for (int d = 0; d <= n; d++) a[s][d] -= a[s ^ (1 << i)][d];
            }
        }
    }
    vector<Z> f(1 << n);
    for (int s = 0; s < (1 << n); s++) f[s] = a[s][popcount((unsigned)s)];
    return f;
}

vector<Z> subsetConv(const vector<Z> &a, const vector<Z> &b)
{
    // a、b 是相同 n 位全集上的子集函数；返回 (a*b)[S]=sum_{T subset S}a[T]b[S-T]。
    int n = countr_zero((unsigned)a.size());
    assert(a.size() == b.size() && n <= 20); // 调试检查，可删。
    auto x = rankedZeta(a, n), y = rankedZeta(b, n);
    for (int s = 0; s < (1 << n); s++)
    {
        for (int d = n; d >= 0; d--)
        {
            Z z = 0;
            for (int i = 0; i <= d; i++) z += x[s][i] * y[s][d - i];
            x[s][d] = z;
        }
    }
    return rankedMobius(move(x), n);
}

vector<Z> transSubConv(vector<Z> s, vector<Z> x)
{
    // s、x 是同一位数的子集函数；返回映射 a->a*s 对 x 的转置作用结果。
    reverse(x.begin(), x.end());
    x = subsetConv(x, s);
    reverse(x.begin(), x.end());
    return x;
}

vector<Z> powerProjEgf(vector<Z> wt, const vector<Z> &s, int n)
{
    // wt 是线性泛函权，s[0] 必须为 0；返回 sum_T wt[T](s^k/k!)[T]，k=0..n。
    vector<Z> ans(n + 1), dp = move(wt);
    ans[0] = dp[0];
    for (int i = 0; i < n; i++)
    {
        vector<Z> nxt(1 << (n - 1 - i));
        for (int j = 0; j < n - i; j++)
        {
            int l = 1 << j, r = 2 << j;
            vector<Z> a(s.begin() + l, s.begin() + r);
            vector<Z> b(dp.begin() + l, dp.begin() + r);
            b = transSubConv(move(a), move(b));
            for (int k = 0; k < (int)b.size(); k++) nxt[k] += b[k];
        }
        dp = move(nxt);
        ans[i + 1] = dp[0];
    }
    return ans;
}

vector<Z> chromPoly(const vector<unsigned long long> &g)
{
    // g 是允许自环的无向图邻接位集；返回按次数升序的色多项式系数，自环图返回全零，支持至多 20 点。
    int n = g.size(), N = 1 << n;
    assert(n <= 20 && n < Z::mod()); // 调试检查，可删：模数须为大于 n 的素数。
    for (int u = 0; u < n; u++)
    {
        if (g[u] >> u & 1) return vector<Z>(n + 1);
    }
    vector<char> ind(N, true);
    for (int s = 1; s < N; s++)
    {
        int u = countr_zero((unsigned)s);
        ind[s] = ind[s ^ (1 << u)] && !(g[u] & (s ^ (1 << u)));
    }
    vector<Z> f(N), wt(N);
    for (int s = 0; s < N; s++) if (ind[s]) f[s] = 1;
    wt[N - 1] = 1;
    Z c = f[0];
    f[0] -= c;
    vector<Z> egf = powerProjEgf(wt, f, n);
    vector<Z> val(n + 1), pw(n + 1), fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    for (int k = 0; k <= n; k++)
    {
        if (k == 0) pw[k] = 1;
        else pw[k] = pw[k - 1] * c;
        Z sum = 0;
        for (int i = 0; i <= k; i++) sum += egf[i] * pw[k - i] / fact[k - i];
        val[k] = sum * fact[k];
    }
    vector<Z> ans(n + 1), cur{1};
    Z fac = 1;
    for (int k = 0; k <= n; k++)
    {
        Z d = val[0] / fac;
        for (int i = 0; i < (int)cur.size(); i++)
        {
            ans[i] += d * cur[i];
        }
        for (int i = 0; i + 1 < (int)val.size(); i++)
        {
            val[i] = val[i + 1] - val[i];
        }
        val.pop_back();
        if (k == n)
        {
            break;
        }
        fac *= k + 1;
        vector<Z> nxt(cur.size() + 1);
        for (int i = 0; i < (int)cur.size(); i++)
        {
            nxt[i] -= cur[i] * k;
            nxt[i + 1] += cur[i];
        }
        cur.swap(nxt);
    }
    return ans;
}
