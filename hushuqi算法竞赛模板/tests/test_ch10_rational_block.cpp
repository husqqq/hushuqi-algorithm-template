#include "../include/full/10_4_006.hpp"

Poly brute(const Poly &p, const Poly &q, int n)
{
    Poly a(n);
    for (int i = 0; i < n; i++)
    {
        Z z = i < (int)p.size() ? p[i] : Z(0);
        for (int j = 1; j < (int)q.size() && j <= i; j++)
        {
            z -= q[j] * a[i - j];
        }
        a[i] = z / q[0];
    }
    return a;
}

signed main()
{
    assert((ratBlock({0, 1}, {1, -1, -1}, 10, 4) == Poly{55, 89, 144, 233}));
    assert((ratBlock({1}, {1, -2, 1}, 100, 3) == Poly{101, 102, 103}));
    assert((ratBlock({1, 0, 0, 1}, {1, -1}, 0, 6) == Poly{1, 1, 1, 2, 2, 2}));
    assert((ratBlock({2, 4, 6}, {2}, 1, 3) == Poly{2, 3, 0}));
    assert(ratBlock({1}, {1, -1}, 0, 0).empty());
    assert((ratBlock({3, 4, 5}, {1, 0, 0}, 0, 5) == Poly{3, 4, 5, 0, 0}));
    assert((ratBlock({1}, {1, -1}, numeric_limits<unsigned long long>::max(), 1) == Poly{1}));

    auto [zp, zq] = recFraction({3, 4, 5}, {0, 0, 0});
    assert((ratBlock(zp, zq, 0, 6) == Poly{3, 4, 5, 0, 0, 0}));

    mt19937_64 rng(0);
    for (int tc = 0; tc < 500; tc++)
    {
        int dp = rng() % 8;
        int dq = rng() % 5;
        Poly p(dp + 1), q(dq + 1 + rng() % 3);
        for (auto &x : p) x = rng() % 21 - 10;
        for (int i = 0; i <= dq; i++) q[i] = rng() % 21 - 10;
        if (q[0] == Z(0)) q[0] = 1;
        if (q[dq] == Z(0)) q[dq] = 1;
        int k = rng() % 30;
        int m = rng() % 10;
        Poly a = brute(p, q, k + m);
        Poly got = ratBlock(p, q, k, m);
        assert((int)got.size() == m);
        for (int i = 0; i < m; i++)
        {
            assert(got[i] == a[k + i]);
        }
    }

    for (int tc = 0; tc < 100; tc++)
    {
        int d = rng() % 8 + 1;
        Poly q(d + 1);
        q[0] = 1;
        for (int i = 1; i <= d; i++) q[i] = rng() % 21 - 10;
        if (q.back() == Z(0)) q.back() = 1;
        unsigned long long k = rng() % 100;
        Poly a = brute({1}, q, k + d);
        Poly got = invTail(q, k);
        assert((int)got.size() == d);
        for (int i = 0; i < d; i++) assert(got[i] == a[k + i]);

        Poly p(d);
        for (auto &x : p) x = rng() % 21 - 10;
        k += 1ULL << 60;
        got = ratBlock(p, q, k, 5);
        for (int i = 0; i < 5; i++)
        {
            assert(got[i] == bostanMori(p, q, k + i));
        }
    }
    cout << "OK\n";
    return 0;
}
