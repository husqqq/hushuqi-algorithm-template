#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace mitm
{
struct State
{
    __int128 sum;            // sum 是该子集的元素和。
    unsigned long long mask; // mask 的第 i 位表示半边内第 i 个元素是否被选。
};

vector<State> sums(const vector<int> &a, int l, int r)
{
    // a 是原数组，[l,r) 是一半的下标范围；返回这一半全部 2^(r-l) 个子集的和与选择掩码。
    vector<State> v{{0, 0}};
    for (int i = l; i < r; i++)
    {
        int n = v.size();
        for (int j = 0; j < n; j++)
        {
            v.push_back({v[j].sum + a[i], v[j].mask | (1ULL << (i - l))});
        }
    }
    return v;
}

struct Result
{
    __int128 sum;        // sum 是所选子集的和。
    vector<int> indices; // indices 是所选元素的原下标，升序。
};

optional<Result> bound(const vector<int> &a, __int128 target, bool ceil = false)
{
    // a 至多 46 个元素且可为负；ceil=false 求不超过 target 的最大子集和，
    // ceil=true 求不小于 target 的最小子集和；返回和与下标，不存在时返回空。
    assert(a.size() <= 46); // 调试检查，可删。
    int k = a.size() / 2;
    auto x = sums(a, 0, k), y = sums(a, k, a.size());
    sort(y.begin(), y.end(), [](const State &u, const State &v) { return u.sum < v.sum; });
    optional<Result> ans;
    unsigned long long mx = 0, my = 0;
    for (auto u : x)
    {
        // 在后半找最接近 target-u.sum 的和：it 是第一个不小于它的位置。
        auto it = lower_bound(y.begin(), y.end(), target - u.sum,
                              [](const State &v, __int128 s) { return v.sum < s; });
        if (!ceil)
        {
            if (it == y.end() || it->sum != target - u.sum)
            {
                if (it == y.begin())
                {
                    continue;
                }
                --it;
            }
        }
        else if (it == y.end())
        {
            continue;
        }
        __int128 s = u.sum + it->sum;
        if (!ans || (ceil ? s < ans->sum : s > ans->sum))
        {
            ans = Result{s, {}};
            mx = u.mask;
            my = it->mask;
        }
    }
    if (ans)
    {
        for (int i = 0; i < (int)a.size(); i++)
        {
            if (i < k ? (mx >> i & 1) : (my >> (i - k) & 1))
            {
                ans->indices.push_back(i);
            }
        }
    }
    return ans;
}

Result closest(const vector<int> &a, __int128 target)
{
    // a 至多 46 个元素；返回与 target 绝对差最小的子集和及下标，两侧等距时取较小的和。
    auto lo = bound(a, target), hi = bound(a, target, true);
    if (!lo)
    {
        return *hi;
    }
    if (!hi || target - lo->sum <= hi->sum - target)
    {
        return *lo;
    }
    return *hi;
}

optional<array<int, 4>> fourSum(const array<vector<int>, 4> &a, int target)
{
    // a 是四个独立数组；返回 {i,j,k,l} 使 a[0][i]+a[1][j]+a[2][k]+a[3][l]=target，不存在时返回空。
    // v 的三个分量依次是 a[0][i]+a[1][j]、i、j。
    vector<tuple<__int128, int, int>> v;
    for (int i = 0; i < (int)a[0].size(); i++)
    {
        for (int j = 0; j < (int)a[1].size(); j++)
        {
            v.emplace_back((__int128)a[0][i] + a[1][j], i, j);
        }
    }
    sort(v.begin(), v.end());
    for (int i = 0; i < (int)a[2].size(); i++)
    {
        for (int j = 0; j < (int)a[3].size(); j++)
        {
            __int128 need = (__int128)target - a[2][i] - a[3][j];
            auto it = lower_bound(v.begin(), v.end(), tuple<__int128, int, int>{need, -1, -1});
            if (it != v.end() && get<0>(*it) == need)
            {
                return array<int, 4>{get<1>(*it), get<2>(*it), i, j};
            }
        }
    }
    return nullopt;
}
}
