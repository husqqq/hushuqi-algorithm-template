#pragma once

#include "12_1_001.hpp"

// 知识点 12.2.005：完全背包找硬币的最少枚数与方案数。
namespace coin_problem
{
array<int, 2> coinChange(const vector<int> &coin, int target, int mod)
{
    // coin 是不同正面值，target 是目标金额，mod 是方案数模数；返回最少枚数和无序方案数，无解时第一维为 -1。
    assert(target >= 0 && mod > 0); // 调试检查，可删。
    set<int> seen;
    for (int x : coin)
    {
        assert(x > 0 && seen.insert(x).second); // 调试检查，可删。
    }
    const int inf = numeric_limits<int>::max() / 4;
    vector<int> best(target + 1, inf);
    vector<int> ways(target + 1);
    best[0] = 0;
    ways[0] = 1;
    for (int x : coin)
    {
        for (int s = x; s <= target; s++)
        {
            if (best[s - x] != inf)
            {
                best[s] = min(best[s], best[s - x] + 1);
            }
            ways[s] = addMod(ways[s], ways[s - x], mod);
        }
    }
    return {best[target] == inf ? -1 : best[target], ways[target]};
}
}
