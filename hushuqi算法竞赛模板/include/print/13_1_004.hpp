#pragma once

#include "13_1_001.hpp"

vector<int> sg(const vector<vector<int>> &to)
{
    // to[x] 是状态 x 的后继编号表；返回每个 DAG 状态的 Sprague-Grundy 值。
    int n = to.size();
    vector<int> deg(n);
    for (int x = 0; x < n; x++)
    {
        for (int y : to[x])
        {
            assert(0 <= y && y < n); // 调试检查，可删。
            deg[y]++;
        }
    }
    queue<int> q;
    for (int x = 0; x < n; x++)
    {
        if (deg[x] == 0)
        {
            q.push(x);
        }
    }
    vector<int> ord;
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        ord.push_back(x);
        for (int y : to[x])
        {
            deg[y]--;
            if (deg[y] == 0)
            {
                q.push(y);
            }
        }
    }
    assert((int)ord.size() == n); // 调试检查，可删。
    reverse(ord.begin(), ord.end());
    vector<int> f(n), seen(n + 1, -1);
    for (int x : ord)
    {
        for (int y : to[x])
        {
            seen[f[y]] = x; // 用状态编号作时间戳，无需逐点清空。
        }
        int g = 0;
        while (seen[g] == x)
        {
            g++;
        }
        f[x] = g;
    }
    return f;
}
