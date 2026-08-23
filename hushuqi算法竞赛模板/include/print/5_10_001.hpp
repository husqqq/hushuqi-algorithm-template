#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Chordal
{
    // ok 表示图是否为弦图。
    bool ok;
    // peo 保存最大势搜索得到的完美消除序列，弦图时有效。
    vector<int> peo;
    // 非弦图时，bad 是破坏候选 PEO 的三元组 (u,p,v)。
    array<int, 3> bad{-1, -1, -1};
};

Chordal chordal(const vector<vector<int>> &g)
{
    // g 是简单无向图邻接表；返回弦图判定、候选 PEO 和失败三元组。
    int n = g.size();
    vector<int> w(n), pos(n), peo;
    vector<bool> used(n);
    for (int k = 0; k < n; k++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!used[i] && (u == -1 || w[i] > w[u]))
            {
                u = i;
            }
        }
        used[u] = true;
        peo.push_back(u);
        for (int v : g[u])
        {
            if (!used[v])
            {
                w[v]++;
            }
        }
    }
    reverse(peo.begin(), peo.end());
    for (int i = 0; i < n; i++)
    {
        pos[peo[i]] = i;
    }
    vector<int> mark(n, -1);
    for (int i = 0; i < n; i++)
    {
        int u = peo[i], p = -1;
        for (int v : g[u])
        {
            if (pos[v] > i && (p == -1 || pos[v] < pos[p]))
            {
                p = v;
            }
        }
        if (p == -1)
        {
            continue;
        }
        for (int v : g[p])
        {
            mark[v] = u;
        }
        for (int v : g[u])
        {
            if (v != p && pos[v] > i && mark[v] != u)
            {
                return {false, peo, {u, p, v}};
            }
        }
    }
    return {true, peo};
}

struct ChordalCert
{
    // ok 表示图是否为弦图。
    bool ok;
    // peo 是弦图的一组完美消除序列；非弦图时为空。
    vector<int> peo;
    // cycle 是非弦图的一条诱导环；弦图时为空。
    vector<int> cycle;
};

ChordalCert chordalCert(int n, const vector<pair<int, int>> &e)
{
    // n 是点数，e 是简单无向边；返回 PEO 或长度至少四的诱导环证书。
    assert(n >= 0); // 调试检查，可删。
    vector<set<int>> g(n);
    for (auto [u, v] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
        g[u].insert(v);
        g[v].insert(u);
    }
    vector<int> w(n), pos(n), ord;
    set<pair<int, int>> cand;
    for (int u = 0; u < n; u++) cand.insert({0, u});
    while (!cand.empty())
    {
        int u = prev(cand.end())->second;
        cand.erase(prev(cand.end()));
        pos[u] = ord.size();
        ord.push_back(u);
        w[u] = -1;
        for (int v : g[u])
        {
            if (w[v] != -1)
            {
                cand.erase({w[v], v});
                cand.insert({++w[v], v});
            }
        }
    }
    for (int u : ord)
    {
        int pre = -1;
        for (int v : g[u])
        {
            if (pos[v] < pos[u]) pre = max(pre, pos[v]);
        }
        if (pre == -1) continue;
        int p = ord[pre];
        for (int s : g[u])
        {
            if (pos[s] >= pos[p] || g[p].contains(s)) continue;
            vector<int> from(n, -1);
            queue<int> q;
            from[s] = s;
            q.push(s);
            while (!q.empty() && from[p] == -1)
            {
                int x = q.front();
                q.pop();
                for (int v : g[x])
                {
                    if (v == u || (v != p && g[u].contains(v)) || from[v] != -1) continue;
                    from[v] = x;
                    q.push(v);
                }
            }
            assert(from[p] != -1); // MCS 反例保证该路径存在。
            vector<int> cyc;
            for (int x = p; x != s; x = from[x]) cyc.push_back(x);
            cyc.push_back(s);
            cyc.push_back(u);
            return {false, {}, move(cyc)};
        }
    }
    reverse(ord.begin(), ord.end());
    return {true, move(ord), {}};
}
