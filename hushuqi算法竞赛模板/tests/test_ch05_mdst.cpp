#include "../include/full/5_5_012.hpp"

struct E
{
    int u, v;
    long long w;
};

long long diameter(int n, const vector<E> &e)
{
    vector<vector<pair<int, long long>>> g(n);
    for (auto [u, v, w] : e)
    {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    long long ans = 0;
    for (int s = 0; s < n; s++)
    {
        vector<long long> d(n, -1);
        d[s] = 0;
        auto dfs = [&](auto &&self, int u) -> void
        {
            for (auto [v, w] : g[u])
            {
                if (d[v] == -1)
                {
                    d[v] = d[u] + w;
                    self(self, v);
                }
            }
        };
        dfs(dfs, s);
        for (auto x : d)
        {
            if (x == -1)
            {
                return -1;
            }
            ans = max(ans, x);
        }
    }
    return ans;
}

long long brute(int n, const vector<E> &e)
{
    long long ans = numeric_limits<long long>::max();
    int m = e.size();
    for (int s = 0; s < (1LL << m); s++)
    {
        if (popcount((unsigned long long)s) != n - 1)
        {
            continue;
        }
        vector<E> t;
        for (int i = 0; i < m; i++)
        {
            if (s >> i & 1)
            {
                t.push_back(e[i]);
            }
        }
        long long d = diameter(n, t);
        if (d >= 0)
        {
            ans = min(ans, d);
        }
    }
    return ans;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 800; tc++)
    {
        int n = rng() % 6 + 1;
        vector<E> e;
        MinDiamTree q(n);
        for (int i = 1; i < n; i++)
        {
            int p = rng() % i;
            long long w = rng() % 9;
            e.push_back({i, p, w});
            q.addEdge(i, p, w);
        }
        int extra = rng() % min<int>(6, n * (n - 1) / 2 + 1);
        while (extra--)
        {
            int u = rng() % n, v = rng() % n;
            if (u == v)
            {
                extra++;
                continue;
            }
            long long w = rng() % 9;
            e.push_back({u, v, w});
            q.addEdge(u, v, w);
        }
        auto [got, es] = q.solve();
        assert(got == brute(n, e));
        assert((int)es.size() == n - 1);
        vector<E> t;
        for (auto [u, v] : es)
        {
            long long w = numeric_limits<long long>::max();
            for (auto x : e)
            {
                if ((x.u == u && x.v == v) || (x.u == v && x.v == u))
                {
                    w = min(w, x.w);
                }
            }
            assert(w != numeric_limits<long long>::max());
            t.push_back({u, v, w});
        }
        assert(diameter(n, t) == got);
        agg = agg * 1000000007ULL + got;
    }
    cout << agg << '\n';
    return 0;
}
