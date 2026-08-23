#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace incl_excl
{
int countMult(int n, vector<int> d)
{
    // n 是闭区间上界，d 是正除数表；返回 [1,n] 中至少被一个 d[i] 整除的整数个数。
    assert(n >= 0 && d.size() < 63); // 调试检查，可删。
    for (int x : d)
    {
        assert(x > 0); // 调试检查，可删。
    }
    int ans = 0;
    for (unsigned long long s = 1; s < (1ULL << d.size()); s++)
    {
        int l = 1;
        bool ok = true;
        for (int i = 0; i < (int)d.size(); i++)
        {
            if (!(s >> i & 1))
            {
                continue;
            }
            int g = gcd(l, d[i]);
            if (l > n / (d[i] / g))
            {
                ok = false;
                break;
            }
            l *= d[i] / g;
        }
        if (ok)
        {
            ans += (popcount(s) & 1 ? 1 : -1) * (n / l);
        }
    }
    assert(0 <= ans && ans <= n); // 调试检查，可删。
    return ans;
}
}
