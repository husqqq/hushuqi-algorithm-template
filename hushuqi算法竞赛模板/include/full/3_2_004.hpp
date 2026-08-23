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

template <class T> struct RangeFenwick
{
    // n 是数组长度；bit0 维护 d[i]，bit1 维护 i*d[i]。
    int n;
    vector<T> bit0, bit1;

    void add(vector<T> &bit, int x, T value)
    {
        // bit 是一棵内部 BIT，x 是差分下标，value 是增量。
        for (++x; x <= n; x += x & -x)
        {
            bit[x] += value;
        }
    }

    T sum(const vector<T> &bit, int x) const
    {
        // bit 是一棵内部 BIT；返回其前 x 个差分量之和。
        T result{};
        for (; x > 0; x -= x & -x)
        {
            result += bit[x];
        }
        return result;
    }

    explicit RangeFenwick(int n = 0) : n(n), bit0(n + 1), bit1(n + 1)
    {
        // n 是数组长度；初值全为零。
        assert(n >= 0); // 调试检查，可删
    }

    explicit RangeFenwick(const vector<T> &a) : RangeFenwick((int)a.size())
    {
        // a 是初始数组；逐点加入其初值。
        for (int i = 0; i < n; ++i)
        {
            rangeAdd(i, i + 1, a[i]);
        }
    }

    void rangeAdd(int l, int r, T value)
    {
        // l、r 是半开区间端点，value 是增量；给 [l,r) 整段增加 value。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        add(bit0, l, value);
        add(bit0, r, -value);
        add(bit1, l, value * l);
        add(bit1, r, -value * r);
    }

    T prefixSum(int r) const
    {
        // r 是前缀右端点；返回 [0,r) 的元素和。
        assert(0 <= r && r <= n); // 调试检查，可删
        return sum(bit0, r) * r - sum(bit1, r);
    }

    T rangeSum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的元素和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return prefixSum(r) - prefixSum(l);
    }
};
