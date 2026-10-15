#pragma once

#include "12_1_001.hpp"

vector<int> windowDP(const vector<int> &c, int k)
{
    // c[i] 是进入状态 i 的代价，k 是前驱窗口长度；返回 f[0]=c[0] 的最小代价 DP。
    if (c.empty())
    {
        return {};
    }
    assert(k > 0); // 调试检查，可删。
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
