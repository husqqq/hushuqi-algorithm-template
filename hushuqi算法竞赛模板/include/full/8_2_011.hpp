#pragma once

#include "1_1_008.hpp"

Z detField(vector<vector<Z>> a)
{
    // a 是有限域上的方阵；返回其行列式。
    int n = a.size();
    for (const auto &row : a)
    {
        assert((int)row.size() == n); // 调试检查，可删。
    }
    Z ans = 1;
    for (int c = 0; c < n; c++)
    {
        int p = c;
        while (p < n && a[p][c] == Z(0))
        {
            p++;
        }
        if (p == n)
        {
            return 0;
        }
        if (p != c)
        {
            swap(a[p], a[c]);
            ans = -ans;
        }
        ans *= a[c][c];
        Z iv = a[c][c].inv();
        for (int i = c + 1; i < n; i++)
        {
            Z q = a[i][c] * iv;
            for (int j = c; j < n; j++)
            {
                a[i][j] -= q * a[c][j];
            }
        }
    }
    return ans;
}

Z spanTreeCnt(int n, const vector<pair<int, int>> &e)
{
    // n 是点数，e 是无向边端点列表且允许重边；返回生成树数量。
    assert(n >= 0); // 调试检查，可删。
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
    }
    if (n <= 1)
    {
        return 1;
    }
    vector a(n - 1, vector<Z>(n - 1));
    for (auto [u, v] : e)
    {
        if (u < n - 1)
        {
            a[u][u] += 1;
        }
        if (v < n - 1)
        {
            a[v][v] += 1;
        }
        if (u < n - 1 && v < n - 1)
        {
            a[u][v] -= 1;
            a[v][u] -= 1;
        }
    }
    return detField(move(a));
}

Z arboCount(int n, int root, const vector<pair<int, int>> &e)
{
    // n 是点数，root 是根，e 是有向边端点列表且允许重边；返回从 root 向外的生成树数量。
    assert(n >= 1 && 0 <= root && root < n); // 调试检查，可删。
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
    }
    if (n == 1)
    {
        return 1;
    }
    vector<int> id(n, -1);
    for (int u = 0, k = 0; u < n; u++)
    {
        if (u != root)
        {
            id[u] = k++;
        }
    }
    vector a(n - 1, vector<Z>(n - 1));
    for (auto [u, v] : e)
    {
        if (v == root)
        {
            continue;
        }
        a[id[v]][id[v]] += 1;
        if (u != root)
        {
            a[id[v]][id[u]] -= 1;
        }
    }
    return detField(move(a));
}
