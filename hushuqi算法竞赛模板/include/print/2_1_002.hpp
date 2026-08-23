#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

template <class F> long long ternaryInt(long long l, long long r, F f)
{
    // f 在整数区间 [l,r) 上单峰；返回一个最大值位置。
    while (r - l > 3)
    {
        long long x = l + (r - l) / 3;
        long long y = r - (r - l) / 3;
        if (f(x) < f(y))
        {
            l = x + 1;
        }
        else
        {
            r = y;
        }
    }
    long long p = l;
    for (long long x = l + 1; x < r; x++)
    {
        if (f(p) < f(x))
        {
            p = x;
        }
    }
    return p;
}

// 求单峰函数最大点；最小值改成比较符号相反。
template <class F> long double ternary(long double l, long double r, F f, int it = 100)
{
    // f 在 [l,r] 上单峰；三分 it 次后返回最大点近似。
    while (it--)
    {
        long double x = (2 * l + r) / 3;
        long double y = (l + 2 * r) / 3;
        if (f(x) < f(y))
        {
            l = x;
        }
        else
        {
            r = y;
        }
    }
    return (l + r) / 2;
}
