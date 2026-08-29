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

inline void tutteRanked(vector<Z> &a, int n, bool inverse)
{
    // a 按子集和次数压平存储，n 是位数；执行分层 Zeta 或 Mobius 变换。
    int N = 1 << n, w = n + 1;
    for (int bit = 0; bit < n; bit++) for (int s = 0; s < N; s++) if (s >> bit & 1)
    {
        Z *to = a.data() + (size_t)s * w;
        Z *from = a.data() + (size_t)(s ^ (1 << bit)) * w;
        for (int d = 0; d <= n; d++)
            if (inverse) to[d] -= from[d]; else to[d] += from[d];
    }
}

inline vector<Z> tutteSetExp(const vector<Z> &f, int n)
{
    // f 是常数项为 0 的 n 位集合幂级数；返回集合卷积意义下的 exp(f)。
    int N = 1 << n, w = n + 1;
    vector<Z> a((size_t)N * w);
    for (int s = 0; s < N; s++) a[(size_t)s * w + popcount((unsigned)s)] = f[s];
    tutteRanked(a, n, false);
    vector<Z> log(w);
    for (int s = 0; s < N; s++)
    {
        Z *v = a.data() + (size_t)s * w;
        copy(v, v + w, log.begin());
        v[0] = 1;
        for (int d = 1; d <= n; d++)
        {
            Z sum = 0;
            for (int k = 1; k <= d; k++) sum += Z(k) * log[k] * v[d - k];
            v[d] = sum / Z(d);
        }
    }
    tutteRanked(a, n, true);
    vector<Z> ans(N);
    for (int s = 0; s < N; s++) ans[s] = a[(size_t)s * w + popcount((unsigned)s)];
    return ans;
}

inline Z tutteEval(const vector<unsigned long long> &g, Z x, Z y)
{
    // g 是无自环无向图邻接位集；x、y 是 Tutte 多项式点值，点数不超过 20。
    int n = g.size(), N = 1 << n;
    assert(0 < n && n <= 20); // 调试检查，可删。
    vector<Z> connected(N), temp(N);
    vector<Z> geo(n + 1);
    for (int i = 1; i <= n; i++) geo[i] = geo[i - 1] * y + Z(1);
    for (int u = 0; u < n; u++)
    {
        int M = 1 << u;
        for (int s = 0; s < M; s++)
            temp[s] = connected[s] * geo[popcount(g[u] & (unsigned long long)s)];
        vector<Z> add(temp.begin(), temp.begin() + M);
        add = tutteSetExp(add, u);
        for (int s = 0; s < M; s++) connected[M | s] = add[s];
    }
    vector<char> vis(n), hasRoot(N);
    for (int start = 0; start < n; start++) if (!vis[start])
    {
        vector<int> q{start};
        vis[start] = 1;
        for (int h = 0; h < (int)q.size(); h++)
        {
            int u = q[h];
            for (int v = 0; v < n; v++) if ((g[u] >> v & 1) && !vis[v])
                vis[v] = 1, q.push_back(v);
        }
        for (int s = 0; s < N; s++) if (s >> start & 1) hasRoot[s] = 1;
    }
    for (int s = 1; s < N; s++) if (!hasRoot[s]) connected[s] *= x - Z(1);
    return tutteSetExp(connected, n).back();
}
