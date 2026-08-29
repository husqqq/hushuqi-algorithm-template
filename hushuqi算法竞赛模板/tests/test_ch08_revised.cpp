#include "../include/full/8_1_001.hpp"
#include "../include/full/8_1_004.hpp"
#include "../include/full/8_1_007.hpp"
#include "../include/full/8_1_008.hpp"
#include "../include/full/8_1_010.hpp"
#include "../include/full/8_1_011.hpp"
#include "../include/full/8_2_005.hpp"
#include "../include/full/8_2_012.hpp"
#include "../include/full/8_2_013.hpp"
#include "../include/full/8_2_014.hpp"
#include "../include/full/8_2_015.hpp"
#include "../include/full/8_2_017.hpp"
#include "../include/full/8_3_002.hpp"
#include "../include/full/8_3_003.hpp"
#include "../include/full/8_3_004.hpp"
#include "../include/full/8_3_005.hpp"

int qpowTest(int a, int n, int p)
{
    // a 是底数，n 是非负指数，p 是正模数；返回 a^n mod p。
    int r = 1 % p;
    while (n)
    {
        if (n & 1)
        {
            r = (__int128)r * a % p;
        }
        a = (__int128)a * a % p;
        n >>= 1;
    }
    return r;
}

int qBinomDp(int n, int k, int q, int p)
{
    // n、k 是 q-二项式参数，q 是参数，p 是正模数；用定义递推返回 [n choose k]_q mod p。
    if (k < 0 || k > n)
    {
        return 0;
    }
    vector<int> f(k + 1);
    f[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = min(i, k); j >= 1; j--)
        {
            f[j] = (f[j - 1] + (__int128)qpowTest(q, j, p) * f[j]) % p;
        }
    }
    return f[k];
}

void checkMagic(int n)
{
    // n 是可构造阶数；验证元素唯一及全部行列、对角线和。
    auto a = magicSquare(n);
    assert((int)a.size() == n);
    int total = n * n;
    int want = n * (total + 1) / 2;
    vector<char> seen(total + 1);
    int d1 = 0;
    int d2 = 0;
    for (int i = 0; i < n; i++)
    {
        int row = 0;
        int col = 0;
        for (int j = 0; j < n; j++)
        {
            int x = a[i][j];
            assert(1 <= x && x <= total && !seen[x]);
            seen[x] = 1;
            row += x;
            col += a[j][i];
        }
        assert(row == want && col == want);
        d1 += a[i][i];
        d2 += a[i][n - 1 - i];
    }
    assert(d1 == want && d2 == want);
}

