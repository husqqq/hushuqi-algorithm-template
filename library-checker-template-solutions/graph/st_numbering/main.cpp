
#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<int>> stNumbering(const vector<vector<int>> &g, int s, int t)
{
    // g 是简单无向图；返回 st 编号的排名，无法构造时返回空。
    int n = g.size();
    assert(n >= 2 && 0 <= s && s < n && 0 <= t && t < n && s != t); // 调试检查，可删。
    vector<int> pre(n, -1), low(n), par(n, -1), order;
    pre[s] = 0;
    low[s] = s;
    order.push_back(s);
    pre[t] = 1;
    low[t] = t;
    order.push_back(t);
    auto dfs = [&](auto &&self, int u) -> void
    {
        for (int v : g[u])
        {
            assert(0 <= v && v < n && v != u); // 调试检查，可删。
            if (pre[v] == -1)
            {
                pre[v] = order.size();
                low[v] = v;
                par[v] = u;
                order.push_back(v);
                self(self, v);
                if (pre[low[v]] < pre[low[u]])
                {
                    low[u] = low[v];
                }
            }
            else if (pre[v] < pre[low[u]])
            {
                low[u] = v;
            }
        }
    };
    dfs(dfs, t);
    if ((int)order.size() != n)
    {
        return nullopt;
    }

    vector<int> prev(n, -1), next(n, -1), sign(n);
    next[s] = t;
    prev[t] = s;
    sign[s] = -1;
    for (int i = 2; i < n; i++)
    {
        int v = order[i], p = par[v];
        if (p == -1)
        {
            return nullopt;
        }
        if (sign[low[v]] == -1)
        {
            int q = prev[p];
            if (q == -1)
            {
                return nullopt;
            }
            next[q] = v;
            next[v] = p;
            prev[v] = q;
            prev[p] = v;
            sign[p] = 1;
        }
        else
        {
            int q = next[p];
            if (q == -1)
            {
                return nullopt;
            }
            next[p] = v;
            next[v] = q;
            prev[v] = p;
            prev[q] = v;
            sign[p] = -1;
        }
    }

    vector<int> rank(n, -1);
    int u = s;
    for (int i = 0; i < n && u != -1; i++)
    {
        rank[u] = i;
        u = next[u];
    }
    if (u != -1 || rank[t] != n - 1 || count(rank.begin(), rank.end(), -1))
    {
        return nullopt;
    }
    for (int v = 0; v < n; v++)
    {
        bool left = false, right = false;
        for (int w : g[v])
        {
            assert(0 <= w && w < n && w != v); // 调试检查，可删。
            left |= rank[w] < rank[v];
            right |= rank[v] < rank[w];
        }
        if ((v != s && !left) || (v != t && !right))
        {
            return nullopt;
        }
    }
    return rank;
}

signed main()
{
    int t; cin >> t;
    while (t--)
    {
        int n, m, s, z; cin >> n >> m >> s >> z;
        vector<vector<int>> g(n);
        while (m--)
        {
            int u, v; cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        auto ans = stNumbering(g, s, z);
        if (!ans) cout << "No\n";
        else
        {
            cout << "Yes\n";
            for (int i = 0; i < n; i++) cout << (*ans)[i] << " \n"[i + 1 == n];
        }
    }
}
