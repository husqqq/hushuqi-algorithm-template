#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace mitm
{
struct State { __int128 sum; unsigned long long mask; };
inline vector<State> sums(const vector<long long> &a, int l, int r)
{
    vector<State> v{{0, 0}};
    for (int i = l; i < r; ++i)
    {
        size_t n = v.size();
        for (size_t j = 0; j < n; ++j)
            v.push_back({v[j].sum + a[i], v[j].mask | (1ULL << (i - l))});
    }
    return v;
}
struct Result { __int128 sum; vector<int> indices; };
// 支持负数；ceil=false 求 <=target 的最大和，ceil=true 求 >=target 的最小和。
inline optional<Result> bound(const vector<long long> &a, __int128 target, bool ceil = false)
{
    if (a.size() > 46) throw invalid_argument("MITM: n must be <=46");
    int k = a.size() / 2;
    auto x = sums(a, 0, k), y = sums(a, k, a.size());
    sort(y.begin(), y.end(), [](auto u, auto v) { return u.sum < v.sum; });
    optional<Result> ans;
    unsigned long long mx = 0, my = 0;
    for (auto u : x)
    {
        auto it = lower_bound(y.begin(), y.end(), target - u.sum,
                              [](auto v, __int128 s) { return v.sum < s; });
        if (!ceil)
        {
            if (it == y.end() || it->sum != target - u.sum)
            {
                if (it == y.begin()) continue;
                --it;
            }
        }
        else if (it == y.end()) continue;
        __int128 s = u.sum + it->sum;
        if (!ans || (ceil ? s < ans->sum : s > ans->sum))
        {
            ans = Result{s, {}};
            mx = u.mask; my = it->mask;
        }
    }
    if (ans) for (int i = 0; i < (int)a.size(); ++i)
        if (i < k ? (mx >> i & 1) : (my >> (i - k) & 1)) ans->indices.push_back(i);
    return ans;
}
inline Result closest(const vector<long long> &a, __int128 target)
{
    auto lo = bound(a, target), hi = bound(a, target, true);
    if (!lo) return *hi;
    if (!hi || target - lo->sum <= hi->sum - target) return *lo;
    return *hi;
}
// 四个独立数组各选一个数；不是同一数组中四个不同下标的问题。
inline optional<array<int, 4>> fourSum(const array<vector<long long>, 4> &a, long long target)
{
    vector<tuple<__int128, int, int>> v;
    for (int i = 0; i < (int)a[0].size(); ++i)
        for (int j = 0; j < (int)a[1].size(); ++j) v.emplace_back((__int128)a[0][i] + a[1][j], i, j);
    sort(v.begin(), v.end());
    for (int i = 0; i < (int)a[2].size(); ++i)
        for (int j = 0; j < (int)a[3].size(); ++j)
        {
            __int128 need = (__int128)target - a[2][i] - a[3][j];
            auto it = lower_bound(v.begin(), v.end(), tuple{need, -1LL, -1LL});
            if (it != v.end() && get<0>(*it) == need) return array<int, 4>{get<1>(*it), get<2>(*it), i, j};
        }
    return nullopt;
}
}
