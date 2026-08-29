#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

bool bashWin(long long n, long long m)
{
    assert(n >= 0 && m >= 1);
    if (m == numeric_limits<long long>::max())
    {
        return n != 0;
    }
    return n % (m + 1) != 0;
}
