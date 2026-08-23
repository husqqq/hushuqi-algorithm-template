#pragma once

#include "12_1_001.hpp"

vector<int> windowDP(const vector<int> &c, int k)
{
    if (c.empty())
    {
        return {};
    }
    assert(k > 0);
    int n = c.size();
    vector<int> f(n);
    deque<int> q;
    f[0] = c[0];
    q.push_back(0);
    for (int i = 1; i < n; i++)
    {
        while (!q.empty() && q.front() < i - k)
        {
            q.pop_front();
        }
        f[i] = f[q.front()] + c[i];
        while (!q.empty() && f[q.back()] >= f[i])
        {
            q.pop_back();
        }
        q.push_back(i);
    }
    return f;
}
