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

template <class T, class F> struct AggDeque
{
    // op 按队首到队尾合并两个摘要，须可在 const 对象上调用；e 是双侧幺元。
    F op;
    T e;
    // l、r 是队首侧与队尾侧栈；每项为 {原值,按队列方向的本栈聚合}。
    vector<pair<T, T>> l, r;

    AggDeque(F combine, T identity) : op(move(combine)), e(move(identity))
    {
        // combine(a,b) 返回相邻段 a 后接 b 的摘要，须满足结合律；identity 是幺元，初始队列为空。
    }

    int size() const
    {
        // 返回队列元素数。
        return l.size() + r.size();
    }
    bool empty() const
    {
        // 返回队列是否为空。
        return l.empty() && r.empty();
    }
    void pushFront(T x)
    {
        // x 是新元素；插入队首并更新左栈聚合。
        l.push_back({x, l.empty() ? x : op(x, l.back().second)});
    }
    void pushBack(T x)
    {
        // x 是新元素；插入队尾并更新右栈聚合。
        r.push_back({x, r.empty() ? x : op(r.back().second, x)});
    }
private:
    void rebuild(int k)
    {
        // k 是重建后放入队首侧的元素数；保持整个队列顺序不变。
        assert(0 <= k && k <= size()); // 调试检查，可删
        vector<T> a;
        a.reserve(size());
        for (auto i = l.rbegin(); i != l.rend(); i++)
        {
            a.push_back(i->first);
        }
        for (const auto &x : r)
        {
            a.push_back(x.first);
        }
        l.clear();
        r.clear();
        for (int i = k - 1; i >= 0; i--)
        {
            pushFront(a[i]);
        }
        for (int i = k; i < (int)a.size(); i++)
        {
            pushBack(a[i]);
        }
    }
public:
    void popFront()
    {
        // 当前队列须非空；删除队首元素，无返回值。
        assert(!empty()); // 调试检查，可删
        if (l.empty())
        {
            rebuild(size() - size() / 2);
        }
        l.pop_back();
    }
    void popBack()
    {
        // 当前队列须非空；删除队尾元素，无返回值。
        assert(!empty()); // 调试检查，可删
        if (r.empty())
        {
            rebuild(size() / 2);
        }
        r.pop_back();
    }
    const T &front() const
    {
        // 当前队列须非空；返回队首元素的只读引用，修改队列前使用。
        assert(!empty()); // 调试检查，可删
        return l.empty() ? r.front().first : l.back().first;
    }
    const T &back() const
    {
        // 当前队列须非空；返回队尾元素的只读引用，修改队列前使用。
        assert(!empty()); // 调试检查，可删
        return r.empty() ? l.front().first : r.back().first;
    }
    T query() const
    {
        // 按队首到队尾顺序聚合全部元素；空队列返回 e。
        if (l.empty())
        {
            return r.empty() ? e : r.back().second;
        }
        if (r.empty())
        {
            return l.back().second;
        }
        return op(l.back().second, r.back().second);
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

struct Aff
{
    Z a = 1, b = 0; // 函数 x -> ax+b 的斜率与截距。
};

struct Compose
{
    Aff operator()(const Aff &f, const Aff &g) const
    {
        // f、g 是相邻仿射函数；返回先作用 f、再作用 g 的复合。
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    AggDeque<Aff, Compose> que(Compose{}, Aff{});
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            Aff f;
            int a, b;
            cin >> a >> b;
            f = {Z(a), Z(b)};
            que.pushBack(f);
        }
        else if (type == 1)
        {
            que.popFront();
        }
        else
        {
            int x;
            cin >> x;
            auto f = que.query();
            cout << (f.a * Z(x) + f.b).val() << '\n';
        }
    }
    return 0;
}
