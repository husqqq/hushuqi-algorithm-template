#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Steiner
{
    struct E
    {
        // to 是邻点编号。
        int to;
        // w 是非负边权。
        int w;
    };

    // n 是图的点数。
    int n;
    // e 是无向图邻接表。
    vector<vector<E>> e;
    // ed[i][0..2] 依次是第 i 条原边的两个端点和非负边权。
    vector<array<int, 3>> ed;

    Steiner(int n = 0) : n(n), e(n)
    {
        // n 是图的点数；构造一张没有边的图。
    }

    void add(int u, int v, int w)
    {
        // u、v 是端点，w 是非负边权；加入一条无向边。
        assert(w >= 0); // 调试检查，可删。
        e[u].push_back({v, w});
        e[v].push_back({u, w});
        ed.push_back({u, v, w});
    }

    optional<int> solve(const vector<int> &key) const
    {
        // key 是关键点编号列表；返回连接全部关键点的最小权值，不可连通时返回空。
        size_t k = key.size();
        if (k == 0)
        {
            return 0;
        }
        assert(k < 63); // 调试检查，可删。
        size_t z = (size_t)1 << k;
        vector f(z, vector<optional<int>>(n));
        for (size_t i = 0; i < k; i++)
        {
            assert(0 <= key[i] && key[i] < n); // 调试检查，可删。
            f[(size_t)1 << i][key[i]] = 0;
        }
        for (size_t s = 1; s < z; s++)
        {
            for (size_t a = (s - 1) & s; a; a = (a - 1) & s)
            {
                size_t b = s ^ a;
                if (!b || a > b)
                {
                    continue;
                }
                for (int u = 0; u < n; u++)
                {
                    if (f[a][u] && f[b][u])
                    {
                        int nd = *f[a][u] + *f[b][u];
                        if (!f[s][u] || nd < *f[s][u])
                        {
                            f[s][u] = nd;
                        }
                    }
                }
            }
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
            for (int u = 0; u < n; u++)
            {
                if (f[s][u])
                {
                    q.push({*f[s][u], u});
                }
            }
            while (!q.empty())
            {
                auto [d, u] = q.top();
                q.pop();
                if (!f[s][u] || d != *f[s][u])
                {
                    continue;
                }
                for (auto [v, w] : e[u])
                {
                    int nd = d + w;
                    if (!f[s][v] || nd < *f[s][v])
                    {
                        f[s][v] = nd;
                        q.push({nd, v});
                    }
                }
            }
        }
        optional<int> ans;
        for (auto x : f.back())
        {
            if (x && (!ans || *x < *ans))
            {
                ans = *x;
            }
        }
        return ans;
    }

    optional<pair<int, vector<int>>> solveTree(const vector<int> &key) const
    {
        // key 是互异关键点编号；返回最小权值和所选原边编号，图不连通时返回空。
        int k = key.size();
        if (k == 0)
        {
            return pair<int, vector<int>>{0, {}};
        }
        assert(k < 20); // 调试检查，可删：本算法指数依赖关键点数。
        int lim = 1 << k;
        vector<vector<array<int, 3>>> g(n);
        for (int i = 0; i < (int)ed.size(); i++)
        {
            auto [u, v, w] = ed[i];
            g[u].push_back({v, w, i});
            g[v].push_back({u, w, i});
        }
        vector dp(lim, vector<optional<int>>(n));
        // pre[s][u][0] 是合并子集或前驱点，pre[s][u][1] 是边编号；-1 表示终端基态。
        vector pre(lim, vector<array<int, 2>>(n, {-1, -1}));
        for (int i = 0; i < k; i++)
        {
            assert(0 <= key[i] && key[i] < n); // 调试检查，可删。
            dp[1 << i][key[i]] = 0;
        }
        for (int s = 1; s < lim; s++)
        {
            if (s & (s - 1))
            {
                for (int a = (s - 1) & s; a; a = (a - 1) & s)
                {
                    int b = s ^ a;
                    if (!b || a > b) continue;
                    for (int u = 0; u < n; u++)
                    {
                        if (dp[a][u] && dp[b][u])
                        {
                            int nd = *dp[a][u] + *dp[b][u];
                            if (!dp[s][u] || nd < *dp[s][u])
                            {
                                dp[s][u] = nd;
                                pre[s][u] = {a, -1};
                            }
                        }
                    }
                }
            }
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
            for (int u = 0; u < n; u++) if (dp[s][u]) q.push({*dp[s][u], u});
            while (!q.empty())
            {
                auto [d, u] = q.top();
                q.pop();
                if (!dp[s][u] || d != *dp[s][u]) continue;
                for (auto [v, w, id] : g[u])
                {
                    int nd = d + w;
                    if (!dp[s][v] || nd < *dp[s][v])
                    {
                        dp[s][v] = nd;
                        pre[s][v] = {u, id};
                        q.push({nd, v});
                    }
                }
            }
        }
        int all = lim - 1, at = -1;
        for (int u = 0; u < n; u++) if (dp[all][u] && (at == -1 || *dp[all][u] < *dp[all][at])) at = u;
        if (at == -1) return nullopt;
        vector<bool> used(ed.size());
        auto trace = [&](auto &&self, int s, int u) -> void
        {
            // s 是待连接终端集合，u 是当前端点；标记该状态的一棵最优树边集。
            auto [a, id] = pre[s][u];
            if (a == -1) return;
            if (id == -1)
            {
                self(self, a, u);
                self(self, s ^ a, u);
            }
            else
            {
                used[id] = true;
                self(self, s, a);
            }
        };
        trace(trace, all, at);
        vector<int> take;
        for (int i = 0; i < (int)ed.size(); i++) if (used[i]) take.push_back(i);
        return pair<int, vector<int>>{*dp[all][at], take};
    }
};
