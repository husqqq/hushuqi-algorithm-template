#include "../include/full/10_1_014.hpp"
#include "../include/full/10_1_015.hpp"

void testMulti()
{
    mt19937_64 rng(0);
    Z sum = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        int k = rng() % 4 + 1;
        vector<int> base(k);
        int n = 1;
        for (auto &x : base)
        {
            x = rng() % 3 + 1;
            n *= x;
        }
        vector<Z> a(n), b(n), want(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = rng() % 100;
            b[i] = rng() % 100;
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int w = 1;
                int p = 0;
                bool ok = true;
                for (int x : base)
                {
                    int d = i / w % x + j / w % x;
                    if (d >= x)
                    {
                        ok = false;
                        break;
                    }
                    p += d * w;
                    w *= x;
                }
                if (ok)
                {
                    want[p] += a[i] * b[j];
                }
            }
        }
        auto got = multiConv(base, a, b);
        assert(got == want);
        for (auto x : got)
        {
            sum += x;
        }
    }
    cout << sum.val() << '\n';
}

void testMultiCyclic()
{
    mt19937_64 rng(1);
    for (int tc = 0; tc < 200; tc++)
    {
        long long p = 17;
        vector<int> base = tc & 1 ? vector<int>{2, 4} : vector<int>{2, 2, 2};
        int n = 8;
        vector<long long> a(n), b(n), want(n);
        for (auto &x : a) x = rng() % p;
        for (auto &x : b) x = rng() % p;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int w = 1, at = 0;
                for (int len : base)
                {
                    at += (i / w % len + j / w % len) % len * w;
                    w *= len;
                }
                want[at] = (want[at] + a[i] * b[j]) % p;
            }
        }
        assert(multiCycConv(p, base, a, b) == want);
    }
    assert(multiCycConv(2, {}, {1}, {1}) == vector<long long>{1});
}

void testMulMod2n()
{
    mt19937_64 rng(0);
    Z sum = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        int n = rng() % 8;
        int m = 1LL << n;
        vector<Z> a(m), b(m), want(m);
        for (int i = 0; i < m; i++)
        {
            a[i] = rng() % 100;
            b[i] = rng() % 100;
        }
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                want[i * j & (m - 1)] += a[i] * b[j];
            }
        }
        auto got = mulMod2n(a, b);
        assert(got == want);
        for (auto x : got)
        {
            sum += x;
        }
    }
    cout << sum.val() << '\n';
}

signed main()
{
    testMulti();
    testMultiCyclic();
    testMulMod2n();
    return 0;
}
