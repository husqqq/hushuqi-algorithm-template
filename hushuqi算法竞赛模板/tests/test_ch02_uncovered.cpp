#include <bits/stdc++.h>
using namespace std;

namespace bsearch_topic {
#include "../include/full/2_1_001.hpp"
}
#undef int
namespace ternary_topic {
#include "../include/full/2_1_002.hpp"
}
#undef int
namespace enumerate_topic {
#include "../include/full/2_1_003.hpp"
}
#undef int
namespace anneal_topic {
#include "../include/full/2_1_008.hpp"
}
#undef int
namespace sort_topic {
#include "../include/full/2_2_001.hpp"
}
#undef int
namespace discrete_topic {
#include "../include/full/2_2_002.hpp"
}
#undef int
namespace inversion_topic {
#include "../include/full/2_2_003.hpp"
}
#undef int
namespace kth_topic {
#include "../include/full/2_2_004.hpp"
}
#undef int
namespace dominance_topic {
#include "../include/full/2_2_005.hpp"
}
#undef int
namespace cascade_topic {
#include "../include/full/2_2_006.hpp"
}
#undef int
namespace sweep_topic {
#include "../include/full/2_2_007.hpp"
}
#undef int
namespace monotone_topic {
#include "../include/full/2_2_008.hpp"
}
#undef int
namespace lis_topic {
#include "../include/full/2_2_011.hpp"
}
#undef int
namespace josephus_topic {
#include "../include/full/2_2_013.hpp"
}
#undef int
namespace rollback_mo_topic {
#include "../include/full/2_3_003.hpp"
}
#undef int
namespace prefix_topic {
#include "../include/full/2_4_001.hpp"
}
#undef int
namespace diff_topic {
#include "../include/full/2_4_002.hpp"
}
#undef int
namespace gcd_topic {
#include "../include/full/2_4_005.hpp"
}
#undef int

