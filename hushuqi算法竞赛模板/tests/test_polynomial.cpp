#include "../include/polynomial.hpp"

Z evalNaive(const Poly &a, Z x)
{
    // a 是多项式，x 是求值点；返回 Horner 求值结果。
    Z ans = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        ans = ans * x + a[i];
    }
    return ans;
}

signed main()
{
    mt19937 rng(20260723);
    for (int n = 1; n <= 80; n++)
    {
        Poly a(n), b(n);
        for (auto &x : a) x = rng() % mod;
        for (auto &x : b) x = rng() % mod;

        auto c = a * b;
        for (int i = 0; i < (int)c.size(); i++)
        {
            Z ans = 0;
            for (int j = 0; j < n; j++)
            {
                if (0 <= i - j && i - j < n) ans += a[j] * b[i - j];
            }
            assert(c[i] == ans);
        }

        if (a[0] == Z(0)) a[0] = 1;
        auto iv = fpsInv(a, n);
        auto ivFast = fpsInvFast(a, n);
        assert(ivFast == iv);
        auto one = (a * iv).cut(n);
        assert(one[0] == Z(1));
        for (int i = 1; i < n; i++) assert(one[i] == Z(0));

        b.back() = 1;
        auto [q, r] = polyDivmod(a, b);
        auto z = b * q + r;
        z.norm(), a.norm();
        assert(z == a && (r.empty() || r.size() < b.size()));

        Poly f(n);
        f[0] = 0;
        for (int i = 1; i < n; i++) f[i] = rng() % mod;
        assert(fpsLog(fpsExp(f, n), n) == f);

        Poly g(n);
        g[0] = rng() % (mod - 1) + 1;
        for (int i = 1; i < n; i++) g[i] = rng() % mod;
        auto sq = (g * g).cut(n);
        auto rt = fpsSqrt(sq, n, g[0]);
        assert(rt && ((*rt * *rt).cut(n) == sq));

        vector<Z> x(n);
        for (int i = 0; i < n; i++) x[i] = i + 1;
        PolyTree tr(x);
        auto y = tr.eval(a);
        assert(multiEval(a, x) == y);
        for (int i = 0; i < n; i++) assert(y[i] == evalNaive(a, x[i]));
        assert(interpPoints(x, y) == a.cut(n));

        Z sh = rng() % mod;
        auto h = taylorShift(a, sh);
        for (int i = 0; i < 20; i++)
        {
            Z x0 = rng() % mod;
            assert(evalNaive(h, x0) == evalNaive(a, x0 + sh));
        }
    }

    Poly a{1};
    vector<pair<int, Z>> b{{0, 1}, {2, -1}};
    assert(sparseDiv(a, b, 8) == Poly({1, 0, 1, 0, 1, 0, 1, 0}));
    assert(shiftSamples({1, 4, 9}, 3, 4) == vector<Z>({16, 25, 36, 49}));

    for (int n : {63, 64, 65, 127, 128, 129, 255, 256, 257, 1000, 4096})
    {
        Poly f(n);
        f[0] = rng() % (mod - 1) + 1;
        for (int i = 1; i < n; i++)
        {
            f[i] = rng() % mod;
        }
        Poly iv = fpsInvFast(f, n);
        assert(iv == fpsInv(f, n));
        Poly one = (f * iv).cut(n);
        assert(one[0] == Z(1));
        for (int i = 1; i < n; i++)
        {
            assert(one[i] == Z(0));
        }
    }
}
