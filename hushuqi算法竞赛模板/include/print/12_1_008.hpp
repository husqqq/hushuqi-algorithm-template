#pragma once

#include "12_1_001.hpp"

struct PackResult
{
    long long val;
    int ways;
    vector<int> pick;
};

PackResult packPlan(int cap, const vector<pair<int, long long>> &a, int mod = 1000000007)
{
    struct Path
    {
        int item;
        int pre;
    };
    assert(cap >= 0 && mod > 0);
    int n = a.size();
    vector<long long> f(cap + 1, -INF);
    vector<int> cnt(cap + 1);
    vector<int> at(cap + 1, -1);
    vector<Path> path;
    f[0] = 0;
    cnt[0] = 1;
    for (int i = 0; i < n; i++)
    {
        auto [w, v] = a[i];
        assert(w > 0);
        for (int j = cap; j >= w; j--)
        {
            if (f[j - w] == -INF)
            {
                continue;
            }
            long long x = f[j - w] + v;
            if (x > f[j])
            {
                f[j] = x;
                cnt[j] = cnt[j - w];
                path.push_back({i, at[j - w]});
                at[j] = path.size() - 1;
            }
            else if (x == f[j] && x > -INF)
            {
                cnt[j] = addMod(cnt[j], cnt[j - w], mod);
            }
        }
    }
    long long val = *max_element(f.begin(), f.end());
    int ways = 0;
    int last = -1;
    for (int j = 0; j <= cap; j++)
    {
        if (f[j] == val)
        {
            ways = addMod(ways, cnt[j], mod);
            last = at[j];
        }
    }
    vector<int> pick;
    while (last >= 0)
    {
        pick.push_back(path[last].item);
        last = path[last].pre;
    }
    reverse(pick.begin(), pick.end());
    return {val, ways, pick};
}
