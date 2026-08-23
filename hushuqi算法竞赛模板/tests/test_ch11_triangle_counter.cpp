#include "../include/full/11_2_010.hpp"

bool inside(Point<long long> a, Point<long long> b, Point<long long> c, Point<long long> p)
{
    long long x = cross(a, b, p);
    long long y = cross(b, c, p);
    long long z = cross(c, a, p);
    return (x > 0 && y > 0 && z > 0) || (x < 0 && y < 0 && z < 0);
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 2000; tc++)
    {
        int n = rng() % 10 + 1, m = rng() % 20;
        vector<Point<long long>> a(n), b(m);
        for (auto &p : a)
        {
            p = {(long long)(rng() % 15) - 7, (long long)(rng() % 15) - 7};
        }
        for (auto &p : b)
        {
            p = {(long long)(rng() % 15) - 7, (long long)(rng() % 15) - 7};
        }
        TriPointCnt q(a, b);
        for (int rep = 0; rep < 100; rep++)
        {
            int x = rng() % n, y = rng() % n, z = rng() % n;
            int want = 0;
            for (auto p : b)
            {
                want += inside(a[x], a[y], a[z], p);
            }
            int got = q.count(x, y, z);
            assert(got == want);
            agg = agg * 1000000007ULL + got;
        }
    }
    cout << agg << '\n';
    return 0;
}
