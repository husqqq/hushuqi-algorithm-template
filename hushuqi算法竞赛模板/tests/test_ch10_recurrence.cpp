#include "../include/full/10_4_002.hpp"
#include "../include/full/10_4_005.hpp"

signed main()
{
    assert((BM<Z>({0, 1, 1, 2, 3, 5, 8, 13}) ==
            vector<Z>{1, 1}));
    assert((BM<Z>({7, 7, 7, 7}) == vector<Z>{1}));
    assert(BM<Z>({0, 0, 0, 0}).empty());
    assert((shortRel(vector<Z>{0, 1, 1, 2, 3, 5}) == vector<Z>{1, -1, -1}));

    assert(linearRec<long long>({0, 1}, {1, 1}, 50) == 12586269025LL);
    assert(linearRec<long long>({0, 1, 999}, {1, 1}, 2) == 1);
    auto [p, q] = recFraction({0, 1}, {1, 1});
    assert((p == Poly{0, 1} && q == Poly{1, -1, -1}));
    for (int n = 0; n <= 30; n++)
    {
        assert(bostanMori(p, q, n) == linearRec<Z>({0, 1}, {1, 1}, n));
    }
    cout << "OK\n";
    return 0;
}
