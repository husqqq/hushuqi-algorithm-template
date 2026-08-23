
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Clique64
{
    int n, best = 0;              // n 是点数，best 是已知最大团大小。
    unsigned long long ans = 0;   // ans 是当前最大团的点位集。
    vector<unsigned long long> e; // e[u] 的第 v 位表示无向边 u-v。

    Clique64(int n = 0) : n(n), e(n)
    {
        // n 是点数；构造空图。
        assert(0 <= n && n <= 64); // 调试检查，可删。
    }

    void add(int u, int v)
    {
        // u、v 是端点；加入无向边，自环直接忽略，无返回值。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        if (u == v)
        {
            return;
        }
        e[u] |= 1ULL << v;
        e[v] |= 1ULL << u;
    }

    void dfs(unsigned long long r, unsigned long long p)
    {
        // r 是已选团，p 是仍可加入的点集；用分支限界更新最大团，无返回值。
        int z = popcount(r);
        if (z + popcount(p) <= best)
        {
            return;
        }
        if (!p)
        {
            best = z;
            ans = r;
            return;
        }
        int u = countr_zero(p);
        for (auto q = p; q; q &= q - 1)
        {
            int v = countr_zero(q);
            if (popcount(p & e[v]) > popcount(p & e[u]))
            {
                u = v;
            }
        }
        unsigned long long q = p & ~e[u];
        while (q)
        {
            int v = countr_zero(q);
            dfs(r | 1ULL << v, p & e[v]);
            p &= ~(1ULL << v);
            q &= q - 1;
            if (z + popcount(p) <= best)
            {
                return;
            }
        }
    }

    vector<int> maxClique()
    {
        // 无参数；返回当前图的一组最大团顶点。
        best = 0;
        ans = 0;
        dfs(0, n == 64 ? ~0ULL : (1ULL << n) - 1);
        vector<int> a;
        for (int i = 0; i < n; i++)
        {
            if (ans >> i & 1)
            {
                a.push_back(i);
            }
        }
        return a;
    }

    vector<int> maxIndependent()
    {
        // 无参数；返回当前图的一组最大独立集顶点。
        unsigned long long all = n == 64 ? ~0ULL : (1ULL << n) - 1;
        auto old = e;
        for (int i = 0; i < n; i++)
        {
            e[i] = all & ~(old[i] | 1ULL << i);
        }
        auto res = maxClique();
        e = move(old);
        return res;
    }

    template <class F>
    void maximalCliques(F f)
    {
        // f 接收一个极大团的点位集；枚举全部极大团并逐个调用 f，无返回值。
        auto go = [&](auto &&self, unsigned long long r, unsigned long long p, unsigned long long x) -> void
        {
            if (!(p | x))
            {
                f(r);
                return;
            }
            int u = countr_zero(p | x);
            unsigned long long q = p & ~e[u];
            while (q)
            {
                int v = countr_zero(q);
                self(self, r | 1ULL << v, p & e[v], x & e[v]);
                p &= ~(1ULL << v);
                x |= 1ULL << v;
                q &= q - 1;
            }
        };
        go(go, 0, n == 64 ? ~0ULL : (1ULL << n) - 1, 0);
    }
};

long long weightedCliques(int n, const vector<long long> &val, const vector<pair<int, int>> &e, long long mod)
{
    // n 是点数，val 是点权，e 是简单无向边，mod 是正模数；返回所有非空团点权乘积之和模 mod。
    assert((int)val.size() == n && mod > 0); // 调试检查，可删。
    vector<vector<int>> g(n);
    vector<vector<char>> adj(n, vector<char>(n));
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v && !adj[u][v]); // 调试检查，可删。
        g[u].push_back(v);
        g[v].push_back(u);
        adj[u][v] = adj[v][u] = 1;
    }
    int lim = sqrt(2.0 * e.size()) + 1;
    vector<char> alive(n, 1);
    auto go = [&](auto &&self, int rem) -> long long
    {
        if (rem == 0) return 0;
        int x = -1;
        for (int u = 0; u < n; u++)
        {
            if (alive[u] && (int)g[u].size() < lim)
            {
                x = u;
                break;
            }
        }
        if (x == -1)
        {
            vector<int> vs;
            for (int u = 0; u < n; u++) if (alive[u]) vs.push_back(u);
            long long ans = 0;
            for (int mask = 1; mask < (1 << vs.size()); mask++)
            {
                long long cur = 1;
                bool ok = true;
                for (int i = 0; i < (int)vs.size() && ok; i++)
                {
                    if (!(mask >> i & 1)) continue;
                    cur = (__int128)cur * (val[vs[i]] % mod) % mod;
                    for (int j = 0; j < i; j++)
                    {
                        if ((mask >> j & 1) && !adj[vs[i]][vs[j]])
                        {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) ans = (ans + cur) % mod;
            }
            return ans;
        }
        long long ans = 0;
        int d = g[x].size();
        for (int mask = 0; mask < (1 << d); mask++)
        {
            long long cur = val[x] % mod;
            bool ok = true;
            for (int i = 0; i < d && ok; i++)
            {
                if (!(mask >> i & 1)) continue;
                int u = g[x][i];
                if (!alive[u])
                {
                    ok = false;
                    break;
                }
                cur = (__int128)cur * (val[u] % mod) % mod;
                for (int j = 0; j < i; j++)
                {
                    if ((mask >> j & 1) && !adj[u][g[x][j]])
                    {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) ans = (ans + cur) % mod;
        }
        alive[x] = 0;
        return (ans + self(self, rem - 1)) % mod;
    };
    return (go(go, n) + mod) % mod;
}

signed main()
{
    int n, m; cin >> n >> m;
    vector<long long> val(n);
    for (long long &x : val) cin >> x;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    cout << weightedCliques(n, val, e, 998244353) << '\n';
}
