#include "../include/full/11_5_001.hpp"
#include "../include/full/11_5_002.hpp"
#include "../include/full/11_5_003.hpp"
#include "../include/full/11_5_004.hpp"
#include "../include/full/11_5_005.hpp"
#include "../include/full/11_5_006.hpp"

void check(const vector<P> &a, Real want)
{
    ClosestPair<Real> q;
    auto r = q.work(a);
    assert(r);
    auto [x, ids] = *r;
    auto [i, j] = ids;
    assert(i >= 0 && i < (int)a.size() && j >= 0 && j < (int)a.size() && i != j);
    assert(eq(x, want) && eq(norm(a[i] - a[j]), x));
}

signed main()
{
    ClosestPair<Real> q;
    assert(!q.work({{0, 0}}));
    check({{0, 0}, {4, 0}, {0, 0}, {2, 2}}, 0);
    check({{-3, 0}, {-1, 0}, {0, 0}, {4, 0}}, 1);
    check({{0, 0}, {1, 0}, {0, 1}, {1, 1}}, 1);
    check({{7, 9}, {-1, 2}}, 113);
    ClosestPair<long long> iq;
    vector<Point<long long>> ipt{{0, 0}, {5, 0}, {2, 1}, {9, 9}};
    auto ir = iq.work(ipt);
    assert(ir && ir->first == 5);
    vector<P> same(5000, P{3, 4});
    check(same, 0);

    Real z = 5E18L;
    assert(eq(farManhattan(vector<P>{{z, z}, {z + 4, z - 7}}), 11));
    assert(eq(farManhattan(vector<P>{}), 0));
    P p{3, -2};
    assert(eq(fromCheb(toCheb(p)), p));
    Point<long long> lo{-1'000'000'000, -1'000'000'000};
    Point<long long> hi{1'000'000'000, 1'000'000'000};
    long long span = 2'000'000'000;
    assert(manhattan(lo, hi) == 2 * span);
    assert(chebyshev(lo, hi) == span);
    assert(farManhattan(vector<Point<long long>>{lo, hi}) == 2 * span);
    auto ip = toCheb(Point<long long>{1'000'000'000, 1'000'000'000});
    auto back = fromCheb(ip);
    assert(back.x == 1'000'000'000 && back.y == 1'000'000'000);

    RectUnion<long long> ru;
    vector<Rectangle<long long>> rect{{0, 0, 2, 2}, {1, 1, 3, 2}, {5, 0, 5, 3}};
    assert(ru.area(rect) == 5 && ru.perimeter(rect) == 10);
    RectUnion<long long, __int128> wide;
    vector<Rectangle<long long>> big{{LLONG_MIN, 0, LLONG_MAX, 1}};
    __int128 wideSpan = (__int128)LLONG_MAX - LLONG_MIN;
    assert(wide.area(big) == wideSpan);
    assert(wide.perimeter(big) == 2 * (wideSpan + 1));
    assert(hexDist({LLONG_MAX, -LLONG_MAX}) == LLONG_MAX);
    const Real pi = acosl(-1);
    assert(eq(sphereDist(pi / 2, 0, -pi / 2, 0, 2), 2 * pi));
    cout << "OK\n";
}
