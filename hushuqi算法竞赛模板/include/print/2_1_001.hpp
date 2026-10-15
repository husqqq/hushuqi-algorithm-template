#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class F> int firstTrue(int l, int r, F f)
{
    // l、r 是非空闭区间，f 由假变真且 f(r) 为真；返回首个真值的位置。
    assert(l <= r);
    while (l < r)
    {
        int m = midpoint(l, r);
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

template <class F> int lastTrue(int l, int r, F f)
{
    // l、r 是非空闭区间，f 由真变假且 f(l) 为真；返回末个真值的位置。
    assert(l <= r);
    while (l < r)
    {
        int m = midpoint(r, l);
        if (f(m))
        {
            l = m;
        }
        else
        {
            r = m - 1;
        }
    }
    return l;
}

template <class F> long double firstReal(long double l, long double r, F f, int it = 100)
{
    // l、r 是有限实数端点，f 由假变真且分界在 [l,r]，it 非负；返回分界近似值。
    assert(isfinite(l) && isfinite(r) && l <= r && it >= 0);
    for (int t = 0; t < it; t++)
    {
        auto m = midpoint(l, r);
        if (f(m))
        {
            r = m;
        }
        else
        {
            l = m;
        }
    }
    return midpoint(l, r);
}

template <class It, class T, class F = less<>> It fastLower(It first, It last, const T &x, F f = {})
{
    // [first,last) 是随机访问区间，x 是查询值；f(元素,x) 先真后假，返回首个假值位置。
    // 区间长度须能由 int 表示；没有这样的元素时返回 last，空区间也返回 last。
    assert(first <= last && last - first <= numeric_limits<int>::max());
    int n = last - first;
    while (n)
    {
        int h = n / 2 + (n & 1);
        n -= h;
        first += (bool)f(first[h - 1], x) * h;
    }
    return first;
}

template <class It, class T, class F = less<>> It fastUpper(It first, It last, const T &x, F f = {})
{
    // [first,last) 是随机访问区间，x 是查询值；f(x,元素) 先假后真，返回首个真值位置。
    // 区间长度须能由 int 表示；没有这样的元素时返回 last，空区间也返回 last。
    assert(first <= last && last - first <= numeric_limits<int>::max());
    int n = last - first;
    while (n)
    {
        int h = n / 2 + (n & 1);
        n -= h;
        first += !f(x, first[h - 1]) * h;
    }
    return first;
}
