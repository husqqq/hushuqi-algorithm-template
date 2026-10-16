
#include <bits/stdc++.h>
using namespace std;
#define int long long



#include <limits>

using namespace std;

constexpr long long INF = numeric_limits<long long>::max() / 4;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更小则更新 a 并返回 true。
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更大则更新 a 并返回 true。
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

constexpr int inf = 1E9;
constexpr long double eps = 1E-12L;

template <class Group> struct PotentialDSU
{
    using T = typename Group::T;

    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;
    // pot[x] 满足 value[x]=value[f[x]]*pot[x]；根的 pot 是单位元。
    vector<T> pot;

    PotentialDSU(int n = 0)
    {
        // n 是元素个数；初始时每个元素自成集合且势能为群单位元。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        pot.assign(n, Group::id());
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、更新到根的右势能并返回集合根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        pot[x] = Group::op(pot[p], pot[x]);
        return f[x];
    }

    bool merge(int x, int y, const T &d)
    {
        // x、y 是元素编号，d 是右势能差；施加 value[x]*d=value[y]，返回约束是否相容。
        int a = find(x), b = find(y);
        if (a == b)
        {
            return Group::op(Group::inv(pot[x]), pot[y]) == d;
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            pot[a] = Group::op(Group::op(pot[y], Group::inv(d)), Group::inv(pot[x]));
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            pot[b] = Group::op(Group::op(pot[x], d), Group::inv(pot[y]));
            s[a] += s[b];
        }
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    T diff(int x, int y)
    {
        // x、y 必须连通；返回 value[x] 的逆乘 value[y]，即 value[x]^{-1}*value[y]。
        int a = find(x), b = find(y); // 必须更新到根的右势能，不能藏在断言中。
        assert(a == b); // 调试检查，可删
        return Group::op(Group::inv(pot[x]), pot[y]);
    }

    int size(int x)
    {
        // x 是元素编号；返回 x 所在集合的元素个数。
        return s[find(x)];
    }
};
#ifndef HUSHUQI_STATIC_MINT_HPP
#define HUSHUQI_STATIC_MINT_HPP
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <uint32_t P> struct MInt
{
    static_assert(1 < P && P <= 1000000007);
    uint32_t x = 0; // 固定 32 位以保证对象为 4 字节，提高大数组的缓存密度。

    MInt(long long v = 0)
    {
        // v 是要转入模 P 剩余类的整数；构造其最小非负代表元。
        v %= (long long)P;
        if (v < 0)
        {
            v += P;
        }
        x = (uint32_t)v;
    }

    static constexpr uint32_t mod()
    {
        // 无参数；返回编译期模数 P。
        return P;
    }

    uint32_t val() const
    {
        // 无参数；返回当前剩余类的最小非负代表元。
        return x;
    }

    MInt operator-() const
    {
        // 无参数；返回当前剩余类的加法逆元。
        MInt ans;
        ans.x = x ? P - x : 0;
        return ans;
    }

    MInt &operator+=(const MInt &o)
    {
        // o 是要加到当前值上的同模剩余类；原地完成模加并返回当前对象引用。
        x += o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator-=(const MInt &o)
    {
        // o 是要从当前值减去的同模剩余类；原地完成模减并返回当前对象引用。
        x += P - o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    MInt &operator*=(const MInt &o)
    {
        // o 是要乘到当前值上的同模剩余类；原地完成模乘并返回当前对象引用。
        // 乘积可能超过 32 位，显式提升到无符号 64 位。
        x = (uint64_t)x * o.x % P;
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

    MInt pow(uint64_t b) const
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

template <uint32_t P> const vector<MInt<P>> &invTable(int n)
{
    // n 是需要的最大下标且 0<=n<P；返回至少覆盖 0..n 的共享模逆元表，其中位置 0 为 0。
    assert(0 <= n && (uint64_t)n < P); // 调试检查，可删
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

constexpr uint32_t mod = 998244353;
using Z = MInt<mod>;

#endif

struct MatrixGroup
{
    using T = array<array<Z, 2>, 2>; // T[i][j] 是第 i 行第 j 列元素；矩阵行列式为一。

    static T id()
    {
        // 无参数；返回单位矩阵。
        return {{{1, 0}, {0, 1}}};
    }

    static T op(const T &a, const T &b)
    {
        // a、b 是同域二阶矩阵；返回按 a*b 顺序相乘的矩阵。
        T c{};
        for (int i = 0; i < 2; ++i)
        {
            for (int k = 0; k < 2; ++k)
            {
                for (int j = 0; j < 2; ++j)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }

    static T inv(const T &a)
    {
        // a 的行列式为一；返回它的逆矩阵，无需再除以行列式。
        return {{{a[1][1], -a[0][1]}, {-a[1][0], a[0][0]}}};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    PotentialDSU<MatrixGroup> st(n);
    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0)
        {
            MatrixGroup::T x;
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    int z;
                    cin >> z;
                    x[i][j] = Z(z);
                }
            }
            cout << st.merge(v, u, x) << '\n';
        }
        else if (!st.same(u, v))
        {
            cout << -1 << '\n';
        }
        else
        {
            auto x = st.diff(v, u);
            cout << x[0][0].val() << ' ' << x[0][1].val() << ' '
                 << x[1][0].val() << ' ' << x[1][1].val() << '\n';
        }
    }
    return 0;
}
