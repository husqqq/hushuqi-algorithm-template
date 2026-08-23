#include "../include/full/3_5_003.hpp"

signed main()
{
    mt19937_64 rng(0);
    unsigned long long acc = 0;
    for (int it = 0; it < 300; it++)
    {
        PerTreap<long long> tr;
        int rt = 0;
        vector<long long> b;
        int snapRt = 0;
        vector<long long> snapB;
        for (int t = 0; t < 60; t++)
        {
            int op = rng() % 4;
            int n = b.size();
            if (op == 0 || n == 0)
            {
                int k = rng() % (n + 1);
                long long v = (long long)(rng() % 1000);
                rt = tr.insert(rt, k, v);
                b.insert(b.begin() + k, v);
            }
            else if (op == 1)
            {
                int l = rng() % n, r = l + 1 + rng() % (n - l);
                rt = tr.reverse(rt, l, r);
                reverse(b.begin() + l, b.begin() + r);
            }
            else if (op == 2)
            {
                int k = rng() % n;
                rt = tr.erase(rt, k);
                b.erase(b.begin() + k);
            }
            else
            {
                int l = rng() % n, r = l + 1 + rng() % (n - l);
                long long s = 0;
                for (int i = l; i < r; i++)
                {
                    s += b[i];
                }
                assert(tr.query(rt, l, r) == s);
                acc += (unsigned long long)s;
            }
            if (t == 30)
            {
                snapRt = rt;
                snapB = b;
            }
        }
        assert(tr.values(rt) == b);
        assert(tr.values(snapRt) == snapB);
    }
    cout << acc << '\n';
    return 0;
}
