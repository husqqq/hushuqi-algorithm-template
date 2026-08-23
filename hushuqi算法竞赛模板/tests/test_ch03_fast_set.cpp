#include "../include/full/3_6_014.hpp"

signed main()
{
    FastSet empty(0);
    assert(empty.next(0) == -1 && empty.prev(-1) == -1);

    FastSet one(1);
    assert(one.next(0) == -1 && one.prev(0) == -1 && one.prev(-1) == -1);
    one.set(0);
    one.set(0);
    assert(one.test(0) && one.next(0) == 0 && one.prev(0) == 0 && one.next(1) == -1);
    one.reset(0);
    one.reset(0);
    assert(!one.test(0));

    FastSet s(130);
    for (int x : {0, 63, 64, 129})
    {
        s.set(x);
    }
    assert(s.next(0) == 0 && s.next(1) == 63 && s.next(63) == 63);
    assert(s.next(64) == 64 && s.next(65) == 129 && s.next(129) == 129 && s.next(130) == -1);
    assert(s.prev(-1) == -1 && s.prev(0) == 0 && s.prev(62) == 0 && s.prev(63) == 63);
    assert(s.prev(64) == 64 && s.prev(128) == 64 && s.prev(129) == 129);
    s.reset(64);
    assert(s.next(64) == 129 && s.prev(64) == 63);

    FastSet deep(64 * 64 + 1);
    deep.set(0);
    deep.set(2048);
    deep.set(4096);
    assert(deep.next(1) == 2048 && deep.prev(4095) == 2048);
    assert(deep.next(2049) == 4096 && deep.prev(4096) == 4096);
    deep.reset(2048);
    assert(deep.next(1) == 4096 && deep.prev(4095) == 0);

    FastSet large(10'000'000);
    large.set(0);
    large.set(9'999'999);
    assert(large.next(1) == 9'999'999 && large.prev(9'999'998) == 0);
    assert(large.next(10'000'000) == -1 && large.prev(-1) == -1);
    for (int q = 0; q < 1'000'000; q++)
    {
        int x = q * 9'999'983 % 10'000'000;
        large.set(x);
        assert(large.test(x));
    }

    mt19937_64 rng(0);
    constexpr int n = 64 * 64 * 64 + 1;
    FastSet randomSet(n);
    set<int> brute;
    for (int q = 0; q < 20'000; q++)
    {
        int x = rng() % n;
        if (rng() % 3 == 0)
        {
            randomSet.set(x);
            brute.insert(x);
        }
        else if (rng() % 3 == 0)
        {
            randomSet.reset(x);
            brute.erase(x);
        }
        else
        {
            assert(randomSet.test(x) == brute.contains(x));
        }
        int y = rng() % n;
        auto it = brute.lower_bound(y);
        int wantNext = it == brute.end() ? -1 : *it;
        auto jt = brute.upper_bound(y);
        int wantPrev = jt == brute.begin() ? -1 : *prev(jt);
        assert(randomSet.next(y) == wantNext && randomSet.prev(y) == wantPrev);
    }
    cout << "OK\n";
}
