#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

bool bashWin(long long n, long long m)
{
    // n 是当前石子数，m 是单步最多取石数；返回先手是否必胜。
    assert(n >= 0 && m >= 1); // 调试检查，可删。
    if (m == numeric_limits<long long>::max())
    {
        return n != 0;
    }
    return n % (m + 1) != 0;
}
