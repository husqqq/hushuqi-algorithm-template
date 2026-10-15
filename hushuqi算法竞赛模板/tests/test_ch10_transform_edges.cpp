#include "../include/full/10_1_008.hpp"
#include "../include/full/10_1_011.hpp"

#ifdef TEMPLATE_PRINT_VARIANT
using FWTInt = int;
#else
using FWTInt = uint32_t;
#endif

signed main()
{
    vector<Z> a{1, 2, 3, 4, 5, 6, 7};
    vector<Z> b{7, 6, 5, 4, 3, 2, 1};
    vector<Z> want(7);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            want[(i + j) % 7] += a[i] * b[j];
        }
    }
    auto x = a;
    auto y = b;
    kFWT(x, 7);
    kFWT(y, 7);
    for (int i = 0; i < 7; i++)
    {
        x[i] *= y[i];
    }
    kFWT(x, 7, true);
    assert(x == want);
    vector<Z> one{7};
    kFWT(one, 7);
    kFWT(one, 7, true);
    assert(one == vector<Z>{7});
    orFWT(one);
    orFWT<true>(one);
    assert(one == vector<Z>{7});
    vector<Z> rows{1, 10, 2, 20, 3, 30, 4, 40};
    auto raw = rows;
    orFWT(rows.data(), 4, 2);
    orFWT(rows.data(), 4, 2, true);
    assert(rows == raw);
    assert((subsetConv(vector<Z>{1, 2, 3, 4}, vector<Z>{5, 6, 7, 8}) ==
            vector<Z>{5, 16, 22, 60}));
    vector<Z> f{1, 2, 3, 4};
    auto r = rankedZeta(f);
    assert(r(0, 0) == 1 && r(1, 1) == 2 && r(2, 1) == 3);
    r(3, 1) += 1;
    assert(r(3, 1) == 6);
    r(3, 1) -= 1;
    const auto &cr = r;
    assert(cr(3, 0) == 1 && cr(3, 1) == 5 && cr(3, 2) == 4);
    assert(rankedMobius(move(r)) == f);
    auto check = []<FWTInt P>()
    {
        mt19937 rng(20261016);
        for (int n = 1; n <= 64; n <<= 1)
        {
            for (int rep = 0; rep < 12; rep++)
            {
                vector<FWTInt> p(n), q(n);
                for (int i = 0; i < n; i++)
                {
                    p[i] = rep == 0 ? P - 1 : rng() % P;
                    q[i] = rep == 1 ? 0 : rng() % P;
                }
                const auto oldp = p, oldq = q;
                auto verify = [&](BitOp op, const vector<FWTInt> &got)
                {
                    vector<FWTInt> want(n);
                    for (int i = 0; i < n; i++)
                    {
                        for (int j = 0; j < n; j++)
                        {
                            int k = op == BitOp::Or ? i | j : op == BitOp::And ? i & j : i ^ j;
                            want[k] = (want[k] + (unsigned long long)p[i] * q[j]) % P;
                        }
                    }
                    assert(got == want);
                    vector<long long> x(p.begin(), p.end()), y(q.begin(), q.end());
                    // 通用整数卷积只用小系数，避免近上界模数下的 Hadamard 乘积超出 long long。
                    if constexpr (P <= 15)
                    {
                        auto z = op == BitOp::Or ? orConv(x, y) : op == BitOp::And ? andConv(x, y) : xorConv(x, y);
                        assert(z == bitConv(x, y, op));
                        for (int i = 0; i < n; i++)
                        {
                            assert(z[i] % P == want[i]);
                        }
                        if (op == BitOp::Or)
                        {
                            orFwt(x);
                            orFwt(x, true);
                        }
                        if (op == BitOp::And)
                        {
                            andFwt(x);
                            andFwt(x, true);
                        }
                        if (op == BitOp::Xor)
                        {
                            xorFwt(x);
                            xorFwt(x, true);
                        }
                        assert(x == vector<long long>(p.begin(), p.end()));
                    }
                };
                verify(BitOp::Or, orConvMod<P>(p, q));
                verify(BitOp::And, andConvMod<P>(p, q));
                assert(orConvMod<P>(p, q) == (bitConvMod<P, BitOp::Or>(p, q)));
                assert(andConvMod<P>(p, q) == (bitConvMod<P, BitOp::And>(p, q)));
                auto x = p;
                orFwtMod<P>(x);
                orFwtMod<P, true>(x);
                assert(x == p);
                andFwtMod<P>(x);
                andFwtMod<P, true>(x);
                assert(x == p);
                fwtMod<P, BitOp::Or>(x.data(), n);
                fwtMod<P, BitOp::Or, true>(x.data(), n);
                assert(x == p);
                if constexpr (P & 1)
                {
                    verify(BitOp::Xor, xorConvMod<P>(p, q));
                    assert(xorConvMod<P>(p, q) == (bitConvMod<P, BitOp::Xor>(p, q)));
                    xorFwtMod<P>(x);
                    xorFwtMod<P, true>(x);
                    assert(x == p);
                }
                assert(p == oldp && q == oldq);
            }
        }
    };
    check.template operator()<2>();
    check.template operator()<10>();
    check.template operator()<15>();
    check.template operator()<998244353>();
    check.template operator()<2147483646>();
    check.template operator()<2147483647>();
    for (int n = 1; n <= 32; n <<= 1)
    {
        for (int d : {1, 2, 7})
        {
            vector<long long> a(n * d), want(n * d);
            iota(a.begin(), a.end(), -3);
            for (int s = 0; s < n; s++)
            {
                for (int t = 0; t < n; t++)
                {
                    if ((s & t) != t) continue;
                    for (int k = 0; k < d; k++)
                    {
                        want[s * d + k] += a[t * d + k];
                    }
                }
            }
            auto b = a;
            orFWT(b.data(), n, d, false);
            assert(b == want);
            orFWT<true>(b.data(), n, d);
            assert(b == a);
        }
    }
    cout << "OK\n";
    return 0;
}
