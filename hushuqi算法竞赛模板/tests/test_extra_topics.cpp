#include "../include/extra_topics.hpp"

int ek(int n, const vector<array<int, 3>> &es, int s, int t)
{
    // 参数 n、es、s、t 是点数、有向容量边、源点和汇点；返回 Edmonds-Karp 最大流作为对拍基准。
    vector<vector<int>> c(n, vector<int>(n));
    for (auto [x, y, w] : es)
    {
        c[x][y] += w;
    }
    int ans = 0;
    while (true)
    {
        vector<int> p(n, -1);
        p[s] = s;
        queue<int> q;
        q.push(s);
        while (!q.empty() && p[t] == -1)
        {
            auto x = q.front();
            q.pop();
            for (int y = 0; y < n; y++)
            {
                if (p[y] == -1 && c[x][y] > 0) p[y] = x, q.push(y);
            }
        }
        if (p[t] == -1) break;
        int f = INF;
        for (int x = t; x != s; x = p[x])
        {
            f = min(f, c[p[x]][x]);
        }
        for (int x = t; x != s; x = p[x])
        {
            c[p[x]][x] -= f;
            c[x][p[x]] += f;
        }
        ans += f;
    }
    return ans;
}

signed main()
{
    mt19937_64 rng(0x20260723ULL);

    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 9;
        vector<Rect2> a(n);
        vector<vector<int>> g(9, vector<int>(9));
        for (auto &r : a)
        {
            r = {static_cast<int>(rng() % 9), static_cast<int>(rng() % 9), static_cast<int>(rng() % 9), static_cast<int>(rng() % 9)};
            if (r[0] > r[2]) swap(r[0], r[2]);
            if (r[1] > r[3]) swap(r[1], r[3]);
            for (int x = r[0]; x < r[2]; x++)
            {
                for (int y = r[1]; y < r[3]; y++)
                {
                    g[x][y]++;
                }
            }
        }
        int want = 0;
        for (auto &v : g)
        {
            for (auto x : v)
            {
                want += x >= 2;
            }
        }
        assert(rectCover2(a) == want);
    }

    Cover2Seg emptyCover;
    assert(emptyCover.twice() == 0);

    PerBitTrie high;
    high.append(1LL << 62);
    assert(high.maxXor(0, 1, 0) == (1LL << 62));

    for (int tc = 0; tc < 300; tc++)
    {
        int n = 1 + rng() % 80;
        vector<int> a(n);
        PerBitTrie tr(12);
        for (auto &x : a)
        {
            x = static_cast<int>(rng() % 4096);
            tr.append(x);
        }
        for (int z = 0; z < 300; z++)
        {
            int l = rng() % n;
            int r = l + 1 + rng() % (n - l);
            int x = rng() % 4096;
            int want = 0;
            for (int i = l; i < r; i++)
            {
                want = max(want, a[i] ^ x);
            }
            assert(tr.maxXor(l, r, x) == want);
        }
    }

    for (int tc = 0; tc < 300; tc++)
    {
        int n = 1 + rng() % 80;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = static_cast<int>(rng() % (n + 5));
        }
        StaticMex mex(a);
        for (int z = 0; z < 300; z++)
        {
            int l = rng() % (n + 1);
            int r = l + rng() % (n - l + 1);
            vector<char> has(n + 2);
            for (int i = l; i < r; i++)
            {
                if (a[i] <= n + 1) has[a[i]] = true;
            }
            int want = 0;
            while (has[want])
            {
                want++;
            }
            assert(mex.query(l, r) == want);
        }
    }

    for (int tc = 0; tc < 1000; tc++)
    {
        int n = 2 + rng() % 8;
        int s = rng() % n;
        int t = rng() % n;
        if (s == t) t = (t + 1) % n;
        vector<array<int, 3>> es;
        ISAP f(n);
        for (int i = 0; i < 25; i++)
        {
            int x = rng() % n;
            int y = rng() % n;
            int c = rng() % 20;
            if (x == y) continue;
            es.push_back({x, y, c});
            f.add(x, y, c);
        }
        assert(f.flow(s, t) == ek(n, es, s, t));
    }

    for (int n = 1; n <= 30; n++)
    {
        vector<vector<int>> g(n);
        for (int x = 1; x < n; x++)
        {
            int y = rng() % x;
            g[x].push_back(y);
            g[y].push_back(x);
        }
        auto z = preorderTab(g, n / 2);
        assert(static_cast<int>(z.ord.size()) == n);
        vector<int> seen(n);
        for (int i = 0; i < n; i++)
        {
            seen[z.ord[i]]++;
            assert(z.pos[z.ord[i]] == i);
        }
        assert(count(seen.begin(), seen.end(), 1) == n);
        for (int x = 0; x < n; x++)
        {
            for (int i = z.pos[x]; i < z.pos[x] + z.siz[x]; i++)
            {
                auto y = z.ord[i];
                while (y != -1 && y != x)
                {
                    y = z.par[y];
                }
                assert(y == x);
            }
        }
    }

    for (int m = 1; m <= 150; m++)
    {
        for (int d = 0; d < m; d++)
        {
            for (int l = 0; l < m; l++)
            {
                for (int r = l; r < m; r++)
                {
                    int want = -1;
                    for (int x = 0; x < m; x++)
                    {
                        auto v = d * x % m;
                        if (l <= v && v <= r)
                        {
                            want = x;
                            break;
                        }
                    }
                    assert(minModInt(m, d, l, r) == want);
                }
            }
        }
    }

    ValueGcd vg(100000);
    for (int tc = 0; tc < 1000000; tc++)
    {
        int x = 1 + rng() % 100000;
        int y = 1 + rng() % 100000;
        assert(vg.get(x, y) == gcd(x, y));
    }

    for (int n = 4; n <= 10000; n += 2)
    {
        auto [a, b] = goldbach(n);
        assert(a != -1 && a + b == n);
    }
    assert(frobenius2(4, 7) == 17);
    assert(frobenius2(4, 6) == -1);

    assert(rectKind({GeoPoint{0, 0}, {2, 0}, {2, 1}, {0, 1}}) == 1);
    assert(rectKind({GeoPoint{0, 0}, {1, 0}, {1, 1}, {0, 1}}) == 2);
    assert(rectKind({GeoPoint{0, 0}, {1, 0}, {2, 1}, {0, 1}}) == 0);
    auto cen = triCenters({0, 0}, {4, 0}, {0, 3});
    assert(fabsl(cen[0].x - 1) < 1E-12L && fabsl(cen[0].y - 1) < 1E-12L);
    assert(fabsl(cen[1].x - 2) < 1E-12L && fabsl(cen[1].y - 1.5L) < 1E-12L);
    assert(fabsl(cen[2].x) < 1E-12L && fabsl(cen[2].y) < 1E-12L);

    for (int tc = 0; tc < 10000; tc++)
    {
        int n = 1 + rng() % 8;
        vector<int> c(n), w(n);
        int all = 0;
        for (int i = 0; i < n; i++)
        {
            c[i] = rng() % 6;
            w[i] = 1 + rng() % 12;
            all += c[i] * w[i];
        }
        vector<char> can(all + 1);
        can[0] = true;
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < c[i]; k++)
            {
                for (int s = all; s >= w[i]; s--)
                {
                    can[s] = can[s] || can[s - w[i]];
                }
            }
        }
        for (int s = 0; s <= all; s++)
        {
            assert(boundSubset(c, w, s) == bool(can[s]));
        }
    }

    assert(bigSqrt("0") == "0");
    assert(bigSqrt("99980001") == "9999");
    assert(bigSqrt("152415787532388367501905199875019052100") == "12345678901234567890");
    for (int tc = 0; tc < 10000; tc++)
    {
        int x = rng() % 1000000000;
        unsigned long long n = static_cast<unsigned long long>(x) * x + rng() % (2 * x + 1);
        assert(bigSqrt(to_string(n)) == to_string(x));
    }

    auto q = quadRoots(1, -5, 6);
    assert(q.size() == 2 && fabsl(q[0] - 2) < 1E-12L && fabsl(q[1] - 3) < 1E-12L);
    assert(quadRoots(1, 0, 1).empty());
    for (int n = 1; n <= 200; n++)
    {
        auto p = nQueens(n);
        if (n == 2 || n == 3)
        {
            assert(p.empty());
            continue;
        }
        assert(static_cast<int>(p.size()) == n);
        vector<int> col(n), d1(2 * n), d2(2 * n);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= p[i] && p[i] < n);
            assert(!col[p[i]]++);
            assert(!d1[i + p[i]]++);
            assert(!d2[i - p[i] + n]++);
        }
    }

    cout << "OK\n";
}
