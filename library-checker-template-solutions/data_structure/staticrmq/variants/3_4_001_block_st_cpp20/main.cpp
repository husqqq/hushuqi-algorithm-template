#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选上界；若 b 更小则写入 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选下界；若 b 更大则写入 a 并返回 true。
    return a < b ? a = b, true : false;
}

template <class T, class F> struct ST
{
    // n 是数组长度，f 是幂等合并运算，a[k][i] 保存长度 2^k 的区间信息。
    int n;
    F f;
    vector<vector<T>> a;

    ST(const vector<T> &v, F f = {}) : n((int)v.size()), f(f)
    {
        // v 是静态数组，f 是满足结合律和幂等性的合并运算。
        int m = n ? bit_width((unsigned)n) : 0;
        a.assign(m, vector<T>(n));
        if (!n)
        {
            return;
        }
        a[0] = v;
        for (int k = 1; k < m; k++)
        {
            for (int i = 0; i + (1 << k) <= n; i++)
            {
                a[k][i] = f(a[k - 1][i], a[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // 仅适用于幂等运算（min/max/gcd/and/or），查询 [l,r)。
    T query(int l, int r) const
    {
        // [l,r) 是非空查询区间；返回其合并结果。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        int k = bit_width((unsigned)(r - l)) - 1;
        return f(a[k][l], a[k][r - (1 << k)]);
    }
};

template <class T, class F, uint32_t B = 16> struct BlockST
{
    static_assert(B > 0);

    uint32_t n = 0, m = 0;
    F f;
    vector<T> val, pre, suf;
    vector<vector<T>> st;

    BlockST(const vector<T> &v, F f = {}) : f(f)
    {
        build(v);
    }

    void build(const vector<T> &v)
    {
        n = v.size();
        m = (n + B - 1) / B;
        if (!n)
        {
            val.clear();
            pre.clear();
            suf.clear();
            st.clear();
            return;
        }
        uint32_t z = m * B;
        val.assign(z, v.back());
        copy(v.begin(), v.end(), val.begin());
        pre.resize(z);
        suf.resize(z);
        vector<T> top(m);
        for (uint32_t i = 0; i < z; i++)
        {
            pre[i] = i % B ? f(pre[i - 1], val[i]) : val[i];
        }
        for (uint32_t l = 0; l < z; l += B)
        {
            suf[l + B - 1] = val[l + B - 1];
            for (uint32_t i = l + B - 1; i-- > l;)
            {
                suf[i] = f(val[i], suf[i + 1]);
            }
            top[l / B] = suf[l];
        }
        uint32_t h = bit_width((unsigned long long)m);
        st.assign(h, vector<T>(m));
        st[0] = move(top);
        for (uint32_t k = 1; k < h; k++)
        {
            for (uint32_t i = 0; i + (1U << k) <= m; i++)
            {
                st[k][i] = f(st[k - 1][i], st[k - 1][i + (1U << (k - 1))]);
            }
        }
    }

    T query(uint32_t l, uint32_t r) const
    {
        assert(l < r && r <= n); // 调试检查，可删
        r--;
        uint32_t x = l / B, y = r / B;
        if (x == y)
        {
            T ans = val[l];
            for (uint32_t i = l + 1; i <= r; i++)
            {
                ans = f(ans, val[i]);
            }
            return ans;
        }
        T ans = f(suf[l], pre[r]);
        if (++x < y)
        {
            uint32_t k = bit_width((unsigned long long)(y - x)) - 1;
            ans = f(ans, f(st[k][x], st[k][y - (1U << k)]));
        }
        return ans;
    }
};

struct MinOp
{
    long long operator()(long long a, long long b) const
    {
        return min(a, b);
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<long long> a(n);
    for (auto &x : a) cin >> x;
    BlockST<long long, MinOp> st(a, MinOp{});
    while (q--)
    {
        int l, r; cin >> l >> r; cout << st.query(l, r) << '\n';
    }
}
