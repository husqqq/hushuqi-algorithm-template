#include "../include/full/13_2_002.hpp"

signed main()
{
    // 无参数；检查端点、中心概率、对数选项与次正规结果，成功返回 0。
    assert(fabsl(binomialPmf(4, 2, 0.5L) - 0.375L) < 1E-15L);
    assert(binomialPmf(0, 0, 0.3L) == 1);
    assert(binomialPmf(5, -1, 0.5L) == 0);
    assert(binomialPmf(5, 6, 0.5L, true) == -numeric_limits<long double>::infinity());
    assert(binomialPmf(5, 0, 0, true) == 0);
    assert(binomialPmf(5, 5, 1, true) == 0);
    auto p = binomialPmf(1000000000, 500000000, 0.5L);
    assert(fabsl(p / 2.52313252138937691782E-5L - 1) < 1E-12L);
    auto tiny = numeric_limits<long double>::denorm_min();
    assert(binomialPmf(2, 1, tiny) == 2 * tiny);
    auto z = binomialPmf(1000000000, 0, 0.5L, true);
    assert(isfinite(z) && z < -6E8L && binomialPmf(1000000000, 0, 0.5L) == 0);
    assert((geomMoments(0.5L) == array<long double, 2>{2, 2}));
    assert((geomMoments(1) == array<long double, 2>{1, 0}));
    cout << "OK\n";
}
