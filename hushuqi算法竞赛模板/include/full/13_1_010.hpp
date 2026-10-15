#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> gameGraph(const vector<vector<int>> &g)
{
    // g[u] 是轮到行动者处于状态 u 时的全部后继；无步者负，无限对局平。
    // 返回各状态对当前行动者的结果：1 必胜、-1 必败、0 平局；编号为 [0,g.size())。
    int n = g.size();
    vector<vector<int>> rev(n);
    vector<int> left(n), ans(n);
    queue<int> q;
    for (int u = 0; u < n; u++)
    {
        left[u] = g[u].size(); // 尚未证实为对手必胜的走法数，重边也计数。
        for (int v : g[u])
        {
            assert(0 <= v && v < n);
            rev[v].push_back(u);
        }
        if (left[u] == 0)
        {
            ans[u] = -1;
            q.push(u);
        }
    }
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int u : rev[v])
        {
            if (ans[u] != 0) continue;
            if (ans[v] == -1)
            {
                ans[u] = 1;
            }
            else if (--left[u] == 0)
            {
                ans[u] = -1;
            }
            else continue;
            q.push(u);
        }
    }
    return ans;
}
