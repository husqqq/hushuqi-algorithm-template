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

    long double unit()
    {
        // 无参数；返回 [0,1) 内含 53 个随机位的伪随机实数，不用于密码学安全场景。
        return ldexpl((long double)(next() >> 11), -53);
    }

    unsigned long long operator()(unsigned long long n)
    {
        // n 是正上界；返回 [0,n) 内的伪随机整数；无符号类型用于 64 位移位与自然溢出。
        assert(n > 0); // 调试检查，可删。
        const auto mx = numeric_limits<unsigned long long>::max();
        unsigned long long lim = mx - mx % n;
        unsigned long long x;
        do
        {
            x = next() - 1; // 非零输出平移到 [0,2^64-2]，共有 2^64-1 个值。
        } while (x >= lim);
        return x % n;
    }
};
