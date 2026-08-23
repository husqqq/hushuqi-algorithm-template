#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Unicycle
{
    // 点数与无向邻接表。
    int n;
    vector<vector<int>> e;
    // cycle 按顺序保存唯一环上的顶点；root[v] 记 v 所挂的环点；parent[v]、dep[v] 是 v 在挂树内的父亲与深度。
    vector<int> cycle, root, parent, dep;
    // on[u] 表示 u 是否位于环上（剥叶子后仍未被删）。
    vector<bool> on;

    Unicycle(int n = 0) : n(n), e(n)
    {
        // n 是点数；构造空图。
    }

    void add(int u, int v)
    {
        // 加入简单无向边 u-v。
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        e[u].push_back(v);
        e[v].push_back(u);
    }

    bool build()
    {
        // 重建环与挂树信息；输入不是基环树时返回 false。
        cycle.clear();
        root.clear();
        parent.clear();
        dep.clear();
        on.clear();
        int m = 0;
        vector<int> d(n);
        queue<int> q;
        for (int u = 0; u < n; u++)
        {
            d[u] = e[u].size();
            m += d[u];
            if (d[u] <= 1)
            {
                q.push(u);
            }
        }
        if (m != 2 * n)
        {
            return false;
        }
        on.assign(n, true);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            on[u] = false;
            for (int v : e[u])
            {
                if (--d[v] == 1)
                {
                    q.push(v);
                }
            }
        }
        int s = find(on.begin(), on.end(), true) - on.begin();
        if (s == n)
        {
            return false;
        }
        for (int u = 0; u < n; u++)
        {
            if (!on[u])
            {
                continue;
            }
            int cnt = 0;
            for (int v : e[u])
            {
                cnt += on[v];
            }
            if (cnt != 2)
            {
                return false;
            }
        }
        cycle = {s};
        int p = -1, u = s;
        while (true)
        {
            int v = -1;
            for (int x : e[u])
            {
                if (on[x] && x != p)
                {
                    v = x;
                    break;
                }
            }
            if (v == s)
            {
                break;
            }
            if (v < 0 || (int)cycle.size() > n)
            {
                return false;
            }
            cycle.push_back(v);
            p = u;
            u = v;
        }
        root.assign(n, -1);
        parent.assign(n, -1);
        dep.assign(n, 0);
        for (int x : cycle)
        {
            root[x] = x;
            q.push(x);
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : e[u])
            {
                if (on[v] || root[v] != -1)
                {
                    continue;
                }
                parent[v] = u;
                root[v] = root[u];
                dep[v] = dep[u] + 1;
                q.push(v);
            }
        }
        return count(on.begin(), on.end(), true) == (int)cycle.size() &&
               count(root.begin(), root.end(), -1) == 0;
    }
};
