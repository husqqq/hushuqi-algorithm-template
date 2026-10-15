#pragma once

#include "5_2_002.hpp"

template <class Dist = long long> struct Johnson
{
    // n 是点数，e 是全部有向边。
    int n;
    vector<DEdge> e;

    explicit Johnson(int n) : n(n)
    {
        // n 是点数；构造一张没有边的图。
    }

    void addEdge(int u, int v, int w)
    {
        // u、v 是端点，w 是边权；加入一条有向边。
        e.push_back({u, v, w});
    }

    optional<vector<vector<optional<Dist>>>> solve() const
    {
        // 无参数；返回全点对最短距离，图含负环时返回空。
        vector<Dist> h(n);
        for (int k = 0; k < n; k++)
        {
            bool upd = false;
            for (auto [u, v, w] : e)
            {
                Dist nh = h[u] + (Dist)w;
                if (h[v] > nh)
                {
                    h[v] = nh;
                    upd = true;
                }
            }
            if (!upd)
            {
                break;
            }
            if (k == n - 1)
            {
                return nullopt;
            }
        }
        vector<vector<pair<int, Dist>>> g(n);
        for (auto [u, v, w] : e)
        {
            Dist nw = (Dist)w + h[u] - h[v];
            assert(nw >= 0); // 调试检查，可删：势函数应使重标权非负。
            g[u].push_back({v, nw});
        }
        vector ans(n, vector<optional<Dist>>(n));
        for (int s = 0; s < n; s++)
        {
            priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
            ans[s][s] = 0;
            q.push({0, s});
            while (!q.empty())
            {
                auto [du, u] = q.top();
                q.pop();
                if (!ans[s][u].has_value() || du != *ans[s][u])
                {
                    continue;
                }
                for (auto [v, w] : g[u])
                {
                    Dist nd = du + w;
                    if (!ans[s][v].has_value() || nd < *ans[s][v])
                    {
                        ans[s][v] = nd;
                        q.push({nd, v});
                    }
                }
            }
            for (int v = 0; v < n; v++)
            {
                if (ans[s][v].has_value())
                {
                    ans[s][v] = *ans[s][v] + h[v] - h[s];
                }
            }
        }
        return ans;
    }
};