signed main()
{
    Comb<Z> comb;
    assert(comb.C(10, 3).val() == 120);
    assert(comb.A(10, 3).val() == 720);
    assert(comb.C(132, 66).val() == 241200029);
    Comb<MInt<1000000007>> comb1e9;
    assert(comb1e9.C(132, 66).val() == 598375978);
    assert(comb.C(0, 0).val() == 1);
    assert(comb.C(10, 11).val() == 0);
    assert(BinomMod(1).C(20, 7) == 0);
    assert(BinomMod(7).C(100, 50) == 4);
    assert(BinomMod(8).C(10, 3) == 0);
    assert(BinomMod(12).C(10, 3) == 0);
    assert(BinomMod(72).C(20, 6) == 24);
    mt19937_64 rng(0);
    unsigned long long extAcc = 0;
    for (int it = 0; it < 500; it++)
    {
        int md = 1 + rng() % 200;
        int n = rng() % 60;
        int k = rng() % 65;
        vector c(n + 1, vector<int>(n + 1));
        for (int i = 0; i <= n; i++)
        {
            c[i][0] = 1 % md;
            for (int j = 1; j <= i; j++)
            {
                c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % md;
            }
        }
        int want = k <= n ? c[n][k] : 0;
        assert(BinomMod(md).C(n, k) == want);
        extAcc += want;
    }

    assert(incl_excl::countMult(numeric_limits<int>::max(), {1, 2}) ==
           numeric_limits<int>::max());
    assert(minmax_inv::kthMaxMinSum({0, 7, 4, 1}, 1) == 4);
    assert(minmax_inv::kthMaxMinSum({0, 7, 4, 1}, 2) == 2);
    assert(minmax_inv::kthMaxMinSum({0, -4, -12, -5}, 3) == -5);
    assert(minmax_inv::kthMaxMinSum({0, numeric_limits<int>::min()}, 1) ==
           numeric_limits<int>::min());
    assert(minmax_inv::kthMaxMinSum({0, numeric_limits<int>::max()}, 1) ==
           numeric_limits<int>::max());
    assert(balls_boxes::pigeonBound(numeric_limits<int>::max(), 2) ==
           numeric_limits<int>::max() / 2 + 1);
    assert(balls_boxes::unlabBoxDist<__int128>(
               numeric_limits<int>::max(), 2, false) == (__int128)numeric_limits<int>::max() + 1);

    for (int p : {2, 3, 5, 7, 11})
    {
        for (int q = 0; q < p; q++)
        {
            QBinomPrime cb(p, q);
            for (int n = 0; n < p; n++)
            {
                for (int k = 0; k <= n; k++)
                {
                    assert(cb.C(n, k) == qBinomDp(n, k, q, p));
                }
            }
        }
    }

    int lim = 90;
    vector s1(lim + 1, vector<int>(lim + 1));
    vector s2(lim + 1, vector<int>(lim + 1));
    s1[0][0] = 1;
    s2[0][0] = 1;
    for (int i = 1; i <= lim; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            s1[i][j] = (s1[i - 1][j - 1] - (__int128)(i - 1) * s1[i - 1][j]) % mod;
            if (s1[i][j] < 0)
            {
                s1[i][j] += mod;
            }
            s2[i][j] = (s2[i - 1][j - 1] + (__int128)j * s2[i - 1][j]) % mod;
        }
    }
    for (int k : {0, 1, 2, 7, 45, 90})
    {
        auto a = stirling1K(lim, k);
        auto b = stirling2K(lim, k);
        for (int i = k; i <= lim; i++)
        {
            assert(a[i - k].val() == s1[i][k]);
            assert(b[i - k].val() == s2[i][k]);
        }
    }

    for (int p : {2, 3, 5, 7})
    {
        Stirling1P a(p);
        Stirling2P b(p);
        vector x(lim + 1, vector<int>(lim + 1));
        vector y(lim + 1, vector<int>(lim + 1));
        x[0][0] = 1;
        y[0][0] = 1;
        for (int n = 1; n <= lim; n++)
        {
            for (int k = 1; k <= n; k++)
            {
                x[n][k] = (x[n - 1][k - 1] - (__int128)(n - 1) * x[n - 1][k]) % p;
                if (x[n][k] < 0)
                {
                    x[n][k] += p;
                }
                y[n][k] = (y[n - 1][k - 1] + (__int128)k * y[n - 1][k]) % p;
            }
        }
        for (int n = 0; n <= lim; n++)
        {
            for (int k = 0; k <= n; k++)
            {
                assert(a.get(n, k) == x[n][k]);
                assert(b.get(n, k) == y[n][k]);
            }
        }
        int n = 1000000000000000000LL + p;
        int c2;
        if (n & 1)
        {
            c2 = (__int128)(n % p) * ((n - 1) / 2 % p) % p;
        }
        else
        {
            c2 = (__int128)(n / 2 % p) * ((n - 1) % p) % p;
        }
        assert(a.get(n, 1) == 0);
        assert(a.get(n, n - 1) == (p - c2) % p);
        assert(a.get(n, n) == 1 % p);
        assert(b.get(n, 1) == 1 % p);
        assert(b.get(n, 2) == (qpowTest(2, n - 1, p) + p - 1) % p);
        assert(b.get(n, n) == 1 % p);
    }
    int bigN = 1000000000000000003LL;
    Stirling1P s1p(5);
    Stirling2P s2p(5);
    assert(s1p.get(6, 2) == 4);
    assert(s1p.get(bigN, bigN - 1) == 2);
    assert(s2p.get(6, 3) == 0);
    assert(s2p.get(bigN, 2) == 3);

    vector<vector<int>> rot{{0, 1, 2, 3}, {1, 2, 3, 0}, {2, 3, 0, 1}, {3, 0, 1, 2}};
    assert(polya::burnside(rot, 2) == 6);
    vector<vector<int>> id{{0, 1, 2, 3}};
    __int128 big = 1;
    for (int i = 0; i < 4; i++)
    {
        big *= 1000000000;
    }
    assert(polya::burnside<__int128>(id, 1000000000) == big);
    assert(young_tab::youngTab({2, 1}) == 2);
    assert(young_tab::youngTab({10, 10}) == 16796);
    assert(young_tab::youngTab({200}) == 1);

    vector<int> p(20);
    iota(p.begin(), p.end(), 0);
    assert(cantor(p) == 0);
    unsigned long long fac = 1;
    for (int i = 1; i <= 20; i++)
    {
        fac *= i;
    }
    reverse(p.begin(), p.end());
    assert(cantor(p) == fac - 1);
    assert(invCantor(20, fac - 1) == p);
    assert((pythagorean(30) == vector<PTriple>{{3, 4, 5}, {5, 12, 13}, {8, 15, 17}, {7, 24, 25},
                                                {20, 21, 29}}));

    auto h = hadamard(8);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int dot = 0;
            for (int k = 0; k < 8; k++)
            {
                dot += h[i][k] * h[j][k];
            }
            assert(dot == (i == j ? 8 : 0));
        }
    }
    assert(magicSquare(2).empty());
    for (int n : {1, 3, 4, 6, 8, 10})
    {
        checkMagic(n);
    }

    cout << "OK " << extAcc << '\n';
    return 0;
}
