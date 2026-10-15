#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct YenPaths
{
    struct Edge { int u, v; long long w; };
    struct Path
    {
        __int128 cost = 0; vector<int> vertices, edges;
        bool operator<(const Path &o) const { return tie(cost, edges) < tie(o.cost, o.edges); }
    };
    int n; vector<Edge> edges; vector<vector<int>> adj;
    explicit YenPaths(int n) : n(n), adj(n) {}
    int add(int u, int v, long long w)
    {
        if (u < 0 || u >= n || v < 0 || v >= n || w < 0) throw invalid_argument("invalid edge");
        int id = edges.size(); edges.push_back({u, v, w}); adj[u].push_back(id); return id;
    }
    optional<Path> shortest(int s, int t, const vector<char> &banV, const vector<char> &banE) const
    {
        const __int128 INF = (__int128)1 << 120;
        vector<__int128> d(n, INF); vector<int> prev(n, -1); vector<char> done(n);
        priority_queue<pair<__int128, int>, vector<pair<__int128, int>>, greater<>> q;
        if (banV[s] || banV[t]) return nullopt;
        d[s] = 0; q.push({0, s});
        while (!q.empty())
        {
            auto [dist, u] = q.top(); q.pop(); if (done[u]) continue; done[u] = true; if (u == t) break;
            for (int id : adj[u])
            {
                auto e = edges[id]; if (banE[id] || banV[e.v] || done[e.v]) continue;
                if (dist + e.w < d[e.v]) { d[e.v] = dist + e.w; prev[e.v] = id; q.push({d[e.v], e.v}); }
            }
        }
        if (d[t] == INF) return nullopt;
        Path path; path.cost = d[t];
        for (int u = t;;) { path.vertices.push_back(u); if (u == s) break; int id = prev[u]; path.edges.push_back(id); u = edges[id].u; }
        reverse(path.vertices.begin(), path.vertices.end()); reverse(path.edges.begin(), path.edges.end()); return path;
    }
    vector<Path> solve(int s, int t, int k) const
    {
        if (s < 0 || s >= n || t < 0 || t >= n) throw invalid_argument("invalid endpoint");
        vector<Path> ans; if (k <= 0) return ans;
        vector<char> banV(n), banE(edges.size()); auto first = shortest(s, t, banV, banE);
        if (!first) return ans; ans.push_back(*first); set<Path> cand; set<vector<int>> seen; seen.insert(first->edges);
        while ((int)ans.size() < k)
        {
            const Path &last = ans.back(); __int128 rootCost = 0;
            fill(banV.begin(), banV.end(), false);
            for (int i = 0; i < (int)last.edges.size(); ++i)
            {
                fill(banE.begin(), banE.end(), false);
                for (const Path &p : ans) if ((int)p.edges.size() > i && equal(last.edges.begin(), last.edges.begin() + i, p.edges.begin())) banE[p.edges[i]] = true;
                auto spur = shortest(last.vertices[i], t, banV, banE);
                if (spur)
                {
                    Path p; p.cost = rootCost + spur->cost;
                    p.edges.assign(last.edges.begin(), last.edges.begin() + i); p.edges.insert(p.edges.end(), spur->edges.begin(), spur->edges.end());
                    p.vertices.assign(last.vertices.begin(), last.vertices.begin() + i); p.vertices.insert(p.vertices.end(), spur->vertices.begin(), spur->vertices.end());
                    if (seen.insert(p.edges).second) cand.insert(move(p));
                }
                banV[last.vertices[i]] = true; rootCost += edges[last.edges[i]].w;
            }
            if (cand.empty()) break; ans.push_back(*cand.begin()); cand.erase(cand.begin());
        }
        return ans;
    }
};
