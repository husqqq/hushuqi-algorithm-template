#pragma once

#include "14_1_001.hpp"

vector<long double> quadRoots(long double a, long double b, long double c)
{
    // a、b、c 是方程 ax^2+bx+c=0 的系数；返回升序实根，重根只返回一次。
    // 调试检查，可删。
    assert(isfinite(a) && isfinite(b) && isfinite(c));
    bool hasA = a != 0, hasB = b != 0;
    long double scale = max({fabsl(a), fabsl(b), fabsl(c)});
    if (scale == 0)
    {
        return {};
    }
    a /= scale;
    b /= scale;
    c /= scale;
    assert((!hasA || a != 0) && (!hasB || b != 0)); // 调试检查，可删。
    if (!hasA)
    {
        return !hasB ? vector<long double>{} : vector<long double>{-c / b};
    }
    long double bb = b * b;
    long double ac4 = 4 * a * c;
    long double d = fmal(-4 * a, c, bb);
    long double tol = 64 * numeric_limits<long double>::epsilon()
                      * (fabsl(bb) + fabsl(ac4));
    if (d < -tol)
    {
        return {};
    }
    if (fabsl(d) <= tol)
    {
        return {-b / (2 * a)};
    }
    long double q = -0.5L * (b + copysignl(sqrtl(d), b));
    vector<long double> ans{q / a, c / q};
    sort(ans.begin(), ans.end());
    return ans;
}
