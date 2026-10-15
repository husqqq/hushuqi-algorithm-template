#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Dist = long long>
optional<vector<Dist>> diffCons(int n, const vector<array<int, 3>> &a)
{
    // n 是变量个数；a[i][0..2] 依次是 u、v、w，表示 x[v]-x[u]<=w；无解返回空，否则返回一组可行 x。
    vector<Dist> x(n);
    if (n == 0)
    {
        return x;
    }
    for (int k = 0; k < n; k++)
    {
        bool upd = false;
        for (auto [u, v, w] : a)
        {
            assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
            Dist nx = x[u] + (Dist)w;
            if (x[v] > nx)
            {
                x[v] = nx;
                upd = true;
            }
        }
        if (!upd)
        {
            return x;
        }
    }
    return nullopt;
}
