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

template <class T> struct MaxBIT
{
    // 当前是“单点只增大、前缀取最大值”版。改成最小值版时，
    // 将 max/lowest/chmax/maxPrefix 分别换成 min/numeric_limits<T>::max()/chmin/minPrefix，且更新方向改为单点只减小。
    // n 是数组长度，e 是空前缀的幺元，a[i] 是 lowbit 块最大值。
    int n;
    T e;
    vector<T> a;

    MaxBIT(int n = 0, T e = numeric_limits<T>::lowest()) : n(n), e(e), a(n + 1, e)
    {
        // n 是数组长度，e 是无元素贡献时的返回值。
        assert(n >= 0); // 调试检查，可删
    }

    void chmax(int x, T v)
    {
        // x 是 0 下标位置，v 是候选新值；令该点不低于 v。
        assert(0 <= x && x < n); // 调试检查，可删
        for (++x; x <= n; x += x & -x)
        {
            a[x] = max(a[x], v);
        }
    }

    T maxPrefix(int r) const
    {
        // r 是前缀右端点；返回 [0,r) 的最大值。
        assert(0 <= r && r <= n); // 调试检查，可删
        T ans = e;
        for (; r; r -= r & -r)
        {
            ans = max(ans, a[r]);
        }
        return ans;
    }
};
