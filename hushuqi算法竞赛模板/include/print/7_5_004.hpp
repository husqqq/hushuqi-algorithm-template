#pragma once
#include "7_5_001.hpp"

int minModLinear(int n, int m, int a, int b)
{
    // n、m 为正，a、b 是整数；返回 0<=i<n 时 (a*i+b) mod m 的最小值。
    assert(n > 0 && m > 0); // 调试检查，可删
    a %= m;
    b %= m;
    if (a < 0)
    {
        a += m;
    }
    if (b < 0)
    {
        b += m;
    }
    auto count = [&](int x)
    {
        // x 是余数上界；返回余数落在 [0,x] 的下标数量。
        return floorSum(n, m, a, b) - floorSum(n, m, a, b - x - 1);
    };
    int l = -1;
    int r = m - 1;
    while (r - l > 1)
    {
        int x = midpoint(l, r);
        if (count(x))
        {
            r = x;
        }
        else
        {
            l = x;
        }
    }
    return r;
}
