#include "../include/full/11_1_004.hpp"
#include "../include/full/11_1_009.hpp"
#include "../include/full/11_1_010.hpp"
#include "../include/full/11_1_011.hpp"

signed main()
{
    using I = Point<int>;
    I a{2'000'000'000LL, 0};
    I b{0, 2'000'000'000LL};
    assert(cross(a, b) == 4'000'000'000'000'000'000LL);
    I c{4'000'000'000LL, 0}, d{0, 4'000'000'000LL};
    __int128 wide = (__int128)c.x * d.y - (__int128)c.y * d.x;
    assert(wide == (__int128)16'000'000'000'000'000'000ULL);
    assert(norm(I{3, 4}) == 25);
    assert(fabsl(abs(I{3, 4}) - 5) < eps);
    Real pi = acosl(-1);
    assert(fabsl(angle(I{1, 0}, I{0, 1}) - pi / 2) < eps);
    assert(fabsl(angle(I{0, 1}, I{1, 0}) + pi / 2) < eps);
    assert(fabsl(fabsl(angle(I{1, 0}, I{-1, 0})) - pi) < eps);

    vector<P> p{{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {2, 0}};
    polarSort(p);
    assert(eq(p[0], P{1, 0}));
    assert(eq(p[1], P{2, 0}));
    assert(eq(p[2], P{0, 1}));
    assert(eq(p[3], P{-1, 0}));
    assert(eq(p[4], P{0, -1}));

    auto l = toDir(Line<int>{{1, 2}, {4, 6}});
    assert(eq(l.at(1), P{4, 6}));
    assert(parallel(Line<Real>{{0, 0}, {1, 1E-15L}},
                    Line<Real>{{0, 1}, {1, 1 + 2E-15L}}));
    assert(parallel(Line<int>{{0, 0}, {2, 2}}, Line<int>{{1, 0}, {3, 2}}));
    assert(perp(Line<int>{{0, 0}, {1, 2}}, Line<int>{{0, 0}, {-2, 1}}));
    assert(side(Line<int>{{0, 0}, {2, 0}}, I{0, 3}) == 1);
    assert(segMeet(Line<int>{{-1'000'000'000LL, 0}, {1'000'000'000LL, 0}},
                               Line<int>{{0, -1'000'000'000LL}, {0, 1'000'000'000LL}}));
    assert(fabsl(distancePS(P{3, 4}, Line<Real>{{0, 0}, {0, 0}}) - 5) < eps);
    assert(eq(lineMeet(Line<int>{{0, 0}, {1, 1}},
                               Line<int>{{0, 1}, {1, 0}}),
              P{0.5L, 0.5L}));
    assert(rectKind(array<P, 4>{P{0, 0}, P{2, 0}, P{2, 1}, P{0, 1}}) == 1);
    assert(rectKind(array<P, 4>{P{0, 0}, P{1, 0}, P{1, 1}, P{0, 1}}) == 2);
    assert(rectKind(array<I, 4>{I{0, 0}, I{3, 0}, I{3, 2}, I{0, 2}}) == 1);
    cout << "OK\n";
    return 0;
}
