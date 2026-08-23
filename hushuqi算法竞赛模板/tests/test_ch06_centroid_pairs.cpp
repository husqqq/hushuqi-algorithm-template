#include "../include/full/6_2_001.hpp"

signed main()
{
    mt19937_64 rng(0);
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 25 + 1;
        vector<vector<array<int, 2>>> g(n);
        for (int v = 1; v < n; v++)
        {
            int p = rng() % v;
            int w = rng() % 10;
            g[p].push_back({v, w});
            g[v].push_back({p, w});
        }
        vector<vector<int>> d(n, vector<int>(n));
        for (int s = 0; s < n; s++)
        {
            auto dfs = [&](auto &&self, int u, int p) -> void
            {
                for (auto [v, w] : g[u])
                {
                    if (v != p)
                    {
                        d[s][v] = d[s][u] + w;
                        self(self, v, u);
                    }
                }
            };
            dfs(dfs, s, -1);
        }
        CentPairs q(g);
        for (int lim = 0; lim <= 50; lim += 5)
        {
            int want = 0;
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    want += d[i][j] <= lim;
                }
            }
            assert(q.countAtMost(lim) == want);
        }
    }
    cout << "OK\n";
    return 0;
}
