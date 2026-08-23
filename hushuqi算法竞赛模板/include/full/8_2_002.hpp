#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<T> catalan(int n)
{
    // n 是要计算的最大下标；返回 C_0 到 C_n 的 Catalan 数。
    assert(n >= 0); // 调试检查，可删。
    vector<T> a(n + 1);
    a[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            a[i] += a[j] * a[i - 1 - j];
        }
    }
    return a;
}
