#pragma once
#include <bits/stdc++.h>
#include "support/linear_algebra_field.hpp"
using namespace std;

template <class T> T pfaffian(vector<vector<T>> a)
{
    // a 是偶数阶斜对称矩阵，T 是域；返回 Pfaffian。
    int n = a.size();
    assert(n % 2 == 0); // 调试检查，可删。
    // 调试检查，可删。
    for (int i = 0; i < n; i++)
    {
        assert((int)a[i].size() == n && a[i][i] == T(0));
        for (int j = 0; j < n; j++)
        {
            assert(a[i][j] == -a[j][i]);
        }
    }
    T ans = 1;
    for (int k = 0; k < n; k += 2)
    {
        int p = k + 1;
        while (p < n && a[k][p] == T(0))
        {
            p++;
        }
        if (p == n)
        {
            return T(0);
        }
        if (p != k + 1)
        {
            swap(a[p], a[k + 1]);
            for (int i = 0; i < n; i++)
            {
                swap(a[i][p], a[i][k + 1]);
            }
            ans = -ans;
        }
        T v = a[k][k + 1];
        ans *= v;
        for (int i = k + 2; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                a[i][j] -= (a[k][i] * a[k + 1][j] - a[k][j] * a[k + 1][i]) / v;
                a[j][i] = -a[i][j];
            }
        }
    }
    return ans;
}
