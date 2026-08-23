#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

bool tourScore(vector<long long> a)
{
    // a 是竞赛图各点出度；返回是否存在恰有该出度多重集的竞赛图。
    sort(a.begin(), a.end());
    int n = a.size();
    __int128 sum = 0;
    for (int k = 1; k <= n; k++)
    {
        if (a[k - 1] < 0 || a[k - 1] >= n)
        {
            return false;
        }
        sum += (__int128)a[k - 1];
        if (sum < (__int128)k * (k - 1) / 2)
        {
            return false;
        }
    }
    return sum == (__int128)n * (n - 1) / 2;
}
