#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<vector<int>> hadamard(int n)
{
    // n 是正的 2 的幂；返回元素只取 1 和 -1、任意两条不同行正交的 Sylvester 阿达马矩阵。
    assert(n > 0 && has_single_bit((unsigned long long)n)); // 调试检查，可删。
    assert((unsigned long long)n <= numeric_limits<size_t>::max()); // 调试检查，可删。
    size_t side = (size_t)n;
    assert(side <= numeric_limits<size_t>::max() / sizeof(int) / side); // 调试检查，可删。
    vector<vector<int>> a{{1}};
    while ((int)a.size() < n)
    {
        int m = a.size();
        vector b(2 * m, vector<int>(2 * m));
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                b[i][j] = a[i][j];
                b[i][j + m] = a[i][j];
                b[i + m][j] = a[i][j];
                b[i + m][j + m] = -a[i][j];
            }
        }
        a.swap(b);
    }
    return a;
}
