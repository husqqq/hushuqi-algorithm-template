#pragma once

#include "12_1_001.hpp"

namespace hanoi
{
array<int, 3> kthMove(int n, unsigned long long k)
{
    assert(1 <= n && n < 64 && 1 <= k && k < (1ULL << n));
    int disk = countr_zero(k) + 1;
    unsigned long long before = (k >> disk) + 1;
    int dir = ((n - disk) & 1) ? 1 : -1;
    int from = (dir * (int)(before - 1) % 3 + 3) % 3;
    int to = (from + dir + 3) % 3;
    return {disk, from, to};
}
}
