#include "../include/full/9_1_001.hpp"
#include "../include/full/9_1_002.hpp"
#include "../include/full/9_1_003.hpp"
#include "../include/full/9_1_004.hpp"
#include "../include/full/9_1_005.hpp"
#include "../include/full/9_1_006.hpp"
#include "../include/full/9_1_007.hpp"
#include "../include/full/9_1_008.hpp"
#include "../include/full/9_1_009.hpp"
#include "../include/full/9_1_010.hpp"
#include "../include/full/9_1_011.hpp"
#include "../include/full/9_1_012.hpp"
#include "../include/full/9_2_001.hpp"
#include "../include/full/9_2_002.hpp"
#include "../include/full/9_2_003.hpp"
#include "../include/full/9_2_004.hpp"
#include "../include/full/9_2_005.hpp"
#include "../include/full/9_2_006.hpp"
#include "../include/full/9_2_007.hpp"
#include "../include/full/9_2_008.hpp"
#include "../include/full/9_3_001.hpp"
#include "../include/full/9_3_002.hpp"
#include "../include/full/9_3_003.hpp"
#include "../include/full/9_3_004.hpp"
#include "../include/full/9_3_005.hpp"
#include "../include/full/9_3_006.hpp"

template <int P> struct Mint
{
    int x;

    Mint(long long v = 0) : x((int)(v % P))
    {
        if (x < 0)
        {
            x += P;
        }
    }

    Mint operator-() const
    {
        return x ? P - x : 0;
    }

    Mint &operator+=(const Mint &o)
    {
        x += o.x;
        if (x >= P)
        {
            x -= P;
        }
        return *this;
    }

    Mint &operator-=(const Mint &o)
    {
        x -= o.x;
        if (x < 0)
        {
            x += P;
        }
        return *this;
    }

    Mint &operator*=(const Mint &o)
    {
        x = (long long)x * o.x % P;
        return *this;
    }

    Mint pow(long long b) const
    {
        Mint a = *this, ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans *= a;
            }
            a *= a;
            b >>= 1;
        }
        return ans;
    }

    Mint &operator/=(const Mint &o)
    {
        return *this *= o.pow(P - 2);
    }

    friend Mint operator+(Mint a, const Mint &b) { return a += b; }
    friend Mint operator-(Mint a, const Mint &b) { return a -= b; }
    friend Mint operator*(Mint a, const Mint &b) { return a *= b; }
    friend Mint operator/(Mint a, const Mint &b) { return a /= b; }
    friend bool operator==(const Mint &, const Mint &) = default;
};

template <class T>
vector<vector<T>> naiveMul(const vector<vector<T>> &a, const vector<vector<T>> &b)
{
    int n = a.size(), m = b.size(), k = m ? b[0].size() : 0;
    vector c(n, vector<T>(k));
    for (int i = 0; i < n; i++)
    {
        for (int p = 0; p < m; p++)
        {
            for (int j = 0; j < k; j++)
            {
                c[i][j] += a[i][p] * b[p][j];
            }
        }
    }
    return c;
}

long long bruteHafnian(const vector<vector<long long>> &a, int mask)
{
    if (!mask)
    {
        return 1;
    }
    int i = countr_zero((unsigned)mask);
    int rest = mask ^ (1 << i);
    long long ans = 0;
    for (int s = rest; s; s &= s - 1)
    {
        int j = countr_zero((unsigned)s);
        ans += a[i][j] * bruteHafnian(a, rest ^ (1 << j));
    }
    return ans;
}

Z brutePfaffian(const vector<vector<Z>> &a)
{
    int n = a.size();
    if (!n)
    {
        return 1;
    }
    Z ans = 0;
    for (int j = 1; j < n; j++)
    {
        vector<vector<Z>> b;
        for (int x = 1; x < n; x++)
        {
            if (x == j)
            {
                continue;
            }
            vector<Z> row;
            for (int y = 1; y < n; y++)
            {
                if (y != j)
                {
                    row.push_back(a[x][y]);
                }
            }
            b.push_back(move(row));
        }
        Z term = a[0][j] * brutePfaffian(b);
        ans += j & 1 ? term : -term;
    }
    return ans;
}

