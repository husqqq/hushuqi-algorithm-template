#pragma once

#include "13_1_001.hpp"

namespace fib_game
{
int zeckMin(int n)
{
    assert(n > 0);
    vector<int> f = {1, 2};
    while (f.back() <= n - f[f.size() - 2])
    {
        f.push_back(f.back() + f[f.size() - 2]);
    }
    int last = 0;
    for (auto it = f.rbegin(); it != f.rend(); ++it)
    {
        if (*it <= n)
        {
            n -= *it;
            last = *it;
        }
    }
    return last;
}
}
