#include "../include/full/10_3_001.hpp"
#include "../include/full/10_3_002.hpp"
#include "../include/full/10_3_003.hpp"
#include "../include/full/10_3_004.hpp"
#include "../include/full/10_3_005.hpp"

signed main()
{
    using ogf::ogfProduct;
    using egf::egfProduct;
    using gauss_binom::qBinomial;
    using binom_trans::binomTrans;
    using lagrange::lagrangeCoef;

    assert((ogfProduct({1, 2}, {3, 4}) == vector<int>{3, 10, 8}));
    assert((egfProduct({1, 2}, {3, 4}) == vector<int>{3, 10, 16}));
    assert((qBinomial(4, 2) == vector<int>{1, 1, 2, 1, 1}));
    vector<int> a{1, 2, 3, 4};
    auto b = binomTrans(a);
    assert((b == vector<int>{1, 3, 8, 20}));
    assert(binomTrans(b, true) == a);
    assert(lagrangeCoef({1, 2, 1}, 1) == 1);
    assert(lagrangeCoef({1, 2, 1}, 4) == 14);
    cout << "OK\n";
    return 0;
}
