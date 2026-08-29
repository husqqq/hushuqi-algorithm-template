#include "../include/full/10_1_003.hpp"
#include "../include/full/10_1_004.hpp"

signed main()
{
    assert(nttFit(0, NTT_LIM + 100));
    assert(nttFit(1, NTT_LIM));
    assert(!nttFit(33, NTT_LIM));
    assert(convFit(33, NTT_LIM));
    assert(convFit(65, NTT_LIM - 62));
    assert(!convFit(65, NTT_LIM - 61));

    vector<Z> a{1, -2, 3, -4, 5};
    vector<Z> b{6, 7, -8, 9, -10};
    vector<Z> want(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            want[i + j] += a[i] * b[j];
        }
    }
    assert(conv(a, b) == want);
    assert(plain_ntt::conv(a, b) == want);

    vector<Z> e(65), f(66);
    for (int i = 0; i < (int)e.size(); i++)
    {
        e[i] = i * i + 3;
    }
    for (int i = 0; i < (int)f.size(); i++)
    {
        f[i] = 2 * i + 5;
    }
    vector<Z> ew(e.size() + f.size() - 1);
    for (int i = 0; i < (int)e.size(); i++)
    {
        for (int j = 0; j < (int)f.size(); j++)
        {
            ew[i + j] += e[i] * f[j];
        }
    }
    assert(conv(e, f) == ew); // 结果长 130，覆盖端点拆分的 +2 分支。

    vector<Z> rt(1024);
    for (int i = 0; i < (int)rt.size(); i++)
    {
        rt[i] = i * i * i + 7 * i + 11;
    }
    auto old = rt;
    dft(rt);
    idft(rt);
    assert(rt == old);

#ifdef TEMPLATE_PRINT_VARIANT
    vector<int> u(16), v(16);
#else
    vector<uint32_t> u(16), v(16);
#endif
    for (int i = 0; i < (int)a.size(); i++)
    {
        u[i] = a[i].val();
        v[i] = b[i].val();
    }
    NTT32 nt(16);
    nt.dft(u.data(), u.size());
    nt.dft(v.data(), v.size());
    for (int i = 0; i < (int)u.size(); i++)
    {
        u[i] = (uint64_t)u[i] * v[i] % mod;
    }
    nt.idft(u.data(), u.size());
    for (int i = 0; i < (int)want.size(); i++)
    {
        assert(u[i] == (uint32_t)want[i].val());
    }
    for (int i = want.size(); i < (int)u.size(); i++)
    {
        assert(u[i] == 0);
    }

    vector<long double> x{1, -2, 3, -4, 5};
    vector<long double> y{6, 7, -8, 9, -10};
    auto z = convolution(x, y);
    for (int i = 0; i < (int)z.size(); i++)
    {
        long long w = (long long)want[i].val();
        assert(llroundl(z[i]) == w - (w > mod / 2 ? mod : 0));
    }

    assert((convMod({-1, 2}, {3, -4}, 5) == vector<long long>{2, 0, 2}));
    vector<long long> ca(101), cb(103), cw(203);
    for (int i = 0; i < (int)ca.size(); i++) ca[i] = i * i % 1000000007;
    for (int i = 0; i < (int)cb.size(); i++) cb[i] = (3 * i + 1) % 1000000007;
    for (int i = 0; i < (int)ca.size(); i++)
    {
        for (int j = 0; j < (int)cb.size(); j++)
        {
            cw[i + j] = (cw[i + j] + ca[i] * cb[j]) % 1000000007;
        }
    }
    assert(convNTTMod(ca, cb, 1000000007) == cw);
    assert(convNaiveMod({}, {1}, 5).empty());
    assert(convNaiveMod({1}, {}, 5).empty());
    long long p = numeric_limits<long long>::max();
    assert(convMod({p - 1}, {p - 1}, p)[0] == 1);
    cout << "OK\n";
    return 0;
}
