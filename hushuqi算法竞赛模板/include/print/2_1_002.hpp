#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class F> int ternaryInt(int l, int r, F f)
{
    // l、r 是非空整数闭区间；f 在峰顶平台外严格递增、递减，返回一个最大值位置。
    // 求最小值要把收缩循环和末尾扫描中的 f 比较同时反向；只改一处会破坏不变量。
    assert(l <= r);
    while ((__int128)r - l > 4)
    {
        auto d = (__int128)r - l; // 跨 64 位端点的差值需要加宽。
        auto x = l + (int)(d / 3);
        auto y = r - (int)(d / 3);
        if (f(x) < f(y))
        {
            l = x + 1;
        }
        else
        {
            r = y - 1;
        }
    }
    auto p = l;
    auto x = l;
    while (x < r)
    {
        x++;
        if (f(p) < f(x))
        {
            p = x;
        }
    }
    return p;
}

template <class F> long double ternaryReal(long double l, long double r, F f, int it = 120)
{
    // l、r 是有限实数端点，it 是非负迭代次数；返回 [l,r] 上最大值位置的近似值。
    // f 在峰顶平台外严格递增、递减，求值须保持大小关系且不产生 NaN。
    // 求最小值要反向循环内的 f(x)、f(y) 比较；若改传 -f，先确认取负不会溢出。
    assert(isfinite(l) && isfinite(r) && l <= r && it >= 0);
    for (int t = 0; t < it; t++)
    {
        auto x = lerp(l, r, 1.0L / 3);
        auto y = lerp(l, r, 2.0L / 3);
        if (f(x) < f(y))
        {
            l = x;
        }
        else
        {
            r = y;
        }
    }
    return midpoint(l, r);
}

template <class F> long double goldenMax(long double l, long double r, F f, int it = 120)
{
    // l、r 是有限实数端点，it 是非负迭代次数；返回 [l,r] 上最大值位置的近似值。
    // f 在峰顶平台外严格递增、递减，求值须保持大小关系且不产生 NaN。
    // 求最小值时反向 fx、fy 的比较；其余复用点和区间更新方向不变。
    assert(isfinite(l) && isfinite(r) && l <= r && it >= 0);
    constexpr long double ratio = 0.6180339887498948482L;
    auto x = lerp(l, r, 1 - ratio);
    auto y = lerp(l, r, ratio);
    auto fx = f(x);
    auto fy = f(y);
    for (int t = 0; t < it; t++)
    {
        if (fx < fy)
        {
            l = x;
            x = y;
            fx = fy;
            y = lerp(l, r, ratio);
            fy = f(y);
        }
        else
        {
            r = y;
            y = x;
            fy = fx;
            x = lerp(l, r, 1 - ratio);
            fx = f(x);
        }
    }
    return midpoint(l, r);
}
