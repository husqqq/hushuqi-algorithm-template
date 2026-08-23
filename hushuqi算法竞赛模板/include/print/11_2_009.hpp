#pragma once

#include "11_1_001.hpp"

pair<long long, long long> pick(const vector<pair<long long, long long>> &a)
{
    using i128 = __int128_t;
    int n = (int)a.size();
    assert(n >= 3); // 调试检查，可删。
    auto ab = [&](i128 x)
    {
        return x < 0 ? -x : x;
    };
    auto gcd128 = [&](i128 x, i128 y)
    {
        while (y != 0)
        {
            i128 r = x % y;
            x = y;
            y = r;
        }
        return x;
    };
    i128 s = 0;
    i128 b = 0;
    for (int i = 0; i < n; i++)
    {
        auto [x, y] = a[i];
        auto [u, v] = a[(i + 1) % n];
        s += (i128)x * v - (i128)y * u;
        i128 dx = ab((i128)x - u);
        i128 dy = ab((i128)y - v);
        b += gcd128(dx, dy);
    }
    s = ab(s);
    assert(s > 0); // 调试检查，可删。
    assert((s - b) % 2 == 0); // 调试检查，可删。
    i128 in = (s - b + 2) / 2;
    assert(in >= 0 && in <= LLONG_MAX && b <= LLONG_MAX); // 调试检查，可删。
    return {(long long)in, (long long)b};
}
