#include "../include/full/14_1_002.hpp"
#include "../include/full/14_1_003.hpp"
#include "../include/full/14_1_004.hpp"
#include "../include/full/14_1_005.hpp"
#include "../include/full/14_1_006.hpp"
#include "../include/full/14_1_007.hpp"
#include "../include/full/14_1_008.hpp"
#include "../include/full/14_1_009.hpp"

struct TestMod
{
    long long x = 0;

    TestMod(long long x = 0) : x((x % mod() + mod()) % mod())
    {
    }

    static constexpr long long mod()
    {
        return 1000000007;
    }

    TestMod &operator-=(const TestMod &o)
    {
        x -= o.x;
        if (x < 0)
        {
            x += mod();
        }
        return *this;
    }

    friend TestMod operator*(const TestMod &a, const TestMod &b)
    {
        return a.x * b.x;
    }
};

struct TestComb
{
    TestMod C(long long n, long long k)
    {
        if (k < 0 || k > n)
        {
            return 0;
        }
        long long ans = 1;
        for (long long i = 1; i <= k; i++)
        {
            ans = ans * (n - k + i) / i;
        }
        return ans;
    }
};

signed main()
{
    assert(BigInt::addAbs(0, 0).str() == "0");
    BigInt x("123456789012345678901234567890");
    BigInt y("9876543210987654321");
    assert((x * y).str() == "1219326311370217952249657064223746380111126352690");
    assert((x / y).str() == "12499999886");
    assert((x % y).str() == "925925941327160484");
    assert((-x / y) * y + (-x % y) == -x);

    assert(Frac(2, -4) == Frac(-1, 2));
    assert(Frac(1, 6) + Frac(1, 3) == Frac(1, 2));
    assert(Frac(5, 6) - Frac(1, 4) == Frac(7, 12));
    assert(Frac(2, 3) * Frac(9, 4) == Frac(3, 2));
    assert(Frac(2, 3) / Frac(4, 9) == Frac(3, 2));
    assert(Frac(-7, 3) < Frac(-2));

    Expr e;
    assert(e.eval("2+3*4") == 14);
    assert(e.eval("2*-3") == -6);
    assert(e.eval(" 2 + 3 * 4 ") == 14);
    assert(!e.eval("1 2"));
    assert(e.eval("--5") == 5);
    assert(e.eval("-9223372036854775808") == LLONG_MIN);
    assert(!e.eval("") && !e.eval("1+") && !e.eval("1/0"));

    assert((pokerScore({14, 5, 4, 3, 2}, {0, 1, 2, 3, 0})
            == array<int, 6>{4, 5, 0, 0, 0, 0}));
    assert(pokerScore({5, 5, 14, 10, 3}, {0, 1, 0, 2, 3})
           > pokerScore({5, 5, 14, 10, 2}, {0, 1, 0, 2, 3}));

    assert((Date{1970, 1, 1}.days() == 0));
    assert(Date::fromDays(LLONG_MIN).days() == LLONG_MIN);
    assert(Date::fromDays(LLONG_MAX).days() == LLONG_MAX);

    assert(convertBase("FF", 16, 2) == "11111111");
    assert(convertBase("-z", 62, 10) == "-61");
    assert(convertBase("61", 10, 62) == "z");
    assert(convertBase("+000", 62, 2) == "0");
    assert(bigPow(BigInt("12"), 10).str() == "61917364224");

    Sudoku sdk;
    assert(sdk.solve());
    assert((sdk.a[0] == array<int, 9>{1, 2, 3, 4, 5, 6, 7, 8, 9}));
    Sudoku bad;
    bad.a[0][0] = 1;
    bad.a[0][1] = 1;
    assert(!bad.solve());

    assert(bin_packing::exactBins({6, 4, 4, 2}, 8) == 2);
    assert(bin_packing::exactBins({LLONG_MAX / 2 + 1, LLONG_MAX / 2 + 1},
                                  LLONG_MAX) == 2);
    auto fit = bin_packing::firstFit({5, 3, 4, 4, 2}, 8);
    assert(fit.first == 3);
    assert((fit.second == vector<int>{0, 0, 1, 1, 2}));
    assert(bin_packing::firstFit({}, 8).first == 0);

    TestComb comb;
    auto ways = grid_count::gridPaths<TestMod>(3, 3, {{1, 1}}, comb);
    assert(ways.x == 2);
    ways = grid_count::gridPaths<TestMod>(3, 3, {{2, 2}}, comb);
    assert(ways.x == 0);
    cout << "OK\n";
    return 0;
}
