#include "../include/full/10_2_018.hpp"
#include "../include/full/10_2_019.hpp"

signed main()
{
    assert(productPolys({}) == Poly{1});
    assert((productPolys({Poly{1, 1}, Poly{1, -1}, Poly{2}}) == Poly{2, 0, -2}));
    assert((productPolys({Poly{2}, Poly{3}, Poly{4}}) == Poly{24}));
    assert((productPolys({Poly{0}, Poly{1, 2}, Poly{3, 4}}) == Poly(3)));

    mt19937_64 rng(0);
    for (int n = 1; n <= 128; n++)
    {
        Poly f(n);
        vector<Z> p(n);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        for (auto &x : p)
        {
            x = rng() % 8;
        }
        auto b = monoToNewton(f, p);
        Poly got(n);
        Poly base{1};
        for (int i = 0; i < n; i++)
        {
            got += base * b[i];
            base = (base * Poly{-p[i], 1}).cut(n);
        }
        assert(got.cut(n) == f);
    }
    cout << "OK\n";
    return 0;
}
