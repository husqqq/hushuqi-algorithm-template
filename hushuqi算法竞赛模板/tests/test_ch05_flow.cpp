#include "../include/full/5_8_002.hpp"
#include "../include/full/5_8_003.hpp"
#include "../include/full/5_8_004.hpp"
#include "../include/full/5_8_005.hpp"
#include "../include/full/5_8_006.hpp"
#include "../include/full/5_8_007.hpp"
#include "../include/full/5_8_008.hpp"
#include "../include/full/5_8_009.hpp"
#include "../include/full/5_8_010.hpp"
#include "../include/full/5_8_011.hpp"

struct RefMaxFlow
{
    int n;
    vector<vector<int>> g;
    vector<array<long long, 2>> e;

    RefMaxFlow(int n) : n(n), g(n) {}

    void add(int u, int v, long long cap)
    {
        g[u].push_back(e.size());
        e.push_back({v, cap});
        g[v].push_back(e.size());
        e.push_back({u, 0});
    }

    long long flow(int s, int t)
    {
        long long ans = 0;
        while (true)
        {
            vector<int> pre(n, -1);
            queue<int> q;
            pre[s] = -2;
            q.push(s);
            while (!q.empty() && pre[t] == -1)
            {
                int u = q.front();
                q.pop();
                for (int id : g[u])
                {
                    int v = e[id][0];
                    if (e[id][1] > 0 && pre[v] == -1)
                    {
                        pre[v] = id;
                        q.push(v);
                    }
                }
            }
            if (pre[t] == -1)
            {
                return ans;
            }
            long long f = LLONG_MAX;
            for (int v = t; v != s; v = e[pre[v] ^ 1][0])
            {
                f = min(f, e[pre[v]][1]);
            }
            for (int v = t; v != s; v = e[pre[v] ^ 1][0])
            {
                e[pre[v]][1] -= f;
                e[pre[v] ^ 1][1] += f;
            }
            ans += f;
        }
    }
};

struct RefCostFlow
{
    struct E
    {
        int to, rev;
        long long cap;
        __int128 cost;
    };

    int n;
    vector<vector<E>> e;

    RefCostFlow(int n) : n(n), e(n) {}

    void add(int u, int v, long long cap, long long cost)
    {
        int id = e[u].size();
        int rev = e[v].size() + (u == v);
        e[u].push_back({v, rev, cap, (__int128)cost});
        e[v].push_back({u, id, 0, -(__int128)cost});
    }

    pair<long long, __int128> flow(int s, int t)
    {
        long long f = 0;
        __int128 cost = 0;
        while (true)
        {
            vector<optional<__int128>> d(n);
            vector<int> pv(n), pe(n);
            d[s] = 0;
            for (int it = 1; it < n; it++)
            {
                bool upd = false;
                for (int u = 0; u < n; u++)
                {
                    if (!d[u].has_value())
                    {
                        continue;
                    }
                    for (int i = 0; i < (int)e[u].size(); i++)
                    {
                        auto &a = e[u][i];
                        __int128 nd = *d[u] + a.cost;
                        if (a.cap > 0 && (!d[a.to].has_value() || nd < *d[a.to]))
                        {
                            d[a.to] = nd;
                            pv[a.to] = u;
                            pe[a.to] = i;
                            upd = true;
                        }
                    }
                }
                if (!upd)
                {
                    break;
                }
            }
            if (!d[t].has_value())
            {
                return {f, cost};
            }
            long long x = LLONG_MAX;
            for (int v = t; v != s; v = pv[v])
            {
                x = min(x, e[pv[v]][pe[v]].cap);
            }
            for (int v = t; v != s; v = pv[v])
            {
                auto &a = e[pv[v]][pe[v]];
                a.cap -= x;
                e[v][a.rev].cap += x;
            }
            f += x;
            cost += (__int128)x * *d[t];
        }
    }
};

