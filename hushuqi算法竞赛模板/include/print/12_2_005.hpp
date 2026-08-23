#pragma once

#include "12_1_001.hpp"

namespace coin_problem
{
array<int, 2> coinChange(const vector<int> &coin, int target, int mod)
{
    assert(target >= 0 && mod > 0);
    set<int> seen;
    for (int x : coin)
    {
        assert(x > 0 && seen.insert(x).second);
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
