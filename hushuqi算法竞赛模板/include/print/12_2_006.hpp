#pragma once

#include "12_1_001.hpp"

// 知识点 12.2.006：汉诺塔第 k 步移动。
namespace hanoi
{
array<int, 3> kthMove(int n, unsigned long long k)
{
    // n 是盘数，k 从 1 开始且不超过 2^n-1；返回盘号、起始柱和目标柱，柱编号为 0..2。
    assert(1 <= n && n < 64 && 1 <= k && k < (1ULL << n)); // 调试检查，可删。
    int disk = countr_zero(k) + 1;
    unsigned long long before = (k >> disk) + 1; // 移动次数依赖 64 位无符号步数。
    int dir = ((n - disk) & 1) ? 1 : -1;
    int from = (dir * (int)((before - 1) % 3) + 3) % 3;
    int to = (from + dir + 3) % 3;
    return {disk, from, to};
}
}
