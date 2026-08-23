#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<int>> bipartite(const vector<vector<int>> &g)
{
    // g 是无向邻接表；返回 0/1 颜色，存在奇环时返回空。
    vector<int> col(g.size(), -1);
    for (int s = 0; s < (int)g.size(); s++)
    {
        if (col[s] != -1)
        {
            continue;
        }
        queue<int> q;
        col[s] = 0;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : g[u])
            {
                if (col[v] == -1)
                {
                    col[v] = col[u] ^ 1;
                    q.push(v);
                }
                else if (col[v] == col[u])
                {
                    return nullopt;
                }
            }
        }
    }
    return col;
}
