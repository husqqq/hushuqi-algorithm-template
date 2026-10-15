#pragma once

#include "10_2_005.hpp"
using namespace std;
#define int long long

template <class T> vector<T> partitions(int n)
{
    // n 是要计算的最大整数；返回 p(0) 到 p(n)。
    assert(n >= 0); // 调试检查，可删。
    vector<T> a(n + 1);
    a[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        for (int k = 1;; k++)
        {
            int x = k / 2;
            int y = 3 * k - 1;
            if (k & 1)
            {
                x = k;
                y /= 2;
            }
            if (x > i / y)
            {
                break;
            }
            int g = x * y;
            if (k & 1)
            {
                a[i] += a[i - g];
            }
            else
            {
                a[i] -= a[i - g];
            }
            if (g <= i - k)
            {
                g += k;
                if (k & 1)
                {
                    a[i] += a[i - g];
                }
                else
                {
                    a[i] -= a[i - g];
                }
            }
        }
    }
    return a;
}

vector<Z> fastPart(int n)
{
    // n 是最大整数且 n<998244353；返回 p(0)..p(n)，复杂度 O(n log n)。
    assert(0 <= n && n < mod);
    vector<Z> inv(n + 1), f(n + 1);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; i++) inv[i] = -Z(mod / i) * inv[mod % i];
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; i * j <= n; j++) f[i * j] += inv[j];
    }
    return fpsExp(f, n + 1);
}
