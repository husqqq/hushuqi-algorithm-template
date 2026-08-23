#include <bits/stdc++.h>
using namespace std;

namespace geometry2d_topic {
#include "../include/full/11_1_001.hpp"
#include "../include/full/11_1_002.hpp"
#include "../include/full/11_1_003.hpp"
#include "../include/full/11_1_005.hpp"
#include "../include/full/11_1_006.hpp"
#include "../include/full/11_1_007.hpp"
#include "../include/full/11_1_008.hpp"
#include "../include/full/11_2_001.hpp"
#include "../include/full/11_2_004.hpp"
#include "../include/full/11_3_003.hpp"
#include "../include/full/11_4_001.hpp"
}
#undef int
signed main()
{
    using namespace geometry2d_topic;
    Point<long long> x{3, 4}, y{-4, 3};
    assert(sgn(-2LL) == -1 && eq(1.0L, 1.0L + 1E-13L));
    assert(dot(x, y) == 0 && cross(x, y) == 25 && norm(x) == 25);
    assert(abs(abs(x) - 5) < 1E-12L && rot(x) == Point<long long>(-4, 3));
    Line<long long> horizontal{{0, 0}, {4, 0}}, vertical{{2, -1}, {2, 3}};
    assert(perp(horizontal, vertical) && !parallel(horizontal, vertical));
    assert(side(horizontal, Point<long long>{0, 2}) == 1);
    assert(eq(lineMeet(horizontal, vertical), P{2, 0}));
    assert(eq(projection(horizontal, Point<long long>{3, 2}), P{3, 0}));
    assert(eq(reflection(horizontal, Point<long long>{3, 2}), P{3, -2}));
    assert(abs(distancePL(Point<long long>{3, 2}, horizontal) - 2) < 1E-12L);
    assert(abs(distancePS(Point<long long>{6, 0}, horizontal) - 2) < 1E-12L);
    vector<Point<long long>> square{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    assert(area2(square) == 32 && abs(perimeter(square) - 16) < 1E-12L);
    assert(eq(cen(square), P{2, 2}) && inConvex(square, Point<long long>{4, 2}));
    assert(abs(area(cutPoly({P{0, 0}, P{4, 0}, P{4, 4}, P{0, 4}}, {{2, 0}, {0, 1}})) - 8) < 1E-12L);
    assert(in(C{{0, 0}, 2}, P{1, 1}) && circleRel(C{{0, 0}, 1}, C{{2, 0}, 1}) == 1);

    cout << "OK\n";
}
