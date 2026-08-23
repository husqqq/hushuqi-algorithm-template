#include "../include/full/11_4_002.hpp"
#include "../include/full/11_4_003.hpp"
#include "../include/full/11_4_004.hpp"
#include "../include/full/11_4_006.hpp"
#include "../include/full/11_4_007.hpp"
#include "../include/full/11_4_008.hpp"
#include "../include/full/11_4_009.hpp"
#include "../include/full/11_4_010.hpp"
#include "../include/full/11_4_011.hpp"
#include "../include/full/11_4_012.hpp"

signed main()
{
    C z{{0, 0}, 0};
    assert(circleIsect(z, z).size() == 1);
    assert(circleIsect(z, {{1, 0}, 0}).empty());
    assert(lineCircle({{-1, 0}, {1, 0}}, z).size() == 1);
    assert(eq(circleArea(z, {{0, 0}, 2}), 0));

    auto tp = tangents(z, P{1, 0});
    assert(tp.size() == 1 && eq(tp[0], P{}));
    auto tz = tangents(z, C{{1, 0}, 0});
    assert(tz.size() == 1 && eq(tz[0].first, P{}) && eq(tz[0].second, P{1, 0}));
    auto tc = tangents(z, C{{3, 0}, 1});
    assert(tc.size() == 2);

    C col = circle(P{0, 0}, P{2, 0}, P{1, 0});
    assert(eq(col.o, P{1, 0}) && eq(col.r, 1));
    vector<P> a{{0, 0}, {4, 0}, {0, 3}, {1, 1}};
    C c0 = minCircle(a);
    C c1 = minCircle(a, 1);
    assert(eq(c0.o, c1.o) && eq(c0.r, c1.r) && eq(c0.o, P{2, 1.5L}));
    assert(minCircleBd({{0, 0}, {4, 0}, {0, 3}, {1, 1}}) == "1110");
    assert(minCircleBd({{0, 0}, {1, 0}, {2, 0}}) == "101");
    assert(fixedCover({{0, 0}, {0, 0}, {1, 0}}, 0) == 2);

    assert(!invert({0, 0}, {{0, 0}, 2}));
    auto p = invert({4, 0}, {{0, 0}, 2});
    assert(p && eq(*p, P{1, 0}));
    auto p2 = invert(*p, {{0, 0}, 2});
    assert(p2 && eq(*p2, P{4, 0}));
    auto reflectedLine = invertLine(Line<Real>{{1, -2}, {1, 2}}, {{0, 0}, 2});
    assert(holds_alternative<C>(reflectedLine));
    C reflectedCircle = get<C>(reflectedLine);
    for (P x : vector<P>{{1, -2}, {1, 0}, {1, 2}})
    {
        auto y = invert(x, {{0, 0}, 2});
        assert(y && eq(abs(*y - reflectedCircle.o), reflectedCircle.r));
    }
    auto sourceCircle = invertCircle(C{{3, 0}, 1}, {{0, 0}, 2});
    assert(sourceCircle && holds_alternative<C>(*sourceCircle));
    C sourceImage = get<C>(*sourceCircle);
    for (P x : vector<P>{{4, 0}, {3, 1}, {3, -1}})
    {
        auto y = invert(x, {{0, 0}, 2});
        assert(y && eq(abs(*y - sourceImage.o), sourceImage.r));
    }
    assert(!apollonius({0, 0}, {2, 0}, 1));
    auto ap = apollonius({0, 0}, {2, 0}, 0);
    assert(ap && eq(ap->o, P{}) && eq(ap->r, 0));
    const Real pi = acosl(-1);
    assert(eq(circleUnion({{{0, 0}, 1}}), pi));
    assert(eq(circleGap({{0, 0}, 5}, {{1, 0}, 1}), 3));
    auto cc = nearCircles(vector<C>{{{0, 0}, 5}});
    assert(!cc);
    auto ca = nearCircles(vector<C>{{{0, 0}, 5}, {{20, 0}, 1}});
    assert(ca && eq(get<0>(*ca), 14));
    assert(eq(arcCentroid({{0, 0}, 2}, 0, pi / 2), P{4 / pi, 4 / pi}));
    assert(circlePoints(25).size() == 12);
    assert(circlePoints(3).empty());
    assert(circlePoints(0).size() == 1);
    cout << "OK\n";
}
