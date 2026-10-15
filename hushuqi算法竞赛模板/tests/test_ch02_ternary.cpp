#include <bits/stdc++.h>
using namespace std;
#include "../include/search_enum.hpp"
#undef int
namespace topic
{
#include "../include/full/2_1_002.hpp"
}
#undef int

template <class Search> void checkInt(Search search)
{
    // search 接收闭区间端点与求值回调；检查端点、峰顶平台和固定种子小规模对拍。
    auto lo = numeric_limits<long long>::min();
    auto hi = numeric_limits<long long>::max();
    auto inc = [](long long x) { return x; };
    auto dec = [](long long x) { return -(__int128)x; };
    assert(search(lo, lo, inc) == lo);
    assert(search(hi, hi, inc) == hi);
    assert(search(lo, hi, inc) == hi);
    assert(search(lo, hi, dec) == lo);
    for (auto peak : {lo, lo + 2, -1LL, 0LL, hi - 2, hi})
    {
        auto f = [=](long long x) -> __int128
        {
            auto d = (__int128)x - peak;
            return d < 0 ? d : -d;
        };
        assert(search(lo, hi, f) == peak);
    }
    mt19937_64 rng(20261016);
    for (int t = 0; t < 4000; t++)
    {
        long long l = (long long)(rng() % 101) - 50;
        long long r = l + rng() % 101;
        long long a = l + rng() % (r - l + 1);
        long long b = a + rng() % (r - a + 1);
        long long up = 1 + rng() % 9, down = 1 + rng() % 9;
        auto f = [=](long long x)
        {
            return x < a ? (x - a) * up : x > b ? (b - x) * down : 0;
        };
        auto p = search(l, r, f);
        auto best = f(l);
        for (auto x = l; x < r;)
        {
            ++x;
            best = max(best, f(x));
        }
        assert(l <= p && p <= r && f(p) == best);
    }
}

template <class Search> void checkReal(Search search, bool golden)
{
    // search 接收实数闭区间、求值回调和轮数；golden 指明是否有两次初始化求值。
    auto f = [](long double x) { return -abs(x - 1.25L); };
    assert(abs(search(-5, 5, f, 120) - 1.25L) < 1E-15L);
    assert(search(3, 3, f, 120) == 3);
    assert(search(-5, 5, f, 0) == 0);
    auto platform = search(-20, 20, [](long double x)
    {
        return x < 7 ? x - 7 : x > 9 ? 9 - x : 0;
    }, 120);
    assert(7 - 1E-15L <= platform && platform <= 9 + 1E-15L);
    auto hi = numeric_limits<long double>::max();
    for (auto [l, r, target] : vector<array<long double, 3>>{
             {hi / 2, hi, .75L}, {-hi, hi, .25L}, {-hi, -hi / 2, -.75L}})
    {
        auto ans = search(l, r, [=](long double x) { return -abs(x / hi - target); }, 160);
        assert(isfinite(ans) && l <= ans && ans <= r);
        assert(abs(ans / hi - target) < 1E-15L);
    }
    for (int it : {0, 1, 120})
    {
        int calls = 0;
        search(-5, 5, [&](long double x) { ++calls; return f(x); }, it);
        assert(calls == (golden ? it + 2 : 2 * it));
    }
}

signed main()
{
    checkInt([](auto l, auto r, auto f) { return ternaryInt(l, r, f); });
    checkInt([](auto l, auto r, auto f) { return topic::ternaryInt(l, r, f); });
    checkReal([](auto l, auto r, auto f, auto it) { return ternaryReal(l, r, f, it); }, false);
    checkReal([](auto l, auto r, auto f, auto it) { return topic::ternaryReal(l, r, f, it); }, false);
    checkReal([](auto l, auto r, auto f, auto it) { return goldenMax(l, r, f, it); }, true);
    checkReal([](auto l, auto r, auto f, auto it) { return topic::goldenMax(l, r, f, it); }, true);
    cout << "OK\n";
}
