// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 44 Tutte 多项式



#include <bits/stdc++.h>
using namespace std;
#define int long long

template <long long P> struct MInt
{
    static_assert(P > 1);
    using V = conditional_t<(P <= numeric_limits<int32_t>::max()),
                            uint32_t, unsigned long long>;
    static constexpr V M = (V)P; // 与 x 同宽的模数，避免热路径被 P 提升为有符号 64 位
    V x = 0; // 当前剩余类在 [0,P) 内的代表元；常见小模使用无符号 32 位

    MInt() = default;

    MInt(long long v)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= P;
        if (v < 0)
        {
            v += P;
        }
        x = (V)v;
    }

    static constexpr long long mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    long long val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? M - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += M - o.x;
        if (x >= M)
        {
            x -= M;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        if constexpr (P <= 2147483647LL)
        {
            x = (unsigned long long)x * o.x % M;
        }
        else
        {
            x = (unsigned __int128)x * o.x % M;
        }
        return *this;
    }

    MInt &operator/=(const MInt &o)
    {
        // o 是非零同模剩余类且 P 必须为素数；原地乘以 o 的逆元并返回当前对象引用。
        return *this *= o.inv();
    }

    friend MInt operator+(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a+b。
        return a += b;
    }

    friend MInt operator-(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a-b。
        return a -= b;
    }

    friend MInt operator*(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类；返回 a*b。
        return a *= b;
    }

    friend MInt operator/(MInt a, const MInt &b)
    {
        // a、b 是同模剩余类且 b 非零；返回 a/b。
        return a /= b;
    }

    friend bool operator==(const MInt &, const MInt &) = default;

    MInt pow(unsigned long long b) const
    {
        // b 是非负指数；返回当前剩余类的 b 次幂。
        MInt a = *this;
        MInt ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans *= a;
            }
            a *= a;
            b >>= 1;
        }
        return ans;
    }

    MInt inv() const
    {
        // 当前值必须非零且 P 必须为素数；返回乘法逆元。
        assert(x != 0); // 调试检查，可删
        return pow(P - 2);
    }
};

template <long long P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && n < P); // 调试检查，可删
    static vector<MInt<P>> iv{0, 1};
    int old = iv.size();
    if (old <= n)
    {
        iv.resize(n + 1);
        for (int i = old; i <= n; i++)
        {
            iv[i] = MInt<P>(0) - MInt<P>(P / i) * iv[P % i];
        }
    }
    return iv;
}

constexpr int mod = 998244353;
using Z = MInt<mod>;


Z detField(vector<vector<Z>> a)
{
    // a 是有限域上的方阵；返回其行列式。
    int n = a.size();
    for (const auto &row : a)
    {
        assert((int)row.size() == n); // 调试检查，可删。
    }
    Z ans = 1;
    for (int c = 0; c < n; c++)
    {
        int p = c;
        while (p < n && a[p][c] == Z(0))
        {
            p++;
        }
        if (p == n)
        {
            return 0;
        }
        if (p != c)
        {
            swap(a[p], a[c]);
            ans = -ans;
        }
        ans *= a[c][c];
        Z iv = a[c][c].inv();
        for (int i = c + 1; i < n; i++)
        {
            Z q = a[i][c] * iv;
            for (int j = c; j < n; j++)
            {
                a[i][j] -= q * a[c][j];
            }
        }
    }
    return ans;
}

Z spanTreeCnt(int n, const vector<pair<int, int>> &e)
{
    // n 是点数，e 是无向边端点列表且允许重边；返回生成树数量。
    assert(n >= 0); // 调试检查，可删。
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
    }
    if (n <= 1)
    {
        return 1;
    }
    vector a(n - 1, vector<Z>(n - 1));
    for (auto [u, v] : e)
    {
        if (u < n - 1)
        {
            a[u][u] += 1;
        }
        if (v < n - 1)
        {
            a[v][v] += 1;
        }
        if (u < n - 1 && v < n - 1)
        {
            a[u][v] -= 1;
            a[v][u] -= 1;
        }
    }
    return detField(move(a));
}

Z arboCount(int n, int root, const vector<pair<int, int>> &e)
{
    // n 是点数，root 是根，e 是有向边端点列表且允许重边；返回从 root 向外的生成树数量。
    assert(n >= 1 && 0 <= root && root < n); // 调试检查，可删。
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
    }
    if (n == 1)
    {
        return 1;
    }
    vector<int> id(n, -1);
    for (int u = 0, k = 0; u < n; u++)
    {
        if (u != root)
        {
            id[u] = k++;
        }
    }
    vector a(n - 1, vector<Z>(n - 1));
    for (auto [u, v] : e)
    {
        if (v == root)
        {
            continue;
        }
        a[id[v]][id[v]] += 1;
        if (u != root)
        {
            a[id[v]][id[u]] -= 1;
        }
    }
    return detField(move(a));
}


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
