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
    mt19937 rng(0);
    for (int n = 1; n <= 32; n <<= 1)
    {
        vector<FWTInt> p(n), q(n);
        for (auto &v : p) v = rng() % mod;
        for (auto &v : q) v = rng() % mod;
        vector<Z> pz(p.begin(), p.end()), qz(q.begin(), q.end());
        auto check = [&](BitOp op, const vector<FWTInt> &got)
        {
            auto want = bitConv(pz, qz, op);
            for (int i = 0; i < n; i++)
            {
                assert(got[i] == want[i].val());
            }
        };
        check(BitOp::Or, bitConvMod<mod, BitOp::Or>(p, q));
        check(BitOp::And, bitConvMod<mod, BitOp::And>(p, q));
        check(BitOp::Xor, bitConvMod<mod, BitOp::Xor>(p, q));
        auto z = p;
        fwtMod<mod, BitOp::Xor>(z);
        fwtMod<mod, BitOp::Xor, true>(z);
        assert(z == p);
    }
    cout << "OK\n";
    return 0;
}
