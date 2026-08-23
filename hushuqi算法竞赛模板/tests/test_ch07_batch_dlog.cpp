#include "../include/full/7_4_008.hpp"

vector<int> factors(int n)
{
    vector<int> p;
    for (int x = 2; x * x <= n; x++)
    {
        if (n % x == 0)
        {
            p.push_back(x);
            while (n % x == 0)
            {
                n /= x;
            }
        }
    }
    if (n > 1)
    {
        p.push_back(n);
    }
    return p;
}

int primRoot(int p)
{
    auto f = factors(p - 1);
    for (int g = 1; g < p; g++)
    {
        bool ok = true;
        for (int q : f)
        {
            if (powMod64(g, (p - 1) / q, p) == 1)
            {
                ok = false;
            }
        }
        if (ok)
        {
            return g;
        }
    }
    return -1;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int p = 2; p <= 3000; p++)
    {
        bool prime = true;
        for (int d = 2; d * d <= p; d++)
        {
            if (p % d == 0)
            {
                prime = false;
            }
        }
        if (!prime)
        {
            continue;
        }
        int g = primRoot(p);
        vector<int> lg(p), a;
        long long x = 1;
        for (int e = 0; e < p - 1; e++)
        {
            lg[x] = e;
            x = x * g % p;
        }
        int q = min<long long>(p - 1, rng() % 100 + 1);
        for (int i = 0; i < q; i++)
        {
            a.push_back(rng() % (p - 1) + 1);
        }
        auto got = batchDLog(p, g, a);
        for (int i = 0; i < q; i++)
        {
            assert(got[i] == lg[a[i]]);
            agg = agg * 1000000007ULL + got[i];
        }
    }
    cout << agg << '\n';
    return 0;
}
