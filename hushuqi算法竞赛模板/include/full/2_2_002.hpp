#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

template <class T> struct Discrete
{
    vector<T> a; // a 保存升序且去重后的原值。

    Discrete() = default;

    explicit Discrete(vector<T> v) : a(move(v))
    {
        // v 收集所有可能出现的值。
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }

    int id(const T &x) const
    {
        // x 必须已存在；返回 x 从 0 开始的离散编号。
        auto it = lower_bound(a.begin(), a.end(), x);
        assert(it != a.end() && *it == x); // 调试检查，可删
        return it - a.begin();
    }

    int lower(const T &x) const
    {
        // x 是查询值；返回第一个不小于 x 的离散位置。
        return lower_bound(a.begin(), a.end(), x) - a.begin();
    }

    const T &value(int p) const
    {
        // p 是合法离散编号；返回对应原值的常量引用。
        return a[p];
    }
};
