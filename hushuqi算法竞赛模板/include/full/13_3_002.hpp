#pragma once

#include "13_1_001.hpp"

array<__int128, 3> powerSums(int n)
{
    assert(0 <= n && n <= 5107605666LL);
    __int128 x = n;
    __int128 s1 = x * (x + 1) / 2;
    __int128 a = x;
    __int128 b = x + 1;
    __int128 c = 2 * x + 1;
    if (a % 2 == 0)
    {
        a /= 2;
    }
    else
    {
        b /= 2;
    }
    if (a % 3 == 0)
    {
        a /= 3;
    }
    else if (b % 3 == 0)
    {
        b /= 3;
    }
    else
    {
        c /= 3;
    }
    return {s1, a * b * c, s1 * s1};
}

long double harmonic(int n)
{
    assert(n >= 0);
    long double ans = 0;
    for (int i = 1; i <= n;)
    {
        ans += 1.0L / i;
        if (i == n)
        {
            break;
        }
        i++;
    }
    return ans;
}
