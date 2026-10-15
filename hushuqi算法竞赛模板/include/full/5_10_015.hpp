#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace ramsey
{
optional<array<int, 3>> monoTriangle(const vector<vector<int>> &col)
{
    // col 是完全图的对称 0/1 边染色；返回一个单色三角形，不存在时返回空。
    int n = col.size();
    for (int i = 0; i < n; i++)
    {
        assert((int)col[i].size() == n); // 调试检查，可删。
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            assert((col[i][j] == 0 || col[i][j] == 1) &&
                   col[i][j] == col[j][i]); // 调试检查，可删。
            for (int k = j + 1; k < n; k++)
            {
                if (col[i][j] == col[i][k] && col[i][j] == col[j][k])
                {
                    return array<int, 3>{i, j, k};
                }
            }
        }
    }
    return nullopt;
}
}
