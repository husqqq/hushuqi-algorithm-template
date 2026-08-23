#include "../include/full/3_7_004.hpp"

signed main()
{
    KDTree<> empty(vector<array<int, 3>>{});
    assert(!empty.nearest(0, 0));
    assert(empty.rectSum(-1, 1, -1, 1) == 0);

    KDTree<> one(vector<array<int, 3>>{{5, -2, 7}});
    assert(one.rectSum(5, 5, -2, -2) == 7);
    assert(one.nearest(5, -2) == optional<long long>(0));

    KDTree<__int128> wide(vector<array<int, 3>>{{3000000000LL, 3000000000LL, 1}});
    __int128 d = 6000000000LL;
    assert(wide.nearest(-3000000000LL, -3000000000LL) == optional<__int128>(2 * d * d));

    mt19937_64 rng(0);
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 40 + 1;
        vector<array<int, 3>> a(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = {(int)(rng() % 200) - 100, (int)(rng() % 200) - 100, (int)(rng() % 20) + 1};
        }
        KDTree<> tr(a);
        int xl = (int)(rng() % 200) - 100, xr = xl + (int)(rng() % 100);
        int yl = (int)(rng() % 200) - 100, yr = yl + (int)(rng() % 100);
        int sum = 0;
        for (auto &p : a)
        {
            if (xl <= p[0] && p[0] <= xr && yl <= p[1] && p[1] <= yr)
            {
                sum += p[2];
            }
        }
        assert(tr.rectSum(xl, xr, yl, yr) == sum);

        int x = (int)(rng() % 200) - 100, y = (int)(rng() % 200) - 100;
        long long best = numeric_limits<long long>::max();
        for (auto &p : a)
        {
            long long dx = p[0] - x, dy = p[1] - y;
            best = min(best, dx * dx + dy * dy);
        }
        assert(tr.nearest(x, y) == optional<long long>(best));
    }
    cout << "OK\n";
}
