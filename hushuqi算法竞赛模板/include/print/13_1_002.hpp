#pragma once

#include "13_1_001.hpp"

bool nimWin(const vector<unsigned long long> &a)
{
    unsigned long long x = 0;
    for (unsigned long long v : a)
    {
        x ^= v;
    }
    return x != 0;
}

bool antiNimWin(const vector<unsigned long long> &a)
{
    bool allOne = true;
    unsigned long long x = 0;
    int one = 0;
    for (unsigned long long v : a)
    {
        allOne &= v <= 1;
        x ^= v;
        one += v == 1;
    }
    return allOne ? one > 0 && one % 2 == 0 : x != 0;
}
