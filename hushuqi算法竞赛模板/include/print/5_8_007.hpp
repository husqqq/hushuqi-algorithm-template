#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct GlobalCut
{
    // 最小割值与割的一侧。
    int w;
    vector<int> side;
};

GlobalCut stoerWagner(vector<vector<int>> g)
{
    // g 是无向图的对称非负权矩阵；返回全局最小割值和一侧点集。
    int n = g.size();
    if (n <= 1)
    {
        return {0, n ? vector<int>{0} : vector<int>{}};
    }
    for (int i = 0; i < n; i++)
    {
        assert((int)g[i].size() == n); // 调试检查，可删。
        for (int j = 0; j < n; j++)
        {
            assert(g[i][j] >= 0 && g[i][j] == g[j][i]); // 调试检查，可删。
        }
        g[i][i] = 0;
    }
    vector<int> v(n);
    vector<vector<int>> part(n);
    iota(v.begin(), v.end(), 0);
    for (int i = 0; i < n; i++)
    {
        part[i] = {i};
    }
    GlobalCut ans{numeric_limits<int>::max(), {}};
    while (v.size() > 1)
    {
        vector<int> w(n);
        vector<bool> used(n);
        int s = -1, t = -1;
        for (int z = 0; z < (int)v.size(); z++)
        {
            s = t;
            t = -1;
            for (int u : v)
            {
                if (!used[u] && (t < 0 || w[u] > w[t]))
                {
                    t = u;
                }
            }
            if (z + 1 == (int)v.size())
            {
                if (w[t] < ans.w)
                {
                    ans = {w[t], part[t]};
                }
                for (int u : v)
                {
                    if (u != s)
                    {
                        g[s][u] += g[t][u];
                        g[u][s] = g[s][u];
                    }
                }
                part[s].insert(part[s].end(), part[t].begin(), part[t].end());
                v.erase(find(v.begin(), v.end(), t));
                break;
            }
            used[t] = true;
            for (int u : v)
            {
                w[u] += g[t][u];
            }
        }
    }
    return ans;
}
