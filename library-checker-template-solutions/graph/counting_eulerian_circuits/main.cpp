

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

Z spanningTrees(int n, const vector<pair<int, int>> &e)
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

Z arborescences(int n, int root, const vector<pair<int, int>> &e)
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

Z bestTheorem(int n, int root, const vector<pair<int, int>> &e, bool fixedFirst = false)
{
    // n 是点数，root 是起点，e 是有编号有向边；fixedFirst 表示是否固定首边。
    if (n == 0)
    {
        return e.empty() ? Z(1) : Z(0);
    }
    assert(0 <= root && root < n); // 调试检查，可删。
    vector<int> out(n), in(n);
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        out[u]++;
        in[v]++;
    }
    for (int u = 0; u < n; u++)
    {
        if (out[u] != in[u])
        {
            return 0;
        }
    }
    if (e.empty())
    {
        return 1;
    }
    if (out[root] == 0)
    {
        return 0;
    }
    vector<int> id(n, -1), active;
    for (int u = 0; u < n; u++)
    {
        if (out[u] > 0)
        {
            id[u] = active.size();
            active.push_back(u);
        }
    }
    vector<pair<int, int>> rev;
    for (auto [u, v] : e)
    {
        rev.push_back({id[v], id[u]});
    }
    Z ans = arborescences(active.size(), id[root], rev);
    for (int u : active)
    {
        for (int k = 1; k < out[u]; k++)
        {
            ans *= k;
        }
    }
    if (!fixedFirst)
    {
        ans *= out[root];
    }
    return ans;
}

signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << bestTheorem(n, e[0].first, e, true).val() << '\n';
}
