#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<vector<int>> magicSquare(int n)
{
    // n 是阶数；返回恰好填入 1 到 n^2 的幻方，n=2 时无解并返回空矩阵。
    assert(n > 0); // 调试检查，可删。
    assert(n <= numeric_limits<int>::max() / n); // 调试检查，可删。
    assert((unsigned long long)n <= numeric_limits<size_t>::max()); // 调试检查，可删。
    size_t side = (size_t)n;
    assert(side <= numeric_limits<size_t>::max() / sizeof(int) / side); // 调试检查，可删。
    int total = n * n;
    if (n == 2)
    {
        return {};
    }
    if (n & 1)
    {
        vector a(n, vector<int>(n));
        int x = 0;
        int y = n / 2;
        for (int v = 1; v <= total; v++)
        {
            a[x][y] = v;
            int nx = (x + n - 1) % n;
            int ny = (y + 1) % n;
            if (a[nx][ny])
            {
                x = (x + 1) % n;
            }
            else
            {
                x = nx;
                y = ny;
            }
        }
        return a;
    }
    if (n % 4 == 0)
    {
        vector a(n, vector<int>(n));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int v = i * n + j + 1;
                bool rev = i % 4 == j % 4 || i % 4 + j % 4 == 3;
                a[i][j] = rev ? total + 1 - v : v;
            }
        }
        return a;
    }
    int m = n / 2;
    int k = (n - 2) / 4;
    int z = m * m;
    auto b = magicSquare(m);
    vector a(n, vector<int>(n));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            a[i][j] = b[i][j];
            a[i][j + m] = b[i][j] + 2 * z;
            a[i + m][j] = b[i][j] + 3 * z;
            a[i + m][j + m] = b[i][j] + z;
        }
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (i != m / 2 || j)
            {
                swap(a[i][j], a[i + m][j]);
            }
        }
        for (int j = n - k + 1; j < n; j++)
        {
            swap(a[i][j], a[i + m][j]);
        }
    }
    swap(a[m / 2][k], a[m / 2 + m][k]);
    return a;
}