signed main()
{
    using ll = long long;
    assert(bsearch_topic::firstTrue(-10, 10, [](ll x) { return x * x >= 25 && x >= 0; }) == 5);
    assert(bsearch_topic::lastTrue(-10, 10, [](ll x) { return x <= 3; }) == 3);
    assert(abs(bsearch_topic::firstReal(0, 2, [](long double x) { return x * x >= 2; }) - sqrtl(2)) < 1E-12L);
    assert(ternary_topic::ternaryInt(-10, 10, [](ll x) { return -(x - 3) * (x - 3); }) == 3);
    assert(abs(ternary_topic::ternary(-5, 5, [](long double x) { return -(x - 1.25L) * (x - 1.25L); }) - 1.25L) < 1E-8L);

    int permutations = 0;
    enumerate_topic::eachPerm(vector<int>{1, 1, 2}, [&](const auto &) { ++permutations; });
    assert(permutations == 3);
    int subsets = 0, triples = 0;
    enumerate_topic::eachSubset(5, [&](auto) { ++subsets; });
    enumerate_topic::eachKSubset(6, 3, [&](auto) { ++triples; });
    assert(subsets == 32 && triples == 20);
    for (unsigned long long x = 0; x < 1000; ++x)
        assert(enumerate_topic::invGray(enumerate_topic::gray(x)) == x);

    mt19937_64 rng(1);
    auto [state, score] = anneal_topic::anneal<ll>(
        7, [](ll x) { return (long double)abs(x); },
        [](ll x, auto &, long double) { return max(0LL, x - 1); }, rng, 1, .01L, .5L, 2);
    assert(state == 0 && score == 0);

    vector<ll> a{5, -1, 5, 0, numeric_limits<ll>::min(), numeric_limits<ll>::max()};
    auto want = a;
    ranges::sort(want);
    auto x = a, y = a;
    sort_topic::mergeSort(x);
    sort_topic::radixSort(y);
    assert(x == want && y == want);
    discrete_topic::Discrete<ll> dis({9, -2, 9, 4});
    assert(dis.id(4) == 1 && dis.lower(5) == 2 && dis.value(0) == -2);
    assert(inversion_topic::invMerge(vector<int>{3, 1, 2, 1}) == 4);
    assert(inversion_topic::invBit(vector<int>{3, 1, 2, 1}) == 4);
    kth_topic::KthOffline kth;
    assert((kth.solve({5, 1, 4, 1, 3}, {{0, 5, 3}, {1, 4, 2}, {2, 5, 1}}) == vector<ll>{3, 1, 1}));

    using dominance_topic::Point3;
    assert((dominance_topic::dom3(vector<Point3>{{1, 1, 1, 0}, {2, 1, 2, 1}, {1, 1, 1, 2}, {3, 3, 3, 3}})
            == vector<ll>{2, 3, 2, 4}));
    cascade_topic::FracCascade<int> fc({{1, 4, 9}, {0, 4, 7}, {2, 3, 8}});
    assert((fc.lowerBound(4) == vector<ll>{1, 1, 2}));
    using Rect = sweep_topic::Rectangle<ll>;
    sweep_topic::RectUnion<ll> ru;
    vector<Rect> rects{{0, 0, 2, 2}, {1, 0, 3, 1}};
    assert(ru.area(rects) == 5 && ru.perimeter(rects) == 10);

    assert((monotone_topic::prevLess(vector<int>{3, 1, 2, 2, 0}) == vector<ll>{-1, -1, 1, 1, -1}));
    assert((monotone_topic::windowMin(vector<int>{3, 1, 4, 1, 5}, 3) == vector<int>{1, 1, 1}));
    assert(monotone_topic::maxRect(vector<ll>{2, 1, 5, 6, 2, 3}) == 10);
    assert(monotone_topic::maxRect(vector<vector<ll>>{{1, 0, 1}, {1, 1, 1}}) == 3);
    assert(lis_topic::lis2({{1, 1}, {1, 2}, {2, 2}, {3, 4}}) == 3);
    assert(lis_topic::lcis(vector<int>{3, 4, 9, 1}, vector<int>{5, 3, 8, 9, 10, 2, 1}) == 2);
    assert(josephus_topic::josephus(7, 3) == 3);

    vector<int> values{2, 7, 1, 8, 2, 8};
    vector<int> stateValues;
    auto answers = rollback_mo_topic::rollbackMo<ll>(
        values.size(), {{0, 3, 0}, {1, 5, 1}, {2, 6, 2}},
        [&](ll i) { stateValues.push_back(values[i]); },
        [&]() { return stateValues.size(); },
        [&](size_t s) { stateValues.resize(s); },
        [&]() { return accumulate(stateValues.begin(), stateValues.end(), 0LL); });
    assert((answers == vector<ll>{10, 18, 19}));

    assert((prefix_topic::prefix(vector<int>{2, -1, 4}) == vector<int>{0, 2, 1, 5}));
    prefix_topic::Prefix2<int> pre2({{1, 2, 3}, {4, 5, 6}});
    assert(pre2.sum(0, 1, 2, 3) == 16);
    diff_topic::Diff<int> d(5);
    d.add(1, 4, 3);
    d.add(2, 5, -1);
    assert((d.build() == vector<int>{0, 3, 2, 2, -1}));
    diff_topic::Diff2<int> d2(2, 3);
    d2.add(0, 1, 2, 3, 4);
    assert((d2.build() == vector<vector<int>>{{0, 4, 4}, {0, 4, 4}}));
    assert((diff_topic::kthDiff(vector<int>{1, 4, 9, 16}, 2) == vector<int>{2, 2}));
    assert(diff_topic::maxSubarray(vector<int>{-2, 3, -1, 4, -5}) == 6);
    assert(diff_topic::maxSubmatrix(vector<vector<int>>{{-1, 2}, {3, 4}}) == 8);
    assert(diff_topic::rangeXor(3, 9) == (3ULL ^ 4 ^ 5 ^ 6 ^ 7 ^ 8 ^ 9));
    gcd_topic::GcdSegs<int> gs({12, 18, 6, 15});
    assert(gs.query(0, 2) == 6 && gs.query(2, 3) == 3 && !gs.endingAt(3).empty());
    cout << "OK\n";
}
