#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> nQueens(int n)
{
    // 参数 n 是棋盘边长；返回每行皇后的 0 下标列号，n=2 或 3 时返回空表。
    assert(n >= 1); // 调试检查，可删。
    if (n == 1)
    {
        return {0};
    }
    if (n < 4)
    {
        return {};
    }
    vector<int> p(n);
    int m = n - (n & 1); // 先构造偶数阶；奇数阶最后补主对角线上的皇后。
    if (n & 1) p[n - 1] = n - 1;
    if (m % 6 != 2)
    {
        for (int i = 1; i <= m / 2; i++)
        {
            p[i - 1] = 2 * i - 1;
        }
        for (int i = m / 2 + 1; i <= m; i++)
        {
            p[i - 1] = 2 * i - m - 2;
        }
    }
    else
    {
        for (int i = 1; i <= m / 2; i++)
        {
            p[i - 1] = (2 * i + m / 2 - 3) % m;
        }
        for (int i = m / 2 + 1; i <= m; i++)
        {
            p[i - 1] = m - 1 - (2 * (m - i + 1) + m / 2 - 3) % m;
        }
    }
    return p;
}
