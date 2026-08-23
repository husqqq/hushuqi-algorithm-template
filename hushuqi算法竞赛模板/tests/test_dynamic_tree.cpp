#include "../include/dynamic_tree.hpp"

vector<int> path(const vector<vector<int>> &g, int s, int t)
{
    // g 是森林邻接表，s、t 是两端；返回简单路径，不连通时返回空。
    vector<int> fa(g.size(), -1);
    queue<int> q;
    fa[s] = s;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : g[u])
        {
            if (fa[v] == -1)
            {
                fa[v] = u;
                q.push(v);
            }
        }
    }
    if (fa[t] == -1)
    {
        return {};
    }
    vector<int> ans;
    for (int u = t; u != s; u = fa[u])
    {
        ans.push_back(u);
    }
    ans.push_back(s);
    return ans;
}

int bridgeCount(const vector<vector<int>> &g)
{
    // g 是允许重边的无向图；返回用删边连通性定义直接算出的桥数。
    vector<array<int, 2>> e;
    for (int u = 0; u < (int)g.size(); u++)
    {
        for (auto v : g[u])
        {
            if (u <= v)
            {
                e.push_back({u, v});
            }
        }
    }
    int ans = 0;
    for (int ban = 0; ban < (int)e.size(); ban++)
    {
        auto [s, t] = e[ban];
        vector<int> vis(g.size());
        queue<int> q;
        vis[s] = 1;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            int seen = 0;
            for (auto v : g[u])
            {
                bool same = (u == s && v == t) || (u == t && v == s);
                if (same && seen++ == 0)
                {
                    continue;
                }
                if (!vis[v])
                {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
        ans += !vis[t];
    }
    return ans;
}

signed main()
{
    mt19937 rng(20260724);
    {
        vector<vector<int>> tr(7);
        for (auto [u, v] : vector<pair<int, int>>{{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {5, 6}})
        {
            tr[u].push_back(v);
            tr[v].push_back(u);
        }
        vector<long long> a{5, -2, 4, 7, 1, -3, 6};
        TreePathSum q(tr, a);
        assert(q.pathSum(4, 4) == 1);
        assert(q.pathSum(3, 6) == 17);
        assert(q.subtreeSum(0) == 18);
        assert(q.subtreeSum(1) == 6);
        q.set(0, 10);
        q.set(1, 8);
        q.set(6, -4);
        assert(q.pathSum(3, 6) == 22);
        assert(q.subtreeSum(1) == 16);
        assert(q.subtreeSum(5) == -7);
    }
    {
        vector<vector<int>> tr(4);
        for (int u = 1; u < 4; u++)
        {
            tr[u - 1].push_back(u);
            tr[u].push_back(u - 1);
        }
        vector<__int128> a(4, numeric_limits<long long>::max() / 2);
        __int128 want = a[0] * 4;
        TreePathSum q(tr, a);
        LinkCutTree lct(a);
        SubtreeLCT sub(a);
        for (int u = 1; u < 4; u++)
        {
            assert(lct.link(u - 1, u));
            assert(sub.link(u - 1, u));
        }
        assert(q.pathSum(0, 3) == want);
        assert(lct.pathSum(0, 3) == want);
        assert(sub.pathSum(0, 3) == want);
        assert(sub.subtreeSum(0, 0) == want);
    }
    int n = 24;
    vector<int> w(n);
    for (auto &x : w)
    {
        x = rng() % 31 - 15;
    }
    LinkCutTree lct(w);
    SubtreeLCT sub(w);
    vector<vector<int>> g(n);
    vector<array<int, 2>> edges;
    for (int z = 0; z < 4000; z++)
    {
        int op = rng() % 4, u = rng() % n, v = rng() % n;
        if (u == v)
        {
            v = (v + 1) % n;
        }
        auto p = path(g, u, v);
        if (op == 0 && p.empty())
        {
            assert(lct.link(u, v) && sub.link(u, v));
            g[u].push_back(v);
            g[v].push_back(u);
            edges.push_back({u, v});
        }
        else if (op == 1 && !edges.empty())
        {
            int id = rng() % edges.size();
            auto [a, b] = edges[id];
            assert(lct.cut(a, b) && sub.cut(a, b));
            erase(g[a], b);
            erase(g[b], a);
            edges.erase(edges.begin() + id);
        }
        else if (op == 2)
        {
            int x = rng() % 31 - 15;
            w[u] = x;
            lct.set(u, x);
            sub.set(u, x);
        }
        else if (!p.empty())
        {
            __int128 ans = 0;
            for (int x : p)
            {
                ans += w[x];
            }
            assert(lct.pathSum(u, v) == ans);
            assert(sub.pathSum(u, v) == ans);

            vector<int> fa(n, -1);
            fa[u] = u;
            auto dfs = [&](auto &&self, int x) -> void
            {
                for (auto y : g[x])
                {
                    if (fa[y] == -1)
                    {
                        fa[y] = x;
                        self(self, y);
                    }
                }
            };
            dfs(dfs, u);
            __int128 sum = 0;
            for (int x = 0; x < n; x++)
            {
                if (fa[x] == -1)
                {
                    continue;
                }
                int y = x;
                while (y != u && y != v)
                {
                    y = fa[y];
                }
                if (y == v)
                {
                    sum += w[x];
                }
            }
            assert(sub.subtreeSum(u, v) == sum);
        }
    }

    for (int n = 2; n <= 12; n++)
    {
        vector<vector<int>> tr(n);
        for (int u = 1; u < n; u++)
        {
            int p = rng() % u;
            tr[u].push_back(p);
            tr[p].push_back(u);
        }
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 30;
        }
        DynamicMWIS dp(tr, a);
        for (int z = 0; z < 100; z++)
        {
            int u = rng() % n, v = rng() % 30;
            a[u] = v;
            dp.set(u, v);
            __int128 ans = 0;
            for (int s = 0; s < (1LL << n); s++)
            {
                __int128 cur = 0;
                bool ok = true;
                for (int x = 0; x < n; x++)
                {
                    if (s >> x & 1)
                    {
                        cur += a[x];
                        for (auto y : tr[x])
                        {
                            if (s >> y & 1)
                            {
                                ok = false;
                            }
                        }
                    }
                }
                if (ok)
                {
                    chmax(ans, cur);
                }
            }
            assert(dp.answer() == ans);
        }
    }

    {
        vector<vector<int>> tr(5);
        for (int v = 1; v < 5; v++)
        {
            tr[0].push_back(v);
            tr[v].push_back(0);
        }
        vector<__int128> a(5, numeric_limits<long long>::max() / 2);
        DynamicMWIS dp(tr, a);
        assert(dp.answer() == a[0] * 4);
        dp.set(0, numeric_limits<long long>::max());
        assert(dp.answer() == a[0] * 4);
    }

    {
        DynamicCut q(4);
        int e0 = q.addEdge(0, 1);
        int e1 = q.addEdge(1, 2);
        int e2 = q.addEdge(2, 0);
        int e3 = q.addEdge(2, 3);
        q.addEdge(3, 3);
        assert((q.analyze() == pair{vector<int>{e3}, vector<int>{2}}));
        assert(q.eraseEdge(e2));
        assert((q.analyze() == pair{vector<int>{e0, e1, e3}, vector<int>{1, 2}}));
    }
    {
        DynamicCut q(3);
        int e0 = q.addEdge(0, 1);
        int e1 = q.addEdge(0, 1);
        int e2 = q.addEdge(1, 2);
        assert((q.analyze() == pair{vector<int>{e2}, vector<int>{1}}));
        assert(q.eraseEdge(e1));
        assert((q.analyze() == pair{vector<int>{e0, e2}, vector<int>{1}}));
    }

    for (int n = 2; n <= 20; n++)
    {
        OnlineBridge b(n);
        DynamicCut cut(n);
        vector<vector<int>> g(n);
        for (int z = 0; z < 50; z++)
        {
            int u = rng() % n, v = rng() % n;
            if (u == v)
            {
                v = (v + 1) % n;
            }
            b.addEdge(u, v);
            cut.addEdge(u, v);
            g[u].push_back(v);
            g[v].push_back(u);
            assert(b.bridges() == bridgeCount(g));
            assert((int)cut.analyze().first.size() == bridgeCount(g));
        }
    }
    {
        constexpr int N = 2000;
        vector<vector<int>> tr(N);
        DynamicCut cut(N);
        for (int u = 1; u < N; u++)
        {
            tr[u - 1].push_back(u);
            tr[u].push_back(u - 1);
            cut.addEdge(u - 1, u);
        }
        vector<long long> a(N, 1);
        TreePathSum sum(tr, a);
        DynamicMWIS dp(tr, a);
        assert(sum.pathSum(0, N - 1) == N);
        assert(dp.answer() == (N + 1) / 2);
        auto [bridges, points] = cut.analyze();
        assert((int)bridges.size() == N - 1);
        assert((int)points.size() == N - 2);
    }
    cout << "OK\n";
}
