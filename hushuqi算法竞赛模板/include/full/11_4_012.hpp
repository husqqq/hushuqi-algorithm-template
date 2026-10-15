#pragma once

#include "11_1_001.hpp"

vector<pair<long long, long long>> circlePoints(long long n)
{
    assert(n >= 0); // 调试检查，可删。
    set<pair<long long, long long>> s;
    for (long long x = 0; x == 0 || x <= n / x; x++)
    {
        long long y2 = n - x * x;
        long long y = sqrtl(y2);
        while (y + 1 <= y2 / (y + 1))
        {
            y++;
        }
        while (y > 0 && y > y2 / y)
        {
            y--;
        }
        if (y * y != y2)
        {
            continue;
        }
        for (int sx : {-1, 1})
        {
            for (int sy : {-1, 1})
            {
                s.insert({sx * x, sy * y});
                s.insert({sx * y, sy * x});
            }
        }
    }
    return {s.begin(), s.end()};
}
