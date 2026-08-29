#include "../include/full/7_1_001.hpp"
#include "../include/full/7_1_008.hpp"
#include "../include/full/7_1_010.hpp"
#include "../include/full/7_2_001.hpp"
#include "../include/full/7_2_003.hpp"
#include "../include/full/7_2_004.hpp"
#include "../include/full/7_3_003.hpp"
#include "../include/full/7_3_005.hpp"
#include "../include/full/7_3_006.hpp"
#include "../include/full/7_3_007.hpp"
#include "../include/full/7_3_008.hpp"
#include "../include/full/7_4_002.hpp"
#include "../include/full/7_4_004.hpp"
#include "../include/full/7_4_005.hpp"
#include "../include/full/7_4_006.hpp"
#include "../include/full/7_5_003.hpp"
#include "../include/full/7_5_004.hpp"
#include "../include/full/7_5_005.hpp"
#include "../include/full/7_5_006.hpp"
#include "../include/full/7_5_007.hpp"

long long floorDiv(__int128 x, long long y)
{
    __int128 q = x / y;
    __int128 r = x % y;
    return (long long)(q - (r < 0));
}

signed main()
{
    mt19937 rng(20260829);
    for (int i = 0; i < 1000; i++)
    {
        int x = 1 + rng() % (998244353 - 1);
        assert((long long)x * InvTable<998244353>::get(x) % 998244353 == 1);
    }
    assert(binaryGcd(numeric_limits<long long>::min(), 0) == (1ULL << 63));
    auto l = safeLcm(numeric_limits<long long>::min(), 1);
    assert(l && *l == (1ULL << 63));
    assert(!safeLcm(numeric_limits<long long>::min(), 3));

    Barrett one(1);
    assert(one.mul(0, 0) == 0);
    assert(pow64(123, 0, 1) == 0);
    assert(!isPrime(3825123056546413051ULL));
    assert((factor64(600851475143ULL) ==
            vector<unsigned long long>{71, 839, 1471, 6857}));
    assert((factor64(1000000007ULL * 1000000009ULL) ==
            vector<unsigned long long>{1000000007ULL, 1000000009ULL}));
    assert((factor64(4294967291ULL * 4294967279ULL) ==
            vector<unsigned long long>{4294967279ULL, 4294967291ULL}));

    static_assert(sizeof(Z) == sizeof(uint32_t));
    static_assert(is_same_v<decltype(Z{}.x), uint32_t>);
    assert((Z(mod - 1) + Z(mod - 1)).val() == mod - 2);
    assert((Z(0) - Z(1)).val() == mod - 1);
    assert((Z(mod - 1) * Z(mod - 1)).val() == 1);
    using SmallMint = MInt<1000000007>;
    assert((SmallMint(-1) + SmallMint(-1)).val() == 1000000005);
    assert((SmallMint(0) - SmallMint(1)).val() == 1000000006);
    assert((SmallMint(-1) * SmallMint(-1)).val() == 1);
    const auto &iv = invTable<998244353>(1000);
    for (int i = 1; i <= 1000; i++)
    {
        assert(iv[i] * Z(i) == Z(1));
    }
    assert(Mont<1>(123).val() == 0);
    assert((Mont<998244353>(123456) * Mont<998244353>(789012)).val() ==
           (unsigned)((__int128)123456 * 789012 % 998244353));

    auto empty = multTable(0, [](int p, int e)
    {
        return p + e;
    });
    assert(empty.size() == 1 && empty[0] == 0);

    vector<long long> zeta{0, 1, 0, 0, 0};
    auto original = zeta;
    divisorZeta(zeta);
    assert((zeta == vector<long long>{0, 1, 1, 1, 1}));
    divisorMu(zeta);
    assert(zeta == original);

    constexpr int lim = 2000;
    auto f = multTable(lim, [](int p, int e)
    {
        return p + e;
    });
    Z want = 0;
    for (int i = 1; i <= lim; i++)
    {
        want += f[i];
    }
    Min25 min25(lim);
    assert(min25.linearPPow(1, 1) == want);

    DuSieve du(1000000);
    assert(du.sumMu(10) == -1);
    assert(du.sumPhi(10) == 32);
    assert(du.sumPhi(1000000) == (__int128)303963552392LL);

    LehmerPi pi;
    assert((enumPrimes(20) == vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));
    assert(pi.phi(1, 2) == 1);
    assert(pi.phi(2, 2) == 1);
    assert(pi(10) == 4);
    assert(pi(1000000) == 78498);

    assert(bsgs(0, 0, 1) == optional<long long>(0));
    assert(PH(3, 13, 17) == optional<int>(4));
    assert(primRoot(998244353) == 3);
    assert(powerMod(primRoot(1000000007), 500000003, 1000000007) != 1);
    assert(sqrtMod(1, 2) == optional<long long>(1));
    assert(sqrtMod(13, 17) == optional<long long>(8));
    auto fourthRoot = kthResidue(13, 4, 17);
    assert(fourthRoot && powerMod(*fourthRoot, 4, 17) == 13);
    assert(!kthResidue(3, 2, 7));
    assert(kthResidue(1, 0, 7) == optional<int>(0));
    assert(!kthResidue(0, 0, 7));
    for (int p : {2LL, 3LL, 5LL, 7LL, 11LL, 17LL, 29LL})
    {
        for (int k = 0; k <= 2 * p; k++)
        {
            for (int a = 0; a < p; a++)
            {
                bool exists = false;
                for (int x = 0; x < p; x++)
                {
                    exists |= powerMod(x, k, p) == a;
                }
                auto root = kthResidue(a, k, p);
                assert((bool)root == exists);
                if (root)
                {
                    assert(powerMod(*root, k, p) == a);
                }
            }
        }
    }

    assert(PowerTower::eval(0, 0, 7) == 1);
    assert(PowerTower::eval(0, 1, 7) == 0);
    assert(PowerTower::eval(0, 2, 7) == 1);
    assert(PowerTower::eval(0, 3, 7) == 0);

    for (int n = 0; n <= 20; n++)
    {
        for (int m = 1; m <= 20; m++)
        {
            for (int a = -20; a <= 20; a++)
            {
                for (int b = -20; b <= 20; b++)
                {
                    __int128 wantSum = 0;
                    for (int i = 0; i < n; i++)
                    {
                        wantSum += floorDiv((__int128)a * i + b, m);
                    }
                    assert(floorSum(n, m, a, b) == wantSum);
                }
            }
        }
    }

    for (int n = 1; n <= 20; n++)
    {
        for (int m = 1; m <= 20; m++)
        {
            for (int a = -20; a <= 20; a++)
            {
                for (int b = -20; b <= 20; b++)
                {
                    int wantMin = m;
                    for (int i = 0; i < n; i++)
                    {
                        int v = (int)(((__int128)a * i + b) % m);
                        if (v < 0)
                        {
                            v += m;
                        }
                        wantMin = min(wantMin, v);
                    }
                    assert(minModLinear(n, m, a, b) == wantMin);
                }
            }
        }
    }

    auto flat = floorPath(0, 7, 3, 5, {0, 1, 0}, {1, 0, 0});
    assert(flat.x == 5 && flat.y == 0 && flat.xy == 0);
    auto path = floorPath(4, 11, 3, 5, {0, 1, 0}, {1, 0, 0});
    assert(path.x == 5 && path.y == 2 && path.xy == 5);
    for (int p = 0; p <= 10; p++)
    {
        for (int q = 1; q <= 10; q++)
        {
            for (int r = 0; r < q; r++)
            {
                for (int n = 0; n <= 10; n++)
                {
                    auto got = floorPath(p, q, r, n,
                                         {0, 1, 0}, {1, 0, 0});
                    __int128 sum = 0;
                    for (int i = 1; i <= n; i++)
                    {
                        sum += ((__int128)p * i + r) / q;
                    }
                    assert(got.x == n);
                    assert(got.y == ((__int128)p * n + r) / q);
                    assert(got.xy == sum);
                }
            }
        }
    }

    auto pell2 = pell(2);
    assert(pell2 && pell2->first == 3 && pell2->second == 2);
    assert(!pell(4));
    assert(circlePoints(25).size() == 12);
    assert(circlePoints(3).empty());
    assert((circlePoints(0) == vector<pair<long long, long long>>{{0, 0}}));
    for (int n = 0; n <= 200; n++)
    {
        vector<pair<long long, long long>> brute;
        for (int x = 0; x <= n; x++)
        {
            for (int y = 0; y <= n; y++)
            {
                if (x * x + y * y == n) brute.push_back({x, y});
            }
        }
        assert(twoSqRep(n) == brute);
    }
    assert((sternLca(3, 2, 5, 3) == pair<long long, long long>(3, 2)));
    assert((sternChild(1, 1) == pair{pair{1LL, 2LL}, pair{2LL, 1LL}}));
    assert((sternRange(3, 2) == pair{pair{1LL, 1LL}, pair{2LL, 1LL}}));
    assert((sternAnc(1, 3, 2) == optional<pair<long long, long long>>({2, 1})));
    assert(!sternAnc(3, 3, 2));
    assert((ratApprox(3, 7, 5) == pair{pair{1LL, 1LL}, pair{3LL, 2LL}}));

    cout << "ch07 revised tests passed\n";
}
