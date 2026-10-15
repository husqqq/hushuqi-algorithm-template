#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct YenPaths
{
    struct Path
    {
        __int128 cost = 0;    // cost 是路径边权和。
        vector<int> vertices; // vertices 是从起点到终点的顶点序列。
        vector<int> edges;    // edges 是依次经过的边编号，平行边据此区分。

        bool operator<(const Path &o) const
        {
            // 先比代价，再比边编号序列，使候选集合有确定的顺序。
            return tie(cost, edges) < tie(o.cost, o.edges);
        }
    };

    int n; // n 是点数。
    vector<array<int, 3>> edges; // edges[id] 的三维依次是有向边的起点、终点和非负边权。
    vector<vector<int>> adj; // adj[u] 是从 u 出发的边编号。

    YenPaths(int n) : n(n), adj(n)
    {
        // n 是点数，点编号为 [0,n)；构造空图。
    }

    int add(int u, int v, int w)
    {
        // 加入有向边 u->v，w 为非负边权；返回边编号。
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        edges.push_back({u, v, w});
        adj[u].push_back(edges.size() - 1);
        return edges.size() - 1;
    }

    optional<Path> shortest(int s, int t, const vector<char> &banV, const vector<char> &banE) const
    {
        // 在删去 banV 中的点和 banE 中的边后求 s 到 t 的最短路；不可达时返回空。
        const __int128 INF = (__int128)1 << 120;
        if (banV[s] || banV[t])
        {
            return nullopt;
        }
        vector<__int128> d(n, INF);
        vector<int> pre(n, -1); // pre[v] 是最短路树上进入 v 的边编号。
        vector<char> done(n);
        priority_queue<pair<__int128, int>, vector<pair<__int128, int>>, greater<>> q;
        d[s] = 0;
        q.push({0, s});
        while (!q.empty())
        {
            auto [du, u] = q.top();
            q.pop();
            if (done[u])
            {
                continue;
            }
            done[u] = true;
            if (u == t)
            {
                break;
            }
            for (int id : adj[u])
            {
                auto [x, v, w] = edges[id];
                if (banE[id] || banV[v] || done[v])
                {
                    continue;
                }
                if (du + w < d[v])
                {
                    d[v] = du + w;
                    pre[v] = id;
                    q.push({d[v], v});
                }
            }
        }
        if (d[t] == INF)
        {
            return nullopt;
        }
        Path p;
        p.cost = d[t];
        for (int u = t;; u = edges[pre[u]][0])
        {
            p.vertices.push_back(u);
            if (u == s)
            {
                break;
            }
            p.edges.push_back(pre[u]);
        }
        reverse(p.vertices.begin(), p.vertices.end());
        reverse(p.edges.begin(), p.edges.end());
        return p;
    }

    vector<Path> solve(int s, int t, int k) const
    {
        // 返回 s 到 t 代价最小的前 k 条互不相同的简单路径，按代价升序；不足 k 条时全部返回。
        assert(0 <= s && s < n && 0 <= t && t < n); // 调试检查，可删。
        vector<Path> ans;
        vector<char> banV(n), banE(edges.size());
        auto first = shortest(s, t, banV, banE);
        if (k <= 0 || !first)
        {
            return ans;
        }
        ans.push_back(*first);
        set<Path> cand;               // cand 是尚未输出的候选路径。
        set<vector<int>> seen{first->edges}; // seen 是已生成过的边序列，避免重复入候选。
        while ((int)ans.size() < k)
        {
            const Path &last = ans.back();
            __int128 root = 0; // root 是 last 前 i 条边的代价，即根路径代价。
            fill(banV.begin(), banV.end(), 0);
            for (int i = 0; i < (int)last.edges.size(); i++)
            {
                // 偏离点是 last.vertices[i]：禁掉所有共享这段根路径的已选路径的下一条边，
                // 根路径上更早的点已在 banV 中，保证拼接后仍是简单路径。
                fill(banE.begin(), banE.end(), 0);
                for (const Path &p : ans)
                {
                    if ((int)p.edges.size() > i && equal(last.edges.begin(), last.edges.begin() + i, p.edges.begin()))
                    {
                        banE[p.edges[i]] = 1;
                    }
                }
                auto spur = shortest(last.vertices[i], t, banV, banE);
                if (spur)
                {
                    Path p;
                    p.cost = root + spur->cost;
                    p.edges.assign(last.edges.begin(), last.edges.begin() + i);
                    p.edges.insert(p.edges.end(), spur->edges.begin(), spur->edges.end());
                    p.vertices.assign(last.vertices.begin(), last.vertices.begin() + i);
                    p.vertices.insert(p.vertices.end(), spur->vertices.begin(), spur->vertices.end());
                    if (seen.insert(p.edges).second)
                    {
                        cand.insert(move(p));
                    }
                }
                banV[last.vertices[i]] = 1;
                root += edges[last.edges[i]][2];
            }
            if (cand.empty())
            {
                break;
            }
            ans.push_back(*cand.begin());
            cand.erase(cand.begin());
        }
        return ans;
    }
};
