#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class C> auto vandermonde(int m, int n, int r, C comb)
{
    // m、n 是两组大小，r 是总选择数，comb(x,y) 返回 C(x,y)；返回 Vandermonde 左侧求和。
    assert(m >= 0 && n >= 0); // 调试检查，可删。
    using T = decltype(comb(0, 0));
    T ans{};
    for (int k = max<int>(0, r - n); k <= min(m, r); k++)
    {
        ans += comb(m, k) * comb(n, r - k);
    }
    return ans;
}
