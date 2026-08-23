#include "../include/full/5_10_003.hpp"

vector<int> reachSet(const vector<vector<int>> &g, int rt, int ban)
{
    int n = g.size();
    vector<int> vis(n);
    if (rt == ban)
    {
        return vis;
    }
    vis[rt] = 1;
    auto dfs = [&](auto &&self, int u) -> void
    {
        for (int v : g[u])
        {
            if (v != ban && !vis[v])
            {
                vis[v] = 1;
                self(self, v);
            }
        }
    };
    dfs(dfs, rt);
    return vis;
}

vector<int> bruteDom(const vector<vector<int>> &g, int rt)
{
    int n = g.size();
    auto base = reachSet(g, rt, -1);
    vector<vector<int>> d(n);
    for (int v = 0; v < n; v++)
    {
        if (!base[v] || v == rt)
        {
            continue;
        }
        for (int w = 0; w < n; w++)
        {
            if (w != v && w != rt && !reachSet(g, rt, w)[v])
            {
                d[v].push_back(w);
            }
        }
    }
    vector<int> ans(n, -1);
    ans[rt] = rt;
    for (int v = 0; v < n; v++)
    {
        if (!base[v] || v == rt)
        {
            continue;
        }
        int p = rt, dep = 0;
        for (int w : d[v])
        {
            if ((int)d[w].size() + 1 > dep)
            {
                dep = d[w].size() + 1;
                p = w;
            }
        }
        ans[v] = p;
    }
    return ans;
}

signed main()
{
    vector<vector<int>> diamond = {{1, 2}, {3}, {3}, {}, {0}};
    Dominator dm(diamond);
    assert(dm.work(0) == vector<int>({0, 0, 0, 0, -1}));
    assert(dm.work(4) == vector<int>({4, 0, 0, 0, 4}));

    mt19937_64 rng(0);
    for (int t = 0; t < 2000; t++)
    {
        int n = 2 + rng() % 7;
        vector<vector<int>> g(n);
        int m = rng() % (2 * n + 1);
        for (int i = 0; i < m; i++)
        {
            int u = rng() % n, v = rng() % n;
            if (u != v)
            {
                g[u].push_back(v);
            }
        }
        Dominator cur(g);
        assert(cur.work(0) == bruteDom(g, 0));
        int rt = rng() % n;
        assert(cur.work(rt) == bruteDom(g, rt));
    }
    puts("OK");
    return 0;
}