void testMatrixPower()
{
    vector<vector<long long>> a{{1, 2}, {3, 4}};
    assert(matPow(a, 3) == vector<vector<long long>>({{37, 54}, {81, 118}}));
    Mat<long long> m(2, 2);
    m.a = a;
    assert(m.pow(3).a == matPow(a, 3));

    mt19937_64 rng(0);
    vector<vector<vector<Z>>> special{
        {},
        {{0, 0}, {0, 0}},
        {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
        {{1, 1, 0}, {0, 1, 0}, {0, 0, 2}},
        {{0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 0}},
    };
    int seed = 1;
    for (auto x : special)
    {
        FrobPower<Z> fp(x, {}, seed++);
        for (unsigned long long b : {0ULL, 1ULL, 2ULL, 17ULL})
        {
            assert(fp.pow(b) == matPow(x, b));
        }
    }
    for (int it = 0; it < 40; it++)
    {
        int n = 1 + rng() % 5;
        vector a(n, vector<Z>(n));
        for (auto &r : a)
        {
            for (auto &x : r)
            {
                x = rng() % 11;
            }
        }
        unsigned long long b = rng() % 40;
        FrobPower<Z> fp(a, {}, 100 + it);
        assert(fp.pow(b) == matPow(a, b));
    }
}

void testMatrixUtilities()
{
    constexpr long long inf = 4'000'000'000'000'000'000LL;
    auto add = [](long long x, long long y) { return min(x, y); };
    auto mul = [&](long long x, long long y)
    {
        return x == inf || y == inf ? inf : x + y;
    };
    vector<vector<long long>> g{{0, 3}, {inf, 0}};
    assert(semiringPow(g, 2, inf, 0LL, add, mul)[0][1] == 3);

    auto inv = matInvAny({{2, 3}, {3, 2}}, 6);
    assert(inv && *inv == vector<vector<long long>>({{2, 3}, {3, 2}}));
    assert(!matInvAny({{2, 0}, {0, 1}}, 6));
    assert(detPrime({{2, 3}, {3, 2}}, 7) == 2);
    assert(detAny({{2, 3}, {3, 2}}, 6) == 1);
    assert(detPrimeFast<998244353>({}) == 1);
    assert(detPrimeFast<998244353>({{2, 3}, {3, 2}}) == 998244348);
    assert(detPrimeFast<998244353>({{-1, 2}, {3, -4}}) == 998244351);

    auto c = charPoly<long double>({{1, 2}, {3, 4}});
    assert(c.size() == 3 && fabsl(c[0] + 2) < 1E-10L &&
           fabsl(c[1] + 5) < 1E-10L && fabsl(c[2] - 1) < 1E-10L);

    SparseMat<long long> s(2, 3);
    s.add(0, 1, 2);
    s.add(0, 1, 3);
    s.add(1, 2, 4);
    assert(s * vector<long long>({7, 11, 13}) == vector<long long>({55, 52}));

    using LM = LinearMint<998244353>;
    SparseMat<LM> sd(3, 3);
    sd.add(0, 0, 2); sd.add(0, 1, 1); sd.add(1, 1, 3);
    sd.add(1, 2, 1); sd.add(2, 0, 4); sd.add(2, 2, 5);
    assert(sparseDet(sd) == LM(34));

    mt19937_64 matrixRng(123);
    for (int n = 0; n <= 7; n++)
    {
        for (int it = 0; it < 30; it++)
        {
            vector a(n, vector<LM>(n));
            for (auto &r : a) for (auto &x : r) x = matrixRng() % 17;
            auto p = charPoly(a);
            assert(p.back() == LM(1));
            SparseMat<LM> sp(n, n);
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (a[i][j] != LM(0)) sp.add(i, j, a[i][j]);
            assert(sparseDet(sp) == detPrime([&]
            {
                vector z(n, vector<long long>(n));
                for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) z[i][j] = a[i][j].val();
                return z;
            }(), 998244353));
            vector z(n, vector<long long>(n));
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    z[i][j] = a[i][j].val();
                }
            }
            assert(detPrimeFast<998244353>(z) == detPrime(z, 998244353));
        }
    }

    vector a(35, vector<long long>(33));
    vector b(33, vector<long long>(34));
    for (int i = 0; i < 35; i++)
    {
        for (int j = 0; j < 33; j++)
        {
            a[i][j] = (i + 2 * j) % 7 - 3;
        }
    }
    for (int i = 0; i < 33; i++)
    {
        for (int j = 0; j < 34; j++)
        {
            b[i][j] = (3 * i + j) % 5 - 2;
        }
    }
    assert(strassen_mul::strassen(a, b) == naiveMul(a, b));

    using woodbury::Mat;
    Mat ai{{1, 0}, {0, 1}};
    assert(woodbury::mul(Mat{{}, {}}, {}).size() == 2);
    assert(woodbury::woodbury(ai, {{}, {}}, {}, {}) == ai);
    auto got = woodbury::woodbury(ai, {{1}, {0}}, {{1}}, {{1, 0}});
    assert(fabsl(got[0][0] - 0.5L) < 1E-10L && fabsl(got[1][1] - 1) < 1E-10L);

    assert(adjugateP({{1, 2}, {3, 4}}, 5) ==
           vector<vector<long long>>({{4, 3}, {2, 1}}));
    assert(adjugateP({{1, 2}, {2, 4}}, 5) ==
           vector<vector<long long>>({{4, 3}, {3, 1}}));
    assert(adjugateP({{0, 0}, {0, 0}}, 5) ==
           vector<vector<long long>>({{0, 0}, {0, 0}}));
    assert(adjugateP({{0}}, 5) == vector<vector<long long>>({{1}}));
}

