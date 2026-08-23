#include "../include/full/6_3_005.hpp"
#include "../include/full/6_5_006.hpp"

signed main()
{
    mt19937_64 rng(0);
    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 30 + 1;
        int root = rng() % n;
        vector<vector<int>> tree(n);
        vector<vector<RerootEdge>> g(n);
        vector<int> weight;
        for (int v = 1; v < n; v++)
        {
            int u = rng() % v;
            int edgeId = weight.size();
            int w = rng() % 10;
            weight.push_back(w);
            tree[u].push_back(v);
            tree[v].push_back(u);
            g[u].push_back({v, edgeId});
            g[v].push_back({u, edgeId});
        }

        vector<int> parent(n, -1), depth(n), order{root};
        for (int i = 0; i < n; i++)
        {
            int u = order[i];
            for (int v : tree[u])
            {
                if (v == parent[u])
                {
                    continue;
                }
                parent[v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }

        TreeDiff<int> diff(tree, root);
        vector<int> wantVertex(n), wantEdge(n);
        for (int step = 0; step < 100; step++)
        {
            int u = rng() % n;
            int v = rng() % n;
            int delta = (int)(rng() % 21) - 10;
            diff.addVtxPath(u, v, delta);
            diff.addEdgePath(u, v, delta);
            int x = u;
            int y = v;
            while (depth[x] > depth[y])
            {
                wantVertex[x] += delta;
                wantEdge[x] += delta;
                x = parent[x];
            }
            while (depth[y] > depth[x])
            {
                wantVertex[y] += delta;
                wantEdge[y] += delta;
                y = parent[y];
            }
            while (x != y)
            {
                wantVertex[x] += delta;
                wantVertex[y] += delta;
                wantEdge[x] += delta;
                wantEdge[y] += delta;
                x = parent[x];
                y = parent[y];
            }
            wantVertex[x] += delta;
        }
        assert(diff.val() == wantVertex);
        assert(diff.edgeValues() == wantEdge);
        diff.clear();
        assert(diff.val() == vector<int>(n));
        assert(diff.edgeValues() == vector<int>(n));

        // State[0] 是子树点数，State[1] 是这些点到当前根的距离和。
        using State = array<int, 2>;
        vector<State> base(n, {1, 0});
        auto merge = [](const State &a, const State &b)
        {
            return State{a[0] + b[0], a[1] + b[1]};
        };
        auto lift = [&](const State &a, int from, int to, int edgeId)
        {
            // a 是 from 一侧的状态，to 是跨边后的根，edgeId 指定边权；返回转移后的状态。
            (void)from;
            (void)to;
            return State{a[0], a[1] + a[0] * weight[edgeId]};
        };
        auto got = rerooting(g, base, State{0, 0}, merge, lift, root);
        for (int s = 0; s < n; s++)
        {
            vector<int> distance(n, -1);
            distance[s] = 0;
            vector<int> q{s};
            for (int i = 0; i < n; i++)
            {
                int u = q[i];
                for (auto [v, edgeId] : g[u])
                {
                    if (distance[v] == -1)
                    {
                        distance[v] = distance[u] + weight[edgeId];
                        q.push_back(v);
                    }
                }
            }
            assert(got[s][0] == n);
            assert(got[s][1] == accumulate(distance.begin(), distance.end(), 0LL));
        }
    }
    cout << "OK\n";
    return 0;
}
