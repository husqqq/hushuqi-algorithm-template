#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class F> long long firstTrue(long long l, long long r, F f)
{
    // 在闭区间 [l,r] 内返回第一个使单调谓词 f 为真的整数。
    while (l < r)
    {
        long long m = l + (r - l) / 2;
        if (f(m))
        {
            r = m;
        }
        else
        {
            l = m + 1;
        }
    }
    return l;
}

template <class F> long long lastTrue(long long l, long long r, F f)
{
    // 在闭区间 [l,r] 内返回最后一个使单调谓词 f 为真的整数。
    return firstTrue(l,
                     r,
                     [&](long long x)
                     {
                         return !f(x);
                     }) -
           1;
}

// f 在 [l,r] 上由 false 变 true；固定迭代次数避免精度死循环。
template <class F> long double firstReal(long double l, long double r, F f, int it = 100)
{
    // f 从假变真；二分 it 次后返回边界的中点近似。
    while (it--)
    {
        long double m = (l + r) / 2;
        if (f(m))
        {
            r = m;
        }
        else
        {
            l = m;
        }
    }
    return (l + r) / 2;
}

template <class It, class T, class F = less<>> It fastLower(It first, It last, const T &x, F f = {})
{
    // [first,last) 必须支持随机访问；返回第一个不小于 x 的位置。
    assert(first <= last && (unsigned long long)(last - first) <= UINT32_MAX); // 调试检查，可删
    uint32_t n = last - first;
    for (n++; n > 1;)
    {
        uint32_t h = n / 2;
        n -= h;
        first += f(first[h - 1], x) * h;
    }
    return first;
}

template <class It, class T, class F = less<>> It fastUpper(It first, It last, const T &x, F f = {})
{
    // [first,last) 必须支持随机访问；返回第一个大于 x 的位置。
    assert(first <= last && (unsigned long long)(last - first) <= UINT32_MAX); // 调试检查，可删
    uint32_t n = last - first;
    for (n++; n > 1;)
    {
        uint32_t h = n / 2;
        n -= h;
        first += !f(x, first[h - 1]) * h;
    }
    return first;
}
