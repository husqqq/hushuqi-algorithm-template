#pragma once

#include "13_1_001.hpp"

struct RNG
{
    // s 是 xorshift 伪随机数生成器的非零状态。
    unsigned long long s;

    RNG(unsigned long long seed = chrono::steady_clock::now().time_since_epoch().count()) : s(seed ? seed : 1)
    {
        // seed 是随机种子；0 会替换为 1。
    }

    unsigned long long next()
    {
        // 无参数；推进随机数状态并返回下一个随机数。
        s ^= s << 13;
        s ^= s >> 7;
        s ^= s << 17;
        return s;
    }

    unsigned long long operator()(unsigned long long n)
    {
        // n 是正上界；返回 [0,n) 内的伪随机整数。
        assert(n > 0); // 调试检查，可删。
        unsigned long long lim = -n % n;
        unsigned long long x;
        do
        {
            x = next();
        } while (x < lim);
        return x % n;
    }
};
