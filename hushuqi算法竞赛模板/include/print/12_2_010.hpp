#pragma once

#include "12_1_001.hpp"

// 返回不同非空子序列个数，值相同的元素视为相同字符。
int distinctSub(const vector<long long> &a, int mod = 998244353)
{
    // a 是输入序列，mod 是正模数；返回不同非空子序列数量模 mod。
    assert(mod > 0); // 调试检查，可删。
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
