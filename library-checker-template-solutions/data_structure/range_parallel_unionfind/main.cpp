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

struct DSU
{
    // f[x] 是 x 的父亲；s[x] 仅在 x 为根时表示集合大小。
    vector<int> f, s;

    DSU(int n = 0)
    {
        // n 是元素个数；初始时每个元素自成集合。
        init(n);
    }

    void init(int n)
    {
        // n 是新的元素个数；清空旧状态并重建单点集合。
        assert(n >= 0); // 调试检查，可删
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
    }

    int find(int x)
    {
        // x 是元素编号；压缩父链并返回集合根。
        return x == f[x] ? x : f[x] = find(f[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是元素编号；合并两集合并返回是否真正发生合并。
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return false;
        }
        if (s[x] < s[y])
        {
            swap(x, y);
        }
        f[y] = x;
        s[x] += s[y];
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否同属一个集合。
        return find(x) == find(y);
    }

    int size(int x)
    {
        // x 是元素编号；返回其集合大小。
        return s[find(x)];
    }
};

struct RangeDSU
{
    struct D
    {
        // f[x]<0 表示 x 是块集合根且大小为 -f[x]；否则 f[x] 是父亲。
        // 性能头压缩父亲和负大小；元素数须在有符号 32 位范围内。
        vector<int32_t> f;
        D(int n = 0) : f(n, -1)
        {
            // n 是本层可作为块起点的位置数。
        }
        int find(int x)
        {
            // x 是本层块起点；压缩父链并返回块集合根。
            return f[x] < 0 ? x : f[x] = find(f[x]);
        }
        pair<int, int> merge(int x, int y)
        {
            // x、y 是本层块起点；返回 {新根,被吸收根}，已经同集时返回 {-1,-1}。
            x = find(x);
            y = find(y);
            if (x == y)
            {
                return {-1, -1};
            }
            if (f[x] > f[y])
            {
                swap(x, y);
            }
            f[x] += f[y];
            f[y] = x;
            return {x, y};
        }
    };

    // n 是原序列长度；d[k] 维护所有长度为 2^k 的块之间的等价关系。
    int n;
    vector<D> d;

    RangeDSU(int n) : n(n)
    {
        // n 是原序列长度；预建每个倍增层的块并查集。
        assert(n >= 0); // 调试检查，可删
        int m = max<int>(1, bit_width((unsigned long long)n));
        for (int k = 0; k < m; k++)
        {
            d.emplace_back(n - (1LL << k) + 1);
        }
    }

    template <class F> void mergeBlock(int k, int x, int y, F &f)
    {
        // k 是块层级，x、y 是两个块起点；首次连通单点块时调用 f(新根,被吸收根)。
        auto [a, b] = d[k].merge(x, y);
        if (a < 0)
        {
            return;
        }
        if (!k)
        {
            return f(a, b);
        }
        int h = 1LL << (k - 1);
        mergeBlock(k - 1, x, y, f);
        mergeBlock(k - 1, x + h, y + h, f);
    }

    template <class F> void merge(int x, int y, int len, F &&f)
    {
        // x、y 是起点，len 是长度；合并对应位置，每个新单点合并调用一次 f(新根,被吸收根)。
        // f 返回 void，按引用调用且不复制；本方法无返回值。
        assert(0 <= x && x <= n && 0 <= y && y <= n && 0 <= len && len <= n - x && len <= n - y); // 调试检查，可删
        if (!len)
        {
            return;
        }
        int k = bit_width((unsigned long long)len) - 1;
        int z = 1LL << k;
        mergeBlock(k, x, y, f);
        mergeBlock(k, x + len - z, y + len - z, f);
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

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Z> sum(n);
    for (auto &x : sum)
    {
        int v;
        cin >> v;
        x = Z(v);
    }
    RangeDSU st(n);
    Z ans;
    while (q--)
    {
        int k, a, b;
        cin >> k >> a >> b;
        st.merge(a, b, k, [&](int root, int absorbed)
        {
            ans += sum[root] * sum[absorbed];
            sum[root] += sum[absorbed];
        });
        cout << ans.val() << '\n';
    }
    return 0;
}
