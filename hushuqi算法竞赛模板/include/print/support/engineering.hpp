#pragma once

#include <bits/stdc++.h>

using namespace std;

inline volatile int speedRounds = 10000000; // 每块生成次数
inline volatile int speedBlocks = 1;        // 重复块数
inline constexpr uint32_t speedCheck = 3894306128U; // 固定序列的正确摘要

#if defined(__GNUC__)
__attribute__((noinline))
#endif
inline uint32_t speedBlock()
{
    // 返回固定随机序列的滚动异或摘要
    int n = speedRounds;
    mt19937 rng(712367821);
    uint32_t h = 0;
    for (int i = 0; i < n; i++)
    {
        h = rotl(h, 7) ^ rng();
    }
    return h;
}

inline uint32_t speedTest()
{
    // 重复 speedBlocks 块；全部摘要正确时返回 0
    int k = speedBlocks;
    uint32_t bad = 0;
    for (int i = 0; i < k; i++)
    {
        bad |= speedBlock() ^ speedCheck;
    }
    return bad;
}
