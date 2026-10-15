#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct EppsteinEdge
{
    // u、v 是有向边端点，w 是非负边权。
    int u, v, w;
};

template<class Dist = long long>
vector<Dist> eppWalks(
    int n, const vector<EppsteinEdge> &e, int s, int t,
    unsigned long long k, bool keep0 = true)
{
    // n 是点数，e 是边表，s、t 是起终点，k 是要枚举的项数；返回前至多 k 小游走长度。
    assert(k >= 1 && 0 <= s && s < n && 0 <= t && t < n); // 调试检查，可删。
    // g[u] 的每项依次是终点、边权和原边编号，rg 是反图。
    vector<vector<array<int, 3>>> g(n), rg(n);
    for (int i = 0; i < (int)e.size(); i++)
    {
        auto [u, v, w] = e[i];
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        g[u].push_back({v, w, i});
        rg[v].push_back({u, w, i});
    }
    vector<optional<Dist>> d(n);
    vector<int> par(n, -1), pe(n, -1);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
    d[t] = 0;
    q.push({0, t});
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if (!d[u] || du != *d[u])
        {
            continue;
        }
        for (auto [v, w, id] : rg[u])
        {
            Dist nd = du + (Dist)w;
            if (!d[v] || nd < *d[v])
            {
                d[v] = nd;
                par[v] = u;
                pe[v] = id;
                q.push({nd, v});
            }
        }
    }
    if (!d[s])
    {
        return {};
    }

    struct Node
    {
        // l、r 是左偏堆儿子编号，rk 是右链长度，val 是偏离代价，to 是偏离边终点。
        int l = 0, r = 0, rk = 1, to = -1;
        Dist val{};
    };
    vector<Node> hp(1);
    hp[0].rk = 0; // 编号 0 是空堆，右链长度为零。
    auto meld = [&](auto &&self, int a, int b) -> int
    {
        // a、b 是持久左偏堆根编号；返回合并后新堆根，不修改旧版本。
        if (!a)
        {
            return b;
        }
        if (!b)
        {
            return a;
        }
        if (hp[a].val > hp[b].val)
        {
            swap(a, b);
        }
        int x = hp.size();
        hp.push_back(hp[a]);
        hp[x].r = self(self, hp[x].r, b);
        if (hp[hp[x].l].rk < hp[hp[x].r].rk)
        {
            swap(hp[x].l, hp[x].r);
        }
        hp[x].rk = hp[hp[x].r].rk + 1;
        return x;
    };
    vector<vector<int>> child(n);
    for (int u = 0; u < n; u++)
    {
        if (par[u] != -1)
        {
            child[par[u]].push_back(u);
        }
    }
    vector<int> root(n);
    queue<int> bfs;
    bfs.push(t);
    while (!bfs.empty())
    {
        int u = bfs.front();
        bfs.pop();
        if (par[u] != -1)
        {
            root[u] = meld(meld, root[u], root[par[u]]);
        }
        for (auto [v, w, id] : g[u])
        {
            if (id == pe[u] || !d[v])
            {
                continue;
            }
            // 该边相对最短路树的非负偏离代价。
            Dist extra = (Dist)w - *d[u] + *d[v];
            hp.push_back({0, 0, 1, v, extra});
            root[u] = meld(meld, root[u], (int)hp.size() - 1);
        }
        for (int v : child[u])
        {
            bfs.push(v);
        }
    }

    // 虚根代表不采用偏离边的最短路；s=t 时按参数决定是否跳过空游走。
    hp.push_back({0, 0, 1, s, *d[s]});
    using State = pair<Dist, int>;
    priority_queue<State, vector<State>, greater<>> pq;
    pq.push({*d[s], (int)hp.size() - 1});
    bool skip0 = s == t && !keep0;
    vector<Dist> ans;
    ans.reserve(k);
    while (!pq.empty())
    {
        auto [len, x] = pq.top();
        pq.pop();
        if (skip0)
        {
            skip0 = false;
        }
        else
        {
            ans.push_back(len);
            if (ans.size() == k)
            {
                return ans;
            }
        }
        for (int y : {hp[x].l, hp[x].r})
        {
            if (y)
            {
                pq.push({len + hp[y].val - hp[x].val, y});
            }
        }
        int r = root[hp[x].to];
        if (r)
        {
            pq.push({len + hp[r].val, r});
        }
    }
    return ans;
}

template<class Dist = long long>
optional<Dist> eppWalk(
    int n, const vector<EppsteinEdge> &e, int s, int t,
    unsigned long long k, bool keep0 = true)
{
    // 参数含义同 eppWalks；返回第 k 小游走长度，不足 k 条时返回空。
    auto ans = eppWalks<Dist>(n, e, s, t, k, keep0);
    if (ans.size() < k)
    {
        return nullopt;
    }
    return ans.back();
}
