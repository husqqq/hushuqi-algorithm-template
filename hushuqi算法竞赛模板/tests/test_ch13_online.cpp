#include "../include/full/13_2_008.hpp"
#include "../include/full/13_2_003.hpp"

signed main()
{
    // 无参数；核对流式加入、分组合并及旧矩接口共存，成功返回 0。
    exp_var::OnlineMom a, b;
    assert(!a.get());
    a.add(numeric_limits<long double>::quiet_NaN(), 0);
    a.merge(b);
    assert(!a.get());
    long double base = ldexpl(1, numeric_limits<long double>::digits - 5);
    a.add(base, 1.0L / 32);
    b.add(base + 1, 31.0L / 32);
    auto copy = b;
    a.merge(b);
    assert(b.w == copy.w && b.base == copy.base && b.mean == copy.mean && b.m2 == copy.m2);
    assert(fabsl((*a.get())[1] - 31.0L / 1024) < 1E-15L);
    auto want = a.get();
    a.merge(a);
    assert(a.w == 2 && a.get() == want);
    a = {};
    assert(!a.get());
    for (int i = 0; i < 100000; i++)
    {
        a.add(base + i % 2);
    }
    assert(a.w == 100000 && (*a.get())[0] == base + 0.5L);
    assert(fabsl((*a.get())[1] - 0.25L) < 1E-14L);
    auto old = exp_var::moments({base, base + 1}, {0.5L, 0.5L});
    assert(fabsl(old[1] - 0.25L) < 1E-15L);
    cout << "OK\n";
}
