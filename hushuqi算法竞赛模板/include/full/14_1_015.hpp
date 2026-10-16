#pragma once

#include "5_6_001.hpp"

namespace factor_ext
{
optional<vector<vector<int>>> extendFactor(vector<vector<int>> a, int m)
{
    // a 是部分染色矩阵，m 是颜色数；成功时返回完整染色，本策略失败时返回空。
    // used[v][c] 标记顶点 v 已用颜色 c，ec[c] 是颜色 c 的已有边数。
    int n = (int)a.size();
    if (m <= 0 || m % 2 == 0 || m == numeric_limits<int>::max() || n > m + 1)
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

        vector<int> ord(m), deg(m); // ord 是增广次序到颜色的映射，deg 是每种颜色的候选数。
        iota(ord.begin(), ord.end(), 0);
        for (int c = 0; c < m; c++)
        {
            deg[c] = n - 2 * ec[c] + (n - ec[c] < cap ? m - n : 0);
        }
        stable_sort(ord.begin(), ord.end(), [&](int x, int y)
        {
            // x、y 是颜色；候选点较少者先增广，同度保持原颜色顺序。
            return deg[x] < deg[y];
        });
        Hungarian match(m, m);
        for (int u = 0; u < m; u++)
        {
            int c = ord[u];
            for (int v = 0; v < n; v++)
            {
                if (!used[v][c]) match.addEdge(u, v);
            }
            if (n - ec[c] < cap)
            {
                for (int v = n; v < m; v++)
                {
                    match.addEdge(u, v);
                }
            }
        }
        if (match.matching() != m) return nullopt;
        const auto &mt = match.matchR(); // 右点匹配的是增广次序，需经 ord 还原颜色。

        for (auto &r : a)
        {
            r.push_back(-1);
        }
        a.push_back(vector<int>(n + 1, -1));
        used.push_back(vector<char>(m));
        for (int v = 0; v < n; v++)
        {
            int c = ord[mt[v]];
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
} // 命名空间 factor_ext
