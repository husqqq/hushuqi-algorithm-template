#include "../include/full/2_1_001.hpp"

signed main()
{
    mt19937_64 rng(0);
    for (int n = 0; n <= 300; n++)
    {
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 31 - 15;
        }
        sort(a.begin(), a.end());
        for (int x = -20; x <= 20; x++)
        {
            assert(fastLower(a.begin(), a.end(), x) == lower_bound(a.begin(), a.end(), x));
            assert(fastUpper(a.begin(), a.end(), x) == upper_bound(a.begin(), a.end(), x));
        }
    }
    vector<pair<int, int>> a{{1, 4}, {1, 2}, {3, 0}};
    sort(a.begin(), a.end());
    auto cmp = [](const pair<int, int> &x, int y)
    {
        return x.first < y;
    };
    assert(fastLower(a.begin(), a.end(), 2, cmp) == a.begin() + 2);
    cout << "OK\n";
}
