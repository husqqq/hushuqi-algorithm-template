#include "../include/full/10_2_002.hpp"
#include "../include/full/10_2_005.hpp"
#include "../include/full/10_2_006.hpp"

signed main()
{
#ifdef TEMPLATE_PRINT_VARIANT
    static_assert(sizeof(Z) == sizeof(int));
#else
    static_assert(sizeof(Z) == 4);
#endif
    auto [q, r] = polyDivmod({1, 0, 1}, {1, 1});
    assert((q == Poly{-1, 1} && r == Poly{2}));
    assert(fpsInv({}, 0).empty());
    assert(fpsInvFast({}, 0).empty());
    assert((fpsInv({1, 1}, 6) == Poly{1, -1, 1, -1, 1, -1}));
    assert((fpsInvFast({1, 1}, 6) == Poly{1, -1, 1, -1, 1, -1}));
    Poly constant = fpsInvFast({2}, 65);
    assert(constant[0] == Z(2).inv());
    for (int i = 1; i < 65; i++)
    {
        assert(constant[i] == Z(0));
    }
    assert(deriv({}).empty());
    assert((integr({1, 2}) == Poly{0, 1, 1}));
    Poly sq{1, 2, 3};
    sq *= sq;
    assert((sq == Poly{1, 4, 10, 12, 9}));

    int n = 6;
    Poly h{1, 2, 3, 4, 5, 6};
    assert(fpsExp(fpsLog(h, n), n) == h);
    assert(fpsPow(h, 3, n) == (h * h * h).cut(n));
    assert(fpsPow(Poly{0, 1}, numeric_limits<unsigned long long>::max(), 4) == Poly(4));
    Poly pre(65);
    pre[0] = 1;
    for (int i = 1; i < (int)pre.size(); i++)
    {
        pre[i] = i * i + 3;
    }
    Poly tail = pre;
    tail.resize(NTT_LIM);
    tail.back() = 7;
    assert(fpsLog(tail, pre.size()) == fpsLog(pre, pre.size()));
    assert(fpsPow(tail, 3, pre.size()) == fpsPow(pre, 3, pre.size()));
    tail.clear();
    tail.shrink_to_fit();
    auto s = fpsSqrt((h * h).cut(n), n, 1);
    assert(s && *s == h);
    auto t = fpsSqrt((h * h).cut(n), n);
    assert(t && ((*t * *t).cut(n) == (h * h).cut(n)));
    assert(!fpsSqrt(Poly{0, 1}, 2, 1));
    assert((sparseDiv({1}, {{0, 1}, {2, -1}}, 8) ==
            Poly{1, 0, 1, 0, 1, 0, 1, 0}));
    assert((sparseDiv({2}, {{2, 3}, {0, 1}, {0, 1}, {2, -3}}, 4) ==
            Poly{1, 0, 0, 0}));
    assert((sparseDiv({1}, {{0, 1}, {1, 2}, {1, -2}, {9, 7}}, 4) ==
            Poly{1, 0, 0, 0}));

    mt19937_64 rng(0);
    for (int m : {2, 64, 256})
    {
        Poly b(m), q(300), r(m - 1);
        for (auto &x : b)
        {
            x = rng() % mod;
        }
        for (auto &x : q)
        {
            x = rng() % mod;
        }
        for (auto &x : r)
        {
            x = rng() % mod;
        }
        b.back() = q.back() = 1;
        auto [gotQ, gotR] = polyDivmod(b * q + r, b);
        assert(gotQ == q && gotR == r);
    }
    for (int len : {65, 127, 128, 129, 257})
    {
        int m = len / 2 + 1;
        Poly a(len), b(m), want(len - m + 1);
        for (auto &x : a)
        {
            x = rng() % mod;
        }
        for (auto &x : b)
        {
            x = rng() % mod;
        }
        for (int i = 0; i < (int)want.size(); i++)
        {
            for (int j = 0; j < m; j++)
            {
                want[i] += b[j] * a[i + j];
            }
        }
        assert(midProd(a, b) == want);
    }
    for (int m : {64, 65, 127, 128, 129, 511, 512, 513, 4096})
    {
        Poly a(m);
        a[0] = rng() % (mod - 1) + 1;
        for (int i = 1; i < m; i++)
        {
            a[i] = rng() % mod;
        }
        Poly iv = fpsInvFast(a, m);
        assert(iv == fpsInv(a, m));
        Poly one = (a * iv).cut(m);
        assert(one[0] == Z(1));
        for (int i = 1; i < m; i++)
        {
            assert(one[i] == Z(0));
        }
    }
    cout << "OK\n";
    return 0;
}
