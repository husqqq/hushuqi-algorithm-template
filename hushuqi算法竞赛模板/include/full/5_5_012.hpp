#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct MinDiamTree
{
    struct Edge
    {
        int to;
        long long w;
    };

    struct State
    {
        long long d;
        int u, fa;

        bool operator>(const State &o) const
        {
            return tie(d, u, fa) > tie(o.d, o.u, o.fa);
        }
    };

    int n;
    vector<vector<Edge>> g;
    bool valid = true;
    __int128 totalWeight2 = 0;

    MinDiamTree(int n_) : n(max(0LL, n_)), g(max(0LL, n_))
    {
        valid = n_ > 0;
    }

    bool addEdge(int u, int v, long long w)
    {
        if (u < 0 || u >= n || v < 0 || v >= n || w < 0 ||
            (__int128)w * 2 + totalWeight2 >= numeric_limits<long long>::max() / 4)
        {
            valid = false;
            return false;
        }
        totalWeight2 += (__int128)w * 2;
        g[u].push_back({v, w * 2});
        g[v].push_back({u, w * 2});
        return true;
    }

    pair<vector<long long>, vector<int>> dijkstra(int s, const vector<vector<Edge>> &a) const
    {
        int m = a.size();
        const long long INF = numeric_limits<long long>::max() / 4;
        vector<long long> dis(m, INF);
        vector<int> fa(m, -1);
        vector<char> vis(m);
        priority_queue<State, vector<State>, greater<State>> q;
        q.push({0, s, s});
        while (!q.empty())
        {
            auto [d, u, p] = q.top();
            q.pop();
            if (vis[u])
            {
                continue;
            }
            vis[u] = true;
            dis[u] = d;
            fa[u] = p;
            for (auto [v, w] : a[u])
            {
                if (!vis[v])
                {
                    if (d <= INF - w) q.push({d + w, v, u});
                }
            }
        }
        return {dis, fa};
    }

    optional<pair<long long, vector<pair<int, int>>>> solve() const
    {
        if (!valid || n == 0) return nullopt;
        const long long INF = numeric_limits<long long>::max() / 4;
        vector<vector<long long>> dis(n);
        vector<vector<int>> ord(n, vector<int>(n));
        long long bestP = INF;
        int center = 0;
        for (int s = 0; s < n; s++)
        {
            dis[s] = dijkstra(s, g).first;
            iota(ord[s].begin(), ord[s].end(), 0);
            stable_sort(ord[s].begin(), ord[s].end(), [&](int x, int y)
            {
                return dis[s][x] > dis[s][y];
            });
            if (dis[s][ord[s][0]] >= INF) return nullopt;
            long long cur = dis[s][ord[s][0]] * 2;
            if (cur < bestP)
            {
                bestP = cur;
                center = s;
            }
        }

        long long bestEdge = INF, left = 0, right = 0;
        int cu = 0, cv = 0;
        for (int u = 0; u < n; u++)
        {
            for (auto [v, w] : g[u])
            {
                if (v <= u)
                {
                    continue;
                }
                int y = ord[u][0];
                for (int x : ord[u])
                {
                    if (dis[v][x] > dis[v][y])
                    {
                        long long cur = dis[v][y] + dis[u][x] + w;
                        if (cur < bestEdge)
                        {
                            bestEdge = cur;
                            cu = u;
                            cv = v;
                            left = cur / 2 - dis[u][x];
                            right = w - left;
                        }
                        y = x;
                    }
                }
            }
        }

        vector<vector<Edge>> a = g;
        int root = center;
        bool inside = bestEdge < bestP;
        if (inside)
        {
            root = n;
            a.push_back({});
            a[root].push_back({cu, left});
            a[root].push_back({cv, right});
        }
        auto fa = dijkstra(root, a).second;
        vector<pair<int, int>> tree;
        if (inside)
        {
            for (int u = 0; u < n; u++)
            {
                if (u != cu && u != cv)
                {
                    tree.push_back({u, fa[u]});
                }
            }
            tree.push_back({cu, cv});
        }
        else
        {
            for (int u = 0; u < n; u++)
            {
                if (u != root)
                {
                    tree.push_back({u, fa[u]});
                }
            }
        }
        return pair<long long, vector<pair<int, int>>>{min(bestP, bestEdge) / 2, move(tree)};
    }
};
