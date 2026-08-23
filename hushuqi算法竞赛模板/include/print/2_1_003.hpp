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

template <class T, class F> void eachPerm(vector<T> a, F f)
{
    // 按字典序枚举多重集合 a 的不同排列，并把每个排列传给 f。
    sort(a.begin(), a.end());
    do
    {
        f(a);
    } while (next_permutation(a.begin(), a.end()));
}

template <class F> void eachSubset(int n, F f)
{
    // 枚举 n 个元素的全部子集，并把二进制掩码传给 f。
    assert(0 <= n && n < 63);
    for (unsigned long long s = 0; s < (1ULL << n); s++)
    {
        f(s);
    }
}

template <class F> void eachKSubset(int n, int k, F f)
{
    // 枚举 n 个元素中大小恰为 k 的子集，并把二进制掩码传给 f。
    assert(0 <= k && k <= n && n < 63);
    if (!k)
    {
        f(0);
        return;
    }
    unsigned long long s = (1ULL << k) - 1, lim = 1ULL << n;
    while (s < lim)
    {
        f(s);
        unsigned long long x = s & -s, y = s + x;
        s = y | ((s ^ y) >> 2) / x;
    }
}

unsigned long long gray(unsigned long long x)
{
    // 返回非负整数 x 的 Gray Code。
    return x ^ (x >> 1);
}

unsigned long long invGray(unsigned long long x)
{
    // 返回 Gray Code x 对应的原整数。
    for (unsigned long long y = x >> 1; y; y >>= 1)
    {
        x ^= y;
    }
    return x;
}
