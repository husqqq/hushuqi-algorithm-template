#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<pair<int, int>>> havelHakimi(const vector<int> &d)
{
    // d[i] 是点 i 要求的度数；能构造简单无向图时返回其边表，否则返回空。
    int n = d.size();
    priority_queue<pair<int, int>> q; // q 存 (剩余度数, 点)，每次取剩余度数最大的点。
    vector<pair<int, int>> e;
    for (int i = 0; i < n; i++)
    {
        if (d[i] < 0 || d[i] >= n)
        {
            return nullopt;
        }
        if (d[i])
        {
            q.push({d[i], i});
        }
    }
    while (!q.empty())
    {
        auto [k, u] = q.top();
        q.pop();
        if (k > (int)q.size())
        {
            return nullopt;
        }
        // u 连向剩余度数最大的 k 个点，各自度数减一后放回。
        vector<pair<int, int>> tmp;
        while (k--)
        {
            auto [s, v] = q.top();
            q.pop();
            e.push_back({u, v});
            if (s > 1)
            {
                tmp.push_back({s - 1, v});
            }
        }
        for (auto x : tmp)
        {
            q.push(x);
        }
    }
    return e;
}

bool galeRyser(vector<int> a, vector<int> b)
{
    // a、b 是二分图左右两侧各点要求的度数；返回是否存在对应的简单二分图。
    __int128 sa = 0, sb = 0;
    for (int x : a)
    {
        if (x < 0 || x > (int)b.size())
        {
            return false;
        }
        sa += x;
    }
    for (int x : b)
    {
        if (x < 0 || x > (int)a.size())
        {
            return false;
        }
        sb += x;
    }
    if (sa != sb)
    {
        return false;
    }
    // a 降序后检查：前 k 大之和 <= sum min(b_j, k)。b 升序以便双指针累计 min。
    sort(a.rbegin(), a.rend());
    sort(b.begin(), b.end());
    __int128 lhs = 0, small = 0; // small 是 b 中小于 k 的那些度数之和。
    int j = 0;
    for (int k = 1; k <= (int)a.size(); k++)
    {
        lhs += a[k - 1];
        while (j < (int)b.size() && b[j] < k)
        {
            small += b[j++];
        }
        if (lhs > small + (__int128)k * (b.size() - j))
        {
            return false;
        }
    }
    return true;
}

optional<vector<pair<int, int>>> bipDegree(const vector<int> &a, const vector<int> &b)
{
    // a、b 同 galeRyser；可构造时返回边表 (左点, 右点)，否则返回空。
    if (!galeRyser(a, b))
    {
        return nullopt;
    }
    vector<pair<int, int>> left, e;
    priority_queue<pair<int, int>> q; // q 存右侧 (剩余度数, 点)。
    for (int i = 0; i < (int)a.size(); i++)
    {
        left.push_back({a[i], i});
    }
    for (int j = 0; j < (int)b.size(); j++)
    {
        if (b[j])
        {
            q.push({b[j], j});
        }
    }
    // 左侧按度数从大到小，每个点连向右侧剩余度数最大的若干点。
    sort(left.rbegin(), left.rend());
    for (auto [k, u] : left)
    {
        vector<pair<int, int>> tmp;
        while (k--)
        {
            auto [s, v] = q.top();
            q.pop();
            e.push_back({u, v});
            if (s > 1)
            {
                tmp.push_back({s - 1, v});
            }
        }
        for (auto x : tmp)
        {
            q.push(x);
        }
    }
    return e;
}
