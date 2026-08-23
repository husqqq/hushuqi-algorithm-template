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

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Coord = long long, class T = long long>
struct KDRectAffine
{
    struct Node
    {
        Coord xl{}, xr{}, yl{}, yr{};
        int cnt = 0;
        T sum = 0, mul = 1, add = 0;
    };

    int n;
    vector<Coord> x, y;
    vector<int> ord, pos;
    vector<Node> tr;

    KDRectAffine(vector<Coord> x, vector<Coord> y, const vector<optional<T>> &w)
        : n(x.size()), x(move(x)), y(move(y)), ord(n), pos(n), tr(max<int>(1, 4 * n))
    {
        assert((int)this->y.size() == n && (int)w.size() == n);
        iota(ord.begin(), ord.end(), 0);
        if (n)
        {
            build(1, 0, n, 0, w);
        }
    }

    void pull(int k)
    {
        auto &a = tr[2 * k];
        auto &b = tr[2 * k + 1];
        tr[k].xl = min(a.xl, b.xl);
        tr[k].xr = max(a.xr, b.xr);
        tr[k].yl = min(a.yl, b.yl);
        tr[k].yr = max(a.yr, b.yr);
        tr[k].cnt = a.cnt + b.cnt;
        tr[k].sum = a.sum + b.sum;
    }

    void build(int k, int l, int r, int d, const vector<optional<T>> &w)
    {
        if (r - l == 1)
        {
            int id = ord[l];
            pos[id] = k;
            tr[k].xl = tr[k].xr = x[id];
            tr[k].yl = tr[k].yr = y[id];
            if (w[id])
            {
                tr[k].cnt = 1;
                tr[k].sum = *w[id];
            }
            return;
        }
        int m = (l + r) / 2;
        nth_element(ord.begin() + l, ord.begin() + m, ord.begin() + r, [&](int a, int b)
        {
            return d == 0 ? x[a] < x[b] : y[a] < y[b];
        });
        build(2 * k, l, m, d ^ 1, w);
        build(2 * k + 1, m, r, d ^ 1, w);
        pull(k);
    }

    void applyAt(int k, T a, T b)
    {
        tr[k].sum = a * tr[k].sum + b * T(tr[k].cnt);
        tr[k].mul = a * tr[k].mul;
        tr[k].add = a * tr[k].add + b;
    }

    void push(int k)
    {
        if (tr[k].mul == T(1) && tr[k].add == T(0))
        {
            return;
        }
        applyAt(2 * k, tr[k].mul, tr[k].add);
        applyAt(2 * k + 1, tr[k].mul, tr[k].add);
        tr[k].mul = T(1);
        tr[k].add = T(0);
    }

    bool disjoint(int k, Coord xl, Coord xr, Coord yl, Coord yr) const
    {
        return xr <= tr[k].xl || tr[k].xr < xl || yr <= tr[k].yl || tr[k].yr < yl;
    }

    bool covered(int k, Coord xl, Coord xr, Coord yl, Coord yr) const
    {
        return xl <= tr[k].xl && tr[k].xr < xr && yl <= tr[k].yl && tr[k].yr < yr;
    }

    T query(int k, Coord xl, Coord xr, Coord yl, Coord yr)
    {
        if (disjoint(k, xl, xr, yl, yr))
        {
            return T(0);
        }
        if (covered(k, xl, xr, yl, yr))
        {
            return tr[k].sum;
        }
        push(k);
        return query(2 * k, xl, xr, yl, yr) + query(2 * k + 1, xl, xr, yl, yr);
    }

    void apply(int k, Coord xl, Coord xr, Coord yl, Coord yr, T a, T b)
    {
        if (disjoint(k, xl, xr, yl, yr))
        {
            return;
        }
        if (covered(k, xl, xr, yl, yr))
        {
            applyAt(k, a, b);
            return;
        }
        push(k);
        apply(2 * k, xl, xr, yl, yr, a, b);
        apply(2 * k + 1, xl, xr, yl, yr, a, b);
        pull(k);
    }

    void set(int id, T w)
    {
        assert(0 <= id && id < n);
        int k = pos[id];
        vector<int> path;
        for (int z = k / 2; z; z /= 2)
        {
            path.push_back(z);
        }
        reverse(path.begin(), path.end());
        for (int z : path)
        {
            push(z);
        }
        tr[k].cnt = 1;
        tr[k].sum = w;
        tr[k].mul = T(1);
        tr[k].add = T(0);
        for (k /= 2; k; k /= 2)
        {
            pull(k);
        }
    }

    T query(Coord xl, Coord xr, Coord yl, Coord yr)
    {
        assert(xl <= xr && yl <= yr);
        return n ? query(1, xl, xr, yl, yr) : T(0);
    }

    void apply(Coord xl, Coord xr, Coord yl, Coord yr, T a, T b)
    {
        assert(xl <= xr && yl <= yr);
        if (n)
        {
            apply(1, xl, xr, yl, yr, a, b);
        }
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> xs(n), ys(n);
    vector<optional<Z>> weight(n);
    for (int i = 0; i < n; i++)
    {
        long long w;
        cin >> xs[i] >> ys[i] >> w;
        weight[i] = Z(w);
    }
    struct Query
    {
        int type;
        array<long long, 6> v{};
    };
    vector<Query> query(q);
    for (auto &e : query)
    {
        cin >> e.type;
        if (e.type == 0)
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2];
            xs.push_back(e.v[0]);
            ys.push_back(e.v[1]);
            weight.push_back(nullopt);
        }
        else if (e.type == 1)
        {
            cin >> e.v[0] >> e.v[1];
        }
        else if (e.type == 2)
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2] >> e.v[3];
        }
        else
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2] >> e.v[3] >> e.v[4] >> e.v[5];
        }
    }
    KDRectAffine<long long, Z> tree(xs, ys, weight);
    int point_count = n;
    for (const auto &e : query)
    {
        if (e.type == 0)
        {
            tree.set(point_count++, Z(e.v[2]));
        }
        else if (e.type == 1)
        {
            tree.set(e.v[0], Z(e.v[1]));
        }
        else if (e.type == 2)
        {
            cout << tree.query(e.v[0], e.v[2], e.v[1], e.v[3]).val() << '\n';
        }
        else
        {
            tree.apply(e.v[0], e.v[2], e.v[1], e.v[3], Z(e.v[4]), Z(e.v[5]));
        }
    }
    return 0;
}
