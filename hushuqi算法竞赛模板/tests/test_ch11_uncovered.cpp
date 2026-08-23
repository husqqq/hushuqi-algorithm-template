#include <bits/stdc++.h>
using namespace std;
namespace geo_topic {
#include "../include/full/11_2_005.hpp"
#include "../include/full/11_2_003.hpp"
#include "../include/full/11_1_012.hpp"
}
#undef int

signed main()
{
    using geo_topic::P;
    auto c = geo_topic::triCenters(P{0, 0}, P{4, 0}, P{0, 3});
    assert(geo_topic::abs(c[0] - P{1, 1}) < 1E-12L);
    assert(geo_topic::abs(c[1] - P{2, 1.5L}) < 1E-12L);

    vector<P> concave{{0, 0}, {3, 0}, {3, 3}, {2, 1}, {0, 3}};
    assert(geo_topic::segInPoly(concave, P{.5L, .5L}, P{2.5L, .5L}));
    assert(!geo_topic::segInPoly(concave, P{.5L, 2.5L}, P{2.5L, 2.5L}));

    vector<P> square{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    auto cut = geo_topic::cutPoly(square, {{2, 0}, {0, 1}});
    assert(abs(geo_topic::area(cut) - 8) < 1E-12L);
    auto kernel = geo_topic::polyKernel(square);
    assert(abs(geo_topic::area(kernel) - 16) < 1E-12L);
    cout << "OK\n";
}
