#include "../include/full/5_6_001.hpp"
#include "../include/full/5_6_002.hpp"
#include "../include/full/5_6_003.hpp"
#include "../include/full/5_6_004.hpp"
#include "../include/full/5_6_005.hpp"
#include "../include/full/5_6_006.hpp"
#include "../include/full/5_6_007.hpp"
#include "../include/full/5_6_008.hpp"
#include "../include/full/5_6_009.hpp"
#include "../include/full/5_6_010.hpp"
#include "../include/full/5_6_011.hpp"

void checkPair(const vector<int> &ml, const vector<int> &mr, int want)
{
    int cnt = 0;
    for (int u = 0; u < (int)ml.size(); u++)
    {
        if (ml[u] != -1)
        {
            assert(0 <= ml[u] && ml[u] < (int)mr.size());
            assert(mr[ml[u]] == u);
            cnt++;
        }
    }
    assert(cnt == want);
}

int bruteIndependent(int n, const vector<pair<int, int>> &edge)
{
    int ans = 0;
    for (unsigned long long s = 0; s < (1ULL << n); s++)
    {
        bool ok = true;
        for (auto [u, v] : edge)
        {
            if ((s >> u & 1) && (s >> v & 1))
            {
                ok = false;
            }
        }
        if (ok)
        {
            ans = max(ans, (int)popcount(s));
        }
    }
    return ans;
}

