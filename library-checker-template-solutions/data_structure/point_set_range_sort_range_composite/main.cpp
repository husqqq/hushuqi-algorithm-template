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

template <class M> class SortSeg
{
  public:
    using T = typename M::T;

  private:
    struct Node
    {
        // l、r 是值域左右儿子；sz 是元素数；f、b 分别按键升序、降序合并。
        int l = 0, r = 0, sz = 0;
        T f, b;

        Node(const T &e) : f(e), b(e) {}
    };

    // n 是序列长度，K 是键值域右端点，z 是外层线段树叶子基址。
    int n, K, z = 1;
    // t 是动态值域线段树节点池；rt[p] 是以 p 开头的有序段根；rev[p] 表示该段降序。
    vector<Node> t;
    vector<int> rt;
    vector<char> rev;
    // cut 保存所有有序段左端点及哨兵 n；seg 仅在段首保存整段幺半群积。
    set<int> cut;
    vector<T> seg;

    int newNode()
    {
        // 新建空值域节点并返回编号。
        t.emplace_back(M::id());
        return (int)t.size() - 1;
    }

    int size(int p) const
    {
        // p 是值域节点编号；返回其元素数，空节点返回 0。
        return p ? t[p].sz : 0;
    }

    void pull(int p)
    {
        // p 是非叶值域节点；由非空儿子重算元素数和双向积。
        int x = t[p].l, y = t[p].r;
        t[p].sz = size(x) + size(y);
        if (!x)
        {
            t[p].f = t[y].f;
            t[p].b = t[y].b;
        }
        else if (!y)
        {
            t[p].f = t[x].f;
            t[p].b = t[x].b;
        }
        else
        {
            t[p].f = M::op(t[x].f, t[y].f);
            t[p].b = M::op(t[y].b, t[x].b);
        }
    }

    int put(int p, int l, int r, int k, const T &x)
    {
        // p 管理键区间 [l,r)，k 是新键，x 是对应值；返回插入后的根。
        if (!p)
        {
            p = newNode();
        }
        if (r - l == 1)
        {
            t[p].sz = 1;
            t[p].f = t[p].b = x;
            return p;
        }
        int m = l + (r - l) / 2;
        if (k < m)
        {
            t[p].l = put(t[p].l, l, m, k, x);
        }
        else
        {
            t[p].r = put(t[p].r, m, r, k, x);
        }
        pull(p);
        return p;
    }

    pair<int, int> split(int p, int k, int l, int r)
    {
        // p 管理键区间 [l,r)，k 是左侧所取元素数；返回按键升序切开的两根。
        if (!k)
        {
            return {0, p};
        }
        if (k == size(p))
        {
            return {p, 0};
        }
        int m = l + (r - l) / 2;
        int q = newNode();
        int s = size(t[p].l);
        if (k <= s)
        {
            auto [x, y] = split(t[p].l, k, l, m);
            int v = t[p].r;
            t[p].l = x;
            t[p].r = 0;
            t[q].l = y;
            t[q].r = v;
        }
        else
        {
            auto [x, y] = split(t[p].r, k - s, m, r);
            int v = t[p].l;
            t[p].l = v;
            t[p].r = x;
            t[q].l = 0;
            t[q].r = y;
        }
        pull(p);
        pull(q);
        return {p, q};
    }

    int join(int p, int q, int l, int r)
    {
        // p、q 管理同一键区间 [l,r) 且键集不交；破坏性合并并返回根。
        if (!p || !q)
        {
            return p | q;
        }
        assert(r - l > 1); // 调试检查，可删：合法键全局互异，不会在同一叶相遇
        int m = l + (r - l) / 2;
        t[p].l = join(t[p].l, t[q].l, l, m);
        t[p].r = join(t[p].r, t[q].r, m, r);
        pull(p);
        return p;
    }

    T all(int p, bool d) const
    {
        // p 是非空值域根，d 表示是否降序；返回该顺序下的整段积。
        return d ? t[p].b : t[p].f;
    }

    void outSet(int p, const T &x)
    {
        // p 是序列位置，x 是新的段首积或幺元；修改外层线段树。
        p += z;
        seg[p] = x;
        while (p >>= 1)
        {
            seg[p] = M::op(seg[2 * p], seg[2 * p + 1]);
        }
    }

    T outProd(int l, int r) const
    {
        // [l,r) 是序列区间；返回外层线段树中段首积的顺序合并。
        T x = M::id(), y = M::id();
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = M::op(x, seg[l++]);
            }
            if (r & 1)
            {
                y = M::op(seg[--r], y);
            }
        }
        return M::op(x, y);
    }

    void splitAt(int p)
    {
        // p 是序列边界；若它落在有序段内部，就把该段切成两段。
        if (p == n || cut.contains(p))
        {
            return;
        }
        auto it = cut.upper_bound(p);
        int r = *it, l = *prev(it);
        cut.insert(p);
        if (!rev[l])
        {
            auto [x, y] = split(rt[l], p - l, 0, K);
            rt[l] = x;
            rt[p] = y;
            rev[l] = rev[p] = false;
        }
        else
        {
            auto [x, y] = split(rt[l], r - p, 0, K);
            rt[l] = y;
            rt[p] = x;
            rev[l] = rev[p] = true;
        }
        outSet(l, all(rt[l], rev[l]));
        outSet(p, all(rt[p], rev[p]));
    }

    void sortRange(int l, int r, bool d)
    {
        // [l,r) 是排序区间，d 表示是否降序；合并其中各段并设置方向。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        splitAt(l);
        splitAt(r);
        auto it = cut.upper_bound(l);
        while (*it < r)
        {
            int p = *it;
            rt[l] = join(rt[l], rt[p], 0, K);
            rt[p] = 0;
            rev[p] = false;
            outSet(p, M::id());
            it = cut.erase(it);
        }
        rev[l] = d;
        outSet(l, all(rt[l], d));
    }

  public:
    SortSeg(int K, const vector<int> &key, const vector<T> &a) : n(key.size()), K(K), rt(n), rev(n)
    {
        // K 是键值域 [0,K) 的右端点；key、a 分别是每个位置的互异键和幺半群值。
        assert(K > 0 && key.size() == a.size()); // 调试检查，可删
        t.emplace_back(M::id());
        while (z < max<int>(1, n))
        {
            z *= 2;
        }
        seg.assign(2 * z, M::id());
        cut.insert(n);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= key[i] && key[i] < K); // 调试检查，可删
            cut.insert(i);
            rt[i] = put(0, 0, K, key[i], a[i]);
            seg[z + i] = a[i];
        }
        for (int p = z - 1; p; p--)
        {
            seg[p] = M::op(seg[2 * p], seg[2 * p + 1]);
        }
    }

    void assign(int p, int k, const T &x)
    {
        // p 是序列位置，k 是替换后的全局唯一键，x 是新值；完成单点赋值。
        assert(0 <= p && p < n && 0 <= k && k < K); // 调试检查，可删
        splitAt(p);
        splitAt(p + 1);
        rt[p] = put(0, 0, K, k, x);
        rev[p] = false;
        outSet(p, x);
    }

    void sortAsc(int l, int r)
    {
        // [l,r) 是排序区间；按键从小到大重排。
        sortRange(l, r, false);
    }

    void sortDesc(int l, int r)
    {
        // [l,r) 是排序区间；按键从大到小重排。
        sortRange(l, r, true);
    }

    T prod(int l, int r)
    {
        // [l,r) 是查询区间；返回当前顺序下的幺半群积。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        splitAt(l);
        splitAt(r);
        return outProd(l, r);
    }

    T prodAll() const
    {
        // 返回整个序列当前顺序下的幺半群积。
        return seg[1];
    }
};

