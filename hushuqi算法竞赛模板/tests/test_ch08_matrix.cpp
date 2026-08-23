#include "../include/full/8_2_011.hpp"

long long bruteSpan(int n, const vector<pair<int, int>> &e)
{
    // n 是点数，e 是无向边表；枚举边子集返回生成树数。
    int m = e.size();
    long long cnt = 0;
    for (int s = 0; s < (1LL << m); s++)
    {
        if (popcount((unsigned long long)s) != n - 1)
        {
            continue;
        }
        vector<int> f(n);
        iota(f.begin(), f.end(), 0);
        auto find = [&](auto &&self, int x) -> int
        {
            if (f[x] == x)
            {
                return x;
            }
            return f[x] = self(self, f[x]);
        };
        int comp = n;
        bool ok = true;
        for (int i = 0; i < m; i++)
        {
            if (!(s >> i & 1))
            {
                continue;
            }
            int a = find(find, e[i].first);
            int b = find(find, e[i].second);
            if (a == b)
            {
                ok = false;
                break;
            }
            f[a] = b;
            comp--;
        }
        if (ok && comp == 1)
        {
            cnt++;
        }
    }
    return cnt;
}

long long bruteArb(int n, int root, const vector<pair<int, int>> &e)
{
    // n 是点数，root 是根，e 是有向边表；枚举边子集返回向外树形图数。
    int m = e.size();
    long long cnt = 0;
    for (int s = 0; s < (1LL << m); s++)
    {
        if (popcount((unsigned long long)s) != n - 1)
        {
            continue;
        }
        vector<int> indeg(n);
        vector<vector<int>> g(n);
        for (int i = 0; i < m; i++)
        {
            if (!(s >> i & 1))
            {
                continue;
            }
            auto [u, v] = e[i];
            indeg[v]++;
            g[u].push_back(v);
        }
        bool ok = true;
        for (int v = 0; v < n; v++)
        {
            if (v == root)
            {
                if (indeg[v] != 0)
                {
                    ok = false;
                }
            }
            else if (indeg[v] != 1)
            {
                ok = false;
            }
        }
        if (!ok)
        {
            continue;
        }
        vector<int> vis(n);
        vis[root] = 1;
        int seen = 1;
        auto dfs = [&](auto &&self, int u) -> void
        {
            for (int v : g[u])
            {
                if (!vis[v])
                {
                    vis[v] = 1;
                    seen++;
                    self(self, v);
                }
            }
        };
        dfs(dfs, root);
        if (seen == n)
        {
            cnt++;
        }
    }
    return cnt;
}

signed main()
{
    assert(spanTreeCnt(1, {}).val() == 1);
    assert(arboCount(1, 0, {}).val() == 1);
    assert(spanTreeCnt(3, {{0, 1}, {1, 2}, {2, 0}}).val() == 3);
    assert(spanTreeCnt(2, {{0, 1}, {0, 1}, {0, 1}}).val() == 3);
    assert(spanTreeCnt(2, {{0, 0}, {0, 1}}).val() == 1);
    assert(spanTreeCnt(3, {{0, 1}}).val() == 0);
    assert(arboCount(3, 0, {{0, 1}, {0, 2}, {1, 2}}).val() == 2);
    assert(arboCount(2, 0, {{0, 1}, {0, 1}}).val() == 2);
    assert(arboCount(2, 0, {{0, 0}, {1, 1}, {0, 1}}).val() == 1);
    assert(arboCount(3, 0, {{0, 1}}).val() == 0);

    mt19937_64 rng(0);
    unsigned long long acc = 0;
    for (int it = 0; it < 600; it++)
    {
        int n = 1 + rng() % 5;
        int m = rng() % 8;
        vector<pair<int, int>> e;
        for (int i = 0; i < m; i++)
        {
            int u = rng() % n;
            int v = rng() % n;
            if (u != v)
            {
                e.push_back({u, v});
            }
        }
        Z sp = spanTreeCnt(n, e);
        assert(sp.val() == bruteSpan(n, e) % mod);
        int root = rng() % n;
        Z ar = arboCount(n, root, e);
        assert(ar.val() == bruteArb(n, root, e) % mod);
        acc += (unsigned long long)sp.val() + ar.val();
    }
    cout << acc << '\n';
    return 0;
}
