#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<vector<T>> pascalTri(int n)
{
    // n 是最大行号；返回第 0..n 行杨辉三角，其中 a[i][j]=C(i,j)。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1);
    for (int i = 0; i <= n; i++)
    {
        a[i].assign(i + 1, T(1));
        for (int j = 1; j < i; j++)
        {
            a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
        }
    }
    return a;
}