signed main()
{
    mt19937_64 rng(0);
    for (int it = 0; it < 1500; it++)
    {
        int nl = rng() % 9;
        int nr = rng() % 9;
        Hungarian a(nl, nr);
        BitHungarian b(nl, nr);
        HopcroftKarp c(nl, nr);
        HopKarpCSR d(nl, nr);
        for (int u = 0; u < nl; u++)
        {
            for (int v = 0; v < nr; v++)
            {
                if (rng() % 3 == 0)
                {
                    a.addEdge(u, v);
                    b.addEdge(u, v);
                    c.addEdge(u, v);
                    d.addEdge(u, v);
                }
            }
        }
        int want = a.matching();
        assert(b.matching() == want);
        assert(c.matching() == want);
        assert(d.matching() == want);
        checkPair(a.leftMatch(), a.matchR(), want);
        checkPair(b.leftMatch(), b.matchR(), want);
        checkPair(c.leftMatch(), c.matchR(), want);
        checkPair(d.leftMatch(), d.matchR(), want);
        assert(d.matching() == want);
    }

    for (int it = 0; it < 500; it++)
    {
        int n = rng() % 5 + 1;
        int m = n + rng() % 3;
        vector<vector<long long>> a(n, vector<long long>(m));
        for (auto &row : a)
        {
            for (auto &x : row)
            {
                x = (long long)(rng() % 31) - 15;
            }
        }
        for (int mx = 0; mx < 2; mx++)
        {
            auto got = assignment(a, mx);
            assert(got.has_value());
            long long want = mx ? LLONG_MIN : LLONG_MAX;
            auto dfs = [&](auto &&self, int i, int used, long long sum) -> void
            {
                if (i == n)
                {
                    want = mx ? max(want, sum) : min(want, sum);
                    return;
                }
                for (int j = 0; j < m; j++)
                {
                    if (!(used >> j & 1))
                    {
                        self(self, i + 1, used | 1LL << j, sum + a[i][j]);
                    }
                }
            };
            dfs(dfs, 0, 0, 0);
            assert(got->cost == want);
        }
    }

    for (int it = 0; it < 500; it++)
    {
        int n = rng() % 6 + 1;
        int m = rng() % 6 + 1;
        BipEdgeColor ec(n, m);
        vector<pair<int, int>> edge;
        vector<int> dl(n), dr(m);
        int z = rng() % 20;
        for (int i = 0; i < z; i++)
        {
            int u = rng() % n;
            int v = rng() % m;
            ec.add(u, v);
            edge.push_back({u, v});
            dl[u]++;
            dr[v]++;
        }
        auto res = ec.work();
        int want = 0;
        for (auto x : dl)
        {
            want = max(want, x);
        }
        for (auto x : dr)
        {
            want = max(want, x);
        }
        assert(res.k == want && res.c.size() == edge.size());
        vector<unsigned long long> usedL(n), usedR(m);
        for (int i = 0; i < z; i++)
        {
            int col = res.c[i];
            assert(0 <= col && col < res.k);
            auto [u, v] = edge[i];
            assert(!(usedL[u] >> col & 1));
            assert(!(usedR[v] >> col & 1));
            usedL[u] |= 1ULL << col;
            usedR[v] |= 1ULL << col;
        }
    }

    {
        vector<vector<int>> g = {{0, 1}, {1, 2}, {0, 2}};
        HopcroftKarp hk(3, 3);
        for (int u = 0; u < 3; u++)
        {
            for (int v : g[u])
            {
                hk.addEdge(u, v);
            }
        }
        int match = hk.matching();
        auto [l, r] = minVtxCover(g, hk.leftMatch(), hk.matchR());
        assert((int)l.size() + (int)r.size() == match);
        auto cover = minEdgeCover(g, hk.leftMatch(), hk.matchR());
        assert(cover.has_value() && (int)cover->size() == 6 - match);
    }

    for (int it = 0; it < 300; it++)
    {
        int n = rng() % 10 + 1;
        vector<pair<int, int>> edge;
        for (int u = 0; u < n; u++)
        {
            for (int v = u; v < n; v++)
            {
                if (rng() % 5 == 0)
                {
                    edge.push_back({u, v});
                }
            }
        }
        assert((int)maxIS(n, edge).size() == bruteIndependent(n, edge));
    }

    for (int it = 0; it < 300; it++)
    {
        int n = rng() % 8 + 1;
        vector<pair<int, int>> edge;
        vector reach(n, vector<bool>(n));
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0)
                {
                    edge.push_back({u, v});
                    reach[u][v] = true;
                }
            }
        }
        for (int k = 0; k < n; k++)
        {
            for (int u = 0; u < n; u++)
            {
                for (int v = 0; v < n; v++)
                {
                    reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
                }
            }
        }
        auto path = dagPathCover(n, edge);
        vector<bool> seen(n);
        for (const auto &p : path)
        {
            for (int i = 0; i < (int)p.size(); i++)
            {
                assert(!seen[p[i]]);
                seen[p[i]] = true;
                if (i)
                {
                    assert(find(edge.begin(), edge.end(), pair{p[i - 1], p[i]}) != edge.end());
                }
            }
        }
        assert(count(seen.begin(), seen.end(), true) == n);

        auto anti = dagAntichain(n, edge);
        for (int i = 0; i < (int)anti.size(); i++)
        {
            for (int j = i + 1; j < (int)anti.size(); j++)
            {
                assert(!reach[anti[i]][anti[j]] && !reach[anti[j]][anti[i]]);
            }
        }
        int want = 0;
        for (int s = 0; s < (1LL << n); s++)
        {
            bool ok = true;
            for (int u = 0; u < n; u++)
            {
                for (int v = u + 1; v < n; v++)
                {
                    if ((s >> u & 1) && (s >> v & 1) && (reach[u][v] || reach[v][u]))
                    {
                        ok = false;
                    }
                }
            }
            if (ok)
            {
                want = max(want, (int)popcount((unsigned long long)s));
            }
        }
        assert((int)anti.size() == want);
    }

    for (int it = 0; it < 500; it++)
    {
        int n = rng() % 8 + 1;
        vector<long long> w(n);
        for (auto &x : w)
        {
            x = (long long)(rng() % 21) - 10;
        }
        vector<array<int, 2>> need;
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                if (u != v && rng() % 12 == 0)
                {
                    need.push_back({u, v});
                }
            }
        }
        auto got = maxClosure(w, need);
        __int128 want = -((__int128)1 << 120);
        for (int s = 0; s < (1LL << n); s++)
        {
            bool ok = true;
            for (auto [u, v] : need)
            {
                if ((s >> u & 1) && !(s >> v & 1))
                {
                    ok = false;
                }
            }
            if (ok)
            {
                __int128 sum = 0;
                for (int u = 0; u < n; u++)
                {
                    if (s >> u & 1)
                    {
                        sum += w[u];
                    }
                }
                want = max(want, sum);
            }
        }
        assert(got.val == want);
    }

    puts("OK");
    return 0;
}
