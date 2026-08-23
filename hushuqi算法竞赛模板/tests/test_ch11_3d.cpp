#include "../include/full/11_6_002.hpp"
#include "../include/full/11_6_003.hpp"
#include "../include/full/11_6_004.hpp"
#include "../include/full/11_6_005.hpp"
#include "../include/full/11_6_006.hpp"
#include "../include/full/11_6_007.hpp"
#include "../include/full/11_6_008.hpp"
#include "../include/full/11_6_009.hpp"

bool eq3(P3 a, P3 b)
{
    return sgn(abs(a - b)) == 0;
}

signed main()
{
    assert(parallel(L3{{}, {1, 0, 0}}, L3{{0, 1, 0}, {2, 0, 0}}));
    assert(perp(L3{{}, {1, 0, 0}}, L3{{}, {0, 1, 0}}));
    auto p = linePlane({{0, 0, 1}, {0, 0, -1}}, {{}, {0, 0, 1}});
    assert(p && eq3(*p, {}));
    auto l = planePlane({{}, {1, 0, 0}}, {{}, {0, 1, 0}});
    assert(l && eq(dis(P3{0, 0, 2}, *l), 0));
    assert(eq(dis(L3{{}, {1, 0, 0}}, L3{{0, 0, 1}, {0, 1, 0}}), 1));

    P3 a{0, 0, 0};
    P3 b{2, 0, 0};
    P3 c{0, 2, 0};
    assert(segTri({0.5L, 0.5L, -1}, {0.5L, 0.5L, 1}, a, b, c));
    assert(segTri({-1, 1, 0}, {1, 1, 0}, a, b, c));
    assert(segTri({0.5L, 0.5L, 0}, {0.5L, 0.5L, 0}, a, b, c));
    assert(!segTri({3, 3, 0}, {4, 4, 0}, a, b, c));

    vector<P3> tetra{{1, 1, 1}, {-1, -1, 1}, {-1, 1, -1}, {1, -1, -1}};
    assert(hull3(tetra).size() == 4);
    vector<P3> cube;
    for (int x : {-1, 1})
    {
        for (int y : {-1, 1})
        {
            for (int z : {-1, 1})
            {
                cube.push_back({(Real)x, (Real)y, (Real)z});
            }
        }
    }
    cube.push_back({0, 0, 1});
    assert(hull3(cube).size() == 12);
    assert(hull3({{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}}).empty());

    vector<P3> axes{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
                    {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    Sphere s = minSphere(axes);
    assert(eq3(s.o, {}) && eq(s.r, 1));
    assert(eq(tetraVolume({0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}), 1.L / 6));
    const Real pi = acosl(-1);
    assert(eq(sphereVolume(7, 0), 1));
    assert(eq(sphereVolume(1, 2), pi));
    assert(eq(sphereVolume(1, 3), 4 * pi / 3));
    cout << "OK\n";
}
