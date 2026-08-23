#pragma once

#include "13_1_001.hpp"

namespace exp_var
{
array<long double, 2> moments(const vector<long double> &x, const vector<long double> &p)
{
    assert(x.size() == p.size());
    assert(!x.empty());
    long double sum = 0;
    long double mean = 0;
    long double m2 = 0;
    for (int i = 0; i < (int)x.size(); i++)
    {
        assert(p[i] >= 0);
        if (p[i] == 0)
        {
            continue;
        }
        long double next = sum + p[i];
        long double d = x[i] - mean;
        mean += d * p[i] / next;
        m2 += p[i] * d * (x[i] - mean);
        sum = next;
    }
    assert(fabsl(sum - 1) <= 1E-12L);
    return {mean, max((long double)0, m2 / sum)};
}
}
