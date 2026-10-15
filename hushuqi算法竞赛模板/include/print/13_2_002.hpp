#pragma once

#include "13_1_001.hpp"

long double binomialPmf(int n, int k, long double p)
{
    // n、k、p 是试验数、成功数和单次成功率；返回二项分布 P(X=k)。
    assert(n >= 0 && 0 <= p && p <= 1); // 调试检查，可删。
    if (k < 0 || k > n)
    {
        return 0;
    }
    if (p == 0)
    {
        return k == 0;
    }
    if (p == 1)
    {
        return k == n;
    }
    long double z = lgammal((long double)n + 1) - lgammal((long double)k + 1)
                  - lgammal((long double)(n - k) + 1);
    z += k * logl(p) + (n - k) * log1pl(-p);
    return expl(z);
}

array<long double, 2> geomMoments(long double p)
{
    // p 是每次成功概率；返回几何分布（首次成功试验数）的 {期望, 方差}。
    assert(0 < p && p <= 1); // 调试检查，可删。
    return {1 / p, (1 - p) / (p * p)};
}
