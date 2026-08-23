#pragma once

#include "13_1_001.hpp"

struct RNG
{
    unsigned long long s;

    RNG(unsigned long long seed = chrono::steady_clock::now().time_since_epoch().count())
        : s(seed ? seed : 1)
    {
    }

    unsigned long long next()
    {
        s ^= s << 13;
        s ^= s >> 7;
        s ^= s << 17;
        return s;
    }

    unsigned long long operator()(unsigned long long n)
    {
        assert(n > 0);
        unsigned long long lim = -n % n;
        unsigned long long x;
        do
        {
            x = next();
        } while (x < lim);
        return x % n;
    }
};
