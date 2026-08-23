#pragma once

#include "14_1_001.hpp"

namespace factor_ext
{
optional<vector<vector<int>>> extendFactor(vector<vector<int>> a, int m)
{
    // a 是部分染色矩阵，m 是颜色数；成功时返回完整染色，本策略失败时返回空。
    int n = (int)a.size();
    if (m <= 0 || m % 2 == 0 || n > m + 1)
    {
        return nullopt;
    }
    for (auto &r : a)
    {
        if ((int)r.size() != n)
        {
            return nullopt;
        }
    }
    vector<vector<char>> used(n, vector<char>(m));
    vector<int> ec(m);
    for (int i = 0; i < n; i++)
    {
        if (a[i][i] != -1)
        {
            return nullopt;
        }
        for (int j = i + 1; j < n; j++)
        {
            int c = a[i][j];
            if (c < 0 || c >= m || a[j][i] != c)
            {
                return nullopt;
            }
            if (used[i][c] || used[j][c])
            {
                return nullopt;
            }
            used[i][c] = true;
            used[j][c] = true;
            ec[c]++;
        }
    }

    int cap = (m + 1) / 2;
    while (n < m + 1)
    {
        for (int c = 0; c < m; c++)
        {
            if (n - ec[c] > cap)
            {
                return nullopt;
            }
        }

        vector<vector<int>> g(m);
        for (int c = 0; c < m; c++)
        {
            for (int v = 0; v < n; v++)
            {
                if (!used[v][c])
                {
                    g[c].push_back(v);
                }
            }
            if (n - ec[c] < cap)
            {
                for (int v = n; v < m; v++)
                {
                    g[c].push_back(v);
                }
            }
        }

        vector<int> mt(m, -1);
        vector<int> ord(m);
        iota(ord.begin(), ord.end(), 0);
        stable_sort(ord.begin(), ord.end(), [&](int x, int y)
        {
            return g[x].size() < g[y].size();
        });
        auto dfs = [&](auto &&self, int c, vector<char> &vis) -> bool
        {
            for (int v : g[c])
            {
                if (vis[v])
                {
                    continue;
                }
                vis[v] = true;
                if (mt[v] == -1 || self(self, mt[v], vis))
                {
                    mt[v] = c;
                    return true;
                }
            }
            return false;
        };
        for (int c : ord)
        {
            vector<char> vis(m);
            if (!dfs(dfs, c, vis))
            {
                return nullopt;
            }
        }

        for (auto &r : a)
        {
            r.push_back(-1);
        }
        a.push_back(vector<int>(n + 1, -1));
        used.push_back(vector<char>(m));
        for (int v = 0; v < n; v++)
        {
            int c = mt[v];
            a[v][n] = c;
            a[n][v] = c;
            used[v][c] = true;
            used[n][c] = true;
            ec[c]++;
        }
        n++;
    }
    return a;
}
}
