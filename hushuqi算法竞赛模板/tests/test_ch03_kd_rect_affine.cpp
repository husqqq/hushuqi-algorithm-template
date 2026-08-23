#include "../include/full/3_7_009.hpp"

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 30 + 1;
        vector<long long> x(n), y(n);
        vector<optional<long long>> w(n), brute(n);
        for (int i = 0; i < n; i++)
        {
            x[i] = (int)(rng() % 21) - 10;
            y[i] = (int)(rng() % 21) - 10;
            if (rng() & 1)
            {
                w[i] = brute[i] = (int)(rng() % 21) - 10;
            }
        }
        KDRectAffine tr(x, y, w);
        for (int rep = 0; rep < 150; rep++)
        {
            int op = rng() % 3;
            if (op == 0)
            {
                int id = rng() % n;
                long long z = (int)(rng() % 21) - 10;
                tr.set(id, z);
                brute[id] = z;
                continue;
            }
            long long xl = (int)(rng() % 25) - 12;
            long long xr = xl + rng() % 15;
            long long yl = (int)(rng() % 25) - 12;
            long long yr = yl + rng() % 15;
            if (op == 1)
            {
                long long a = (int)(rng() % 3) - 1;
                long long b = (int)(rng() % 11) - 5;
                tr.apply(xl, xr, yl, yr, a, b);
                for (int i = 0; i < n; i++)
                {
                    if (brute[i] && xl <= x[i] && x[i] < xr && yl <= y[i] && y[i] < yr)
                    {
                        *brute[i] = a * *brute[i] + b;
                    }
                }
            }
            else
            {
                long long want = 0;
                for (int i = 0; i < n; i++)
                {
                    if (brute[i] && xl <= x[i] && x[i] < xr && yl <= y[i] && y[i] < yr)
                    {
                        want += *brute[i];
                    }
                }
                assert(tr.query(xl, xr, yl, yr) == want);
                agg = agg * 1000000007ULL + want + 100000;
            }
        }
    }
    cout << agg << '\n';
    return 0;
}
