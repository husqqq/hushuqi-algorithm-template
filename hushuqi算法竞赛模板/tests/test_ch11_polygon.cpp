#include "../include/full/11_2_006.hpp"
#include "../include/full/11_2_002.hpp"
#include "../include/full/11_2_007.hpp"
#include "../include/full/11_2_008.hpp"
#include "../include/full/11_2_009.hpp"

struct C
{
    P o;
    Real r;
};

signed main()
{
    using I = Point<int>;
    vector<I> isq{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert(area2(isq) == 8 && eq(area(isq), 4));
    assert(inPoly(isq, I{1, 1}) == 2 && inPoly(isq, I{2, 1}) == 1);
    assert(triangulate(isq).size() == 2);

    assert(triangulate(vector<P>{}).empty());
    vector<P> col{{0, 0}, {1, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert(triangulate(col).size() == 2);
    vector<P> concave{{0, 0}, {3, 0}, {3, 3}, {1, 1}, {0, 3}};
    assert(triangulate(concave).size() == 3);

    vector<P> sq{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert(convexIsect({}, sq).empty());
    assert(convexIsect({{1, 1}}, sq).size() == 1);
    assert(convexIsect({{3, 1}}, sq).empty());
    auto cut = convexIsect({{-1, 1}, {3, 1}}, sq);
    assert(cut.size() == 2 && onSeg({0, 1}, cut[0], cut[1])
           && onSeg({2, 1}, cut[0], cut[1]));
    auto overlap = convexIsect({{0, 0}, {3, 0}}, {{1, 0}, {4, 0}});
    assert(overlap.size() == 2 && onSeg({1, 0}, overlap[0], overlap[1])
           && onSeg({3, 0}, overlap[0], overlap[1]));
    vector<P> right{{2, 0}, {4, 0}, {4, 2}, {2, 2}};
    assert(convexIsect(sq, right).size() == 2);
    vector<P> corner{{2, 2}, {3, 2}, {3, 3}, {2, 3}};
    assert(convexIsect(sq, corner).size() == 1);
    vector<P> shifted{{1, 1}, {3, 1}, {3, 3}, {1, 3}};
    assert(eq(area(convexIsect(sq, shifted)), 1));
    assert(eq(convexUnion(sq, shifted), 7));

    C c{{0, 0}, 1};
    vector<P> box{{-2, -2}, {2, -2}, {2, 2}, {-2, 2}};
    assert(fabsl(circlePoly(box, c.o, c.r) - acosl(-1)) < 1E-10L);

    auto [in1, b1] = pick({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    assert(in1 == 0 && b1 == 4);
    long long m = LLONG_MAX - 4;
    auto [in2, b2] = pick({{m, m}, {m + 2, m}, {m + 2, m + 2}, {m, m + 2}});
    assert(in2 == 1 && b2 == 8);
    cout << "OK\n";
}
