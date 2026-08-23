#include "../include/full/10_1_012.hpp"

signed main()
{
    long long lo = numeric_limits<long long>::min();
    long long hi = numeric_limits<long long>::max();
    assert((minPlus<__int128>({hi, lo}, {1, -1}) ==
            vector<__int128>{(__int128)hi + 1, (__int128)lo + 1, (__int128)lo - 1}));
    assert((minPlusCC<__int128>({0, 1, 4}, {0, 2, 7}) ==
            vector<__int128>{0, 1, 3, 6, 11}));
    assert((minPlusCA<__int128>({0, 1, 4}, {5, 0, 6}) ==
            vector<__int128>{5, 0, 1, 4, 10}));
    mt19937_64 rng(123);
    for (int it = 0; it < 500; it++)
    {
        int n = 1 + rng() % 20, m = 1 + rng() % 20;
        vector<long long> a(n), b(m);
        long long diff = rng() % 20;
        a[0] = rng() % 30;
        for (int i = 1; i < n; i++)
        {
            diff -= rng() % 4;
            a[i] = a[i - 1] + diff;
        }
        for (auto &x : b) x = rng() % 100;
        assert(minPlusConc(a, b) == minPlus(a, b));
    }
    cout << "OK\n";
    return 0;
}
