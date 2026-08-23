#include "../include/full/10_2_015.hpp"

bool irreducible(const Poly &f)
{
    int n = f.size() - 1;
    Poly x{0, 1}, y = polyMod(x, f);
    for (int k = 1; k <= n; k++)
    {
        y = polyPowMod(y, Z::mod(), f);
        if (2 * k <= n && polyGcd(f, y - x).size() > 1)
        {
            return false;
        }
    }
    Poly r = polyMod(y - x, f);
    r.norm();
    return r.empty();
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 120; tc++)
    {
        int n = rng() % 11 + 1;
        Poly f(n + 1);
        for (auto &x : f)
        {
            x = rng() % 1000;
        }
        if (f.back() == Z(0))
        {
            f.back() = 1;
        }
        auto fac = factorPoly(f, tc);
        Poly prod{1};
        for (auto [g, k] : fac)
        {
            assert(irreducible(g));
            for (int i = 0; i < k; i++)
            {
                prod *= g;
            }
            agg = agg * 1000000007ULL + g.size() * 31 + k;
            for (auto x : g)
            {
                agg = agg * 1000000007ULL + x.val();
            }
        }
        prod.norm();
        assert(prod == polyMonic(f));
    }
    for (int tc = 0; tc < 100; tc++)
    {
        int n = rng() % 8 + 1;
        vector<Z> want;
        Poly f{1};
        for (int i = 0; i < n; i++)
        {
            Z r = rng() % 1000;
            want.push_back(r);
            f *= Poly{-r, 1};
        }
        sort(want.begin(), want.end(), [](Z a, Z b) { return a.val() < b.val(); });
        assert(finiteRoots(f, tc + 1000) == want);
    }
    cout << agg << '\n';
    return 0;
}
