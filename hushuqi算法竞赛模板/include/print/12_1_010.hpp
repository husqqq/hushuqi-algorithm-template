#pragma once

#include "12_1_001.hpp"

bool boundSubset(const vector<int> &cnt, const vector<int> &w, int sum)
{
    assert(cnt.size() == w.size() && sum >= 0);
    for (int i = 0; i < (int)w.size(); i++)
    {
        assert(cnt[i] >= 0 && w[i] > 0);
    }
    vector<int> rem(sum + 1, -1);
    rem[0] = 0;
    for (int i = 0; i < (int)w.size(); i++)
    {
        for (int s = 0; s <= sum; s++)
        {
            if (rem[s] >= 0)
            {
                rem[s] = cnt[i];
            }
            else if (s < w[i] || rem[s - w[i]] <= 0)
            {
                rem[s] = -1;
            }
            else
            {
                rem[s] = rem[s - w[i]] - 1;
            }
        }
    }
    return rem[sum] >= 0;
}
