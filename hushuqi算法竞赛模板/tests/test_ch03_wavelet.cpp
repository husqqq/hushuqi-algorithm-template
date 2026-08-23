#include "../include/full/3_4_005.hpp"

signed main()
{
    Wavelet<9> a({0, 1, 255, 511, 511});
    assert(a.kth(0, 5, 0) == 0 && a.kth(0, 5, 4) == 511);
    assert(a.countLess(0, 5, 0) == 0);
    assert(a.countLess(0, 5, 512) == 5);
    assert(a.frequency(0, 5, 511) == 2);
    auto [c1, s1] = a.countSumLE(0, 5, 511);
    assert(c1 == 5 && s1 == 1278);

    int hi = (1LL << 62) - 1;
    vector<int> v{0, 1LL << 60, hi, 1, 2};
    Wavelet<62> b(v);
    assert(b.countLess(0, 5, 1LL << 62) == 5);
    assert(b.frequency(0, 5, hi) == 1);
    auto [c2, s2] = b.countSumLE(0, 5, hi);
    int want = accumulate(v.begin(), v.end(), 0LL);
    assert(c2 == 5 && s2 == want);

    KthWavelet<9> c({0, 1, 255, 511, 511});
    assert(c.kth(0, 5, 0) == 0 && c.kth(1, 5, 2) == 511);
    cout << "OK\n";
    return 0;
}
