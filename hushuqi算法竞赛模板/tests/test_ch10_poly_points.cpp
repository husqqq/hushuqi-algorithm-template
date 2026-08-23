#include "../include/full/10_2_008.hpp"
#include "../include/full/10_2_009.hpp"

signed main()
{
    Poly f{1, 2, 1};
    vector<Z> x{0, 1, 2};
    vector<Z> y{1, 4, 9};
    PolyTree tr(x);
    assert(tr.eval(f) == y);
    assert(multiEval(f, x) == y);
    assert(tr.eval(Poly{1, 2, 1, 0, 0}) == y);
    assert(interpPoints(x, y) == f);
    assert(lagrangeSeq(y, 10) == Z(121));
    assert((taylorShift(f, 4) == Poly{25, 10, 1}));
    assert((batchInv({Z(2), Z(0), Z(4)}) == vector<Z>{Z(2).inv(), 0, Z(4).inv()}));
    assert((shiftSamples(y, 3, 4) == vector<Z>{16, 25, 36, 49}));
    assert((shiftSamples(y, mod - 2, 4) == vector<Z>{1, 0, 1, 4}));
    assert(shiftSamples(y, mod, 0).empty());
    assert(lagrangeSeq(y, numeric_limits<long long>::min()) ==
           Z(numeric_limits<long long>::min()) * Z(numeric_limits<long long>::min()) +
           Z(2) * Z(numeric_limits<long long>::min()) + Z(1));
    assert((shiftSamples(y, numeric_limits<long long>::max(), 3) ==
           vector<Z>{Z(numeric_limits<long long>::max()).pow(2) +
                         Z(2) * Z(numeric_limits<long long>::max()) + Z(1),
                     (Z(numeric_limits<long long>::max()) + Z(1)).pow(2) +
                         Z(2) * (Z(numeric_limits<long long>::max()) + Z(1)) + Z(1),
                     (Z(numeric_limits<long long>::max()) + Z(2)).pow(2) +
                         Z(2) * (Z(numeric_limits<long long>::max()) + Z(2)) + Z(1)}));

    mt19937_64 rng(20260816);
    for (int n : {32LL, 64LL, 128LL, 129LL, 257LL})
    {
        Poly a(n);
        vector<Z> p(n);
        for (auto &v : a)
        {
            v = rng() % mod;
        }
        for (auto &v : p)
        {
            v = rng() % mod;
        }
        auto got = PolyTree(p).eval(a);
        assert(multiEval(a, p) == got);
        for (int i = 0; i < n; i++)
        {
            Z want = 0;
            for (auto it = a.rbegin(); it != a.rend(); ++it)
            {
                want = want * p[i] + *it;
            }
            assert(got[i] == want);
        }
    }
    cout << "OK\n";
    return 0;
}
