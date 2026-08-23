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

// n 个人编号 0..n-1，每次数到 k 的人出局，返回最后留下的编号。
long long josephus(long long n, long long k)
{
    // n 人从 0 编号、每数到第 k 人出局；返回最后留下的编号。
    assert(n > 0 && k > 0);
    long long x = 0;
    if (n >= 2)
    {
        long long i = 2;
        while (true)
        {
            long long z = k % i;
            x = x >= i - z ? x - (i - z) : x + z;
            if (i == n)
            {
                break;
            }
            i++;
        }
    }
    return x;
}
