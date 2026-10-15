#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> vector<vector<T>> eulerian(int n)
{
    // n 是最大排列长度；返回第 0..n 行 Eulerian 数，列下标是下降位置数。
    assert(n >= 0); // 调试检查，可删。
    vector<vector<T>> a(n + 1, vector<T>(n + 1));
    a[0][0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (j)
            {
                a[i][j] += T(i - j) * a[i - 1][j - 1];
            }
            a[i][j] += T(j + 1) * a[i - 1][j];
        }
    }
    return a;
}
