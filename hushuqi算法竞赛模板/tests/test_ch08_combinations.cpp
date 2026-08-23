#include "../include/full/8_1_002.hpp"
#include "../include/full/8_1_003.hpp"
#include "../include/full/8_1_004.hpp"
#include "../include/full/8_1_005.hpp"
#include "../include/full/8_1_006.hpp"
#include "../include/full/8_1_009.hpp"
#include "../include/full/8_1_010.hpp"
#include "../include/full/8_1_011.hpp"
#include "../include/full/8_2_010.hpp"

signed main()
{
    auto a = pascalTri<int>(5);
    assert((a[5] == vector<int>{1, 5, 10, 10, 5, 1}));
    assert(lucas(100, 20, 7) == 0);
    assert(lucas(100, 50, 7) == 4);
    assert(PrimeBinom(7, 100).C(100, 50) == 4);
    assert(QBinomPrime(7, 2, 6).C(6, 3) == 2);
    assert(BinomMod(72).C(20, 6) == 24);
    assert(binomAnyMod(10, 3, 12) == 0);
    assert(binomAnyMod(10, 4, 1000) == 210);

    auto c = [&](int n, int k)
    {
        return k < 0 || k > n ? 0LL : a[n][k];
    };
    assert(vandermonde(2, 3, 2, c) == c(5, 2));

    assert(roots_unity::rootFilter({-1, 2, 3, 4}, 2, 0) == 2);
    assert(roots_unity::rootFilter({-1, 2, 3, 4}, 2, 1) == 6);
    assert(balls_boxes::unlabBoxDist(5, 3, false) == 21);
    assert(balls_boxes::unlabBoxDist(5, 3, true) == 6);
    assert(balls_boxes::pigeonBound(numeric_limits<int>::max(), 2) ==
           numeric_limits<int>::max() / 2 + 1);

    assert((tree_count::cayley(1, 1000000007) == array<int, 2>{1, 1}));
    assert((tree_count::cayley(4, 1000000007) == array<int, 2>{16, 64}));
    cout << "OK\n";
    return 0;
}
