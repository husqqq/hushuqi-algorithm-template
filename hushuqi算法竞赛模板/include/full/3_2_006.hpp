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

template <class T> struct SuffixBIT
{
    // n 是数组长度；a[i] 是反向 lowbit 块和。
    int n;
    vector<T> a;

    SuffixBIT(int n = 0) : n(n), a(n + 1)
    {
        // n 是数组长度；初值全为零。
        assert(n >= 0); // 调试检查，可删
    }

    void add(int x, T v)
    {
        // x 是 0 下标位置，v 是增量；把该点增加 v。
        assert(0 <= x && x < n); // 调试检查，可删
        for (++x; x; x -= x & -x)
        {
            a[x] += v;
        }
    }

    T sumSuffix(int l) const
    {
        // l 是后缀左端点；返回 [l,n) 的和，允许 l=n。
        assert(0 <= l && l <= n); // 调试检查，可删
        T ans{};
        for (++l; l <= n; l += l & -l)
        {
            ans += a[l];
        }
        return ans;
    }

    T sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sumSuffix(l) - sumSuffix(r);
    }
};
