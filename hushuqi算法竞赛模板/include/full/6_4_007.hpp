#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

// Euler-tour treap：连通块点权和与整块加；不提供 LCT 路径聚合。
class EulerTourTree
{
    struct Node
    {
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        unsigned long long pri;
        int size = 1, count;
        bool own;
        __int128 val = 0, sum = 0, lazy = 0;
        explicit Node(unsigned long long key, bool vertex) : pri(key), count(vertex), own(vertex) {}
    };
    vector<unique_ptr<Node>> pool;
    vector<Node *> vertex;
    map<pair<int, int>, pair<Node *, Node *>> edge;
    mt19937_64 rng{712367};
    static int size(Node *x) { return x ? x->size : 0; }
    static int count(Node *x) { return x ? x->count : 0; }
    static __int128 sum(Node *x) { return x ? x->sum : 0; }
    static void apply(Node *x, __int128 d)
    {
        if (!x) return;
        if (x->own) x->val += d;
        x->sum += d * x->count; x->lazy += d;
    }
    static void push(Node *x) { if (x && x->lazy) { apply(x->l, x->lazy); apply(x->r, x->lazy); x->lazy = 0; } }
    static void pull(Node *x)
    {
        // 点节点贡献 val；边节点 val 永远为 0。
        x->size = 1 + size(x->l) + size(x->r);
        x->count = x->own + count(x->l) + count(x->r);
        x->sum = x->val + sum(x->l) + sum(x->r);
        if (x->l) x->l->p = x; if (x->r) x->r->p = x;
    }
    static Node *merge(Node *a, Node *b)
    {
        if (!a) { if (b) b->p = nullptr; return b; }
        if (!b) { a->p = nullptr; return a; }
        push(a); push(b);
        if (a->pri > b->pri) { a->r = merge(a->r, b); pull(a); a->p = nullptr; return a; }
        b->l = merge(a, b->l); pull(b); b->p = nullptr; return b;
    }
    static pair<Node *, Node *> split(Node *x, int k)
    {
        if (!x) return {nullptr, nullptr}; push(x);
        if (size(x->l) >= k)
        {
            auto [a, b] = split(x->l, k); x->l = b; pull(x); x->p = nullptr; if (a) a->p = nullptr; return {a, x};
        }
        auto [a, b] = split(x->r, k - size(x->l) - 1); x->r = a; pull(x); x->p = nullptr; if (b) b->p = nullptr; return {x, b};
    }
    static Node *root(Node *x) { while (x->p) x = x->p; return x; }
    static int index(Node *x)
    {
        int i = size(x->l); while (x->p) { if (x == x->p->r) i += size(x->p->l) + 1; x = x->p; } return i;
    }
    Node *make(bool v) { pool.push_back(make_unique<Node>(rng(), v)); return pool.back().get(); }
    Node *reroot(int u) { Node *x = vertex.at(u); auto [a, b] = split(root(x), index(x)); return merge(b, a); }
public:
    explicit EulerTourTree(int n) : vertex(n)
    {
        for (auto &x : vertex) x = make(true);
    }
    bool connected(int u, int v) const { return root(vertex.at(u)) == root(vertex.at(v)); }
    bool link(int u, int v)
    {
        if (connected(u, v)) return false;
        Node *a = reroot(u), *b = reroot(v), *uv = make(false), *vu = make(false);
        edge[minmax(u, v)] = {uv, vu}; merge(merge(merge(a, uv), b), vu); return true;
    }
    bool cut(int u, int v)
    {
        auto it = edge.find(minmax(u, v)); if (it == edge.end()) return false;
        auto [x, y] = it->second; int i = index(x), j = index(y); if (i > j) swap(i, j);
        auto [a, rest] = split(root(x), i); auto [one, tail] = split(rest, 1);
        auto [b, last] = split(tail, j - i - 1); auto [two, c] = split(last, 1);
        merge(a, c); edge.erase(it); return true;
    }
    __int128 componentSum(int u) const { return root(vertex.at(u))->sum; }
    int componentSize(int u) const { return root(vertex.at(u))->count; }
    void componentAdd(int u, long long d) { apply(root(vertex.at(u)), d); }
    void add(int u, long long d)
    {
        Node *x = vertex.at(u); vector<Node *> path;
        for (Node *y = x; y; y = y->p) path.push_back(y);
        for (auto it = path.rbegin(); it != path.rend(); ++it) push(*it);
        x->val += d; for (Node *y : path) pull(y);
    }
};
