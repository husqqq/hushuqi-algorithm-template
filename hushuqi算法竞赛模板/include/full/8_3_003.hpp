#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// PTriple 的三个分量依次是两条直角边 a、b 与斜边 c。
using PTriple = array<int, 3>;

vector<PTriple> pythagorean(long long cmax)
{
    // cmax 是非负斜边上界；返回全部满足 a<=b、c<=cmax 的本原三元组 {a,b,c}。
    assert(cmax >= 0); // 调试检查，可删。
    vector<PTriple> ans;
    for (long long m = 2; m <= (cmax - 1) / m; m++)
    {
        long long mm = m * m;
        for (long long n = 1; n < m; n++)
        {
            if ((m - n) % 2 == 0 || gcd(m, n) != 1)
            {
                continue;
            }
            long long nn = n * n;
            if (mm > cmax - nn)
            {
                break;
            }
            long long a = mm - nn;
            long long b = 2 * m * n;
            long long c = mm + nn;
            if (a > b)
            {
                swap(a, b);
            }
            ans.push_back({a, b, c});
        }
    }
    return ans;
}
