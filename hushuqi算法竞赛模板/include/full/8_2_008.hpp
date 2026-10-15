#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<T> derangements(int n)
{
    // n 是最大规模；返回 d[0..n]，d[i] 是 i 个元素无固定点排列数。
    assert(n >= 0); // 调试检查，可删。
    vector<T> d(n + 1);
    d[0] = 1;
    if (n >= 1)
    {
        d[1] = 0;
    }
    for (int i = 2; i <= n; i++)
    {
        d[i] = T(i - 1) * (d[i - 1] + d[i - 2]);
    }
    return d;
}

vector<int> derangeMod(int n, int m)
{
    // n 是最大规模，m 是正模数；返回 d[0..n]，其中 d[i] 是错排数模 m。
    assert(n >= 0 && m >= 1);
    vector<int> d(n + 1);
    d[0] = 1 % m;
    for (int i = 2; i <= n; i++)
    {
        d[i] = ((__int128)i * d[i - 1] + (i & 1 ? m - 1 : 1)) % m;
    }
    return d;
}
