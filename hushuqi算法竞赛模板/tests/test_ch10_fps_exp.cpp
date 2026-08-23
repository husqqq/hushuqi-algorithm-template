#include "../include/full/10_2_005.hpp"

Poly expNaive(const Poly &a, int n)
{
    Poly f(n);
    if (n == 0)
    {
        return f;
    }
    f[0] = 1;
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j <= i && j < (int)a.size(); j++)
        {
            f[i] += Z(j) * a[j] * f[i - j];
        }
        f[i] /= Z(i);
    }
    return f;
}

Poly randomSeries(int size, mt19937_64 &rng)
{
    Poly a(size);
    for (auto &x : a)
    {
        x = rng() % mod;
    }
    if (!a.empty())
    {
        a[0] = 0;
    }
    return a;
}

void requireEqual(const Poly &got, const Poly &want, int n, const char *kind)
{
    if (got == want)
    {
        return;
    }
    cerr << kind << " mismatch at n=" << n << '\n';
    int m = min(got.size(), want.size());
    for (int i = 0; i < m; i++)
    {
        if (got[i] != want[i])
        {
            cerr << "index=" << i << " got=" << got[i].val()
                 << " want=" << want[i].val() << '\n';
            break;
        }
    }
    abort();
}

void checkNaive(const Poly &a, int n)
{
    Poly want = expNaive(a, n);
    requireEqual(fpsExpRelax(a, n), want, n, "relaxed/naive");
    requireEqual(fpsExpNewton(a, n), want, n, "newton/naive");
    requireEqual(fpsExp(a, n), want, n, "default/naive");
}

void checkNewton(const Poly &a, int n)
{
    Poly want = fpsExpNewton(a, n);
    requireEqual(fpsExpRelax(a, n), want, n, "relaxed/newton");
    requireEqual(fpsExp(a, n), want, n, "default/newton");
}

void checkDifferentialIdentity(const Poly &a, int n)
{
    Poly f = fpsExpRelax(a, n);
    Poly lhs = deriv(f);
    Poly rhs = (f * deriv(a.cut(n))).cut(max<int>(0, n - 1));
    requireEqual(lhs, rhs, n, "differential identity");
    requireEqual(fpsExp(a, n), f, n, "default/relaxed");
}

signed main()
{
    mt19937_64 rng(20260816);

    checkNaive({}, 0);
    checkNaive({}, 1);
    checkNaive({0}, 1);
    checkNaive({0, 7, 11}, 1);
    checkNaive({}, 65);

    Poly x(130);
    x[1] = 1;
    Poly ex = fpsExpRelax(x, x.size());
    Z fac = 1;
    for (int i = 0; i < (int)ex.size(); i++)
    {
        if (i)
        {
            fac *= i;
        }
        assert(ex[i] * fac == Z(1));
    }

    for (int n : {2, 3, 31, 32, 33, 63, 64, 65, 127, 128, 129,
                  257, 511, 512, 513, 1000, 1023, 1024, 1025})
    {
        checkNaive(randomSeries(n + 7, rng), n);
        checkNaive(randomSeries(max<int>(0, n / 3), rng), n);
    }

    for (int tc = 0; tc < 120; tc++)
    {
        int n = rng() % 301;
        int stored = rng() % (n + 21);
        checkNaive(randomSeries(stored, rng), n);
    }

    for (int n : {12345, 16383, 16384, 16385, 100000})
    {
        checkNewton(randomSeries(n, rng), n);
    }

    for (int n : {262143, 262144, 262145, 524289})
    {
        checkDifferentialIdentity(randomSeries(n, rng), n);
    }

    Poly largeA = randomSeries(16385, rng);
    Poly smallA = randomSeries(37, rng);
    Poly largeWant = fpsExpRelax(largeA, largeA.size());
    Poly smallWant = fpsExpRelax(smallA, smallA.size());
    requireEqual(fpsExpRelax(largeA, largeA.size()), largeWant,
                 largeA.size(), "repeat large");
    requireEqual(fpsExpRelax({}, 257), Poly({1}).cut(257), 257, "repeat zero");
    requireEqual(fpsExpRelax(smallA, smallA.size()), smallWant,
                 smallA.size(), "repeat small");
    requireEqual(fpsExpRelax(largeA, largeA.size()), largeWant,
                 largeA.size(), "repeat large again");

    cout << "OK\n";
    return 0;
}
