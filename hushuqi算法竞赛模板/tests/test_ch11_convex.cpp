#include "../include/full/11_3_002.hpp"
#include "../include/full/11_3_004.hpp"
#include "../include/full/11_3_005.hpp"
#include "../include/full/11_3_006.hpp"
#include "../include/full/11_3_007.hpp"
#include "../include/full/11_3_008.hpp"
#include "../include/full/11_3_009.hpp"
#include "../include/full/11_3_010.hpp"
#include "../include/full/11_3_011.hpp"
#include "../include/full/11_3_012.hpp"

signed main()
{
    using I2 = Point<int>;
    auto ih = hull(vector<I2>{{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}});
    assert(ih.size() == 4 && inConvex(ih, I2{1, 1}) && diameter2(ih) == 8);
    assert(get<0>(maxTriangle(ih)) == 4);
    auto im = minkowski(ih, vector<I2>{{0, 0}, {1, 0}});
    assert(area2(im) == 12);

    vector<P> sq{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    DynHull<Real> dh;
    for (P p : sq)
    {
        dh.add(p);
    }
    dh.add({0.5L, 0.5L});
    assert(dh.points().size() == 4 && dh.contains({0.5L, 0.5L})
           && dh.contains({1, 0.5L}) && !dh.contains({2, 0.5L}));
    assert(!closestHull({}, {0, 0}));
    auto one = closestHull({{2, 3}}, {5, 7});
    assert(one && eq(one->first, 5) && eq(one->second, P{2, 3}));
    auto tan = hullTangents(sq, {-1, 0});
    assert((tan == pair<int, int>{3, 0} || tan == pair<int, int>{0, 3}));
    assert((hullTangents(sq, {0.5L, 0.5L}) == pair<int, int>{-1, -1}));
    assert(eq(diameter2(vector<P>{{0, 0}, {3, 0}, {3, 4}, {0, 4}}), 25));
    vector<Point<long long>> rect{{0, 0}, {3, 0}, {3, 4}, {0, 4}};
    vector<Point<long long>> weak{{0, 0}, {2, 0}, {4, 0}, {4, 3}, {4, 4}, {0, 4}};
    vector<Point<long long>> line{{0, 0}, {0, 2}, {0, 4}, {0, 6}};
    assert(weakDiam2(weak) == 32);
    assert(weakDiam2(line) == 36);
    auto fp = farthestPair(rect);
    assert(norm(rect[fp.first] - rect[fp.second]) == 25);
    vector<Point<long long>> same{{1, 2}, {1, 2}, {1, 2}};
    auto fsame = farthestPair(same);
    assert(fsame.first != fsame.second);
    mt19937_64 rng(20260822);
    for (int tc = 0; tc < 300; tc++)
    {
        int n = 2 + rng() % 19;
        vector<Point<long long>> p(n);
        for (auto &x : p)
        {
            x = {(long long)(rng() % 21) - 10, (long long)(rng() % 21) - 10};
        }
        long long want = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                want = max(want, norm(p[i] - p[j]));
            }
        }
        auto [x, y] = farthestPair(p);
        assert(x != y && norm(p[x] - p[y]) == want);
    }

    vector<P> h{{0, 0}, {3, 0}, {4, 1}, {3, 4}, {1, 5}, {-1, 3}};
    Real brute = 0;
    for (int i = 0; i < (int)h.size(); i++)
    {
        for (int j = i + 1; j < (int)h.size(); j++)
        {
            for (int k = j + 1; k < (int)h.size(); k++)
            {
                for (int l = k + 1; l < (int)h.size(); l++)
                {
                    brute = max(brute, fabsl(cross(h[i], h[j], h[k]))
                                       + fabsl(cross(h[i], h[k], h[l])));
                }
            }
        }
    }
    assert(eq(maxQuad(h), brute));

    auto sum = minkowski(vector<P>{{0, 0}, {2, 0}}, vector<P>{{0, 0}, {0, 3}});
    assert(sum.size() == 4 && eq(area(sum), 6));

    vector<HP> box{{{0, 1}, {0, 0}}, {{1, 0}, {1, 1}},
                   {{1, 1}, {0, 1}}, {{0, 0}, {1, 0}},
                   {{2, 0}, {2, 1}}};
    auto a = halfPlane(box);
    assert(a && eq(area(*a), 1));
    auto empty = halfPlane(vector<HP>{{{0, 1}, {0, 0}},
                                      {{-1, 0}, {-1, 1}}});
    assert(empty && empty->empty());
    auto unbounded = halfPlane(vector<HP>{{{0, 1}, {0, 0}},
                                          {{1, 0}, {1, 1}},
                                          {{0, 0}, {1, 0}}});
    assert(!unbounded);
    auto seg = halfPlane(vector<HP>{{{0, 1}, {0, 0}}, {{0, 0}, {0, 1}},
                                    {{0, 0}, {1, 0}}, {{1, 1}, {0, 1}}});
    assert(seg && seg->size() == 2);
    auto point = halfPlane(vector<HP>{{{0, 1}, {0, 0}}, {{0, 0}, {0, 1}},
                                      {{0, 0}, {1, 0}}, {{1, 0}, {0, 0}}});
    assert(point && point->size() == 1 && eq((*point)[0], P{}));

    using integer_hpi::ILine;
    vector<ILine> ibox{{1, 0, 0}, {0, 1, 0}, {-1, 0, 2}, {0, -1, 3}};
    auto iq = integer_hpi::halfPlane(ibox);
    assert(iq && iq->size() == 4 && integer_hpi::vertices(*iq).size() == 4);
    auto ie = integer_hpi::halfPlane(vector<ILine>{
        {1, 0, 0}, {-1, 0, 2}, {1, 0, -4}, {0, 1, 0}, {0, -1, 3}});
    assert(ie && ie->empty());
    auto iu = integer_hpi::halfPlane(vector<ILine>{
        {1, 0, 0}, {0, 1, 0}, {-1, 0, 2}});
    assert(!iu);
    auto is = integer_hpi::halfPlane(vector<ILine>{
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 1}});
    assert(is && integer_hpi::vertices(*is).size() == 2);
    auto ip = integer_hpi::halfPlane(vector<ILine>{
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}});
    assert(ip && integer_hpi::vertices(*ip).size() == 1);

    vector<IP> isq{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto de = delaunay(isq);
    assert(de.edge.size() == 5 && de.tri.size() == 2 && de.vor.size() == 5);
    bool d1 = find(de.edge.begin(), de.edge.end(), pair<int, int>{0, 2}) != de.edge.end();
    bool d2 = find(de.edge.begin(), de.edge.end(), pair<int, int>{1, 3}) != de.edge.end();
    assert(d1 != d2);
    assert(delaunay({{0, 0}, {1, 0}, {2, 0}}).edge.size() == 2);
    auto dup = delaunay(vector<IP>{{0, 0}, {1, 0}, {0, 0}});
    assert((dup.edge == vector<pair<int, int>>({{0, 1}})));
    auto five = delaunay(vector<IP>{{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}});
    assert(five.tri.size() == 4 && five.edge.size() == 8 && five.vor.size() == 8);
    P f = fermatPoint({0, 0}, {4, 0}, {0, 3});
    vector<P> fv{{0, 0}, {4, 0}, {0, 3}};
    for (int i = 0; i < 3; i++)
    {
        P u = fv[i] - f;
        P v = fv[(i + 1) % 3] - f;
        assert(fabsl(dot(u, v) / abs(u) / abs(v) + 0.5L) < 1E-10L);
    }
    assert(eq(fermatPoint({0, 0}, {1, 0}, {-1, 0.1L}), P{}));
    cout << "OK\n";
}