void testPairingSums()
{
    vector<vector<long long>> a{{0, 1, 2, 3}, {1, 0, 4, 5},
                                {2, 4, 0, 6}, {3, 5, 6, 0}};
    assert(hafnian(a) == 28);
    mt19937 rng(0);
    for (int n = 0; n <= 4; n++)
    {
        int N = 2 * n;
        for (int it = 0; it < 20; it++)
        {
            vector a(N, vector<long long>(N));
            for (int i = 0; i < N; i++)
            {
                for (int j = i + 1; j < N; j++)
                {
                    a[i][j] = a[j][i] = rng() % 7;
                }
            }
            assert(hafnian(a) == bruteHafnian(a, (1 << N) - 1));
        }
    }

    for (int n = 0; n <= 4; n++)
    {
        int N = 2 * n;
        for (int it = 0; it < 20; it++)
        {
            vector a(N, vector<Z>(N));
            for (int i = 0; i < N; i++)
            {
                for (int j = i + 1; j < N; j++)
                {
                    a[i][j] = rng() % 11;
                    a[j][i] = -a[i][j];
                }
            }
            assert(pfaffian(a) == brutePfaffian(a));
        }
    }
}

void testLinearSystems()
{
    auto one = gaussMod({{1, 1, 3}, {1, -1, 1}}, 2, 5);
    assert(one && one->part == vector<long long>({2, 1}) && one->basis.empty());
    auto many = gaussMod({{1, 1, 1}}, 2, 5);
    assert(many && many->basis.size() == 1 && many->basis[0] == vector<long long>({4, 1}));
    assert(!gaussMod({{1, 1}, {1, 2}}, 1, 5));
    auto none = gaussMod({}, 3, 5);
    assert(none && none->basis.size() == 3);
    assert(rankMod({}, 250000, 998244353) == 0);
    assert(rankMod(vector<vector<long long>>(250000), 0, 998244353) == 0);
    assert(rankMod({{1, 2, 3}, {2, 4, 6}, {0, 1, 1}}, 3, 998244353) == 2);

    vector<F2Vector> f2(3, F2Vector(4));
    f2[0].set(0); f2[0].set(2);
    f2[1].set(1); f2[1].set(2);
    f2[2].set(0); f2[2].set(1);
    assert(rankF2(f2) == 2);
    F2Vector rhs(3); rhs.set(0); rhs.set(1);
    auto f2sol = gaussF2(f2, rhs);
    assert(f2sol && f2sol->basis.size() == 2);
    vector<F2Vector> id(4, F2Vector(4)); for (int i = 0; i < 4; i++) id[i].set(i);
    auto inv2 = matInvF2(id); assert(inv2 && (*inv2)[2].get(2));
    auto inter = xorSpaceMeet({3, 5}, {6});
    assert(inter == vector<int>({6}));

    auto x = gaussPivot({{0, 1, 2}, {1, 1, 3}}, 2);
    assert(x && fabsl((*x)[0] - 1) < 1E-10L && fabsl((*x)[1] - 2) < 1E-10L);
    assert(!gaussPivot({{1, 1, 2}}, 2));
    assert(!gaussPivot({{1, 1}, {1, 2}}, 1));
    assert(gaussPivot({}, 0) && !gaussPivot({}, 2));

    auto mn = minNormSolve({{1, 1}, {1, 1}}, {1, 1}, 2);
    assert(mn && fabsl((*mn)[0] - 0.5L) < 1E-10L && fabsl((*mn)[1] - 0.5L) < 1E-10L);
    assert(!minNormSolve({{1, 1}, {1, 1}}, {1, 2}, 2));
    assert(minNormSolve({}, {}, 2) == optional<vector<long double>>({{0, 0}}));

    auto [val, sol] = Simplex({{1, 1}, {1, 0}, {0, 1}}, {4, 2, 3}, {3, 2}).solve();
    assert(fabsl(val - 10) < 1E-10L && sol.size() == 2);
    auto [phaseOne, p] = Simplex({{-1}}, {-1}, {-1}).solve();
    assert(fabsl(phaseOne + 1) < 1E-10L && p.size() == 1);
    auto [bad, q] = Simplex({{1}, {-1}}, {0, -1}, {1}).solve();
    assert(isinf(bad) && bad < 0 && q.empty());
    auto [unbounded, r] = Simplex({}, {}, {1}).solve();
    assert(isinf(unbounded) && unbounded > 0 && r.empty());
    auto [zeroVar, empty] = Simplex({{}}, {1}, {}).solve();
    assert(fabsl(zeroVar) < 1E-10L && empty.empty());
    auto [zeroBad, emptyBad] = Simplex({{}}, {-1}, {}).solve();
    assert(isinf(zeroBad) && zeroBad < 0 && emptyBad.empty());
    auto [degenerate, t] = Simplex({{0.5L, -5.5L, -2.5L, 9},
                                    {0.5L, -1.5L, -0.5L, 1},
                                    {1, 0, 0, 0}},
                                   {0, 0, 1}, {10, -57, -9, -24})
                               .solve();
    assert(fabsl(degenerate - 1) < 1E-10L && t.size() == 4);

    assert(fabsl(frac_prog::maxRatio({-5, -2, -9}, {1, 1, 1}, 2) + 3.5L) < 1E-10L);

    auto tri = tridiagonal<long double>({0, -1, -1}, {2, 2, 2}, {-1, -1, 0}, {1, 0, 1});
    assert(tri && fabsl((*tri)[0] - 1) < 1E-10L);
    auto cyc = cyclicTri<long double>({0, -1, -1, -1}, {4, 4, 4, 4},
                                               {-1, -1, -1, 0}, -1, -1, {2, 2, 2, 2});
    assert(cyc);
    for (auto v : *cyc)
    {
        assert(fabsl(v - 1) < 1E-10L);
    }
    assert(!tridiagonal<long double>({0, 1}, {0, 1}, {1, 0}, {1, 2}));
}

