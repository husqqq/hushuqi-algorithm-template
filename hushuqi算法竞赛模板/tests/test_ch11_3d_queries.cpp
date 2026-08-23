#include "../include/full/11_6_012.hpp"

bool eqPoint3(P3 a, P3 b)
{
    // a、b 是两个空间点；返回二者是否在统一浮点误差内重合。
    return sgn(abs(a - b)) == 0;
}

signed main()
{
    const Real pi = acosl(-1);
    assert(collinear(P3{}, P3{1, 1, 1}, P3{2, 2, 2}));
    assert(collinear(P3{}, P3{}, P3{1, 2, 3}));
    assert(!collinear(P3{}, P3{1, 0, 0}, P3{0, 1, 0}));
    assert(coplanar(P3{}, P3{1, 0, 0}, P3{0, 1, 0}, P3{2, 3, 0}));
    assert(!coplanar(P3{}, P3{1, 0, 0}, P3{0, 1, 0}, P3{0, 0, 1}));
    assert(onLine3({2, 0, 0}, {{}, {1, 0, 0}}));
    assert(onSegment3({1, 0, 0}, {}, {2, 0, 0}));
    assert(onSegment3({1, 2, 3}, {1, 2, 3}, {1, 2, 3}));
    assert(!onSegment3({}, {1, 2, 3}, {1, 2, 3}));
    assert(onPlane3({2, 3, 0}, {{}, {0, 0, 1}}));
    assert(perp(Plane{{}, {1, 0, 0}}, Plane{{}, {0, 1, 0}}));
    const long long exactLimit = 1'000'000'000'000LL;
    assert(collinearEx(IP3{-exactLimit, -exactLimit, -exactLimit}, IP3{}, IP3{exactLimit, exactLimit, exactLimit}));
    assert(!collinearEx(IP3{}, IP3{exactLimit, 0, 0}, IP3{0, exactLimit, 1}));
    assert(coplanarEx(IP3{}, IP3{exactLimit, 0, 0}, IP3{0, exactLimit, 0}, IP3{exactLimit, exactLimit, 0}));
    assert(!coplanarEx(IP3{}, IP3{exactLimit, 0, 0}, IP3{0, exactLimit, 0}, IP3{0, 0, exactLimit}));
    assert(mixedSign(IP3{}, IP3{exactLimit, 0, 0}, IP3{0, exactLimit, 0}, IP3{0, 0, exactLimit}) > 0);
    assert(mixedSign(IP3{}, IP3{exactLimit, 0, 0}, IP3{0, 0, exactLimit}, IP3{0, exactLimit, 0}) < 0);

    assert(eqPoint3(projection(L3{{}, {2, 0, 0}}, {1, 2, 3}), {1, 0, 0}));
    assert(eqPoint3(projection(Plane{{0, 0, 1}, {0, 0, 2}}, {1, 2, 3}), {1, 2, 1}));
    auto pointSegment = pointSeg3({1, 2, 0}, {}, {2, 0, 0});
    assert(eqPoint3(pointSegment.point, {1, 0, 0}));
    assert(eq(pointSegment.t, 0.5L) && eq(pointSegment.distance, 2));

    auto skew = lineLine3({{}, {1, 0, 0}}, {{0, 0, 1}, {0, 1, 0}});
    assert(skew.relation == LineRel3::Skew && skew.closest);
    assert(eq(skew.distance, 1) && eqPoint3(skew.closest->onA, {}));
    assert(eqPoint3(skew.closest->onB, {0, 0, 1}));
    auto intersecting = lineLine3({{}, {1, 0, 0}}, {{1, -1, 0}, {0, 1, 0}});
    assert(intersecting.relation == LineRel3::Intersecting);
    assert(intersecting.closest && eq(intersecting.distance, 0));
    auto parallelLines = lineLine3({{}, {1, 0, 0}}, {{0, 1, 0}, {2, 0, 0}});
    assert(parallelLines.relation == LineRel3::Parallel);
    assert(!parallelLines.closest && eq(parallelLines.distance, 1));
    auto coincident = lineLine3({{}, {1, 0, 0}}, {{2, 0, 0}, {-3, 0, 0}});
    assert(coincident.relation == LineRel3::Coincident);
    assert(!coincident.closest && eq(coincident.distance, 0));
    auto degenerateLine = lineLine3({{1, 2, 0}, {}}, {{}, {1, 0, 0}});
    assert(degenerateLine.relation == LineRel3::Degenerate && degenerateLine.closest);
    assert(eq(degenerateLine.distance, 2) && eqPoint3(degenerateLine.closest->onB, {1, 0, 0}));
    const Real far = 1E13L;
    auto shallowLines = lineLine3({{}, {far, 0, 0}}, {{0, 1, 0}, {far, -2, 0}});
    assert(shallowLines.relation == LineRel3::Intersecting && shallowLines.closest);
    assert(eq(shallowLines.distance, 0) && eq(shallowLines.closest->tA, 0.5L) && eq(shallowLines.closest->tB, 0.5L));

    auto parallelSegments = segSeg3({}, {2, 0, 0}, {1, 1, 0}, {3, 1, 0});
    assert(eq(parallelSegments.distance, 1));
    assert(eqPoint3(parallelSegments.onA, {1, 0, 0}));
    auto almostParallel = segSeg3({}, {1, 0, 0}, {2, 1E-10L, 0}, {3, 2E-10L, 0});
    assert(eqPoint3(almostParallel.onA, {1, 0, 0}));
    assert(eqPoint3(almostParallel.onB, {2, 1E-10L, 0}));
    auto tinyCross = segSeg3({}, {1E-6L, 0, 0}, {5E-7L, -1E-6L, 0}, {5E-7L, 1E-6L, 0});
    assert(eq(tinyCross.distance, 0));
    auto shallowCross = segSeg3({}, {far, 0, 0}, {0, 1, 0}, {far, -1, 0});
    assert(eq(shallowCross.distance, 0) && eq(shallowCross.tA, 0.5L) && eq(shallowCross.tB, 0.5L));
    auto pointPair = segSeg3({0, 2, 0}, {0, 2, 0}, {}, {2, 0, 0});
    assert(eqPoint3(pointPair.onB, {}) && eq(pointPair.distance, 2));

    mt19937_64 rng(1);
    for (int rep = 0; rep < 500; rep++)
    {
        auto randomPoint = [&]()
        {
            return P3{(Real)((int)(rng() % 2001) - 1000),
                      (Real)((int)(rng() % 2001) - 1000),
                      (Real)((int)(rng() % 2001) - 1000)};
        };
        P3 ra = randomPoint();
        P3 rb = randomPoint();
        P3 rc = randomPoint();
        P3 rd = randomPoint();
        if (sgn(abs(rb - ra)) == 0 || sgn(abs(rd - rc)) == 0)
        {
            rep--;
            continue;
        }
        auto nearest = segSeg3(ra, rb, rc, rd);
        assert(nearest.tA >= -eps && nearest.tA <= 1 + eps && nearest.tB >= -eps && nearest.tB <= 1 + eps);
        assert(eq(nearest.distance, abs(nearest.onA - nearest.onB)));
        P3 va = rb - ra;
        P3 vb = rd - rc;
        P3 delta = nearest.onA - nearest.onB;
        if (nearest.tA > eps && nearest.tA < 1 - eps)
        {
            assert(eq(dot(delta, va), 0));
        }
        if (nearest.tB > eps && nearest.tB < 1 - eps)
        {
            assert(eq(dot(delta, vb), 0));
        }
    }

    assert(eq(angle(L3{{}, {1, 0, 0}}, L3{{}, {0, 1, 0}}), pi / 2));
    assert(eq(angle(L3{{}, {1, 0, 0}}, Plane{{}, {0, 0, 1}}), 0));
    assert(eq(angle(Plane{{}, {1, 0, 0}}, Plane{{}, {0, 1, 0}}), pi / 2));
    auto threePlanes = planesMeet3({{1, 0, 0}, {1, 0, 0}}, {{0, 2, 0}, {0, 1, 0}}, {{0, 0, 3}, {0, 0, 1}});
    assert(threePlanes && eqPoint3(*threePlanes, {1, 2, 3}));
    auto shallowPlanes = planesMeet3({{}, {1, 0, 0}}, {{0, 1, 0}, {1, 1E-13L, 0}}, {{0, 0, 2}, {0, 0, 1}});
    assert(shallowPlanes && eqPoint3(*shallowPlanes, {0, 1, 2}));

    AABB3 box{{0, 0, 0}, {1, 1, 1}};
    assert(in(box, {}) && in(box, {1, 1, 1}) && !in(box, {1 + eps * 2, 0.5L, 0.5L}));
    auto rayBox = rayAABB3({{-1, 0.5L, 0.5L}, {1, 0, 0}}, box);
    assert(rayBox.state == RayState3::Hit);
    assert(eq(rayBox.hit.enter, 1) && eq(rayBox.hit.exit, 2));
    auto grazingBox = rayAABB3({{-1, 1, 0.5L}, {1, 0, 0}}, box);
    assert(grazingBox.state == RayState3::Hit && eq(grazingBox.hit.enter, 1) && eq(grazingBox.hit.exit, 2));
    auto insideBox = rayAABB3({{0.5L, 0.5L, 0.5L}, {1, 0, 0}}, box);
    assert(insideBox.state == RayState3::Hit);
    assert(eq(insideBox.hit.enter, 0) && eq(insideBox.hit.exit, 0.5L));
    auto tinyDirectionBox = rayAABB3({{0.5L, -1, 0.5L}, {0, 1E-30L, 0}}, box);
    assert(tinyDirectionBox.state == RayState3::Hit);
    assert(eq(tinyDirectionBox.hit.enter / 1E30L, 1) && eq(tinyDirectionBox.hit.exit / 1E30L, 2));
    assert(rayAABB3({{-1, 2, 0.5L}, {1, 0, 0}}, box).state == RayState3::None);
    auto cornerBox = rayAABB3({{-1, -1, -1}, {1, 1, 1}}, box);
    assert(cornerBox.state == RayState3::Hit);
    assert(eq(cornerBox.hit.enter, 1) && eq(cornerBox.hit.exit, 2));
    assert(rayAABB3({{}, {}}, box).state == RayState3::Degenerate);
    auto constantBox = aabbRange3({0.5L, 0.5L, 0.5L}, {}, box, 0, 1);
    assert(constantBox && eq(constantBox->enter, 0) && eq(constantBox->exit, 1));
    auto pointSegmentBox = segmentAABB3({0.5L, 0.5L, 0.5L}, {0.5L, 0.5L, 0.5L}, box);
    assert(pointSegmentBox && eq(pointSegmentBox->enter, 0) && eq(pointSegmentBox->exit, 1));
    AABB3 flatBox{{0, 0, 0}, {0, 1, 1}};
    auto flatHit = rayAABB3({{-1, 0.5L, 0.5L}, {1, 0, 0}}, flatBox);
    assert(flatHit.state == RayState3::Hit);
    assert(eq(flatHit.hit.enter, 1) && eq(flatHit.hit.exit, 1));
    auto segmentBox = segmentAABB3({-1, 0.5L, 0.5L}, {2, 0.5L, 0.5L}, box);
    assert(segmentBox && eq(segmentBox->enter, 1.L / 3) && eq(segmentBox->exit, 2.L / 3));
    assert(segmentAABB3({0.5L, 0.5L, 0.5L}, {0.5L, 0.5L, 0.5L}, box));
    assert(sphereAABB3({{2, 0.5L, 0.5L}, 1}, box));
    assert(!sphereAABB3({{3, 0.5L, 0.5L}, 1}, box));

    auto rayPlane = rayPlane3({{0, 0, 1}, {0, 0, -1}}, {{}, {0, 0, 1}});
    assert(rayPlane.state == RayState3::Hit && eq(rayPlane.hit.t, 1));
    auto tinyRayPlane = rayPlane3({{0, 0, 1}, {0, 0, -1E-30L}}, {{}, {0, 0, 1}});
    assert(tinyRayPlane.state == RayState3::Hit && eq(tinyRayPlane.hit.t / 1E30L, 1));
    assert(rayPlane3({{}, {1, 0, 0}}, {{}, {0, 0, 1}}).state == RayState3::Hit);
    assert(rayPlane3({{}, {}}, {{}, {0, 0, 1}}).state == RayState3::Degenerate);

    P3 a{0, 0, 0};
    P3 b{2, 0, 0};
    P3 c{0, 2, 0};
    assert(inTri3({0.5L, 0.5L, 0}, a, b, c));
    assert(!inTri3({0.5L, 0.5L, 1}, a, b, c));
    auto above = pointTri3({0.5L, 0.5L, 3}, a, b, c);
    assert(eqPoint3(above.point, {0.5L, 0.5L, 0}) && eq(above.distance, 3));
    auto outside = pointTri3({3, 0, 0}, a, b, c);
    assert(eqPoint3(outside.point, b) && eq(outside.distance, 1));

    Plane z0{{}, {0, 0, 1}};
    auto emptyCut = triPlane3(a, b, c, {{0, 0, 1}, {0, 0, 1}});
    assert(emptyCut.type == TriPlaneType::Empty);
    auto pointCut = triPlane3({}, {1, 0, 1}, {0, 1, 1}, z0);
    assert(pointCut.type == TriPlaneType::Point && pointCut.points.size() == 1);
    auto edgeCut = triPlane3({}, {1, 0, 0}, {0, 1, 1}, z0);
    assert(edgeCut.type == TriPlaneType::Segment && edgeCut.points.size() == 2);
    auto crossingCut = triPlane3({0, 0, -1}, {1, 0, 1}, {0, 1, 1}, z0);
    assert(crossingCut.type == TriPlaneType::Segment && crossingCut.points.size() == 2);
    auto wholeCut = triPlane3(a, b, c, z0);
    assert(wholeCut.type == TriPlaneType::Triangle && wholeCut.points.size() == 3);

    vector<P3> square{{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
    vector<P3> clipped = clipPolygon3(square, {{}, {1, 0, 0}});
    assert(clipped.size() == 4 && eq(polygonArea3(clipped), 2));
    auto rayTriangle = rayTriangle3({{0.5L, 0.5L, 1}, {0, 0, -1}}, a, b, c);
    assert(rayTriangle.state == RayState3::Hit && eq(rayTriangle.hit.t, 1));
    auto tinyRayTriangle = rayTriangle3({{0.5L, 0.5L, 1}, {0, 0, -1E-30L}}, a, b, c);
    assert(tinyRayTriangle.state == RayState3::Hit && eq(tinyRayTriangle.hit.t / 1E30L, 1));
    auto coplanarRay = rayTriangle3({{-1, 0.5L, 0}, {1, 0, 0}}, a, b, c);
    assert(coplanarRay.state == RayState3::Hit && eq(coplanarRay.hit.t, 1));
    auto edgeRay = rayTriangle3({{-1, 0, 0}, {1, 0, 0}}, a, b, c);
    assert(edgeRay.state == RayState3::Hit && eq(edgeRay.hit.t, 1));
    auto tinyCoplanarRay = rayTriangle3({{-1, 0.5L, 0}, {1E-30L, 0, 0}}, a, b, c);
    assert(tinyCoplanarRay.state == RayState3::Hit && eq(tinyCoplanarRay.hit.t / 1E30L, 1));
    assert(rayTriangle3({{}, {}}, a, b, c).state == RayState3::Degenerate);
    assert(rayTriangle3({{0.5L, 0.5L, 1}, {1, 0, 0}}, a, b, c).state == RayState3::None);
    cout << "OK\n";
}
