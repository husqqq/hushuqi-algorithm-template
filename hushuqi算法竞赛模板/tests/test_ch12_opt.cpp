#include "../include/full/12_3_001.hpp"
#include "../include/full/12_3_002.hpp"
#include "../include/full/12_3_003.hpp"
#include "../include/full/12_3_004.hpp"
#include "../include/full/12_3_005.hpp"

signed main()
{
    assert(windowDP({0, 5, 1, 4}, 2) == vector<int>({0, 5, 1, 5}));

    vector<int> a{1, 2, 3};
    vector<int> pre(a.size() + 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        pre[i + 1] = pre[i] + a[i];
    }
    auto cost = [&](int l, int r)
    {
        return pre[r] - pre[l];
    };
    assert(quad_opt::knuth(3, cost) == 9);
    assert(quad_opt::knuth(1, cost) == 0);
    assert(quad_opt::knuth(2, [](int, int)
                                          { return LLONG_MAX; }) == LLONG_MAX);

    vector<int> x{0, 1, 3, 6};
    vector<int> old{7, 2, 5, 0};
    auto w = [&](int j, int i) -> __int128
    {
        __int128 d = (__int128)x[i] - x[j];
        return d * d;
    };
    auto got = dc_dp::divideLayer(old, 3, w);
    for (int i = 1; i <= 3; i++)
    {
        __int128 want = (__int128)1 << 120;
        for (int j = 0; j < i; j++)
        {
            want = min(want, (__int128)old[j] + w(j, i));
        }
        assert(got[i] == want);
    }
    auto edge = dc_dp::divideLayer(vector<int>{LLONG_MAX, 0}, 1,
                                                [](int, int)
                                                { return 0; });
    assert(edge[1] == LLONG_MAX);

    MonoHull h;
    assert(!h.query(-10));
    MonoHull q;
    q.add(3, 1);
    q.add(3, 2);
    q.add(1, 5);
    q.add(-2, 20);
    assert(q.query(-5) == -14);
    assert(q.query(0) == 1);
    assert(q.query(10) == 0);

    MonoHull<__int128> wideHull;
    vector<pair<int, int>> lines{{LLONG_MAX, LLONG_MIN}, {0, LLONG_MAX},
                                 {LLONG_MIN, LLONG_MIN}};
    for (auto [k, b] : lines)
    {
        wideHull.add(k, b);
    }
    for (int x : {LLONG_MIN, -1LL, 0LL, 1LL, LLONG_MAX})
    {
        __int128 want = ((__int128)1 << 126);
        for (auto [k, b] : lines)
        {
            want = min(want, (__int128)k * x + b);
        }
        assert(wideHull.query(x) == want);
    }

    assert(wqsPartition<__int128>({0, 3000000000LL, 6000000000LL}, 2)
           == (__int128)18000000000000000000ULL);
    assert(wqsPartition({0, 1, 2, 3, 4}, 1) == 16);
    assert(wqsPartition({0, 1, 2, 3, 4}, 4) == 4);
    cout << "OK\n";
}
