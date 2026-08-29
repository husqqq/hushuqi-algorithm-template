#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

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
