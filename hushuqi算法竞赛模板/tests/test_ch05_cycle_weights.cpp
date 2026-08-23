#include "../include/full/5_9_005.hpp"

bool close(long double a, long double b)
{
    // a、b 是有限浮点数；返回二者是否在测试误差内相等。
    return abs(a - b) <= 1E-12L * max(1.0L, max(abs(a), abs(b)));
}

signed main()
{
    assert(!minMeanCycle(3, {{0, 1, 2}, {1, 2, 3}}).has_value());
    auto loop = minMeanCycle(1, {{0, 0, -4}});
    assert(loop.has_value() && close(*loop, -4));
    auto mean = minMeanCycle(3, {{0, 1, 1}, {1, 0, 3}, {2, 2, 5}});
    assert(mean.has_value() && close(*mean, 2));

    assert(!minRatCycle(3, {{0, 1, 2, 1}, {1, 2, 3, 1}}).has_value());
    auto ratioLoop = minRatCycle(1, {{0, 0, 6, 2}});
    assert(ratioLoop.has_value() && close(*ratioLoop, 3));
    auto ratio = minRatCycle(3, {{0, 1, 2, 1}, {1, 0, 6, 3}, {2, 2, 5, 1}});
    assert(ratio.has_value() && close(*ratio, 2));

    puts("OK");
    return 0;
}
