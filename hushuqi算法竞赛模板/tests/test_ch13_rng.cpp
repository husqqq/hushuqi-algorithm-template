#include "../include/full/13_2_004.hpp"

signed main()
{
    // 无参数；核对固定种子、零种子替换、unit() 的端点和有界采样，成功返回 0。
    RNG a(1), b(1);
    assert(a.next() == 1082269761ULL);
    assert(b.next() == 1082269761ULL);
    RNG c(1);
    long double u = c.unit();
    assert(u == ldexpl(528452.0L, -53) && 0 <= u && u < 1);
    RNG zero(0), one(1);
    assert(zero.next() == one.next());
    RNG r(20261017);
    for (auto n : array<unsigned long long, 6>{1, 2, 3, 100, 1ULL << 63, ~0ULL})
    {
        for (int i = 0; i < 100; i++)
        {
            assert(r(n) < n);
        }
    }
    for (int i = 0; i < 1000; i++)
    {
        long double x = r.unit();
        assert(0 <= x && x < 1);
    }
    cout << "OK\n";
}
