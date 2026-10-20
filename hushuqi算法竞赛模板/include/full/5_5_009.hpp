#pragma once

#include "5_2_001.hpp"

struct Steiner
{
    // n 是图的点数。
    int n;
    // e[u] 每项依次为邻点、非负边权。
    vector<vector<array<int, 2>>> e;
    // ed[i][0..2] 依次是第 i 条原边的两个端点和非负边权。
    vector<array<int, 3>> ed;

    Steiner(int n = 0) : n(n), e(n)
    {
        // n>=0 是点数；构造一张没有边的图。
    }

    void add(int u, int v, int w)
    {
        // u、v 是 [0,n) 内端点，w>=0；加入无向边，原边编号为插入次序，允许重边和自环，无返回值。
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        e[u].push_back({v, w});
        e[v].push_back({u, w});
        ed.push_back({u, v, w});
    }

    optional<int> solve(const vector<int> &key) const
    {
        // key 是 [0,n) 内的终端列表；返回最小连通边权和，空列表返回 0，终端不连通时返回空。
        // 所有合并与松弛的中间和须能用 int 表示；重复终端允许，但会重复占用状态位。
        int k = key.size();
        if (k == 0)
        {
            return 0;
        }
        assert(k < numeric_limits<int>::digits); // 调试检查，可删。
        int z = (int)1 << k;
        vector f(z, vector<optional<int>>(n)); // f[s][u]：连接终端集 s 并包含 u 的最小权值。
        for (int i = 0; i < k; i++)
        {
            assert(0 <= key[i] && key[i] < n); // 调试检查，可删。
            f[(int)1 << i][key[i]] = 0;
        }
        for (int s = 1; s < z; s++)
        {
            for (int a = (s - 1) & s; a; a = (a - 1) & s)
            {
                int b = s ^ a;
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
            dijkstraInit(e, f[s], [](int, const array<int, 2> &)
            {
                // 仅求当前子集的最小代价，无需记录前驱。
            });
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
        // key 与数值范围同 solve；返回 {最小权值, 升序且无重复的原边编号}，终端不连通时返回空。
        // 空终端列表返回 {0,{}}；图中其余连通块不影响结果。
        int k = key.size();
        if (k == 0)
        {
            return pair<int, vector<int>>{0, {}};
        }
        assert(k < numeric_limits<int>::digits); // 状态数须能用 int 表示，内存另由调用者保证。
        int lim = (int)1 << k;
        vector<vector<array<int, 3>>> g(n); // 邻接项依次为邻点、边权、原边编号。
        for (int i = 0; i < (int)ed.size(); i++)
        {
            auto [u, v, w] = ed[i];
            g[u].push_back({v, w, i});
            g[v].push_back({u, w, i});
        }
        vector dp(lim, vector<optional<int>>(n)); // 与 solve 的状态相同，另记录最优转移。
        // pre[s][u]：{子集,-1} 表示合并，{前驱点,边号} 表示松弛，{-1,-1} 表示终端基态。
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
            dijkstraInit(g, dp[s], [&](int u, const array<int, 3> &edge)
            {
                // u 是前驱，edge 依次为终点、边权、原边编号；保存当前最优转移。
                pre[s][edge[0]] = {u, edge[2]};
            });
        }
        int all = lim - 1, at = -1;
        for (int u = 0; u < n; u++)
        {
            if (dp[all][u] && (at == -1 || *dp[all][u] < *dp[all][at])) at = u;
        }
        if (at == -1) return nullopt;
        vector<bool> used(ed.size()); // 每条原边是否出现在恢复方案中。
        auto trace = [&](auto &&self, int s, int u) -> void
        {
            // self 是递归自身，s 是终端子集，u 是当前端点；标记该状态的最优树边集。
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
        for (int i = 0; i < (int)ed.size(); i++)
        {
            if (used[i]) take.push_back(i);
        }
        return pair<int, vector<int>>{*dp[all][at], take};
    }
};
