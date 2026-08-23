#pragma once

#include "13_1_001.hpp"

namespace stone_game
{
vector<int> subGameSG(int n, const vector<int> &take)
{
    assert(n >= 0);
    vector<int> step = take;
    sort(step.begin(), step.end());
    step.erase(unique(step.begin(), step.end()), step.end());
    for (int d : step)
    {
        assert(d > 0);
    }
    vector<int> sg(n + 1);
    for (int x = 1; x <= n; x++)
    {
        vector<char> seen(step.size() + 1);
        for (int d : step)
        {
            if (d <= x && sg[x - d] < (int)seen.size())
            {
                seen[sg[x - d]] = true;
            }
        }
        while (sg[x] < (int)seen.size() && seen[sg[x]])
        {
            sg[x]++;
        }
    }
    return sg;
}
}
