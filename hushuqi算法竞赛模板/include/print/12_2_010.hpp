#pragma once

#include "12_1_001.hpp"

int distinctSub(const vector<long long> &a, int mod = 998244353)
{
    assert(mod > 0);
    unordered_map<long long, int> last;
    int sum = 1 % mod;
    for (long long x : a)
    {
        int old = last[x];
        int twice = sum >= mod - sum ? sum - (mod - sum) : sum + sum;
        int next = twice - old;
        if (next < 0)
        {
            next += mod;
        }
        last[x] = sum;
        sum = next;
    }
    return sum == 0 ? mod - 1 : sum - 1;
}
