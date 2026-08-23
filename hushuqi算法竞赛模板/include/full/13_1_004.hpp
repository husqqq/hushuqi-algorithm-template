#pragma once

#include "13_1_001.hpp"

vector<int> sg(const vector<vector<int>> &to)
{
    int n = to.size();
    vector<int> deg(n);
    for (int x = 0; x < n; x++)
    {
        for (int y : to[x])
        {
            assert(0 <= y && y < n);
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
    assert((int)ord.size() == n);
    reverse(ord.begin(), ord.end());
    vector<int> f(n);
    for (int x : ord)
    {
        vector<int> a;
        for (int y : to[x])
        {
            a.push_back(f[y]);
        }
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        int g = 0;
        while (binary_search(a.begin(), a.end(), g))
        {
            g++;
        }
        f[x] = g;
    }
    return f;
}