void checkMax(int n, const vector<array<long long, 3>> &edge)
{
    Flow<long long> a(n);
    PushRelabel<long long> b(n);
    ISAP<long long> c(n);
    HLPP<long long> d(n);
    RefMaxFlow ref(n);
    for (auto x : edge)
    {
        a.add(x[0], x[1], x[2]);
        b.add(x[0], x[1], x[2]);
        c.add(x[0], x[1], x[2]);
        d.add(x[0], x[1], x[2]);
        ref.add(x[0], x[1], x[2]);
    }
    long long want = ref.flow(0, n - 1);
    assert(a.flow(0, n - 1) == want);
    assert(b.flow(0, n - 1) == want);
    assert(c.flow(0, n - 1) == want);
    assert(d.flow(0, n - 1) == want);
}

void checkCost(int n, const vector<array<long long, 4>> &edge)
{
    SpfaCostFlow<long long> a(n);
    CostFlow<long long> b(n);
    RefCostFlow ref(n);
    for (auto x : edge)
    {
        a.add(x[0], x[1], x[2], x[3]);
        b.add(x[0], x[1], x[2], x[3]);
        ref.add(x[0], x[1], x[2], x[3]);
    }
    auto got = a.flow(0, n - 1);
    auto [fr, cr] = ref.flow(0, n - 1);
    auto got1 = b.flow(0, n - 1, 1);
    auto got2 = b.flow(0, n - 1);
    auto [fa, ca] = got;
    auto [f1, c1] = got1;
    auto [f2, c2] = got2;
    assert(fa == fr && ca == cr);
    assert(f1 + f2 == fr && c1 + c2 == cr);
}

__int128 cutValue(int n, const vector<tuple<int, int, long long>> &edge,
                  unsigned long long side)
{
    // side 的第 u 位表示点 u 所在割侧；返回跨割边权和。
    __int128 ans = 0;
    for (auto [u, v, w] : edge)
    {
        if (((side >> u) ^ (side >> v)) & 1)
        {
            ans += w;
        }
    }
    return ans;
}

__int128 pairCut(int n, const vector<tuple<int, int, long long>> &edge, int s, int t)
{
    // s、t 是必须分居两侧的点；穷举并返回该点对最小割。
    __int128 ans = -1;
    for (unsigned long long side = 0; side < (1ULL << n); side++)
    {
        if (!(side >> s & 1) || (side >> t & 1))
        {
            continue;
        }
        __int128 cur = cutValue(n, edge, side);
        if (ans == -1 || cur < ans)
        {
            ans = cur;
        }
    }
    return ans;
}

__int128 treeCut(int n, const vector<tuple<int, int, long long>> &tree, int s, int t)
{
    // tree 是 Gomory-Hu 树；返回 s 到 t 路径上的最小边权。
    vector<vector<pair<int, __int128>>> g(n);
    for (auto [u, v, w] : tree)
    {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    vector<int> pre(n, -1);
    vector<__int128> val(n, -1);
    queue<int> q;
    pre[s] = s;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto [v, w] : g[u])
        {
            if (pre[v] != -1)
            {
                continue;
            }
            pre[v] = u;
            val[v] = u == s ? w : min(val[u], w);
            q.push(v);
        }
    }
    return val[t];
}

