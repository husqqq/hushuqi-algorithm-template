#pragma once

#include "../base.hpp"

template <long long P> struct TreeAffEdge
{
    int u = 0, v = 0;
    long long a = 1, b = 0;
};

template <long long P>
vector<long long> treeAffRoots(const vector<long long> &value,
                                     const vector<TreeAffEdge<P>> &edges)
{
    // value 是点初值，边函数为 a*x+b；返回每个点作根时所有点沿路径传到根后的值之和。
    int n = value.size();
    assert(n > 0 && (int)edges.size() + 1 == n);
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        auto e = edges[i];
        g[e.u].push_back({e.v, i});
        g[e.v].push_back({e.u, i});
    }
    vector<int> parent(n, -1), parEdge(n, -1), order{0}, size(n, 1);
    for (int i = 0; i < (int)order.size(); i++)
    {
        int u = order[i];
        for (auto [v, id] : g[u]) if (v != parent[u])
        {
            parent[v] = u;
            parEdge[v] = id;
            order.push_back(v);
        }
    }
    assert((int)order.size() == n);
    vector<long long> down(n), answer(n);
    for (int i = n - 1; i >= 0; i--)
    {
        int u = order[i];
        down[u] = (value[u] % P + P) % P;
        for (auto [v, id] : g[u]) if (parent[v] == u)
        {
            auto e = edges[id];
            down[u] = (down[u] + e.a * down[v] + e.b * size[v]) % P;
            size[u] += size[v];
        }
    }
    answer[0] = down[0];
    for (int i = 1; i < n; i++)
    {
        int v = order[i], u = parent[v];
        auto e = edges[parEdge[v]];
        long long child = (e.a * down[v] + e.b * size[v]) % P;
        long long outside = (answer[u] - child + P) % P;
        answer[v] = (down[v] + e.a * outside + e.b * (n - size[v])) % P;
    }
    return answer;
}

template <long long P> class DynTreeAff
{
public:
    struct Info
    {
        bool vertex = true;
        long long a = 0, b = 0;
    };

private:
    struct Path
    {
        long long a = 1, b = 0, sum = 0, count = 0;
    };
    struct Node
    {
        int p = 0, ch[2]{};
        bool rev = false;
        Info info;
        long long virSum = 0, virCnt = 0;
        Path forward, backward;
    };
    vector<Node> t;

    static long long norm(long long x)
    {
        x %= P;
        return x < 0 ? x + P : x;
    }
    static Path compress(const Path &upper, const Path &lower)
    {
        // upper 靠近根、lower 远离根；合并后保留从 lower 向 upper 传值的聚合。
        return {upper.a * lower.a % P,
                (upper.a * lower.b + upper.b) % P,
                (upper.sum + upper.a * lower.sum + upper.b * lower.count) % P,
                upper.count + lower.count};
    }
    Path key(int x) const
    {
        const Node &u = t[x];
        if (u.info.vertex)
        {
            return {1, 0, (u.virSum + u.info.a) % P, u.virCnt + 1};
        }
        return {u.info.a, u.info.b,
                (u.info.a * u.virSum + u.info.b * u.virCnt) % P,
                u.virCnt};
    }
    bool auxRoot(int x) const
    {
        int p = t[x].p;
        return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
    }
    void pull(int x)
    {
        Path k = key(x);
        int l = t[x].ch[0], r = t[x].ch[1];
        t[x].forward = compress(compress(t[l].forward, k), t[r].forward);
        t[x].backward = compress(compress(t[r].backward, k), t[l].backward);
    }
    void toggle(int x)
    {
        if (!x) return;
        swap(t[x].ch[0], t[x].ch[1]);
        swap(t[x].forward, t[x].backward);
        t[x].rev = !t[x].rev;
    }
    void push(int x)
    {
        if (!t[x].rev) return;
        toggle(t[x].ch[0]);
        toggle(t[x].ch[1]);
        t[x].rev = false;
    }
    void rotate(int x)
    {
        int y = t[x].p, z = t[y].p;
        int d = t[y].ch[1] == x, b = t[x].ch[d ^ 1];
        if (!auxRoot(y)) t[z].ch[t[z].ch[1] == y] = x;
        t[x].p = z;
        t[x].ch[d ^ 1] = y;
        t[y].p = x;
        t[y].ch[d] = b;
        if (b) t[b].p = y;
        pull(y);
        pull(x);
    }
    void splay(int x)
    {
        vector<int> stack{x};
        for (int y = x; !auxRoot(y); y = t[y].p) stack.push_back(t[y].p);
        while (!stack.empty()) push(stack.back()), stack.pop_back();
        while (!auxRoot(x))
        {
            int y = t[x].p, z = t[y].p;
            if (!auxRoot(y)) rotate((t[y].ch[1] == x) == (t[z].ch[1] == y) ? y : x);
            rotate(x);
        }
    }
    void addVirtual(int parent, int child, int sign)
    {
        if (!child) return;
        const Path &v = t[child].forward;
        t[parent].virSum = norm(t[parent].virSum + sign * v.sum);
        t[parent].virCnt += sign * v.count;
    }
    void access(int x)
    {
        int last = 0;
        for (int y = x; y; y = t[y].p)
        {
            splay(y);
            addVirtual(y, t[y].ch[1], 1);
            addVirtual(y, last, -1);
            t[y].ch[1] = last;
            pull(y);
            last = y;
        }
        splay(x);
    }
    void makeRootImpl(int x)
    {
        access(x);
        toggle(x);
    }
    int findRoot(int x)
    {
        access(x);
        push(x);
        while (t[x].ch[0]) x = t[x].ch[0], push(x);
        splay(x);
        return x;
    }

public:
    explicit DynTreeAff(const vector<Info> &info = {}) : t(info.size() + 1)
    {
        // info 中点节点取 {true,value,0}，边节点取 {false,a,b}。
        for (int i = 0; i < (int)info.size(); i++)
        {
            t[i + 1].info = info[i];
            t[i + 1].info.a = norm(t[i + 1].info.a);
            t[i + 1].info.b = norm(t[i + 1].info.b);
            pull(i + 1);
        }
    }
    bool link(int u, int v)
    {
        // u、v 是不同树中的节点；连边并返回是否成功。
        u++, v++;
        makeRootImpl(u);
        if (findRoot(v) == u) return false;
        access(v);
        t[u].p = v;
        addVirtual(v, u, 1);
        pull(v);
        return true;
    }
    bool cut(int u, int v)
    {
        // u、v 必须是直接相邻节点；删边并返回是否成功。
        u++, v++;
        makeRootImpl(u);
        access(v);
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = t[u].p = 0;
        pull(v);
        return true;
    }
    void setVertex(int u, long long value)
    {
        // u 是点节点编号；更新其点值。
        u++;
        access(u);
        t[u].info = {true, norm(value), 0};
        pull(u);
    }
    void setEdge(int u, long long a, long long b)
    {
        // u 是边节点编号；更新其一次函数 a*x+b。
        u++;
        access(u);
        t[u].info = {false, norm(a), norm(b)};
        pull(u);
    }
    long long query(int root)
    {
        // root 是点节点编号；返回所有点的值沿路径传到 root 后的总和。
        root++;
        makeRootImpl(root);
        return t[root].forward.sum;
    }
};
