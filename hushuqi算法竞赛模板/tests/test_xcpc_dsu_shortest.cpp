#include "../include/full/3_1_007.hpp"
#include "../include/full/5_2_011.hpp"

signed main()
{
    mt19937_64 rng(0);

    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 15 + 1;
        ErasableDSU dsu(n);
        vector<int> group(n);
        iota(group.begin(), group.end(), 0);
        int nextGroup = n;
        for (int step = 0; step < 1000; step++)
        {
            int x = rng() % n;
            int y = rng() % n;
            if (rng() % 2)
            {
                bool changed = group[x] != group[y];
                assert(dsu.merge(x, y) == changed);
                if (changed)
                {
                    int old = group[y];
                    int now = group[x];
                    for (auto &z : group)
                    {
                        if (z == old)
                        {
                            z = now;
                        }
                    }
                }
            }
            else
            {
                int count = 0;
                for (auto z : group)
                {
                    count += z == group[x];
                }
                assert(dsu.erase(x) == (count > 1));
                if (count > 1)
                {
                    group[x] = nextGroup++;
                }
            }
            for (int u = 0; u < n; u++)
            {
                int count = 0;
                for (int v = 0; v < n; v++)
                {
                    assert(dsu.same(u, v) == (group[u] == group[v]));
                    count += group[u] == group[v];
                }
                assert(dsu.size(u) == count);
            }
        }
    }

    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 12 + 1;
        vector<vector<BinaryEdge>> g(n);
        vector<vector<int>> d(n, vector<int>(n, n + 1));
        for (int u = 0; u < n; u++)
        {
            d[u][u] = 0;
            for (int v = 0; v < n; v++)
            {
                if (u != v && rng() % 4 == 0)
                {
                    int w = rng() & 1;
                    g[u].push_back({v, w});
                    d[u][v] = min(d[u][v], w);
                }
            }
        }
        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
        for (int s = 0; s < n; s++)
        {
            auto got = zeroOneBfs(g, s);
            for (int v = 0; v < n; v++)
            {
                assert(got[v].has_value() == (d[s][v] <= n));
                if (got[v])
                {
                    assert(*got[v] == d[s][v]);
                }
            }
        }
    }

    cout << "OK\n";
    return 0;
}
