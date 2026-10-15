#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<T> motzkin(int n)
{
    // n 是要计算的最大下标；返回 M_0 到 M_n 的 Motzkin 数。
    assert(n >= 0); // 调试检查，可删。
    vector<T> a(n + 1);
    a[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        a[i] = a[i - 1];
        for (int j = 0; j + 2 <= i; j++)
        {
            a[i] += a[j] * a[i - 2 - j];
        }
    }
    return a;
}
