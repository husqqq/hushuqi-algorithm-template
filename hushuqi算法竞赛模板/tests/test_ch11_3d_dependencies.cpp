#include <bits/stdc++.h>
using namespace std;

namespace geometry3d_topic {
#include "../include/full/11_6_001.hpp"
#include "../include/full/11_6_010.hpp"
#include "../include/full/11_6_011.hpp"
}
#undef int

signed main()
{
    using namespace geometry3d_topic;
    auto eq3 = [](P3 a, P3 b)
    {
        return abs(a.x - b.x) < 1E-12L && abs(a.y - b.y) < 1E-12L && abs(a.z - b.z) < 1E-12L;
    };
    P3 ex{1, 0, 0}, ey{0, 1, 0}, ez{0, 0, 1};
    assert(dot(ex, ey) == 0 && eq3(cross(ex, ey), ez) && abs(abs(P3{1, 2, 2}) - 3) < 1E-12L);
    assert(eq3(projection(L3{{0, 0, 0}, ex}, P3{2, 3, 4}), P3{2, 0, 0}));
    auto close = segSeg3({0, 0, 0}, {2, 0, 0}, {1, -1, 0}, {1, 1, 0});
    assert(abs(close.distance) < 1E-12L);
    auto point = planesMeet3({{1, 0, 0}, ex}, {{0, 2, 0}, ey}, {{0, 0, 3}, ez});
    assert(point && eq3(*point, P3{1, 2, 3}));
    AABB3 box{{0, 0, 0}, {2, 2, 2}};
    auto hit = rayAABB3({{-1, 1, 1}, ex}, box);
    assert(hit.state == RayState3::Hit && abs(hit.hit.enter - 1) < 1E-12L);
    assert(segmentAABB3({-1, 1, 1}, {3, 1, 1}, box).has_value());
    assert(sphereAABB3({{3, 1, 1}, 1}, box));
    cout << "OK\n";
}