void testNumerics()
{
    auto f = [](long double x) { return x * x * x * x; };
    assert(fabsl(simpson(f, 0, 1) - 0.2L) < 1E-10L);
    assert(fabsl(romberg(f, 0, 1) - 0.2L) < 1E-10L);
    auto root = newton([](long double x) { return x * x - 2; },
                       [](long double x) { return 2 * x; }, 1.L);
    assert(root && fabsl(*root - sqrtl(2.L)) < 1E-10L);
    auto scaledRoot = newton([](long double x) { return 1E-30L * (x - 3); },
                             [](long double) { return 1E-30L; }, 0.L);
    assert(scaledRoot && fabsl(*scaledRoot - 3) < 1E-10L);
    assert(!newton([](long double x) { return x * x + 1; },
                   [](long double x) { return 2 * x; }, 0.L));

    auto roots = cubicRoots(1, -6, 11, -6);
    assert(roots && roots->size() == 3);
    auto quadratic = cubicRoots(0, -1, 3, -2);
    assert(quadratic && quadratic->size() == 2 && (*quadratic)[0] < (*quadratic)[1]);
    auto wideQuadratic = cubicRoots(0, 1, 1E16L, 1);
    assert(wideQuadratic && wideQuadratic->size() == 2 &&
           fabsl((*wideQuadratic)[0] + 1E16L) / 1E16L < 1E-12L &&
           fabsl((*wideQuadratic)[1] + 1E-16L) < 1E-20L);
    auto noRoot = cubicRoots(0, 1, 0, 1);
    assert(noRoot && noRoot->empty() && !cubicRoots(0, 0, 0, 0));

    constexpr long long mod = 2'305'843'009'213'693'951LL;
    long long x = 1'234'567'890'123'456'789LL % mod;
    long long want = (__int128)x * x % mod;
    assert(interpSeq({0, 1, 4}, x, mod) == want);
    constexpr long long bigMod = 9'223'372'036'854'775'783LL;
    assert(interpSeq({0, 1, 4}, bigMod - 1, bigMod) == 1);
    assert(interpSeq({0, 1, 4}, -1, 7) == 1);

    assert(fabsl(sumExpPoly(2.L, vector<long double>{0, 1}, 4) - 34) < 1E-10L);
    assert(fabsl(sumExpPoly(1.L, vector<long double>{0, 1}, 5) - 10) < 1E-10L);
    assert(fabsl(sumExpPLim(0.5L, vector<long double>{0, 1}) - 2) < 1E-10L);
}

signed main()
{
    testMatrixPower();
    testMatrixUtilities();
    testPairingSums();
    testLinearSystems();
    testNumerics();
    cout << "chapter 09 revised tests passed\n";
    return 0;
}
