#pragma once

#include "13_1_001.hpp"

bool stairNimWin(const vector<unsigned long long> &a)
{
    unsigned long long x = 0;
    for (int i = 1; i < (int)a.size(); i += 2)
    {
        x ^= a[i];
    }
    return x != 0;
}
