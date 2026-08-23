#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

bool graphical(vector<long long> a)
{
    // a 是无向简单图各点度数；返回是否存在恰有该度数多重集的简单图。
    sort(a.begin(), a.end(), greater<>());
    int n = a.size();
    vector<long long> s(n + 1);
    for (int i = 0; i < n; i++)
    {
        if (a[i] < 0 || a[i] >= n)
        {
            return false;
        }
        if (s[i] > LLONG_MAX - a[i])
        {
            return false;
        }
        s[i + 1] = s[i] + a[i];
    }
    if (s[n] & 1)
    {
        return false;
    }
    for (int k = 1; k <= n; k++)
    {
        int p = lower_bound(a.begin() + k, a.end(), k, greater<>()) - a.begin();
        long long rhs = 1LL * k * (k - 1) + 1LL * (p - k) * k + s[n] - s[p];
        if (s[k] > rhs)
        {
            return false;
        }
    }
    return true;
}
