#pragma once

#include "13_1_001.hpp"

long double binomialPmf(int n, int k, long double p)
{
    assert(n >= 0 && 0 <= p && p <= 1);
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
    assert(0 < p && p <= 1);
    return {1 / p, (1 - p) / (p * p)};
}
