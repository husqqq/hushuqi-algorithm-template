#pragma once

#include "13_1_001.hpp"

bool nimWin(const vector<unsigned long long> &a)
{
    // 参数 a 表示 普通 Nim 各堆的非负石子数；返回 Nim 局面的先手是否必胜。
    unsigned long long x = 0;
    for (unsigned long long v : a)
    {
        x ^= v;
    }
    return x != 0;
}

bool antiNimWin(const vector<unsigned long long> &a)
{
    // 参数 a 表示 反 Nim 各堆的非负石子数；返回反 Nim 局面的先手是否必胜。
    bool allOne = true;
    unsigned long long x = 0;
    int one = 0;
    for (unsigned long long v : a)
    {
        allOne &= v <= 1;
        x ^= v;
        one += v == 1;
    }
    return allOne ? one % 2 == 0 : x != 0;
}

bool nimKWin(const vector<unsigned long long> &a, int k)
{
    // a 是各堆石子数，k>=1 是每步最多可减少的堆数；返回正常规则下先手是否必胜。
    assert(k >= 1);
    // 无符号类型保留完整 64 位堆大小，并承载 k+1。
    unsigned long long mod = (unsigned long long)k + 1;
    for (int b = 0; b < 64; b++)
    {
        unsigned long long cnt = 0;
        for (auto x : a)
        {
            cnt += x >> b & 1;
        }
        if (cnt % mod != 0) return true;
    }
    return false;
}