struct Aff
{
    Z a = 1, b = 0;
};

struct AffMonoid
{
    using T = Aff;

    static T id()
    {
        return {};
    }

    static T op(const T &f, const T &g)
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> key(n), allKey;
    vector<Aff> a(n);
    allKey.reserve(n + q);
    for (int i = 0; i < n; ++i)
    {
        int p, x, y;
        cin >> p >> x >> y;
        key[i] = p;
        a[i] = {Z(x), Z(y)};
        allKey.push_back(p);
    }
    vector<array<int, 5>> qs(q);
    for (auto &z : qs)
    {
        cin >> z[0];
        if (z[0] == 0)
        {
            cin >> z[1] >> z[2] >> z[3] >> z[4];
            allKey.push_back(z[2]);
        }
        else if (z[0] == 1)
        {
            cin >> z[1] >> z[2] >> z[3];
        }
        else
        {
            cin >> z[1] >> z[2];
        }
    }
    sort(allKey.begin(), allKey.end());
    allKey.erase(unique(allKey.begin(), allKey.end()), allKey.end());
    for (auto &p : key)
    {
        p = lower_bound(allKey.begin(), allKey.end(), p) - allKey.begin();
    }
    SortSeg<AffMonoid> st(allKey.size(), key, a);
    for (auto z : qs)
    {
        if (z[0] == 0)
        {
            int p = lower_bound(allKey.begin(), allKey.end(), z[2]) - allKey.begin();
            st.assign(z[1], p, {Z(z[3]), Z(z[4])});
        }
        else if (z[0] == 1)
        {
            auto f = st.prod(z[1], z[2]);
            cout << (f.a * Z(z[3]) + f.b).val() << '\n';
        }
        else if (z[0] == 2)
        {
            st.sortAsc(z[1], z[2]);
        }
        else
        {
            st.sortDesc(z[1], z[2]);
        }
    }
    return 0;
}
