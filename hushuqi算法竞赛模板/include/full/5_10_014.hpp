#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct TreeDecomp2
{
    // bag 是各树节点包含的原图点集，edge 是 bag 树的边。
    vector<vector<int>> bag;
    vector<pair<int, int>> edge;
};

optional<TreeDecomp2> treeDecomp2(int n, const vector<pair<int, int>> &edge)
{
    // n 是点数，edge 是简单无向边；返回宽度至多 2 的树分解，不存在时返回空。
    vector<set<int>> g(n);
    for (auto [u, v] : edge)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        g[u].insert(v);
        g[v].insert(u);
    }
    struct Event
    {
        int d, v, a, b;
    };
    vector<Event> ev;
    vector<bool> alive(n, true);
    queue<int> q;
    for (int u = 0; u < n; u++)
    {
        q.push(u);
    }
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        if (!alive[v] || g[v].size() >= 3)
        {
            continue;
        }
        int d = g[v].size(), a = -1, b = -1;
        if (d >= 1)
        {
            a = *g[v].begin();
        }
        if (d == 2)
        {
            b = *g[v].rbegin();
            g[a].insert(b);
            g[b].insert(a);
        }
        ev.push_back({d, v, a, b});
        alive[v] = false;
        for (int u : g[v])
        {
            g[u].erase(v);
            q.push(u);
        }
    }
    if (count(alive.begin(), alive.end(), true))
    {
        return nullopt;
    }

    reverse(ev.begin(), ev.end());
    TreeDecomp2 ans;
    vector<int> one(n, -1);
    map<pair<int, int>, int> two;
    for (auto [d, v, a, b] : ev)
    {
        int id = ans.bag.size(), p = -1;
        ans.bag.push_back({v});
        if (d >= 1)
        {
            ans.bag.back().push_back(a);
        }
        if (d == 2)
        {
            ans.bag.back().push_back(b);
            auto it = two.find(minmax(a, b));
            if (it == two.end())
            {
                return nullopt;
            }
            p = it->second;
        }
        else if (d == 1)
        {
            p = one[a];
            if (p < 0)
            {
                return nullopt;
            }
        }
        else if (id > 0)
        {
            p = id - 1;
        }
        if (p >= 0)
        {
            ans.edge.push_back({p, id});
        }
        for (int x : ans.bag.back())
        {
            one[x] = id;
        }
        for (int x : ans.bag.back())
        {
            for (int y : ans.bag.back())
            {
                if (x < y)
                {
                    two[{x, y}] = id;
                }
            }
        }
    }
    return ans;
}
