

#include <bits/stdc++.h>
using namespace std;
#define int long long

enum class BitOp
{
    Or,
    And,
    Xor
};

template <bool Inv = false, class T> void orFWT(T *a, int n, int d)
{
    // a 指向 n 行、每行 d 项的连续集合函数，n 是 2 的幂；沿行掩码做批量 OR 变换。
    assert(a && n > 0 && d > 0 && n <= UINT32_MAX && d <= UINT32_MAX &&
           has_single_bit((unsigned long long)n)); // 调试检查，可删。
    uint32_t m = n;
    uint32_t w = d;
    for (uint32_t h = 1; h < m; h <<= 1)
    {
        for (uint32_t l = 0; l < m; l += h << 1)
        {
            for (uint32_t s = 0; s < h; s++)
            {
                T *x = a + (size_t)(l + s) * w;
                T *y = x + (size_t)h * w;
                if constexpr (!Inv)
                {
                    for (uint32_t k = 0; k < w; k++)
                    {
                        y[k] += x[k];
                    }
                }
                else
                {
                    for (uint32_t k = 0; k < w; k++)
                    {
                        y[k] -= x[k];
                    }
                }
            }
        }
    }
}

template <class T> void orFWT(T *a, int n, int d, bool inv)
{
    // a、n、d 的含义同三参数版本；inv 指定正变换或逆变换。
    if (inv)
    {
        orFWT<true>(a, n, d);
    }
    else
    {
        orFWT<false>(a, n, d);
    }
}

template <bool Inv = false, class T> void orFWT(vector<T> &a)
{
    // a 是按掩码编号的一维集合函数；原地完成 OR 正变换或逆变换。
    assert(!a.empty() && a.size() <= UINT32_MAX && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = a.size();
    for (uint32_t h = 1; h < n; h <<= 1)
    {
        for (uint32_t i = 0; i < n; i += h << 1)
        {
            for (uint32_t j = 0; j < h; j++)
            {
                if constexpr (!Inv)
                {
                    a[i + j + h] += a[i + j];
                }
                else
                {
                    a[i + j + h] -= a[i + j];
                }
            }
        }
    }
}

template <class T> void fwt(vector<T> &a, BitOp op, bool inv = false)
{
    // a 是按掩码编号的系数，op 指定或、与或异或，inv 指定正变换或逆变换；原地完成变换。
    int n = a.size();
    assert(n > 0 && has_single_bit((unsigned long long)n)); // 调试检查，可删。
    if (op == BitOp::Or)
    {
        if (inv)
        {
            orFWT<true>(a);
        }
        else
        {
            orFWT<false>(a);
        }
        return;
    }
    for (int h = 1; h < n; h <<= 1)
    {
        for (int i = 0; i < n; i += h << 1)
        {
            for (int j = 0; j < h; j++)
            {
                T x = a[i + j];
                T y = a[i + j + h];
                if (op == BitOp::Xor)
                {
                    a[i + j] = x + y;
                    a[i + j + h] = x - y;
                }
                else
                {
                    if (!inv)
                    {
                        a[i + j] += y;
                    }
                    else
                    {
                        a[i + j] -= y;
                    }
                }
            }
        }
    }
    if (op == BitOp::Xor && inv)
    {
        for (T &x : a)
        {
            x /= T(n);
        }
    }
}

template <class T> vector<T> bitConv(vector<T> a, vector<T> b, BitOp op)
{
    // a、b 是等长的掩码系数，op 指定卷积下标使用或、与或异或；返回对应的按位卷积。
    assert(!a.empty() && a.size() == b.size()); // 调试检查，可删。
    fwt(a, op);
    fwt(b, op);
    for (int i = 0; i < (int)a.size(); i++)
    {
        a[i] *= b[i];
    }
    fwt(a, op, true);
    return a;
}

template <class T> struct Ranked
{
    int n = 0; // 集合变量数，掩码范围是 [0,2^n)。
    vector<T> a; // 按掩码优先连续保存，每个掩码依次存放 0..n 秩。

    explicit Ranked(int m = 0) : n(m)
    {
        // m 是集合变量数；构造全零秩表。
        assert(0 <= m && m <= 30); // 调试检查，可删。
        a.resize((1LL << m) * (m + 1));
    }

    T &operator()(int s, int k)
    {
        // s 是集合掩码，k 是秩；返回对应元素的可修改引用。
        return a[s * (n + 1) + k];
    }

    const T &operator()(int s, int k) const
    {
        // s 是集合掩码，k 是秩；返回对应元素的只读引用。
        return a[s * (n + 1) + k];
    }
};

template <class T> Ranked<T> rankedZeta(const vector<T> &a)
{
    // a 是长度为 2 的幂的集合函数；返回按子集大小分层后的子集 Zeta 变换。
    assert(!a.empty() && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = countr_zero((unsigned long long)a.size());
    uint32_t m = a.size();
    uint32_t d = n + 1;
    Ranked<T> r(n);
    for (uint32_t s = 0; s < m; s++)
    {
        r.a[(size_t)s * d + popcount(s)] = a[s];
    }
    orFWT<false>(r.a.data(), m, d);
    return r;
}

template <class T> vector<T> rankedMobius(Ranked<T> r)
{
    // r 是按子集大小分层的 Zeta 变换；返回每个集合对应秩层的 Möbius 逆变换系数。
    int n = r.n;
    assert(0 <= n && n <= 30); // 调试检查，可删。
    int m = 1LL << n;
    assert((int)r.a.size() == m * (n + 1)); // 调试检查，可删。
    orFWT<true>(r.a.data(), m, n + 1);
    vector<T> a(m);
    for (uint32_t s = 0; s < (uint32_t)m; s++)
    {
        a[s] = r.a[(size_t)s * (n + 1) + popcount(s)];
    }
    return a;
}

template <class T> vector<T> subsetConvolution(const vector<T> &a, const vector<T> &b)
{
    // a、b 是等长且长度为 2 的幂的集合函数；返回 c[S]=sum(T subset S) a[T]*b[S\T]。
    assert(a.size() == b.size() && !a.empty() && has_single_bit(a.size())); // 调试检查，可删。
    uint32_t n = countr_zero((unsigned long long)a.size());
    uint32_t m = a.size();
    uint32_t d = n + 1;
    auto f = rankedZeta(a);
    auto g = rankedZeta(b);
    for (uint32_t s = 0; s < m; s++)
    {
        T *x = f.a.data() + (size_t)s * d;
        const T *y = g.a.data() + (size_t)s * d;
        for (int32_t k = n; k >= 0; k--)
        {
            T v{};
            for (int32_t i = 0; i <= k; i++)
            {
                v += x[i] * y[k - i];
            }
            x[k] = v;
        }
    }
    return rankedMobius(move(f));
}

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
    x = subsetConvolution(x, f);
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
        a = subsetConvolution(a, b);
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

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    int m = 1LL << n;
    vector<Z> f(m);
    for (auto &x : f)
    {
        int v;
        cin >> v;
        x = v;
    }
    auto g = setExp(f);
    for (int i = 0; i < m; i++)
    {
        cout << g[i].val() << " \n"[i + 1 == m];
    }
    return 0;
}
