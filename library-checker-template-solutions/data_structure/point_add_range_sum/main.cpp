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

template <class T> struct BIT
{
    // n 是数组长度；a[i] 是以内部下标 i 结尾的 lowbit 块和。
    int n;
    vector<T> a;

    BIT(int n = 0)
    {
        // n 是数组长度；初值全为零。
        init(n);
    }

    void init(int n_)
    {
        // n_ 是新的数组长度；清空旧状态并置零。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        a.assign(n + 1, T{});
    }

    void add(int x, T v)
    {
        // x 是 0 下标位置，v 是增量；把 a[x] 增加 v。
        assert(0 <= x && x < n); // 调试检查，可删
        for (++x; x <= n; x += x & -x)
        {
            a[x] += v;
        }
    }

    T sum(int x) const
    {
        // x 是前缀右端点；返回 [0,x) 的和。
        assert(0 <= x && x <= n); // 调试检查，可删
        T ans{};
        for (; x; x -= x & -x)
        {
            ans += a[x];
        }
        return ans;
    }

    T sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(r) - sum(l);
    }

    int kth(T k) const
    {
        // 各点权必须非负且 k>=0；返回满足 sum(x)<=k 的最大前缀长度 x。
        assert(k >= T{}); // 调试检查，可删
        int x = 0;
        T cur{};
        for (int d = bit_floor((unsigned long long)n); d; d >>= 1)
        {
            if (x + d <= n && cur + a[x + d] <= k)
            {
                x += d;
                cur += a[x];
            }
        }
        return x;
    }
};

template <class T> struct FastBIT
{
    // 每 1024 个逻辑槽插一个空洞，打散 Fenwick 热节点的缓存组冲突。
    uint32_t n = 0;
    vector<T> a;

    static constexpr uint32_t pos(uint32_t x)
    {
        return x + (x >> 10);
    }

    FastBIT(uint32_t n = 0)
    {
        init(n);
    }

    explicit FastBIT(const vector<T> &v)
    {
        init(v.size());
        vector<T> pre(n + 1);
        for (uint32_t i = 1; i <= n; i++)
        {
            pre[i] = pre[i - 1] + v[i - 1];
            a[pos(i)] = pre[i] - pre[i & (i - 1)];
        }
    }

    void init(uint32_t m)
    {
        n = m;
        a.assign(pos(n) + 1, T{});
    }

    void add(uint32_t x, T v)
    {
        assert(x < n); // 调试检查，可删
        for (x++; x <= n; x += x & -x)
        {
            a[pos(x)] += v;
        }
    }

    T sum(uint32_t x) const
    {
        assert(x <= n); // 调试检查，可删
        T ans{};
        for (; x; x &= x - 1)
        {
            ans += a[pos(x)];
        }
        return ans;
    }

    T sum(uint32_t l, uint32_t r) const
    {
        assert(l <= r && r <= n); // 调试检查，可删
        return sum(r) - sum(l);
    }

    uint32_t kth(T k) const
    {
        assert(k >= T{}); // 调试检查，可删
        uint32_t x = 0;
        T cur{};
        for (uint32_t d = bit_floor((unsigned long long)n); d; d >>= 1)
        {
            if (x + d <= n && cur + a[pos(x + d)] <= k)
            {
                x += d;
                cur += a[pos(x)];
            }
        }
        return x;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    BIT<long long> bit(n);
    for (int i = 0; i < n; ++i)
    {
        long long x;
        cin >> x;
        bit.add(i, x);
    }
    while (q--)
    {
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 0)
        {
            bit.add(x, y);
        }
        else
        {
            cout << bit.sum(x, y) << '\n';
        }
    }
    return 0;
}