signed main()
{
    MinCostBFlow<long long, long long, __int128> empty(0);
    auto emptyCert = empty.solveCert();
    assert(emptyCert && emptyCert->value == 0 && emptyCert->pot.empty() && emptyCert->flow.empty());

    Flow<long long> a(1);
    PushRelabel<long long> b(1);
    ISAP<long long> c(1);
    HLPP<long long> d(1);
    assert(a.flow(0, 0) == 0);
    assert(b.flow(0, 0) == 0);
    assert(c.flow(0, 0) == 0);
    assert(d.flow(0, 0) == 0);

    checkMax(4, {{0, 1, 3}, {0, 2, 2}, {1, 2, 1}, {1, 3, 2}, {2, 3, 4}});
    checkMax(4, {{0, 1, 7}, {2, 3, 9}});
    checkMax(3, {{0, 0, 9}, {0, 1, 2}, {0, 1, 3}, {1, 2, 4}});

    mt19937_64 rng(0);
    for (int it = 0; it < 2000; it++)
    {
        int n = rng() % 8 + 2;
        int m = rng() % (2 * n) + 1;
        vector<array<long long, 3>> edge;
        for (int i = 0; i < m; i++)
        {
            edge.push_back({(long long)(rng() % n), (long long)(rng() % n), (long long)(rng() % 21)});
        }
        checkMax(n, edge);
    }

    checkCost(4, {{0, 1, 2, 1}, {0, 2, 1, 4}, {1, 2, 1, -2}, {1, 3, 1, 3}, {2, 3, 2, 1}});
    checkCost(3, {{0, 1, 1, -20}, {1, 2, 1, 7}});
    for (int it = 0; it < 2000; it++)
    {
        int n = rng() % 6 + 2;
        vector<array<long long, 4>> edge;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0)
                {
                    edge.push_back({u, v, (long long)(rng() % 4 + 1), (long long)(rng() % 21) - 10});
                }
            }
        }
        checkCost(n, edge);
    }

    BoundFlow feasible(2);
    feasible.add(0, 1, 1, 3);
    feasible.add(1, 0, 1, 3);
    assert(feasible.solve().has_value());
    BoundFlow impossible(2);
    impossible.add(0, 1, 1, 1);
    assert(!impossible.solve().has_value());
    BoundFlow range(2);
    range.add(0, 1, 2, 5);
    assert(range.solve(0, 1, false)->value == 5);
    assert(range.solve(0, 1, true)->value == 2);
    BoundFlow reverse(2);
    reverse.add(1, 0, 0, 10);
    assert(reverse.solve(0, 1, true)->value == 0);

    MinCostBFlow<long long, long long, __int128> bf(3);
    bf.supply(0, 2);
    bf.supply(2, -2);
    bf.add(0, 1, 0, 2, 3);
    bf.add(1, 2, 0, 2, -1);
    auto cert = bf.solveCert();
    assert(cert && cert->value == 4);
    assert(cert->flow == vector<long long>({2, 2}));
    assert(3 + cert->pot[0] - cert->pot[1] <= 0);
    assert(-1 + cert->pot[1] - cert->pot[2] <= 0);

    MinCostBFlow<long long, long long, __int128> neg(2);
    neg.supply(0, -3);
    neg.supply(1, 3);
    neg.add(0, 1, -3, 2, 4);
    neg.add(1, 0, 0, 5, 1);
    auto negCert = neg.solveCert();
    assert(negCert && negCert->value == -12 && negCert->flow == vector<long long>({-3, 0}));

    MinCostBFlow<long long, long long, __int128> loop(1);
    loop.add(0, 0, -2, 3, 5);
    auto loopCert = loop.solveCert();
    assert(loopCert && loopCert->value == -10 && loopCert->flow == vector<long long>({-2}));

    BinaryCut binary(2);
    binary.addCost(0, 3, 0);
    binary.addCost(1, 0, 2);
    binary.imply(0, 1);
    auto binaryResult = binary.solve();
    auto [cost, val] = binaryResult;
    assert(cost == 2 && val == vector<int>({1, 1}));

    for (int it = 0; it < 400; it++)
    {
        int n = rng() % 6 + 2;
        vector<tuple<int, int, long long>> edge;
        vector<vector<long long>> a(n, vector<long long>(n));
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                long long w = rng() % 9;
                if (w)
                {
                    edge.push_back({u, v, w});
                    a[u][v] = w;
                    a[v][u] = w;
                }
            }
        }
        GlobalCut global = stoerWagner(a);
        unsigned long long side = 0;
        for (int u : global.side)
        {
            side |= 1ULL << u;
        }
        assert(side && side + 1 != 1ULL << n);
        assert(global.w == cutValue(n, edge, side));
        __int128 want = pairCut(n, edge, 0, 1);
        for (int s = 0; s < n; s++)
        {
            for (int t = s + 1; t < n; t++)
            {
                want = min(want, pairCut(n, edge, s, t));
            }
        }
        assert(global.w == want);

        auto tree = gomoryHu(n, edge);
        assert((int)tree.size() == n - 1);
        for (int s = 0; s < n; s++)
        {
            for (int t = s + 1; t < n; t++)
            {
                assert(treeCut(n, tree, s, t) == pairCut(n, edge, s, t));
            }
        }
    }

    puts("OK");
    return 0;
}
