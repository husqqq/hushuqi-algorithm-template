#include "../include/full/6_2_002_contour.hpp"

vector<int> distances(const vector<vector<int>> &g, int s)
{
    vector<int> d(g.size(), -1), q{s};
    d[s] = 0;
    for (int i = 0; i < (int)q.size(); i++)
    {
        int u = q[i];
        for (int v : g[u]) if (d[v] < 0) d[v] = d[u] + 1, q.push_back(v);
    }
    return d;
}

signed main()
{
    mt19937 rng(20260822);
    for (int n = 1; n <= 45; n++)
    {
        for (int round = 0; round < 40; round++)
        {
            vector<vector<int>> g(n);
            for (int v = 1; v < n; v++)
            {
                int p = rng() % v;
                g[p].push_back(v), g[v].push_back(p);
            }
            vector<long long> a(n);
            for (auto &x : a) x = rng() % 100;
            PtSetRngSum<long long> sum(g, a);
            PtGetRngAdd<long long> add(g, a);
            vector<long long> aSum = a, aAdd = a;
            for (int step = 0; step < 120; step++)
            {
                int u = rng() % n, l = rng() % (n + 2) - 1, r = l + 1 + rng() % (n + 1);
                if (rng() & 1)
                {
                    long long x = rng() % 100;
                    aSum[u] = x;
                    sum.set(u, x);
                }
                else
                {
                    long long x = rng() % 20;
                    add.apply(u, l, r, x);
                    auto d = distances(g, u);
                    for (int v = 0; v < n; v++) if (l <= d[v] && d[v] < r) aAdd[v] += x;
                }
                auto d = distances(g, u);
                long long expectedSum = 0;
                for (int v = 0; v < n; v++) if (l <= d[v] && d[v] < r) expectedSum += aSum[v];
                assert(sum.prod(u, l, r) == expectedSum);
                assert(add.get(u) == aAdd[u]);
            }
        }
    }
}
