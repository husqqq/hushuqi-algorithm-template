#include "../include/full/11_2_002.hpp"
#include "../include/full/11_2_007.hpp"
#include "../include/full/11_2_011.hpp"
#include "../include/full/11_3_014.hpp"
#include "../include/full/11_4_005.hpp"
#include "../include/full/11_4_008.hpp"
#include "../include/full/11_4_009.hpp"
#include "../include/full/11_5_007.hpp"

pair<__int128, __int128> bruteTriangle(const vector<Point<long long>> &a)
{
    // a 是不少于三个点的整数点集；返回三点双倍面积的最小值和最大值。
    __int128 mn = -1;
    __int128 mx = 0;
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = i + 1; j < (int)a.size(); j++)
        {
            for (int k = j + 1; k < (int)a.size(); k++)
            {
                __int128 x1 = (__int128)a[j].x - a[i].x;
                __int128 y1 = (__int128)a[j].y - a[i].y;
                __int128 x2 = (__int128)a[k].x - a[i].x;
                __int128 y2 = (__int128)a[k].y - a[i].y;
                __int128 s = x1 * y2 - y1 * x2;
                if (s < 0)
                {
                    s = -s;
                }
                if (mn == -1 || s < mn)
                {
                    mn = s;
                }
                mx = max(mx, s);
            }
        }
    }
    return {mn, mx};
}

bool bruteSegments(const vector<Line<long long>> &a)
{
    // a 是整数闭线段集；返回是否存在相交线段对。
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = i + 1; j < (int)a.size(); j++)
        {
            if (segMeet(a[i], a[j]))
            {
                return true;
            }
        }
    }
    return false;
}

vector<Real> rectangleCoverage(const vector<array<int, 4>> &a)
{
    // a 的每项依次为左、下、右、上边界；返回矩形集各覆盖层的精确网格面积。
    vector<int> xs, ys;
    for (auto [l, d, r, u] : a)
    {
        xs.push_back(l);
        xs.push_back(r);
        ys.push_back(d);
        ys.push_back(u);
    }
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    vector<Real> ans(a.size());
    for (int i = 0; i + 1 < (int)xs.size(); i++)
    {
        for (int j = 0; j + 1 < (int)ys.size(); j++)
        {
            Real x = (xs[i] + xs[i + 1]) / 2.0L;
            Real y = (ys[j] + ys[j + 1]) / 2.0L;
            int cnt = 0;
            for (auto [l, d, r, u] : a)
            {
                cnt += l < x && x < r && d < y && y < u;
            }
            Real s = (Real)(xs[i + 1] - xs[i]) * (ys[j + 1] - ys[j]);
            for (int k = 0; k < cnt; k++)
            {
                ans[k] += s;
            }
        }
    }
    return ans;
}

