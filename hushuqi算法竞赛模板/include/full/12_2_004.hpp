#pragma once

#include "12_1_001.hpp"

namespace twoMachSched
{
int twoMachines(const vector<int> &a)
{
    __int128 total = 0;
    for (int x : a)
    {
        assert(x >= 0);
        total += x;
    }
    assert(total <= numeric_limits<int>::max()
           && total < (__int128)numeric_limits<size_t>::max());
    int sum = (int)total;
    vector<char> dp(sum + 1);
    dp[0] = true;
    for (int x : a)
    {
        for (int s = sum; s >= x; s--)
        {
            dp[s] |= dp[s - x];
        }
    }
    int ans = sum;
    for (int s = 0; s <= sum; s++)
    {
        if (dp[s])
        {
            ans = min(ans, max(s, sum - s));
        }
    }
    return ans;
}
}
