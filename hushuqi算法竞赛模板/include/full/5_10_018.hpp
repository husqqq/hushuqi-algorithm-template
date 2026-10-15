#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<int>> graphIso(const vector<vector<int>> &a, const vector<vector<int>> &b)
{
    // a、b 是方阵邻接矩阵（可有向、带自环与重边计数）；同构时返回映射 p，
    // 满足 a[u][v]==b[p[u]][p[v]]，否则返回空。
    int n = a.size();
    if ((int)b.size() != n)
    {
        return nullopt;
    }
    // da[i]、db[i] 是 (出度和, 入度和)，只在度数相同的点之间尝试匹配。
    vector<pair<__int128, __int128>> da(n), db(n);
    for (int i = 0; i < n; i++)
    {
        assert((int)a[i].size() == n && (int)b[i].size() == n); // 调试检查，可删。
        for (int j = 0; j < n; j++)
        {
            da[i].first += a[i][j];
            da[i].second += a[j][i];
            db[i].first += b[i][j];
            db[i].second += b[j][i];
        }
    }
    vector<int> p(n, -1), order(n);
    vector<char> used(n);
    // 度数大的点先定，剪枝更早生效。
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y)
    {
        return da[x] > da[y];
    });
    auto dfs = [&](auto &&self, int k) -> bool
    {
        // 已为 order[0..k) 确定像，尝试为 order[k] 选像。
        if (k == n)
        {
            return true;
        }
        int u = order[k];
        for (int v = 0; v < n; v++)
        {
            if (used[v] || da[u] != db[v] || a[u][u] != b[v][v])
            {
                continue;
            }
            bool ok = true;
            for (int j = 0; j < k && ok; j++)
            {
                int x = order[j];
                ok = a[u][x] == b[v][p[x]] && a[x][u] == b[p[x]][v];
            }
            if (!ok)
            {
                continue;
            }
            p[u] = v;
            used[v] = true;
            if (self(self, k + 1))
            {
                return true;
            }
            used[v] = false;
        }
        return false;
    };
    if (dfs(dfs, 0))
    {
        return p;
    }
    return nullopt;
}

pair<vector<int>, vector<int>> graphCanon(const vector<vector<int>> &a)
{
    // a 是 n<=10 的方阵邻接矩阵；枚举全部顶点排列，返回字典序最小的重排矩阵（按行展开）
    // 以及取到它的顶点顺序。两图同构当且仅当规范编码相同。
    int n = a.size();
    assert(n <= 10); // 调试检查，可删。
    vector<int> p(n), best, order;
    iota(p.begin(), p.end(), 0);
    do
    {
        vector<int> code;
        for (int u : p)
        {
            for (int v : p)
            {
                code.push_back(a[u][v]);
            }
        }
        if (order.empty() || code < best)
        {
            best = move(code);
            order = p;
        }
    } while (next_permutation(p.begin(), p.end()));
    return {best, order};
}
