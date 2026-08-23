#pragma once

#include "5_3_001.hpp"

vector<int> incrSCC(int n, const vector<pair<int, int>> &e)
{
    int m = e.size();
    vector<int> ans(m, m + 1), id(n, -1);
    vector<tuple<int, int, int>> all;
    for (int i = 0; i < m; i++)
    {
        auto [u, v] = e[i];
        assert(0 <= u && u < n && 0 <= v && v < n);
        all.push_back({i, u, v});
    }
    auto solve = [&](auto &&self, const vector<tuple<int, int, int>> &a, int l, int r) -> void
    {
        if (a.empty() || r == l + 1)
        {
            return;
        }
        int mid = (l + r) / 2;
        int cnt = 0;
        for (auto [i, u, v] : a)
        {
            if (id[u] == -1)
            {
                id[u] = cnt++;
            }
            if (id[v] == -1)
            {
                id[v] = cnt++;
            }
        }
        vector<vector<int>> g(cnt);
        for (auto [i, u, v] : a)
        {
            if (i < mid)
            {
                g[id[u]].push_back(id[v]);
            }
        }
        vector<int> bel = tarjanSCC(g);
        vector<tuple<int, int, int>> left, right;
        for (auto [i, u, v] : a)
        {
            int x = id[u], y = id[v];
            if (i < mid && bel[x] == bel[y])
            {
                ans[i] = min(ans[i], mid);
                left.push_back({i, x, y});
            }
            else
            {
                right.push_back({i, bel[x], bel[y]});
            }
        }
        for (auto [i, u, v] : a)
        {
            id[u] = id[v] = -1;
        }
        self(self, left, l, mid);
        self(self, right, mid, r);
    };
    solve(solve, all, 0, m + 1);
    return ans;
}
