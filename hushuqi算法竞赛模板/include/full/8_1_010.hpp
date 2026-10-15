#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace balls_boxes
{
template <class T = long long>
T comb(T n, int k)
{
    // n 是非负上标，k 是下标；返回二项式系数。
    static_assert(numeric_limits<T>::is_integer && numeric_limits<T>::is_signed &&
                  sizeof(T) >= sizeof(long long));
    assert(n >= 0); // 调试检查，可删。
    if (k < 0 || (T)k > n)
    {
        return T(0);
    }
    if ((T)k > n - k)
    {
        k = (int)(n - k);
    }
    T ans = 1;
    for (int i = 1; i <= k; i++)
    {
        T x = n - k + i;
        int y = i;
        int g = gcd((int)(x % y), y);
        x /= g;
        y /= g;
        assert(ans % y == 0); // 调试检查，可删。
        ans /= y;
        ans *= x;
    }
    return ans;
}

template <class T = long long>
T unlabBoxDist(int balls, int boxes, bool nonempty)
{
    // balls 是球数，boxes 是盒数，nonempty 表示每盒非空；返回方案数。
    static_assert(numeric_limits<T>::is_integer && numeric_limits<T>::is_signed &&
                  sizeof(T) >= sizeof(long long));
    assert(balls >= 0 && boxes >= 0); // 调试检查，可删。
    if (boxes == 0)
    {
        return T(balls == 0);
    }
    if (nonempty)
    {
        return balls < boxes ? T(0) : comb<T>((T)balls - 1, boxes - 1);
    }
    T b = balls, m = boxes;
    return comb<T>(b + m - 1, boxes - 1);
}

int pigeonBound(int objects, int boxes)
{
    // objects 是对象数，boxes 是正盒数；返回必有一盒至少包含的对象数。
    assert(objects >= 0 && boxes > 0); // 调试检查，可删。
    return objects / boxes + (objects % boxes != 0);
}
}
