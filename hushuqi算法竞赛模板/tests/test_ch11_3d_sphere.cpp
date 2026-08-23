#include "../include/full/11_6_013.hpp"

bool eqPoint3(P3 a, P3 b)
{
    // a、b 是两个空间点；返回二者是否在统一浮点误差内重合。
    return sgn(abs(a - b)) == 0;
}

signed main()
{
    const Real pi = acosl(-1);
    Sphere unitSphere{{}, 1};
    auto secant = lineSphere3({{}, {1, 0, 0}}, unitSphere);
    assert(secant.size() == 2 && eq(secant[0].t, -1) && eq(secant[1].t, 1));
    auto tangent = lineSphere3({{0, 1, 0}, {1, 0, 0}}, unitSphere);
    assert(tangent.size() == 1 && eq(tangent[0].t, 0));
    assert(lineSphere3({{0, 2, 0}, {1, 0, 0}}, unitSphere).empty());
    auto insideRay = raySphere3({{}, {1, 0, 0}}, unitSphere);
    assert(insideRay.state == RayState3::Hit && insideRay.hits.size() == 1);
    assert(eq(insideRay.hits[0].t, 1));
    auto surfaceRay = raySphere3({{1, 0, 0}, {-1, 0, 0}}, unitSphere);
    assert(surfaceRay.state == RayState3::Hit && surfaceRay.hits.size() == 2);
    assert(eq(surfaceRay.hits[0].t, 0) && eq(surfaceRay.hits[1].t, 2));
    assert(raySphere3({{}, {}}, unitSphere).state == RayState3::Degenerate);

    auto greatCircle = planeSphere3({{}, {0, 0, 1}}, unitSphere);
    assert(greatCircle && eqPoint3(greatCircle->o, {}) && eq(greatCircle->r, 1));
    auto tangentCircle = planeSphere3({{0, 0, 1}, {0, 0, 1}}, unitSphere);
    assert(tangentCircle && eq(tangentCircle->r, 0));
    assert(!planeSphere3({{0, 0, 2}, {0, 0, 1}}, unitSphere));

    assert(spherePair3(unitSphere, {{3, 0, 0}, 1}).relation == SphereRel3::Separate);
    auto external = spherePair3(unitSphere, {{2, 0, 0}, 1});
    assert(external.relation == SphereRel3::ExtTangent && external.circle);
    assert(eqPoint3(external.circle->o, {1, 0, 0}) && eq(external.circle->r, 0));
    auto crossing = spherePair3(unitSphere, {{1, 0, 0}, 1});
    assert(crossing.relation == SphereRel3::Intersecting && crossing.circle);
    assert(eqPoint3(crossing.circle->o, {0.5L, 0, 0}));
    assert(eq(crossing.circle->r, sqrtl(3.L) / 2));
    assert(spherePair3({{}, 2}, {{1, 0, 0}, 1}).relation == SphereRel3::IntTangent);
    assert(spherePair3({{}, 2}, {{0.5L, 0, 0}, 1}).relation == SphereRel3::Contained);
    assert(spherePair3({{}, 2}, {{}, 1}).relation == SphereRel3::Contained);
    assert(spherePair3(unitSphere, unitSphere).relation == SphereRel3::Coincident);
    assert(spherePair3({{}, 0}, {{}, 0}).relation == SphereRel3::Coincident);
    auto pointOnSphere = spherePair3(unitSphere, {{1, 0, 0}, 0});
    assert(pointOnSphere.relation == SphereRel3::IntTangent);
    assert(pointOnSphere.circle && eq(pointOnSphere.circle->r, 0));
    assert(spherePair3(unitSphere, {{0.5L, 0, 0}, 0}).relation == SphereRel3::Contained);

    Circle3 circle{{}, {0, 0, 1}, 1};
    auto inPlane = lineCircle3({{}, {1, 0, 0}}, circle);
    assert(inPlane.size() == 2 && eq(inPlane[0].t, -1) && eq(inPlane[1].t, 1));
    auto throughPlane = lineCircle3({{1, 0, -1}, {0, 0, 1}}, circle);
    assert(throughPlane.size() == 1 && eq(throughPlane[0].t, 1));
    assert(lineCircle3({{0, 0, -1}, {0, 0, 1}}, circle).empty());
    auto pointSphereLine = lineSphere3({{}, {1, 0, 0}}, {{}, 0});
    assert(pointSphereLine.size() == 1 && eq(pointSphereLine[0].t, 0));
    const Real tiny = 1E-20L;
    auto tinySphereLine = lineSphere3({{}, {1, 0, 0}}, {{}, tiny});
    assert(tinySphereLine.size() == 2);
    assert(eq(tinySphereLine[0].t / tiny, -1) && eq(tinySphereLine[1].t / tiny, 1));
    auto scaledRay = raySphere3({{-2, 0, 0}, {1E-30L, 0, 0}}, unitSphere);
    assert(scaledRay.state == RayState3::Hit && scaledRay.hits.size() == 2);
    assert(eq(scaledRay.hits[0].t / 1E30L, 1) && eq(scaledRay.hits[1].t / 1E30L, 3));
    auto scaledCircleLine = lineCircle3({{}, {1E-30L, 0, 0}}, circle);
    assert(scaledCircleLine.size() == 2);
    assert(eq(scaledCircleLine[0].t / 1E30L, -1) && eq(scaledCircleLine[1].t / 1E30L, 1));

    assert(eq(sphereLens3(unitSphere, unitSphere), 4 * pi / 3));
    assert(eq(sphereLens3(unitSphere, {{3, 0, 0}, 1}), 0));
    assert(eq(sphereLens3({{}, 2}, {{0.5L, 0, 0}, 1}), 4 * pi / 3));
    Real x = sphereLens3(unitSphere, {{1, 0, 0}, 1});
    Real y = sphereLens3({{1, 0, 0}, 1}, unitSphere);
    assert(eq(x, y));
    assert(eq(sphereArea(1), 4 * pi));
    assert(eq(capArea(1, 1), 2 * pi));
    assert(eq(capVolume(1, 1), 2 * pi / 3));
    assert(eq(capArea(1, 2), 4 * pi));
    assert(eq(capVolume(1, 2), 4 * pi / 3));
    assert(eq(zoneArea(1, -1, 1), 4 * pi));
    assert(eq(zoneVol3(1, -1, 1), 4 * pi / 3));
    assert(eq(capArea(1, -eps / 2), 0));
    assert(eq(capVolume(1, -eps / 2), 0));
    assert(eq(capArea(1, 2 + eps / 2), 4 * pi));
    assert(eq(zoneArea(1, 0.5L + eps / 2, 0.5L), 0));
    assert(eq(zoneVol3(1, 0.5L + eps / 2, 0.5L), 0));
    cout << "OK\n";
}
