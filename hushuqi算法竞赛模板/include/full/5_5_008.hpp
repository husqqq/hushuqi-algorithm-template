#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DEdgeMST
{
    // u、v 是有向边的起点和终点。
    int u, v;
    // w 是当前收缩图中的边权。
    __int128 w;
};

optional<__int128> directedMST(int n, int root, vector<DEdgeMST> e)
{
    // n 是点数，root 是树根，e 是有向边集；返回最小树形图权值，不存在时返回空。
    assert(n >= 1 && 0 <= root && root < n); // 调试检查，可删。
    __int128 ans = 0;
    while (true)
    {
        vector<optional<__int128>> in(n);
        vector<int> pre(n, -1);
        for (auto [u, v, w] : e)
        {
            if (u != v && (!in[v] || w < *in[v]))
            {
                in[v] = w;
                pre[v] = u;
            }
        }
        in[root] = 0;
        for (int u = 0; u < n; u++)
        {
            if (!in[u])
            {
                return nullopt;
            }
            ans += *in[u];
        }
        int cnt = 0;
        vector<int> id(n, -1), vis(n, -1);
        for (int s = 0; s < n; s++)
        {
            int u = s;
            while (vis[u] != s && id[u] < 0 && u != root)
            {
                vis[u] = s;
                u = pre[u];
            }
            if (u != root && id[u] < 0)
            {
                for (int v = pre[u]; v != u; v = pre[v])
                {
                    id[v] = cnt;
                }
                id[u] = cnt++;
            }
        }
        if (!cnt)
        {
            return ans;
        }
        for (int u = 0; u < n; u++)
        {
            if (id[u] < 0)
            {
                id[u] = cnt++;
            }
        }
        for (auto &[u, v, w] : e)
        {
            int old = v;
            u = id[u];
            v = id[v];
            if (u != v)
            {
                w -= *in[old];
            }
        }
        root = id[root];
        n = cnt;
    }
}

struct DMSTResult
{
    // weight 是最小树形图总权，parent[u] 是 u 的父点且 parent[root]=root。
    long long weight;
    vector<int> parent;
};

struct DMSTFastEdge
{
    // u、v 是有向边端点，w 是非负或负的长整型边权。
    int u, v;
    long long w;
};

optional<DMSTResult> dmstTree(int n, int root, const vector<DMSTFastEdge> &e)
{
    // n 是点数，root 是根，e 是边集；返回最小树形图总权和每点父点，不存在时返回空。
    assert(n >= 1 && 0 <= root && root < n); // 调试检查，可删。
    struct Edge
    {
        // u、v 是边端点，w 是原始边权。
        int u, v;
        long long w;
    };
    struct Heap
    {
        struct Node
        {
            // e 是堆元素，lazy 是子树加法标记，child、next 构成配对堆的儿子链。
            Edge e;
            long long lazy = 0;
            shared_ptr<Node> child, next;
            Node(Edge e) : e(e) {}
        };
        shared_ptr<Node> rt;
        int sz = 0;

        shared_ptr<Node> meld(shared_ptr<Node> a, shared_ptr<Node> b)
        {
            // a、b 是两棵配对堆根；返回合并后的堆根。
            if (!a) return b;
            if (!b) return a;
            if (a->e.w + a->lazy > b->e.w + b->lazy) swap(a, b);
            b->lazy -= a->lazy;
            b->next = a->child;
            a->child = b;
            return a;
        }

        void push(Edge e)
        {
            // e 是新边；插入堆，无返回值。
            rt = meld(rt, make_shared<Node>(e));
            sz++;
        }

        Edge top() const
        {
            // 无参数；返回当前键值最小的原始边。
            return rt->e;
        }

        void pop()
        {
            // 无参数；删除当前键值最小边，无返回值。
            vector<shared_ptr<Node>> a;
            for (auto p = rt->child; p;)
            {
                auto x = p;
                p = p->next;
                x->next = nullptr;
                x->lazy += rt->lazy;
                if (p)
                {
                    auto y = p;
                    p = p->next;
                    y->next = nullptr;
                    y->lazy += rt->lazy;
                    a.push_back(meld(x, y));
                }
                else
                {
                    a.push_back(x);
                }
            }
            rt = nullptr;
            for (int i = (int)a.size() - 1; i >= 0; i--) rt = meld(rt, a[i]);
            sz--;
        }

        void absorb(Heap &other)
        {
            // other 是待合并堆；合并后清空 other，无返回值。
            rt = meld(rt, other.rt);
            sz += other.sz;
            other.rt = nullptr;
            other.sz = 0;
        }

        void setOffset(long long x)
        {
            // x 是当前堆根的绝对懒标记；设置收缩后的边权偏移，无返回值。
            rt->lazy = x;
        }
    };

    vector<Heap> heap(2 * n);
    for (auto [u, v, w] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        heap[v].push({u, v, w});
    }
    vector<int> up(2 * n, -1), dsu(2 * n, -1), vis(2 * n);
    vector<Edge> pick(2 * n);
    auto find = [&](auto &&self, int u) -> int
    {
        // u 是收缩图节点；返回并查集代表。
        return dsu[u] == -1 ? u : dsu[u] = self(self, dsu[u]);
    };
    int tot = n, stamp = 1;
    vis[root] = 1;
    for (int s = 0; s < n; s++)
    {
        if (vis[s]) continue;
        int u = s;
        stamp++;
        while (!vis[u] || vis[u] == stamp)
        {
            if (vis[u] == stamp)
            {
                int c = tot++, v = u;
                do
                {
                    heap[v].setOffset(-pick[v].w);
                    heap[c].absorb(heap[v]);
                    up[v] = dsu[v] = c;
                    v = find(find, pick[v].u);
                } while (v != c);
                u = c;
            }
            vis[u] = stamp;
            while (heap[u].sz && find(find, heap[u].top().u) == u) heap[u].pop();
            if (!heap[u].sz) return nullopt;
            pick[u] = heap[u].top();
            u = find(find, pick[u].u);
        }
    }
    vector<bool> used(tot);
    vector<int> parent(n, root);
    long long sum = 0;
    for (int i = tot - 1; i >= 0; i--)
    {
        if (i == root || used[i]) continue;
        int u = pick[i].v;
        while (u != -1 && !used[u])
        {
            used[u] = true;
            u = up[u];
        }
        sum += pick[i].w;
        parent[pick[i].v] = pick[i].u;
    }
    parent[root] = root;
    return DMSTResult{sum, parent};
}
