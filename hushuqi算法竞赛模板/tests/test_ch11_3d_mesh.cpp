#include "../include/full/11_6_014.hpp"

bool eqPoint3(P3 a, P3 b)
{
    // a、b 是两个空间点；返回二者是否在统一浮点误差内重合。
    return sgn(abs(a - b)) == 0;
}

signed main()
{
    Plane tilted{{1, 2, 3}, {1, 2, 3}};
    PlaneBasis3 basis = planeBasis3(tilted);
    assert(eq(abs(basis.u), 1) && eq(abs(basis.v), 1) && eq(abs(basis.n), 1));
    assert(eq(dot(basis.u, basis.v), 0));
    assert(eqPoint3(cross(basis.u, basis.v), basis.n));
    P local{2, -3};
    P3 point = planePoint3(basis, local);
    assert(eq(planeCoord3(basis, point), local));
    vector<P> localRectangle{{0, 0}, {2, 0}, {2, 3}, {0, 3}};
    vector<P3> mappedRectangle;
    for (P q : localRectangle)
    {
        mappedRectangle.push_back(planePoint3(basis, q));
    }
    assert(eq(polygonArea3(mappedRectangle), 6));

    vector<P3> planePoints;
    for (int x : {-1, 1})
    {
        for (int y : {-1, 1})
        {
            planePoints.push_back(planePoint3(basis, {(Real)x, (Real)y}));
        }
    }
    planePoints.push_back(basis.o);
    vector<P3> planeHull = coplanarHull(planePoints, tilted);
    assert(planeHull.size() == 4 && eq(polygonArea3(planeHull), 4));

    vector<P3> cube{{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}};
    vector<array<int, 3>> faces{{0, 2, 1},
                                {0, 3, 2},
                                {4, 5, 6},
                                {4, 6, 7},
                                {0, 1, 5},
                                {0, 5, 4},
                                {1, 2, 6},
                                {1, 6, 5},
                                {2, 3, 7},
                                {2, 7, 6},
                                {3, 0, 4},
                                {3, 4, 7}};
    assert(eq(meshVolume3(cube, faces), 1));
    vector<P3> tetrahedron{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    vector<array<int, 3>> tetraFaces{{0, 2, 1}, {0, 1, 3}, {0, 3, 2}, {1, 2, 3}};
    assert(eq(meshVolume3(tetrahedron, tetraFaces), 1.L / 6));
    auto section = meshSection3(cube, faces, {{0, 0, 0.5L}, {0, 0, 1}});
    assert(section && section->size() == 1);
    assert(eq((*section)[0].area, 1));
    assert(eq((*section)[0].perimeter, 4));
    auto outside = meshSection3(cube, faces, {{0, 0, 2}, {0, 0, 1}});
    assert(outside && outside->empty());
    vector<P3> twoCubes = cube;
    for (P3 p : cube)
    {
        twoCubes.push_back(p + P3{2, 0, 0});
    }
    vector<array<int, 3>> twoCubeFaces = faces;
    for (auto f : faces)
    {
        for (int &x : f)
        {
            x += 8;
        }
        twoCubeFaces.push_back(f);
    }
    auto twoSections = meshSection3(twoCubes, twoCubeFaces, {{0, 0, 0.5L}, {0, 0, 1}});
    assert(twoSections && twoSections->size() == 2);
    assert(eq((*twoSections)[0].area, 1) && eq((*twoSections)[1].area, 1));
    vector<P3> concaveMesh{{0, 0, 0},
                           {2, 0, 0},
                           {2, 1, 0},
                           {1, 1, 0},
                           {1, 2, 0},
                           {0, 2, 0},
                           {0, 0, 1},
                           {2, 0, 1},
                           {2, 1, 1},
                           {1, 1, 1},
                           {1, 2, 1},
                           {0, 2, 1}};
    vector<array<int, 3>> concaveFaces{{0, 3, 1},  {1, 3, 2},  {0, 5, 3},   {3, 5, 4}, {6, 7, 9},
                                       {7, 8, 9},  {6, 9, 11}, {9, 10, 11}, {0, 1, 7}, {0, 7, 6},
                                       {1, 2, 8},  {1, 8, 7},  {2, 3, 9},   {2, 9, 8}, {3, 4, 10},
                                       {3, 10, 9}, {4, 5, 11}, {4, 11, 10}, {5, 0, 6}, {5, 6, 11}};
    assert(eq(meshVolume3(concaveMesh, concaveFaces), 3));
    auto concaveSection = meshSection3(concaveMesh, concaveFaces, {{0, 0, 0.5L}, {0, 0, 1}});
    assert(concaveSection && concaveSection->size() == 1);
    assert(eq((*concaveSection)[0].area, 3) && eq((*concaveSection)[0].perimeter, 8));
    assert((*concaveSection)[0].points.size() >= 6);
    assert(!meshSection3(cube, faces, {{}, {1, 1, 1}}));
    assert(!meshSection3(cube, faces, {{}, {1, 1, 0}}));
    assert(!meshSection3(cube, faces, {{}, {0, 0, 1}}));
    auto brokenFaces = faces;
    swap(brokenFaces[4][1], brokenFaces[4][2]);
    assert(!meshSection3(cube, brokenFaces, {{0, 0, 0.5L}, {0, 0, 1}}));
    cout << "OK\n";
}
