#pragma once

#include "1_1_008.hpp"

int rankField(vector<vector<Z>> a)
{
    // a 是有限域上的矩阵；返回矩阵秩。
    int n = a.size(), m = n ? a[0].size() : 0, r = 0;
    for (int c = 0; c < m && r < n; c++)
    {
        int p = r;
        while (p < n && a[p][c] == Z(0))
        {
            p++;
        }
        if (p == n)
        {
            continue;
        }
        swap(a[p], a[r]);
        Z iv = a[r][c].inv();
        for (int i = r + 1; i < n; i++)
        {
            Z q = a[i][c] * iv;
            for (int j = c; j < m; j++)
            {
                a[i][j] -= q * a[r][j];
            }
        }
        r++;
    }
    return r;
}

int tutteMatch(int n, const vector<pair<int, int>> &e, unsigned seed = 712367)
{
    // n 是点数，e 是无向边端点列表，seed 是固定随机种子；返回最大匹配大小，存在有限域随机误判概率。
    mt19937 rng(seed);
    vector a(n, vector<Z>(n));
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        if (u == v)
        {
            continue;
        }
        Z x = rng() % (Z::mod() - 1) + 1;
        a[u][v] += x;
        a[v][u] -= x;
    }
    return rankField(move(a)) / 2;
}
