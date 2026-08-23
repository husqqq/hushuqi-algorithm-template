#include "../include/full/5_3_012.hpp"

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 8 + 1;
        int m = rng() % 25;
        vector<pair<int, int>> e(m);
        for (auto &[u, v] : e)
        {
            u = rng() % n;
            v = rng() % n;
        }
        vector<int> want(m, m + 1);
        vector<vector<int>> g(n);
        for (int t = 1; t <= m; t++)
        {
            auto [u, v] = e[t - 1];
            g[u].push_back(v);
            auto bel = tarjanSCC(g);
            for (int i = 0; i < t; i++)
            {
                if (want[i] == m + 1 && bel[e[i].first] == bel[e[i].second])
                {
                    want[i] = t;
                }
            }
        }
        auto got = incrSCC(n, e);
        assert(got == want);
        for (int x : got)
        {
            agg = agg * 1000000007ULL + x;
        }
    }
    cout << agg << '\n';
    return 0;
}
