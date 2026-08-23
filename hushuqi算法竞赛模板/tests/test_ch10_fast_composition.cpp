#include "../include/full/10_2_011.hpp"
#include "../include/full/10_2_012.hpp"
#include "../include/full/10_3_005.hpp"

Poly naiveCompose(const Poly &f, const Poly &g, int n)
{
    Poly ans(n);
    for (int i = (int)f.size() - 1; i >= 0; i--)
    {
        ans = (ans * g).cut(n);
        ans[0] += f[i];
    }
    return ans;
}

vector<Z> naivePowerProjection(const Poly &f, const vector<Z> &w, int m)
{
    vector<Z> ans(m);
    Poly p{1};
    Poly ff = f.cut(w.size());
    for (int k = 0; k < m; k++)
    {
        for (int i = 0; i < min((int)w.size(), (int)p.size()); i++)
        {
            ans[k] += w[i] * p[i];
        }
        p = (p * ff).cut(w.size());
    }
    return ans;
}

Poly naiveCompositionalInverse(const Poly &f, int n)
{
    Poly g(n);
    if (n <= 1)
    {
        return g;
    }
    Z iv = f[1].inv();
    g[1] = iv;
    for (int k = 2; k < n; k++)
    {
        Z known = naiveCompose(f, g, k + 1)[k];
        g[k] = -known * iv;
    }
    return g;
}

signed main()
{
    mt19937_64 rng(0x6b696e6f73686974ULL);

    for (int lg = 1; lg <= 12; lg++)
    {
        int n = 1LL << lg;
        Poly a(n), q(n);
        for (int i = 0; i < n / 2; i++)
        {
            a[i] = rng() % mod;
        }
        Poly orig = a;
        auto b = a;
        dft(b.data(), n);
        for (int i = 0; i < n / 2; i++)
        {
            q[i] = b[2 * i] * b[2 * i + 1];
        }
        idft(q.data(), n / 2);
        Poly neg(a.begin(), a.begin() + n / 2);
        a.resize(n / 2);
        for (int i = 1; i < n / 2; i += 2)
        {
            neg[i] = -neg[i];
        }
        auto c = conv(a, neg);
        for (int i = 0; i < n / 2; i++)
        {
            assert(q[i] == (2 * i < (int)c.size() ? c[2 * i] : Z(0)));
        }
        b = orig;
        dft(b.data(), n);
        idft(b.data(), n);
        assert(orig == b);
    }

    assert(composePoly({}, {1, 2}, 40).empty());
    assert(composePoly({3, 4}, {}, 40) == Poly{3});
    assert(composePoly({1, 2, 3}, {4}, 40) == Poly{57});
    Poly longConstant(1000);
    for (auto &x : longConstant)
    {
        x = rng() % mod;
    }
    Z constantValue = 0;
    for (auto it = longConstant.rbegin(); it != longConstant.rend(); ++it)
    {
        constantValue = constantValue * Z(7) + *it;
    }
    assert(composePoly(longConstant, {7}, 40) == Poly{constantValue});
    for (int tc = 0; tc < 250; tc++)
    {
        int n = 33 + rng() % 96;
        int nf = 1 + rng() % (n + 20);
        int ng = rng() % (n + 10);
        Poly f(nf), g(ng);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        for (auto &x : g)
        {
            x = rng() % mod;
        }
        auto got = composePoly(f, g, n).cut(n);
        auto want = naiveCompose(f, g, n);
        assert(got == want);
    }

    assert(powerProj({}, {}, 10) == vector<Z>(10));
    for (int tc = 0; tc < 250; tc++)
    {
        int n = 33 + rng() % 80;
        int m = 33 + rng() % 80;
        Poly f(n);
        vector<Z> w(n);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        for (auto &x : w)
        {
            x = rng() % mod;
        }
        assert(powerProj(f, w, m) == naivePowerProjection(f, w, m));
    }

    assert(compInverse({0, 7}, 0).empty());
    assert(compInverse({}, 1) == Poly(1));
    assert(compInverse({0, 7}, 1) == Poly(1));
    for (int tc = 0; tc < 40; tc++)
    {
        int n = 33 + rng() % 20;
        Poly f(n);
        f[1] = 1 + rng() % (mod - 1);
        for (int i = 2; i < n; i++)
        {
            f[i] = rng() % mod;
        }
        Poly got = compInverse(f, n);
        Poly want = naiveCompositionalInverse(f, n);
        assert(got == want);
        Poly identity(n);
        identity[1] = 1;
        assert(naiveCompose(f, got, n) == identity);
        assert(naiveCompose(got, f, n) == identity);
    }

    cout << "OK\n";
    return 0;
}
