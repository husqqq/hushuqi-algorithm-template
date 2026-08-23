#include "../include/full/11_6_015.hpp"

bool eqPoint3(P3 a, P3 b)
{
    // a、b 是两个空间点；返回二者是否在统一浮点误差内重合。
    return sgn(abs(a - b)) == 0;
}

signed main()
{
    const Real pi = acosl(-1);
    assert(eqPoint3(reflection(Plane{{}, {0, 0, 1}}, {1, 2, 3}), {1, 2, -3}));
    assert(eq(signedDih3({1, 0, 0}, {0, 1, 0}, {0, 0, 1}), pi / 2));
    assert(eq(signedDih3({1, 0, 0}, {0, 0, 1}, {0, 1, 0}), -pi / 2));
    assert(eq(dihedral3({1, 0, 0}, {0, 1, 0}, {0, 0, 1}), pi / 2));
    P3 geographic = fromLatLon3(pi / 6, pi / 3, 2);
    auto [lat, lon] = toLatLon3(geographic);
    assert(eq(abs(geographic), 2) && eq(lat, pi / 6) && eq(lon, pi / 3));
    auto [poleLatitude, poleLongitude] = toLatLon3({0, 0, 2});
    assert(eq(poleLatitude, pi / 2) && eq(poleLongitude, 0));
    assert(eq(solidAngle3({1, 0, 0}, {0, 1, 0}, {0, 0, 1}), pi / 2));

    Sphere unitSphere{{}, 1};
    vector<P3> octant{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    assert(eq(spherePoly3(octant, unitSphere), pi / 2));
    reverse(octant.begin(), octant.end());
    assert(eq(spherePoly3(octant, unitSphere), -pi / 2));

    vector<P3> noFanDiagonal{{1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}};
    assert(eq(spherePoly3(noFanDiagonal, unitSphere), pi));
    reverse(noFanDiagonal.begin(), noFanDiagonal.end());
    assert(eq(spherePoly3(noFanDiagonal, unitSphere), -pi));

    Sphere shifted{{3, 4, 5}, 2};
    vector<P3> scaled{shifted.o + P3{1, 0, 0} * 2, shifted.o + P3{0, 1, 0} * 2, shifted.o + P3{0, 0, 1} * 2};
    assert(eq(spherePoly3(scaled, shifted), 2 * pi));

    P3 nearAntipode = unit(P3{-1, 1E-9L, 0});
    vector<P3> narrow{{1, 0, 0}, nearAntipode, {0, 0, 1}};
    Real area = spherePoly3(narrow, unitSphere);
    reverse(narrow.begin(), narrow.end());
    Real reverseArea = spherePoly3(narrow, unitSphere);
    assert(isfinite(area) && isfinite(reverseArea) && eq(area, -reverseArea));
    cout << "OK\n";
}
