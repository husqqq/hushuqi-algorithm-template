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

ChordalCert chordalCertFast(int n, const vector<pair<int, int>> &edges)
{
    // edges 是简单无向边；使用桶式 MCS 与 CSR 邻接，返回 PEO 或诱导环证书。
    vector<int> deg(n), off(n + 1);
    for (auto [u, v] : edges) deg[u]++, deg[v]++;
    for (int i = 0; i < n; i++) off[i + 1] = off[i] + deg[i];
    vector<int> to(2 * edges.size()), cur = off;
    for (auto [u, v] : edges) to[cur[u]++] = v, to[cur[v]++] = u;
    vector<int> head(n + 1, -1), nx(n, -1), prv(n, -1), wt(n);
    auto erase = [&](int u)
    {
        int x = wt[u];
        if (prv[u] == -1) head[x] = nx[u]; else nx[prv[u]] = nx[u];
        if (nx[u] != -1) prv[nx[u]] = prv[u];
    };
    auto insert = [&](int u)
    {
        int x = wt[u];
        prv[u] = -1, nx[u] = head[x];
        if (nx[u] != -1) prv[nx[u]] = u;
        head[x] = u;
    };
    for (int i = 0; i < n; i++) insert(i);
    vector<int> ord;
    vector<char> used(n);
    int best = 0;
    while ((int)ord.size() < n)
    {
        while (best >= 0 && head[best] == -1) best--;
        int u = head[best];
        erase(u), used[u] = 1, ord.push_back(u);
        for (int i = off[u]; i < off[u + 1]; i++) if (!used[to[i]])
        {
            int v = to[i];
            erase(v), ++wt[v], insert(v), best = max(best, wt[v]);
        }
    }
    vector<int> pos(n), par(n, -1), child(n, -1), sibling(n, -1);
    for (int i = 0; i < n; i++) pos[ord[i]] = i;
    for (int u = 0; u < n; u++) for (int i = off[u]; i < off[u + 1]; i++)
    {
        int v = to[i];
        if (pos[v] < pos[u] && (par[u] == -1 || pos[par[u]] < pos[v])) par[u] = v;
    }
    for (int u = 0; u < n; u++) if (par[u] != -1) sibling[u] = child[par[u]], child[par[u]] = u;
    vector<int> mark(n, -1);
    int bad = -1, parent = -1, other = -1;
    for (int u = 0; u < n; u++)
    {
        for (int i = off[u]; i < off[u + 1]; i++) mark[to[i]] = u;
        for (int v = child[u]; v != -1; v = sibling[v]) for (int i = off[v]; i < off[v + 1]; i++)
            if (pos[to[i]] < pos[u] && mark[to[i]] != u) bad = v, parent = u, other = to[i];
    }
    if (bad == -1)
    {
        reverse(ord.begin(), ord.end());
        return {true, move(ord), {}};
    }
    vector<char> ban(n);
    for (int i = off[bad]; i < off[bad + 1]; i++)
        if (pos[to[i]] < pos[bad] && to[i] != parent && to[i] != other) ban[to[i]] = 1;
    vector<int> from(n, -1), q(n);
    int l = 0, r = 0;
    from[other] = other, q[r++] = other;
    while (l < r && from[parent] == -1)
    {
        int u = q[l++];
        for (int i = off[u]; i < off[u + 1]; i++)
        {
            int v = to[i];
            if (pos[v] >= pos[bad] || ban[v] || from[v] != -1) continue;
            from[v] = u, q[r++] = v;
        }
    }
    if (from[parent] == -1) return {false, {}, {bad, parent, other}};
    vector<int> path;
    for (int u = parent;; u = from[u])
    {
        path.push_back(u);
        if (u == other) break;
    }
    reverse(path.begin(), path.end());
    vector<int> cycle{bad};
    cycle.insert(cycle.end(), path.begin(), path.end());
    return {false, {}, move(cycle)};
}
