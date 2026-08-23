#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}

template <class T> struct WDSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;
    // w[x] 是 x 到父亲的势能差 val[x]-val[f[x]]。
    vector<T> w;

    WDSU(int n = 0)
    {
        // n 是元素个数；初始势能均为零。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        w.assign(n, T{});
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、累加到根的势能并返回根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        w[x] += w[p];
        return f[x];
    }

    bool merge(int x, int y, T d)
    {
        // 施加 val[y]-val[x]=d；已连通时返回该约束是否一致。
        int a = find(x), b = find(y);
        T v = d + w[x] - w[y];
        if (a == b)
        {
            return v == T{};
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            w[a] = -v;
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            w[b] = v;
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
        // x、y 必须连通；返回 val[y]-val[x]。
        assert(same(x, y)); // 调试检查，可删
        return w[y] - w[x];
    }
};

struct XorDSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s, xr;

    XorDSU(int n = 0)
    {
        // n 是元素个数；初始异或势能均为零。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
        xr.assign(n, 0);
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链、累积到根的异或值并返回根。
        if (x == f[x])
        {
            return x;
        }
        int p = f[x];
        f[x] = find(p);
        xr[x] ^= xr[p];
        return f[x];
    }

    bool merge(int x, int y, int w)
    {
        // 施加 val[x]^val[y]=w；已连通时返回该约束是否一致。
        int a = find(x), b = find(y);
        int v = xr[x] ^ xr[y] ^ w;
        if (a == b)
        {
            return v == 0;
        }
        if (s[a] < s[b])
        {
            f[a] = b;
            xr[a] = v;
            s[b] += s[a];
        }
        else
        {
            f[b] = a;
            xr[b] = v;
            s[a] += s[b];
        }
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    int diff(int x, int y)
    {
        // x、y 必须连通；返回 val[x]^val[y]。
        assert(same(x, y)); // 调试检查，可删
        return xr[x] ^ xr[y];
    }

    int value(int x)
    {
        // x 是元素编号；把当前根的值视为零，返回 val[x]。
        find(x);
        return xr[x];
    }
};
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

    int n, q;
    cin >> n >> q;
    WDSU<Z> st(n);
    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0)
        {
            int x;
            cin >> x;
            cout << st.merge(v, u, Z(x)) << '\n';
        }
        else if (!st.same(u, v))
        {
            cout << -1 << '\n';
        }
        else
        {
            cout << st.diff(v, u).val() << '\n';
        }
    }
    return 0;
}
