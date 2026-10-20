#include "../include/full/2_2_015.hpp"
#include "../include/full/12_1_011.hpp"
#include "../include/full/5_9_009.hpp"
#include "../include/full/9_1_013.hpp"
#include "../include/full/5_10_017.hpp"
#include "../include/full/2_1_010.hpp"
#include "../include/full/4_3_012.hpp"
#include "../include/full/5_2_013.hpp"
#include "../include/full/9_1_014.hpp"
#include "../include/full/12_3_007.hpp"
#include "../include/full/6_4_007.hpp"
#include "../include/full/3_4_013.hpp"
#include "../include/full/7_4_009.hpp"
#include "../include/full/9_3_008.hpp"
#include "../include/full/5_8_014.hpp"
#include "../include/full/5_10_018.hpp"
#include "../include/full/4_1_013.hpp"
#include "../include/full/10_2_022.hpp"

signed main()
{
    mt19937 rng(817);
    for (int tc = 0; tc < 140; ++tc)
    {
        int n = rng() % 11; vector<long long> a(n); for (auto &x : a) x = (int)(rng() % 21) - 10;
        long long target = (int)(rng() % 41) - 20;
        optional<__int128> lo, hi; __int128 dist = (__int128)1 << 80;
        for (int mask = 0; mask < (1LL << n); ++mask)
        {
            __int128 s = 0; for (int i = 0; i < n; ++i) if (mask >> i & 1) s += a[i];
            if (s <= target && (!lo || s > *lo)) lo = s;
            if (s >= target && (!hi || s < *hi)) hi = s;
            dist = min(dist, s < target ? target - s : s - target);
        }
        auto l = mitm::bound(a, target), h = mitm::bound(a, target, true);
        assert((bool)l == (bool)lo && (bool)h == (bool)hi);
        if (l) { assert(l->sum == *lo); __int128 s = 0; for (int i : l->indices) s += a[i]; assert(s == l->sum); }
        if (h) assert(h->sum == *hi);
        auto best = mitm::closest(a, target).sum; assert((best < target ? target - best : best - target) == dist);
    }
    assert(mitm::fourSum({vector<long long>{1}, {2}, {3}, {4}}, 10));
    assert(interval_dp::stones({1, 2, 3}).cost == 9);
    assert(interval_dp::stones({1, 2, 3}).plan(0, 3).size() == 2);
    assert(interval_dp::matrixChain({10, 30, 5, 60}).cost == 4500);
    assert(interval_dp::brackets("())(") == 2);
    for (int tc = 0; tc < 80; ++tc)
    {
        int n = rng() % 18 + 1; vector<int> to(n); vector<long long> w(n);
        for (int i = 0; i < n; ++i) to[i] = rng() % n, w[i] = (int)(rng() % 21) - 10;
        FuncGraph f(to, w);
        for (int u = 0; u < n; ++u)
        {
            int v = u; __int128 sum = 0; optional<long long> mn;
            for (int k = 0; k < 70; ++k) { auto r = f.walk(u, k); assert(r.end == v && r.total == sum && r.minimum == mn); sum += w[v]; mn = mn ? min(*mn, w[v]) : w[v]; v = to[v]; }
            for (int v0 = 0; v0 < n; ++v0)
            {
                int x = u, y = v0; optional<unsigned long long> hit;
                for (int k = 0; k <= n * n; ++k) { if (x == y) { hit = k; break; } x = to[x]; y = to[y]; }
                assert(f.meet(u, v0) == hit);
            }
        }
    }
    FuncGraph one({0}, {-LLONG_MAX}); assert(one.jump(0, ULLONG_MAX) == 0);
    assert(one.walk(0, ULLONG_MAX).total == (__int128)(-LLONG_MAX) * ULLONG_MAX);
    for (int n = 0; n <= 7; ++n) for (int tc = 0; tc < 12; ++tc)
    {
        vector<vector<long long>> a(n, vector<long long>(n)); for (auto &r : a) for (auto &x : r) x = (int)(rng() % 9) - 4;
        long long mod = tc == 0 ? LLONG_MAX : 97; vector<int> p(n); iota(p.begin(), p.end(), 0); __int128 ans = 0;
        do { __int128 prod = 1; for (int i = 0; i < n; ++i) prod *= a[i][p[i]]; ans += prod; } while (next_permutation(p.begin(), p.end()));
        ans %= mod; if (ans < 0) ans += mod; assert(permanent(a, mod) == ans);
    }
    assert(havelHakimi({}));
    vector<pair<int, int>> emptyEdges;
    assert(havelHakimi({0, 0, 0}) == emptyEdges);
    assert(!havelHakimi({3, 3, 1, 1}));
    assert(!havelHakimi({-1, 1}));
    assert(!galeRyser({3, 1}, {2, 2}));
    assert(!bipDegree({3, 1}, {2, 2}));
    assert(bipDegree({0, 0}, {0, 0, 0}) == emptyEdges);
    for (int n = 0; n <= 6; ++n)
    {
        set<vector<int>> possible;
        int edgeCnt = n * (n - 1) / 2;
        for (int mask = 0; mask < (1 << edgeCnt); ++mask)
        {
            vector<int> d(n);
            int bit = 0;
            for (int u = 0; u < n; ++u)
            {
                for (int v = u + 1; v < n; ++v)
                {
                    if (mask >> bit & 1)
                    {
                        ++d[u];
                        ++d[v];
                    }
                    ++bit;
                }
            }
            possible.insert(d);
        }
        int seqCnt = 1;
        for (int i = 0; i < n; ++i)
        {
            seqCnt *= n;
        }
        for (int code = 0; code < seqCnt; ++code)
        {
            int x = code;
            vector<int> d(n);
            for (int &v : d)
            {
                v = x % n;
                x /= n;
            }
            auto e = havelHakimi(d);
            assert((bool)e == possible.count(d));
            if (e)
            {
                vector<int> got(n);
                set<pair<int, int>> edges;
                for (auto [u, v] : *e)
                {
                    assert(u != v && edges.insert(minmax(u, v)).second);
                    ++got[u];
                    ++got[v];
                }
                assert(got == d);
            }
        }
    }
    for (int n = 0; n <= 4; ++n)
    {
        for (int m = 0; m <= 4; ++m)
        {
            set<pair<vector<int>, vector<int>>> possible;
            for (int mask = 0; mask < (1 << (n * m)); ++mask)
            {
                vector<int> a(n), b(m);
                for (int u = 0; u < n; ++u)
                {
                    for (int v = 0; v < m; ++v)
                    {
                        if (mask >> (u * m + v) & 1)
                        {
                            ++a[u];
                            ++b[v];
                        }
                    }
                }
                possible.insert({a, b});
            }
            int seqA = 1, seqB = 1;
            for (int i = 0; i < n; ++i)
            {
                seqA *= m + 1;
            }
            for (int j = 0; j < m; ++j)
            {
                seqB *= n + 1;
            }
            for (int ca = 0; ca < seqA; ++ca)
            {
                vector<int> a(n);
                int x = ca;
                for (int &v : a)
                {
                    v = x % (m + 1);
                    x /= m + 1;
                }
                for (int cb = 0; cb < seqB; ++cb)
                {
                    vector<int> b(m);
                    x = cb;
                    for (int &v : b)
                    {
                        v = x % (n + 1);
                        x /= n + 1;
                    }
                    bool want = possible.count({a, b});
                    assert(galeRyser(a, b) == want);
                    auto e = bipDegree(a, b);
                    assert((bool)e == want);
                    if (e)
                    {
                        vector<int> gotA(n), gotB(m);
                        set<pair<int, int>> edges;
                        for (auto [u, v] : *e)
                        {
                            assert(edges.insert({u, v}).second);
                            ++gotA[u];
                            ++gotB[v];
                        }
                        assert(gotA == a && gotB == b);
                    }
                }
            }
        }
    }
    for (int tc = 0; tc < 100; ++tc)
    {
        int n = rng() % 9 + 1; vector<int> d(n);
        for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) if (rng() & 1) ++d[u], ++d[v];
        auto e = havelHakimi(d); assert(e); vector<int> deg(n); set<pair<int, int>> unique;
        for (auto [u, v] : *e) { assert(u != v && unique.insert(minmax(u, v)).second); ++deg[u]; ++deg[v]; } assert(deg == d);
        int m = rng() % 8 + 1; vector<int> a(n), b(m);
        for (int u = 0; u < n; ++u) for (int v = 0; v < m; ++v) if (rng() & 1) ++a[u], ++b[v];
        auto be = bipDegree(a, b); assert(galeRyser(a, b) && be); vector<int> x(n), y(m); unique.clear();
        for (auto [u, v] : *be) { ++x[u]; ++y[v]; assert(unique.insert({u, v}).second); } assert(x == a && y == b);
    }
    for (int tc = 0; tc < 80; ++tc)
    {
        int n = rng() % 8 + 1; vector<vector<int>> g(n); vector<vector<pair<int, long long>>> wg(n);
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (u != v && rng() % 4 == 0) { g[u].push_back(v); wg[u].push_back({v, 1}); }
        for (int s = 0; s < n; ++s) for (int t = 0; t < n; ++t)
        {
            auto a = iter_search::biBfs(g, s, t), b = iter_search::iddfs(g, s, t, n);
            auto c = iter_search::ida(wg, s, t, vector<long long>(n));
            assert((bool)a == (bool)b && (bool)a == (bool)c); if (a) assert(a->size() == b->size() && c->cost == (int)a->size() - 1);
        }
    }
    for (int tc = 0; tc < 100; ++tc)
    {
        string s; for (int j = rng() % 50; j--;) s += (char)(rng() % 5);
        FMIndex fm(s); assert(fm.restore() == s);
        for (int j = 0; j < 20; ++j)
        {
            string p; for (int k = rng() % 7; k--;) p += (char)(rng() % 5);
            vector<int> want; for (int i = 0; i <= (int)s.size(); ++i) if (s.substr(i, p.size()) == p) want.push_back(i);
            assert(fm.locate(p) == want);
        }
    }
    for (int tc = 0; tc < 70; ++tc)
    {
        int n = rng() % 6 + 2; YenPaths g(n);
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (u != v && rng() % 3 == 0) g.add(u, v, rng() % 5);
        vector<__int128> costs; vector<char> vis(n);
        auto dfs = [&](auto &&self, int u, __int128 cost) -> void { if (u == n - 1) { costs.push_back(cost); return; } vis[u] = true; for (int id : g.adj[u]) if (!vis[g.edges[id][1]]) self(self, g.edges[id][1], cost + g.edges[id][2]); vis[u] = false; };
        dfs(dfs, 0, 0); sort(costs.begin(), costs.end()); auto got = g.solve(0, n - 1, 10); assert(got.size() == min<size_t>(10, costs.size()));
        for (int i = 0; i < (int)got.size(); ++i) { assert(got[i].cost == costs[i]); set<int> v(got[i].vertices.begin(), got[i].vertices.end()); assert(v.size() == got[i].vertices.size()); }
    }
    for (int tc = 0; tc < 120; ++tc)
    {
        int n = rng() % 8, m = rng() % 9; vector<gf2::Row> a(m, gf2::Row((n + 64) / 64));
        for (auto &r : a) r[0] = rng() & ((1ULL << (n + 1)) - 1);
        auto r = gf2::solve(a, n); int solutions = 0;
        for (int s = 0; s < (1LL << n); ++s) { bool ok = true; for (auto row : a) if ((popcount(row[0] & s) & 1) != gf2::bit(row, n)) ok = false; solutions += ok; }
        assert(r.consistent == (solutions > 0));
        if (r.consistent) { assert(solutions == (1LL << r.free)); for (auto row : a) { auto mask = r.solution.empty() ? 0ULL : r.solution[0]; assert((popcount(row[0] & mask) & 1) == gf2::bit(row, n)); for (auto v : r.basis) assert(!(popcount(row[0] & v[0]) & 1)); } }
    }
    SlopeTrick slope; vector<long long> points;
    for (int i = 0; i < 80; ++i) { long long a = (int)(rng() % 31) - 15; slope.addAbs(a); points.push_back(a); for (int x = -20; x <= 20; ++x) { long long want = 0; for (auto p : points) want += abs(x - p); assert(slope.eval(x) == want); } }
    SlopeTrick convex; convex.addAbs(4); convex.shift(-2, 3); assert(convex.eval(0) == 2 && convex.eval(4) == 0 && convex.eval(8) == 1); convex.prefixMin(); assert(convex.eval(100) == 0);
    EulerTour ett(18); vector<set<int>> adj(18); vector<long long> values(18);
    auto component = [&](int s) { vector<int> v{s}; vector<char> seen(18); seen[s] = true; for (int i = 0; i < (int)v.size(); ++i) for (int x : adj[v[i]]) if (!seen[x]) seen[x] = true, v.push_back(x); return v; };
    for (int step = 0; step < 1600; ++step)
    {
        int u = rng() % 18, v = rng() % 18, op = rng() % 5; auto c = component(u); bool same = find(c.begin(), c.end(), v) != c.end();
        if (op == 0) { assert(ett.link(u, v) == !same); if (!same) adj[u].insert(v), adj[v].insert(u); }
        if (op == 1) { bool exists = adj[u].count(v); assert(ett.cut(u, v) == exists); if (exists) adj[u].erase(v), adj[v].erase(u); }
        if (op == 2) { long long d = (int)(rng() % 21) - 10; ett.add(u, d); values[u] += d; }
        if (op == 3) { long long d = (int)(rng() % 21) - 10; ett.compAdd(u, d); for (int x : c) values[x] += d; }
        for (int i = 0; i < 18; ++i) { auto group = component(i); long long s = 0; for (int x : group) s += values[x]; assert(ett.compSum(i) == s && ett.compSize(i) == (int)group.size()); }
    }
    for (int n = 1; n < 250; ++n)
    {
        vector<long long> a(n); for (auto &v : a) v = rng() % 20; FischerHeun rmq(a);
        for (int l = 0; l < n; ++l) { long long best = a[l]; for (int r = l + 1; r <= n; ++r) { best = min(best, a[r - 1]); assert(rmq.query(l, r) == best); } }
    }
    assert(hensel({-2, 0, 1}, 3, 7, 4)); assert(!hensel({0, 0, 1}, 0, 7, 3));
    auto lattice = lll({{1, 1, 1}, {-1, 0, 2}, {3, 5, 6}}); assert(lattice); assert(!lll({{1, 1}, {2, 2}}));
    for (int tc = 0; tc < 50; ++tc)
    {
        int n = rng() % 6 + 2; vector<array<int, 3>> e;
        vector<vector<int>> a(n, vector<int>(n));
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (u != v) { long long w = rng() % 7; e.push_back({u, v, w}); a[u][v] = w; }
        long long best = LLONG_MAX;
        for (int mask = 1; mask + 1 < (1LL << n); ++mask) { long long sum = 0; for (auto [u, v, w] : e) if ((mask >> u & 1) && !(mask >> v & 1)) sum += w; best = min(best, sum); }
        auto [cost, side] = dirMinCut(n, e); assert(cost == best); long long sum = 0; for (auto [u, v, w] : e) if (side[u] && !side[v]) sum += w; assert(sum == cost);
        vector<int> p(n); iota(p.begin(), p.end(), 0); shuffle(p.begin(), p.end(), rng); auto b = a;
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) b[p[u]][p[v]] = a[u][v]; auto iso = graphIso(a, b); assert(iso);
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) assert(a[u][v] == b[(*iso)[u]][(*iso)[v]]);
    }
    for (int tc = 0; tc < 500; ++tc) { string a, b; for (int j = rng() % 65; j--;) a += char(rng() % 5); for (int j = rng() % 90; j--;) b += char(rng() % 5); assert(myersDist(a, b) == editDist(a, b)); }
    for (int tc = 0; tc < 100; ++tc)
    {
        vector<int> a(rng() % 14); for (auto &x : a) x = rng() % 12; int T = 20; vector<Z> want(T + 1); want[0] = 1;
        for (auto x : a) for (int i = T; i >= x; --i) want[i] += want[i - x]; assert(subsetCount(a, T) == want);
    }
}
