#include "../include/full/10_2_020.hpp"

Poly gcdNaive(Poly a, Poly b)
{
    // a、b 是测试用多项式；返回普通欧几里得算法得到的首一 gcd。
    a.norm();
    b.norm();
    while (!b.empty())
    {
        Poly r = polyDivmod(a, b).second;
        a = move(b);
        b = move(r);
    }
    if (!a.empty())
    {
        a *= a.back().inv();
    }
    return a;
}

void checkHalfGcd(const Poly &f, const Poly &g)
{
    // f、g 是不全为零的测试多项式；核对 gcd、Bézout 恒等式和模逆存在性。
    Poly want = gcdNaive(f, g);
    assert(polyGcd(f, g) == want);
    auto [d, x, y] = polyExtGcd(f, g);
    Poly lhs = x * f + y * g;
    lhs.norm();
    d.norm();
    assert(lhs == d);
    if (g.size() > 1)
    {
        auto inv = polyInvMod(f, g);
        assert((bool)inv == (want.size() == 1));
        if (inv)
        {
            assert(polyDivmod(f * *inv, g).second == Poly{1});
        }
    }
}

signed main()
{
    assert((polyGcd({1, 1}, {2, 1}) == Poly{1}));
    assert((polyGcd({1, 1}, {1, 1}) == Poly{1, 1}));
    assert((polyGcd({2, 3, 1}, {3, 4, 1}) == Poly{1, 1}));
    assert((polyGcd({2}, {3}) == Poly{1}));

    auto x = polyInvMod({0, 1}, {1, 0, 1});
    assert((x && *x == Poly{0, -1}));
    assert(!polyInvMod({1, 1}, {1, 2, 1}));
    auto z = polyInvMod({1, 2, 3}, {7});
    assert(z && z->empty());
    checkHalfGcd({}, {1});
    checkHalfGcd({1}, {});
    checkHalfGcd({1, 1}, {1, 1});
    checkHalfGcd({2, 3, 1}, {3, 4, 1});

    mt19937_64 rng(0);
    for (int n = 1; n <= 256; n++)
    {
        Poly f(n), g(n);
        for (auto &v : f)
        {
            v = rng() % mod;
        }
        for (auto &v : g)
        {
            v = rng() % mod;
        }
        f.back() = 1;
        g.back() = 1;
        assert(polyGcd(f, g) == gcdNaive(f, g));
    }
    for (int n = 2; n <= 256; n++)
    {
        Poly f(n), g(n + 1);
        for (auto &v : f)
        {
            v = rng() % mod;
        }
        for (auto &v : g)
        {
            v = rng() % mod;
        }
        g.back() = 1;
        auto [d, a, b] = polyExtGcd(f, g);
        Poly lhs = a * f + b * g;
        lhs.norm();
        d.norm();
        assert(lhs == d);
        bool coprime = gcdNaive(f, g).size() == 1;
        auto inv = polyInvMod(f, g);
        assert((bool)inv == coprime);
        if (inv)
        {
            assert(polyDivmod(f * *inv, g).second == Poly{1});
        }
    }
    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 512 + 1;
        int m = rng() % 512 + 1;
        Poly f(n), g(m);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        for (auto &x : g)
        {
            x = rng() % mod;
        }
        f.back() = rng() % (mod - 1) + 1;
        g.back() = rng() % (mod - 1) + 1;
        checkHalfGcd(f, g);
        if (tc % 4 == 0)
        {
            Poly h(rng() % 16 + 1);
            for (auto &x : h)
            {
                x = rng() % mod;
            }
            h.back() = 1;
            checkHalfGcd(f * h, g * h);
        }
    }
    for (int n : {513, 1024, 2048, 4096})
    {
        Poly f(n), g(n);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        for (auto &x : g)
        {
            x = rng() % mod;
        }
        f.back() = 1;
        g.back() = 1;
        checkHalfGcd(f, g);
    }
    cout << "OK\n";
    return 0;
}
