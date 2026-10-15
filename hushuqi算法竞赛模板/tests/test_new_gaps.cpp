#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../include/full/2_4_007.hpp"
#include "../include/full/7_3_014.hpp"
#include "../include/full/7_3_015.hpp"
#include "../include/full/8_1_013.hpp"
#include "../include/full/8_2_018.hpp"
#include "../include/full/13_2_005.hpp"

signed main()
{
    // 无参数；用固定种子验证六个新增条目的边界与随机行为，返回 0。
    mt19937 rng(20261015);
    for (int n = 0; n <= 80; n++)
    {
        vector<int> v(n);
        for (int &x : v)
        {
            x = (int)(rng() % 101) - 50;
        }
        SqrtRangeAdd a(v);
        for (int q = 0; q < 400; q++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            if (q & 1)
            {
                int x = (int)(rng() % 101) - 50;
                a.add(l, r, x);
                for (int i = l; i < r; i++)
                {
                    v[i] += x;
                }
            }
            else
            {
                assert(a.query(l, r) == accumulate(v.begin() + l, v.begin() + r, 0LL));
            }
        }
    }
    const int lim = 20000;
    vector<int> pi(lim + 1), ps(lim + 1), pw;
    for (int x = 1; x <= lim; x++)
    {
        bool prime = x >= 2, powerful = true;
        int left = x;
        for (int p = 2; p <= x / p; p++)
        {
            if (x % p == 0) prime = false;
            int e = 0;
            while (left % p == 0)
            {
                left /= p;
                e++;
            }
            if (e == 1) powerful = false;
        }
        if (left > 1) powerful = false;
        if (powerful) pw.push_back(x);
        pi[x] = pi[x - 1] + prime;
        ps[x] = ps[x - 1] + (prime ? x : 0);
    }
    assert(lucyPrime(0).empty() && powerfulNums(0).empty());
    for (int trial = 0; trial < 500; trial++)
    {
        int n = trial < 100 ? trial : rng() % lim;
        auto a = lucyPrime(n);
        vector<int> quot;
        for (int i = 1; i <= n; i++)
        {
            quot.push_back(n / i);
        }
        sort(quot.begin(), quot.end());
        quot.erase(unique(quot.begin(), quot.end()), quot.end());
        assert(a.size() == quot.size());
        for (int i = 0; i < (int)a.size(); i++)
        {
            assert(a[i].x == quot[i] && a[i].cnt == pi[quot[i]] && a[i].sum == ps[quot[i]]);
        }
        auto it = upper_bound(pw.begin(), pw.end(), n);
        assert(powerfulNums(n) == vector<int>(pw.begin(), it));
        auto hp = [](int p, int e) -> __int128
        {
            // p、e 是素数和指数；返回仅偶次幂非零的平方约数卷积核。
            if (e & 1) return 0;
            __int128 x = 1;
            for (int i = 0; i < e; i++)
            {
                x *= p;
            }
            return x;
        };
        auto gs = [](int m) -> __int128
        {
            // m 是前缀上界；返回常值函数 1 的前缀和。
            return m;
        };
        __int128 sum = 0;
        for (int d = 1; d <= n / d; d++)
        {
            sum += (__int128)d * d * (n / (d * d));
        }
        assert(powerfulSum<__int128>(n, hp, gs) == sum);
        auto one = [](int, int) { return 1LL; };
        assert(powerfulSum<int>(n, one, [](int x) { return x == 0 ? 0LL : 1LL; }) == it - pw.begin());
    }
    assert(stirlingTr(vector<Z>{}).empty());
    assert((stirlingTr(vector<Z>{1, 2, 3, 4}) == vector<Z>{1, 2, 5, 15}));
    for (int n = 0; n <= 60; n++)
    {
        vector<Z> a(n + 1);
        for (auto &x : a)
        {
            x = rng();
        }
        assert(stirlingTr(stirlingTr(a), true) == a);
        assert(stirlingTr(stirlingTr(a, true)) == a);
    }
    assert(lgvPaths({}, {}, {}, {}) == Z(1));
    // 对小 DAG 枚举单条路径，然后枚举不交路径组，包含负权、重边与两个端点排列。
    for (int trial = 0; trial < 400; trial++)
    {
        int n = 6;
        vector<vector<pair<int, Z>>> g(n);
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0) g[u].push_back({v, (int)(rng() % 5) - 2});
            }
        }
        if (trial % 3 == 0) g[0].push_back({4, 1});
        vector<int> ord{0, 1, 2, 3, 4, 5}, s{0, 1}, t{4, 5};
        vector<vector<vector<pair<int, Z>>>> paths(2, vector<vector<pair<int, Z>>>(2));
        for (int i = 0; i < 2; i++)
        {
            auto dfs = [&](auto &&self, int u, int mask, Z w) -> void
            {
                for (int j = 0; j < 2; j++)
                {
                    if (u == t[j]) paths[i][j].push_back({mask, w});
                }
                for (auto [v, cost] : g[u])
                {
                    self(self, v, mask | (1LL << v), w * cost);
                }
            };
            dfs(dfs, s[i], 1LL << s[i], 1);
        }
        Z sum = 0;
        for (int j = 0; j < 2; j++)
        {
            for (auto [a, x] : paths[0][j])
            {
                for (auto [b, y] : paths[1][1 - j])
                {
                    if (!(a & b)) sum += j == 0 ? x * y : -x * y;
                }
            }
        }
        assert(lgvPaths(g, ord, s, t) == sum);
    }
    assert(expectedDAG({}, {}, {}).empty());
    for (int trial = 0; trial < 200; trial++)
    {
        int n = 8;
        vector<vector<pair<int, long double>>> g(n);
        vector<long double> cost(n);
        vector<int> ord(n);
        iota(ord.rbegin(), ord.rend(), 0);
        for (int u = 0; u < n; u++)
        {
            cost[u] = (int)(rng() % 21) - 10;
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0) g[u].push_back({v, 0.1L});
            }
        }
        auto e = expectedDAG(g, cost, ord);
        for (int u = 0; u < n; u++)
        {
            long double sum = 0;
            auto dfs = [&](auto &&self, int v, long double prob) -> void
            {
                sum += prob * cost[v];
                for (auto [w, p] : g[v])
                {
                    self(self, w, prob * p);
                }
            };
            dfs(dfs, u, 1);
            assert(fabsl(sum - e[u]) < 1E-10L);
        }
    }
    cout << "OK\n";
}
