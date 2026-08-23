#include "../include/full/3_8_003.hpp"

signed main()
{
    vector<unsigned long long> a{5, 3, 8, 6, 1};
    RangeBasis<unsigned long long, 8> b(a);
    for (int l = 0; l < (int)a.size(); l++)
    {
        for (int r = l; r < (int)a.size(); r++)
        {
            unsigned long long want = 0;
            unsigned long long want2 = 2;
            int n = r - l + 1;
            for (int mask = 0; mask < (1LL << n); mask++)
            {
                unsigned long long x = 0;
                for (int i = 0; i < n; i++)
                {
                    if (mask >> i & 1)
                    {
                        x ^= a[l + i];
                    }
                }
                want = max(want, x);
                want2 = max(want2, x ^ 2);
            }
            assert(b.maxXor(l, r) == want);
            assert(b.maxXor(l, r, 2) == want2);
        }
    }
    cout << "OK\n";
}
