#include "../include/full/3_2_001.hpp"

signed main()
{
    mt19937_64 rng(0);
    for (int n : {0, 1, 2, 15, 16, 17, 1003})
    {
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 1000;
        }
        BIT<int> bit(n);
        FastBIT<int> fast(a);
        for (int i = 0; i < n; i++)
        {
            bit.add(i, a[i]);
        }
        for (int q = 0; q < 3000 && n; q++)
        {
            int x = rng() % n;
            if (rng() & 1)
            {
                int v = rng() % 101 - 50;
                bit.add(x, v);
                fast.add(x, v);
            }
            else
            {
                int y = rng() % n;
                if (x > y)
                {
                    swap(x, y);
                }
                y++;
                assert(bit.sum(x, y) == fast.sum(x, y));
            }
        }
    }

    cout << "OK\n";
}
