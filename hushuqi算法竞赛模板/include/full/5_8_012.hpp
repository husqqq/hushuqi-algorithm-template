#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DynStarCut
{
    struct Seg
    {
        int n;
        vector<int> mn, tag; // mn 是区间最小值，tag 是整段增量。
        Seg(int n = 0) { init(n); }
        void init(int m)
        {
            // m 是叶子数；把全部叶子初始化为无穷大。
            n = 1;
            while (n < m)
            {
                n <<= 1;
            }
            mn.assign(n * 2, numeric_limits<int>::max() / 4);
            tag.assign(n * 2, 0);
        }
        void add(int l, int r, int x, int u, int a, int b)
        {
            // [l,r) 加 x，u 覆盖 [a,b)。
            if (r <= a || b <= l)
            {
                return;
            }
            if (l <= a && b <= r)
            {
                mn[u] += x;
                tag[u] += x;
                return;
            }
            int m = (a + b) / 2;
            add(l, r, x, u * 2, a, m);
            add(l, r, x, u * 2 + 1, m, b);
            mn[u] = tag[u] + min(mn[u * 2], mn[u * 2 + 1]);
        }
        int point(int p, int u, int a, int b) const
        {
            // 返回位置 p 的当前值，u 覆盖 [a,b)。
            if (b - a == 1)
            {
                return mn[u];
            }
            int m = (a + b) / 2;
            return tag[u] + (p < m ? point(p, u * 2, a, m) : point(p, u * 2 + 1, m, b));
        }
        void add(int l, int r, int x) { add(l, r, x, 1, 0, n); }
        void set(int p, int x) { add(p, p + 1, x - point(p, 1, 0, n)); }
        int allMin() const { return mn[1]; }
    };

    int n;
    vector<vector<int>> tree; // Stoer-Wagner 收缩树的子节点。
    vector<int> par, dep, heavy, head, pos, val; // HLD 父、深度、重儿子、链头、位置与星边权。
    Seg seg;

    void buildCutTree(vector<vector<pair<int, int>>> g)
    {
        // g 是原图的非零权邻接表；构造对应的收缩二叉树。
        tree.assign(2 * n - 1, {});
        vector<char> alive(2 * n - 1, true);
        for (int step = 0; step < n - 1; step++)
        {
            g.push_back({});
            vector<int> ord(n - step), cost(n + step);
            vector<char> used(n + step);
            priority_queue<pair<int, int>> q;
            for (int i = 0; i < n + step; i++)
            {
                if (alive[i])
                {
                    for (auto [j, w] : g[i])
                    {
                        cost[i] += w;
                    }
                    q.push({-cost[i], i});
                }
            }
            for (int k = 0; k < n - step; k++)
            {
                while (true)
                {
                    auto [neg, v] = q.top();
                    q.pop();
                    if (used[v])
                    {
                        continue;
                    }
                    used[v] = true;
                    ord[k] = v;
                    for (auto [to, w] : g[v])
                    {
                        if (!used[to])
                        {
                            cost[to] -= w;
                            q.push({-cost[to], to});
                        }
                    }
                    break;
                }
            }
            int x = ord[n - step - 1], y = ord[n - step - 2], z = n + step;
            g[x].clear();
            g[y].clear();
            alive[x] = alive[y] = false;
            for (int i = 0; i < n + step; i++)
            {
                if (!alive[i])
                {
                    continue;
                }
                for (auto &e : g[i])
                {
                    if (e.first == x || e.first == y)
                    {
                        e.first = z;
                        g[z].push_back({i, e.second});
                    }
                }
            }
            tree[z] = {x, y};
        }
    }

    int dfsSize(int u)
    {
        // u 是收缩树节点；设置父、深度和重儿子并返回子树大小。
        int size = 1, best = 0;
        for (int v : tree[u])
        {
            par[v] = u;
            dep[v] = dep[u] + 1;
            int s = dfsSize(v);
            size += s;
            if (s > best)
            {
                best = s;
                heavy[u] = v;
            }
        }
        return size;
    }

    void buildHld()
    {
        // 无参数；为收缩树建立重链编号并初始化线段树。
        int all = tree.size(), root = all - 1;
        par.assign(all, -1);
        dep.assign(all, 0);
        heavy.assign(all, -1);
        head.assign(all, -1);
        pos.assign(all, -1);
        dfsSize(root);
        queue<int> q;
        q.push(root);
        int timer = 0;
        while (!q.empty())
        {
            int h = q.front();
            q.pop();
            for (int u = h; u != -1; u = heavy[u])
            {
                head[u] = h;
                pos[u] = timer++;
                for (int v : tree[u])
                {
                    if (v != heavy[u])
                    {
                        q.push(v);
                    }
                }
            }
        }
        seg.init(all);
    }

    void addRootPath(int u, int x)
    {
        // u 是收缩树节点，x 是增量；把根到 u 的节点权整体加 x。
        while (u != -1)
        {
            seg.add(pos[head[u]], pos[u] + 1, x);
            u = par[head[u]];
        }
    }

    DynStarCut(int n, const vector<array<int, 3>> &ed, vector<int> a) : n(n), val(move(a))
    {
        // n 是原图点数，ed 每项为 {u,v,w}，val 是新星点到各原点的初始边权。
        assert(n >= 1 && (int)val.size() == n);
        vector<vector<pair<int, int>>> g(n);
        for (auto [u, v, w] : ed)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && w >= 0);
            if (w)
            {
                g[u].push_back({v, w});
                g[v].push_back({u, w});
            }
        }
        buildCutTree(move(g));
        buildHld();
        vector<int> base(tree.size());
        for (auto [u, v, w] : ed)
        {
            if (w == 0)
            {
                continue;
            }
            while (u != v)
            {
                if (dep[u] < dep[v])
                {
                    swap(u, v);
                }
                base[u] += w;
                u = par[u];
            }
        }
        for (int u = 0; u < (int)tree.size(); u++)
        {
            seg.set(pos[u], base[u]);
        }
        for (int u = 0; u < n; u++)
        {
            addRootPath(u, val[u]);
        }
    }

    int update(int u, int x)
    {
        // u 是被修改的原图点，x 是新星边权；更新后返回增广图的全局最小割值。
        assert(0 <= u && u < n && x >= 0);
        addRootPath(u, x - val[u]);
        val[u] = x;
        return seg.allMin();
    }
};
