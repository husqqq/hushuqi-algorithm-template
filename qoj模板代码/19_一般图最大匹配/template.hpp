// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 19 一般图最大匹配


#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Blossom
{
    // 点数与无向邻接表。
    int n;
    vector<vector<int>> g;
    // 当前配偶、交替森林父点、花基点和 BFS 队列。
    vector<int> match, p, base, q;
    // used 标记偶层点是否入队，flower 标记本轮被收缩的基点。
    vector<bool> used, flower;

    Blossom(int n = 0) : n(n), g(n), match(n, -1), p(n), base(n), q(n), used(n), flower(n)
    {
        // n 是点数；构造空图。
    }

    void add(int u, int v)
    {
        // u、v 是端点；自环不参与匹配，直接忽略。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        if (u == v)
        {
            return;
        }
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int lca(int a, int b)
    {
        // a、b 是同棵交替树中的偶层点；返回新花的基点。
        vector<bool> seen(n);
        while (true)
        {
            a = base[a];
            seen[a] = true;
            if (match[a] < 0)
            {
                break;
            }
            a = p[match[a]];
        }
        while (true)
        {
            b = base[b];
            if (seen[b])
            {
                return b;
            }
            b = p[match[b]];
        }
    }

    void mark(int v, int b, int x)
    {
        // 从 v 沿交替树走到基点 b，并令第一步父点为 x。
        while (base[v] != b)
        {
            flower[base[v]] = true;
            flower[base[match[v]]] = true;
            p[v] = x;
            x = match[v];
            v = p[match[v]];
        }
    }

    int path(int s)
    {
        // 从未匹配点 s 建交替树；返回另一个未匹配端点，不存在则返回 -1。
        fill(used.begin(), used.end(), false);
        fill(p.begin(), p.end(), -1);
        iota(base.begin(), base.end(), 0);
        int ql = 0, qr = 0;
        q[qr++] = s;
        used[s] = true;
        while (ql < qr)
        {
            int v = q[ql++];
            for (int u : g[v])
            {
                if (base[v] == base[u] || match[v] == u)
                {
                    continue;
                }
                if (u == s || (match[u] >= 0 && p[match[u]] >= 0))
                {
                    int b = lca(v, u);
                    fill(flower.begin(), flower.end(), false);
                    mark(v, b, u);
                    mark(u, b, v);
                    for (int i = 0; i < n; i++)
                    {
                        if (flower[base[i]])
                        {
                            base[i] = b;
                            if (!used[i])
                            {
                                used[i] = true;
                                q[qr++] = i;
                            }
                        }
                    }
                }
                else if (p[u] < 0)
                {
                    p[u] = v;
                    if (match[u] < 0)
                    {
                        return u;
                    }
                    u = match[u];
                    used[u] = true;
                    q[qr++] = u;
                }
            }
        }
        return -1;
    }

    int work()
    {
        // 从空匹配重算，返回最大匹配边数。
        fill(match.begin(), match.end(), -1);
        int ans = 0;
        for (int s = 0; s < n; s++)
        {
            if (match[s] >= 0)
            {
                continue;
            }
            int v = path(s);
            if (v < 0)
            {
                continue;
            }
            ans++;
            while (v >= 0)
            {
                int u = p[v];
                int w = u < 0 ? -1 : match[u];
                if (u >= 0)
                {
                    match[v] = u;
                    match[u] = v;
                }
                v = w;
            }
        }
        return ans;
    }
};
