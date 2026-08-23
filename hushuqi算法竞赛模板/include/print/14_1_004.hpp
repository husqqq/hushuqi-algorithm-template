#pragma once

#include "14_1_001.hpp"

array<int, 6> pokerScore(vector<int> r, vector<int> suit)
{
    // r、suit 依次给出五张牌的点数与花色；返回可直接字典序比较的六元评分。
    // 调试检查，可删。
    assert(r.size() == 5 && suit.size() == 5);
    set<pair<int, int>> card;
    for (int i = 0; i < 5; i++)
    {
        // 调试检查，可删。
        assert(2 <= r[i] && r[i] <= 14);
        // 调试检查，可删。
        assert(0 <= suit[i] && suit[i] < 4);
        card.insert({r[i], suit[i]});
    }
    // 调试检查，可删。
    assert(card.size() == 5);
    sort(r.begin(), r.end(), greater<>());
    if (r == vector<int>{14, 5, 4, 3, 2})
    {
        r = {5, 4, 3, 2, 1};
    }
    bool flush = count(suit.begin(), suit.end(), suit[0]) == 5;
    bool straight = true;
    for (int i = 1; i < 5; i++)
    {
        straight &= r[i - 1] == r[i] + 1;
    }
    map<int, int, greater<>> f;
    for (int x : r)
    {
        f[x]++;
    }
    vector<pair<int, int>> a;
    for (auto [x, c] : f)
    {
        a.push_back({c, x});
    }
    sort(a.begin(), a.end(), greater<>());
    array<int, 6> z{};
    if (straight && flush)
    {
        z[0] = 8;
        z[1] = r[0];
    }
    else if (a[0].first == 4)
    {
        z[0] = 7;
        z[1] = a[0].second;
        z[2] = a[1].second;
    }
    else if (a[0].first == 3 && a[1].first == 2)
    {
        z[0] = 6;
        z[1] = a[0].second;
        z[2] = a[1].second;
    }
    else if (flush)
    {
        z[0] = 5;
        copy(r.begin(), r.end(), z.begin() + 1);
    }
    else if (straight)
    {
        z[0] = 4;
        z[1] = r[0];
    }
    else if (a[0].first == 3)
    {
        z[0] = 3;
        z[1] = a[0].second;
        vector<int> q;
        for (auto [c, x] : a)
        {
            if (c == 1)
            {
                q.push_back(x);
            }
        }
        sort(q.begin(), q.end(), greater<>());
        copy(q.begin(), q.end(), z.begin() + 2);
    }
    else if (a[0].first == 2 && a[1].first == 2)
    {
        z[0] = 2;
        z[1] = max(a[0].second, a[1].second);
        z[2] = min(a[0].second, a[1].second);
        z[3] = a[2].second;
    }
    else if (a[0].first == 2)
    {
        z[0] = 1;
        z[1] = a[0].second;
        vector<int> q;
        for (auto [c, x] : a)
        {
            if (c == 1)
            {
                q.push_back(x);
            }
        }
        sort(q.begin(), q.end(), greater<>());
        copy(q.begin(), q.end(), z.begin() + 2);
    }
    else
    {
        copy(r.begin(), r.end(), z.begin() + 1);
    }
    return z;
}
