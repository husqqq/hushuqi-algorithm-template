#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace lex_two_sat
{
optional<vector<int>> lexTwoSat(int n, const vector<array<int, 4>> &clauses)
{
    // n 是变量数；clauses[i] 四维依次是 u、u 的目标布尔值、v、v 的目标布尔值，表示二者至少一个成立。
    auto feasible = [&](const vector<int> &fix) -> bool
    {
        // fix[x] 为 -1 或强制布尔值；返回加入强制条件后公式是否可满足。
        vector<vector<int>> g(2 * n), rg(2 * n);
        auto add = [&](int u, int v)
        {
            // u、v 是文字编号；加入蕴含 u=>v，无返回值。
            g[u].push_back(v);
            rg[v].push_back(u);
        };
        for (auto [u, a, v, b] : clauses)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && (a == 0 || a == 1) && (b == 0 || b == 1)); // 调试检查，可删。
            int x = 2 * u + a, y = 2 * v + b;
            add(x ^ 1, y);
            add(y ^ 1, x);
        }
        for (int i = 0; i < n; i++)
        {
            if (fix[i] >= 0)
            {
                add(2 * i + (fix[i] ^ 1), 2 * i + fix[i]);
            }
        }
        vector<int> vis(2 * n), ord, id(2 * n, -1);
        auto dfs = [&](auto &&self, int u) -> void
        {
            // self 是递归自身，u 是文字；完成正图后序遍历，无返回值。
            vis[u] = 1;
            for (int v : g[u])
            {
                if (!vis[v])
                {
                    self(self, v);
                }
            }
            ord.push_back(u);
        };
        auto rdfs = [&](auto &&self, int u, int c) -> void
        {
            // self 是递归自身，u 是文字，c 是分量编号；标记反图分量，无返回值。
            id[u] = c;
            for (int v : rg[u])
            {
                if (id[v] < 0)
                {
                    self(self, v, c);
                }
            }
        };
        for (int i = 0; i < 2 * n; i++)
        {
            if (!vis[i])
            {
                dfs(dfs, i);
            }
        }
        reverse(ord.begin(), ord.end());
        int c = 0;
        for (int u : ord)
        {
            if (id[u] < 0)
            {
                rdfs(rdfs, u, c++);
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (id[2 * i] == id[2 * i + 1])
            {
                return false;
            }
        }
        return true;
    };
    vector<int> ans(n, -1);
    if (!feasible(ans))
    {
        return nullopt;
    }
    for (int i = 0; i < n; i++)
    {
        ans[i] = 0;
        if (!feasible(ans))
        {
            ans[i] = 1;
        }
    }
    return ans;
}
}