signed main()
{
    using I = Point<long long>;
    vector<I> sq{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert((windingNum(sq, I{1, 1}) == pair<bool, int>{false, 1}));
    reverse(sq.begin(), sq.end());
    assert((windingNum(sq, I{1, 1}) == pair<bool, int>{false, -1}));
    vector<I> twice{{0, 0}, {2, 0}, {2, 2}, {0, 2}, {0, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert((windingNum(twice, I{1, 1}) == pair<bool, int>{false, 2}));
    assert(windingNum(twice, I{2, 1}).first);
    assert((windingNum(vector<I>{}, I{}) == pair<bool, int>{false, 0}));
    vector<I> bow{{0, 0}, {4, 4}, {0, 4}, {4, 0}};
    assert(windingNum(bow, I{2, 2}).first);
    assert((windingNum(bow, I{2, 3}) == pair<bool, int>{false, 1}));
    assert(windingNum(sq, I{0, 0}).first);
    vector<I> zeroEdge{{0, 0}, {2, 0}, {2, 0}, {2, 2}, {0, 2}};
    assert((windingNum(zeroEdge, I{1, 1}) == pair<bool, int>{false, 1}));

    auto in = incircle({0, 0}, {4, 0}, {0, 3});
    assert(in && eq(in->o, P{1, 1}) && eq(in->r, 1));
    auto reversedIn = incircle({0, 3}, {4, 0}, {0, 0});
    assert(reversedIn && eq(reversedIn->o, P{1, 1}) && eq(reversedIn->r, 1));
    assert(!incircle({0, 0}, {0, 0}, {0, 3}));
    assert(!incircle({0, 0}, {1, 0}, {2, 0}));

    C inv{{0, 0}, 2};
    auto lineCircle = invertLine(Line<Real>{{1, -2}, {1, 2}}, inv);
    assert(holds_alternative<C>(lineCircle));
    C lc = get<C>(lineCircle);
    assert(eq(lc.o, P{2, 0}) && eq(lc.r, 2));
    auto sameLine = invertLine(Line<Real>{{-2, 0}, {3, 0}}, inv);
    assert(holds_alternative<Line<Real>>(sameLine));
    assert(side(get<Line<Real>>(sameLine), P{10, 0}) == 0);
    auto regularCircle = invertCircle(C{{3, 0}, 1}, inv);
    assert(regularCircle && holds_alternative<C>(*regularCircle));
    assert(eq(get<C>(*regularCircle).o, P{1.5L, 0}) && eq(get<C>(*regularCircle).r, 0.5L));
    auto throughCenter = invertCircle(C{{2, 0}, 2}, inv);
    assert(throughCenter && holds_alternative<Line<Real>>(*throughCenter));
    assert(side(get<Line<Real>>(*throughCenter), P{1, 5}) == 0);
    auto concentric = invertCircle(C{{0, 0}, 1}, inv);
    assert(concentric && holds_alternative<C>(*concentric));
    assert(eq(get<C>(*concentric).o, P{}) && eq(get<C>(*concentric).r, 4));
    auto pointCircle = invertCircle(C{{4, 0}, 0}, inv);
    assert(pointCircle && holds_alternative<C>(*pointCircle));
    assert(eq(get<C>(*pointCircle).o, P{1, 0}) && eq(get<C>(*pointCircle).r, 0));
    assert(!invertCircle(C{{0, 0}, 0}, inv));

    const Real pi = acosl(-1);
    Real inter = 2 * pi / 3 - sqrtl(3) / 2;
    auto one = circleCov(vector<C>{{{0, 0}, 1}});
    assert(one.size() == 1 && eq(one[0], pi));
    auto duplicateCircle = circleCov(vector<C>{{{0, 0}, 1}, {{0, 0}, 1}});
    assert(eq(duplicateCircle[0], pi) && eq(duplicateCircle[1], pi));
    auto tripleCircle = circleCov(vector<C>{{{0, 0}, 1}, {{0, 0}, 1}, {{1, 0}, 1}});
    assert(eq(tripleCircle[0], 2 * pi - inter) && eq(tripleCircle[1], pi) && eq(tripleCircle[2], inter));
    auto nestedCircle = circleCov(vector<C>{{{0, 0}, 2}, {{0, 0}, 1}});
    assert(eq(nestedCircle[0], 4 * pi) && eq(nestedCircle[1], pi));
    auto tangentCircle = circleCov(vector<C>{{{0, 0}, 1}, {{2, 0}, 1}});
    assert(eq(tangentCircle[0], 2 * pi) && eq(tangentCircle[1], 0));
    auto overlapCircle = circleCov(vector<C>{{{0, 0}, 1}, {{1, 0}, 1}});
    assert(eq(overlapCircle[0], 2 * pi - inter) && eq(overlapCircle[1], inter));
    auto zeroCircle = circleCov(vector<C>{{{0, 0}, 1}, {{0, 0}, 0}});
    assert(eq(zeroCircle[0], pi) && eq(zeroCircle[1], 0));

    vector<vector<I>> overlapPoly{{{0, 0}, {2, 0}, {2, 2}, {0, 2}}, {{1, 1}, {3, 1}, {3, 3}, {1, 3}}};
    auto pa = covArea(overlapPoly);
    assert(eq(pa[0], 7) && eq(pa[1], 1));
    vector<I> outer{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    vector<I> inner{{1, 1}, {3, 1}, {3, 3}, {1, 3}};
    auto weightedPoly = covArea(vector<vector<I>>{outer, {{4, 0}, {4, 4}, {0, 4}, {0, 0}}, inner});
    assert(eq(weightedPoly[0], 16) && eq(weightedPoly[1], 16) && eq(weightedPoly[2], 4));
    vector<P> convexA{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    vector<P> convexB{{1, 0.5L}, {3, 0.5L}, {3, 2.5L}, {1, 2.5L}};
    auto convexCoverage = covArea(vector<vector<P>>{convexA, convexB});
    assert(eq(convexCoverage[0], convexUnion(convexA, convexB)) && eq(convexCoverage[1], 1.5L));
    vector<I> concave{{0, 0}, {4, 0}, {4, 2}, {2, 2}, {2, 4}, {0, 4}};
    auto concaveCoverage = covArea(vector<vector<I>>{concave, {{1, 1}, {2, 1}, {2, 2}, {1, 2}}});
    assert(eq(concaveCoverage[0], 12) && eq(concaveCoverage[1], 1));
    auto touchPoly =
        covArea(vector<vector<I>>{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{1, 1}, {2, 1}, {2, 2}, {1, 2}}});
    assert(eq(touchPoly[0], 2) && eq(touchPoly[1], 0));
    assert((Fraction128{999'999'999'999'999'999LL, 1'000'000'000'000'000'000LL} < Fraction128{1, 1}));

    assert(!triAreaRng2(vector<I>{{0, 0}, {1, 0}}));
    auto rect = triAreaRng2(vector<I>{{0, 0}, {4, 0}, {4, 3}, {0, 3}});
    assert(rect && rect->first == 12 && rect->second == 12);
    auto duplicatePoint = triAreaRng2(vector<I>{{0, 0}, {1, 0}, {0, 1}, {0, 0}});
    assert(duplicatePoint && duplicatePoint->first == 0 && duplicatePoint->second == 1);
    auto collinear = triAreaRng2(vector<I>{{0, 0}, {1, 0}, {2, 0}, {3, 0}});
    assert(collinear && collinear->first == 0 && collinear->second == 0);
    auto partlyCollinear = triAreaRng2(vector<I>{{0, 0}, {1, 0}, {2, 0}, {0, 2}});
    assert(partlyCollinear && partlyCollinear->first == 0 && partlyCollinear->second == 4);
    const long long bigCoord = 1'000'000'000LL;
    auto bigTriangle = triAreaRng2(vector<I>{{-bigCoord, 0}, {bigCoord, 0}, {0, bigCoord}});
    assert(bigTriangle && bigTriangle->first == (__int128)2 * bigCoord * bigCoord &&
           bigTriangle->second == (__int128)2 * bigCoord * bigCoord);
    auto floatingTriangle = triAreaRng2(vector<P>{{0, 0}, {4, 0}, {0, 3}, {1, 1}});
    assert(floatingTriangle && eq(floatingTriangle->first, 3) && eq(floatingTriangle->second, 12));

    vector<Line<long long>> crossing{{{0, 0}, {4, 4}}, {{0, 4}, {4, 0}}};
    auto hit = anySegMeet(crossing);
    assert(hit && segMeet(crossing[hit->first], crossing[hit->second]));
    assert(!anySegMeet(vector<Line<long long>>{{{0, 0}, {1, 0}}, {{2, 0}, {3, 0}}}));
    assert(anySegMeet(vector<Line<long long>>{{{0, 0}, {0, 3}}, {{0, 2}, {0, 4}}}));
    assert(anySegMeet(vector<Line<long long>>{{{1, 1}, {1, 1}}, {{0, 1}, {2, 1}}}));
    assert(anySegMeet(vector<Line<long long>>{{{0, 0}, {1, 1}}, {{1, 1}, {2, 0}}}));
    assert(anySegMeet(vector<Line<long long>>{{{0, 0}, {0, 0}}, {{0, 0}, {0, 1}}, {{0, 0}, {1, 0}}}));
    const long long big = 4'000'000'000'000'000'000LL;
    assert(anySegMeet(vector<Line<long long>>{{{-big, 0}, {big, 0}}, {{0, -big}, {0, big}}}));
    assert(anySegMeet(vector<Line<Real>>{{{0, 0}, {2, 2}}, {{0, 2}, {2, 0}}}));

    mt19937_64 rng(0);
    for (int rep = 0; rep < 2000; rep++)
    {
        int n = 3 + rng() % 7;
        vector<I> p(n);
        for (I &x : p)
        {
            x = {(int)(rng() % 11) - 5, (int)(rng() % 11) - 5};
        }
        auto got = triAreaRng2(p);
        assert(got && *got == bruteTriangle(p));
    }
    for (int rep = 0; rep < 10000; rep++)
    {
        int n = rng() % 9;
        vector<Line<long long>> a(n);
        for (auto &s : a)
        {
            s = {{(int)(rng() % 11) - 5, (int)(rng() % 11) - 5}, {(int)(rng() % 11) - 5, (int)(rng() % 11) - 5}};
        }
        auto got = anySegMeet(a);
        assert((bool)got == bruteSegments(a));
        if (got)
        {
            assert(got->first != got->second && segMeet(a[got->first], a[got->second]));
        }
    }
    for (int rep = 0; rep < 500; rep++)
    {
        int n = 1 + rng() % 5;
        vector<int> xs(2 * n), ys(2 * n);
        iota(xs.begin(), xs.end(), 0);
        iota(ys.begin(), ys.end(), 0);
        shuffle(xs.begin(), xs.end(), rng);
        shuffle(ys.begin(), ys.end(), rng);
        vector<array<int, 4>> rects;
        vector<vector<I>> polys;
        for (int i = 0; i < n; i++)
        {
            int l = min(xs[2 * i], xs[2 * i + 1]);
            int r = max(xs[2 * i], xs[2 * i + 1]);
            int d = min(ys[2 * i], ys[2 * i + 1]);
            int u = max(ys[2 * i], ys[2 * i + 1]);
            rects.push_back({l, d, r, u});
            polys.push_back({{l, d}, {r, d}, {r, u}, {l, u}});
        }
        auto got = covArea(polys);
        auto want = rectangleCoverage(rects);
        for (int i = 0; i < n; i++)
        {
            assert(eq(got[i], want[i]));
        }
    }
    for (int rep = 0; rep < 500; rep++)
    {
        int n = 1 + rng() % 7;
        vector<C> a(n);
        for (C &c : a)
        {
            c = {{(Real)((int)(rng() % 9) - 4), (Real)((int)(rng() % 9) - 4)}, (Real)(rng() % 5)};
        }
        auto got = circleCov(a);
        assert(eq(got[0], circleUnion(a)));
        for (int i = 1; i < n; i++)
        {
            assert(sgn(got[i - 1] - got[i]) >= 0);
        }
    }
    cout << "OK\n";
}
