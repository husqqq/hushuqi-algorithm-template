#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace mitm
{
static vector<__int128> sums(const vector<int> &a, int l, int r)
{
    // a 是原数组，[l,r) 是一半的下标范围；返回全部子集和，不保存掩码以减少常驻内存。
    vector<__int128> v;
    v.reserve(1ULL << (r - l));
    v.push_back(0);
    for (int i = l; i < r; i++)
    {
        int n = v.size();
        for (int j = 0; j < n; j++)
        {
            v.push_back(v[j] + a[i]);
        }
    }
    return v;
}

static optional<unsigned long long> maskSum(const vector<int> &a, int l, int r, __int128 want)
{
    // a[l..r) 是至多 46 个元素的半边；按 Gray 码枚举并返回和为 want 的一个子集掩码。
    unsigned long long count = 1ULL << (r - l);
    __int128 sum = 0;
    for (unsigned long long i = 0; i < count; i++)
    {
        unsigned long long mask = i ^ (i >> 1);
        if (sum == want)
        {
            return mask;
        }
        if (i + 1 < count)
        {
            unsigned long long next = (i + 1) ^ ((i + 1) >> 1);
            int bit = countr_zero(mask ^ next);
            sum += (next >> bit & 1) ? a[l + bit] : -(__int128)a[l + bit];
        }
    }
    return nullopt;
}

struct Result
{
    __int128 sum;        // 46 个 64 位元素的和可能超过 64 位。
    vector<int> indices; // 所选元素的原下标，升序。
};

optional<Result> bound(const vector<int> &a, __int128 target, bool ceil = false)
{
    // a 至多 46 个元素且可为负；ceil=false 求不超过 target 的最大子集和，
    // ceil=true 求不小于 target 的最小子集和；返回和与下标，不存在时返回空。
    if (a.size() > 46)
    {
        throw length_error("mitm::bound supports at most 46 elements");
    }
    __int128 mn = 0, mx = 0;
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (a[i] < 0)
        {
            mn += a[i];
        }
        else
        {
            mx += a[i];
        }
    }
    auto extreme = [&](bool high)
    {
        Result res{high ? mx : mn, {}};
        for (int i = 0; i < (int)a.size(); i++)
        {
            if (high ? a[i] > 0 : a[i] < 0)
            {
                res.indices.push_back(i);
            }
        }
        return res;
    };
    if (!ceil && target < mn)
    {
        return nullopt;
    }
    if (!ceil && target >= mx)
    {
        return extreme(true);
    }
    if (ceil && target <= mn)
    {
        return extreme(false);
    }
    if (ceil && target > mx)
    {
        return nullopt;
    }
    int k = a.size() / 2;
    auto y = sums(a, k, a.size());
    sort(y.begin(), y.end());
    unsigned long long count = 1ULL << k;
    __int128 leftSum = 0, bestSum = 0, bestLeft = 0, bestRight = 0;
    bool found = false;
    for (unsigned long long i = 0; i < count; i++)
    {
        unsigned long long mask = i ^ (i >> 1);
        // 在后半找不超过或不小于 target-leftSum 的最优和。
        __int128 need = target - leftSum;
        auto it = lower_bound(y.begin(), y.end(), need);
        bool valid = true;
        if (!ceil)
        {
            if (it == y.end() || *it != need)
            {
                if (it == y.begin())
                {
                    valid = false;
                }
                else
                {
                    --it;
                }
            }
        }
        else if (it == y.end())
        {
            valid = false;
        }
        if (valid && (!found || (ceil ? leftSum + *it < bestSum : leftSum + *it > bestSum)))
        {
            found = true;
            bestSum = leftSum + *it;
            bestLeft = leftSum;
            bestRight = *it;
        }
        if (i + 1 < count)
        {
            unsigned long long next = (i + 1) ^ ((i + 1) >> 1);
            int bit = countr_zero(mask ^ next);
            leftSum += (next >> bit & 1) ? a[bit] : -(__int128)a[bit];
        }
    }
    if (!found)
    {
        return nullopt;
    }
    auto leftMask = maskSum(a, 0, k, bestLeft);
    auto rightMask = maskSum(a, k, a.size(), bestRight);
    assert(leftMask && rightMask);
    Result ans{bestSum, {}};
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i < k ? (*leftMask >> i & 1) : (*rightMask >> (i - k) & 1))
        {
            ans.indices.push_back(i);
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
    if (!hi)
    {
        return *lo;
    }
    // 无符号差值承接完整有符号 128 位范围内的距离，避免极端 target 相减溢出。
    unsigned __int128 lowGap = (unsigned __int128)target - (unsigned __int128)lo->sum;
    unsigned __int128 highGap = (unsigned __int128)hi->sum - (unsigned __int128)target;
    if (lowGap <= highGap)
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
