#include "../include/full/10_2_005.hpp"
#include "../include/full/10_1_013.hpp"
#include "../include/full/10_2_010.hpp"
#include "../include/full/10_2_011.hpp"
#include "../include/full/10_2_012.hpp"
#include "../include/full/10_2_013.hpp"
#include "../include/full/10_2_014.hpp"

signed main()
{
    assert(fastFact(0) == Z(1));
    assert(fastFact(10) == Z(3628800));
    assert(fastFact(mod - 1) == Z(-1));
    assert((factBatch({5, 0, 3}) == vector<Z>{120, 1, 6}));
    {
        vector<int> q;
        for (int i = 0; i <= 5000; i += 7) q.push_back(i);
        auto got = factBatch(q);
        Z fac = 1;
        int at = 0;
        for (int i = 0; i <= 5000; i++)
        {
            if (i) fac *= i;
            if (at < (int)q.size() && q[at] == i) assert(got[at++] == fac);
        }
    }
    {
        vector<int> q;
        for (int x : {32767, 32768, 32769, 65535, 65536, 65537}) q.push_back(x);
        mt19937 rng(20260822);
        for (int i = 0; i < 200; i++)
        {
            int n = rng() % mod;
            q.push_back(n);
            q.push_back(mod - 1 - n);
        }
        auto got = factBatch(q);
        for (int i = 6; i < (int)q.size(); i += 2)
        {
            int m = q[i + 1];
            assert(got[i] * got[i + 1] == (m & 1 ? Z(1) : Z(-1)));
        }
    }
    assert((sharpSubSum({1, 1, 2}, 4) == vector<Z>{1, 2, 2, 2, 1}));
    assert((powerProj(Poly{1, 1}, {2, 3, 5, 7}, 4) ==
            vector<Z>{2, 5, 13, 33}));
    assert(composePoly({1, 2, 3}, {4, 5}, 0).empty());
    assert((composePoly({1, 2, 3}, {4, 5}, 4) == Poly{57, 130, 75}));
    assert((fallMul({0, 1}, {0, 1}) == vector<Z>{0, 1, 1}));

    vector<pair<int, Z>> f{{0, 1}, {1, 1}};
    vector<pair<int, Z>> e{{1, 1}};
    assert(sparseExp(6, e) == vector<Z>(fpsExp({0, 1}, 6)));
    assert(sparseLog(6, f) == vector<Z>(fpsLog({1, 1}, 6)));
    assert(sparsePow(6, f, 5) == vector<Z>(fpsPow({1, 1}, 5, 6)));
    assert((sparseInv(6, {{0, 1}, {2, -1}}) ==
            vector<Z>{1, 0, 1, 0, 1, 0}));
    assert((sparsePow(8, {{2, 3}, {3, 1}}, 2) ==
            vector<Z>{0, 0, 0, 0, 9, 6, 1, 0}));
    auto sq = sparseSqrt(8, {{2, 4}, {3, 4}, {4, 1}});
    assert(sq);
    assert(((Poly(*sq) * Poly(*sq)).cut(8) == Poly{0, 0, 4, 4, 1, 0, 0, 0}));
    long long k = numeric_limits<long long>::max();
    auto p = sparsePow(3, f, k);
    Z z = Z(k % mod);
    assert(p[1] == z && p[2] == z * (z - Z(1)) / Z(2));
    cout << "OK\n";
    return 0;
}
