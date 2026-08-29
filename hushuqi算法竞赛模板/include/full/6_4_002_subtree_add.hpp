#pragma once

#include "../base.hpp"

template <class Acc = long long> class DynSubAddSum
{
    struct Node
    {
        int p = 0, ch[2]{};
        bool rev = false;
        int size = 1, virSize = 0;
        Acc value = 0, sum = 0, virSum = 0;
        Acc added = 0, parAdd = 0;
    };
    vector<Node> t;

    bool auxRoot(int x) const
    {
        int p = t[x].p;
        return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
    }
    void applyNode(int x, Acc delta)
    {
        if (!x) return;
        t[x].value += delta;
        t[x].sum += delta * t[x].size;
        t[x].virSum += delta * t[x].virSize;
        t[x].added += delta;
    }
    void push(int x)
    {
        if (!x) return;
        if (t[x].p)
        {
            applyNode(x, t[t[x].p].added - t[x].parAdd);
            t[x].parAdd = t[t[x].p].added;
        }
        if (t[x].rev)
        {
            if (t[x].ch[0]) t[t[x].ch[0]].rev = !t[t[x].ch[0]].rev;
            if (t[x].ch[1]) t[t[x].ch[1]].rev = !t[t[x].ch[1]].rev;
            swap(t[x].ch[0], t[x].ch[1]);
            t[x].rev = false;
        }
    }
    void pull(int x)
    {
        int l = t[x].ch[0], r = t[x].ch[1];
        push(l), push(r);
        t[x].size = 1 + t[l].size + t[r].size + t[x].virSize;
        t[x].sum = t[x].value + t[l].sum + t[r].sum + t[x].virSum;
    }
    void rotate(int x)
    {
        int y = t[x].p, z = t[y].p, d = t[y].ch[1] == x;
        int middle = t[x].ch[d ^ 1];
        push(middle);
        t[y].ch[d] = middle;
        if (middle)
        {
            t[middle].p = y;
            t[middle].parAdd = t[y].added;
        }
        t[x].ch[d ^ 1] = y;
        t[x].p = z;
        t[x].parAdd = t[z].added;
        if (z)
        {
            if (t[z].ch[0] == y) t[z].ch[0] = x;
            if (t[z].ch[1] == y) t[z].ch[1] = x;
        }
        t[y].p = x;
        t[y].parAdd = t[x].added;
        pull(y), pull(x);
    }
    void splay(int x)
    {
        while (!auxRoot(x))
        {
            int y = t[x].p;
            if (auxRoot(y))
            {
                push(y), push(x);
                rotate(x);
            }
            else
            {
                int z = t[y].p;
                push(z), push(y), push(x);
                bool same = (t[z].ch[0] == y) == (t[y].ch[0] == x);
                if (same) rotate(y), rotate(x);
                else rotate(x), rotate(x);
            }
        }
        push(x);
    }
    void access(int x)
    {
        int pre = 0;
        for (int cur = x; cur; cur = t[cur].p)
        {
            splay(cur);
            int old = t[cur].ch[1];
            if (old)
            {
                push(old);
                t[cur].virSize += t[old].size;
                t[cur].virSum += t[old].sum;
            }
            t[cur].ch[1] = pre;
            if (pre)
            {
                push(pre);
                t[cur].virSize -= t[pre].size;
                t[cur].virSum -= t[pre].sum;
            }
            pull(cur);
            pre = cur;
        }
        splay(x);
    }
    void makeRootImpl(int x)
    {
        access(x);
        t[x].rev = !t[x].rev;
        push(x);
    }

public:
    explicit DynSubAddSum(const vector<Acc> &value = {}) : t(value.size() + 1)
    {
        t[0].size = 0;
        for (int i = 0; i < (int)value.size(); i++) t[i + 1].value = t[i + 1].sum = value[i];
    }
    bool link(int child, int parent)
    {
        // child、parent 属于不同树；连边并返回是否成功。
        child++, parent++;
        access(child);
        access(parent);
        t[parent].ch[1] = child;
        t[child].p = parent;
        t[child].parAdd = t[parent].added;
        pull(parent);
        return true;
    }
    bool cut(int u, int v)
    {
        // u、v 是直接边端点；删除该边并返回是否成功。
        u++, v++;
        makeRootImpl(u);
        access(v);
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = 0;
        t[u].p = 0;
        t[u].parAdd = 0;
        pull(v);
        return true;
    }
    void addTree(int root, Acc delta)
    {
        // root 所在连通树当前已按 root 暴露；给整棵树加 delta。
        root++;
        access(root);
        applyNode(root, delta);
    }
    Acc treeSum(int root)
    {
        // 返回 root 所在整棵树的点权和。
        root++;
        access(root);
        return t[root].sum;
    }
};

template <class Acc = long long> class DynSubExact
{
    struct Node
    {
        Node *p = nullptr, *ch[2]{nullptr, nullptr};
        Acc value = 0, sum = 0, added = 0, cancel = 0;
        int size = 0, lightSize = 0;
        Acc lightSum = 0;
        bool rev = false;
        void apply(Acc delta)
        {
            value += delta;
            sum += (Acc)size * delta;
            lightSum += (Acc)lightSize * delta;
            added += delta;
        }
    } none;
    vector<Node> nodes;

    void flush(Node *x)
    {
        if (x->p != &none)
        {
            x->apply(x->p->added - x->cancel);
            x->cancel = x->p->added;
        }
        if (x->rev)
        {
            x->ch[0]->rev = !x->ch[0]->rev;
            x->ch[1]->rev = !x->ch[1]->rev;
            swap(x->ch[0], x->ch[1]);
            x->rev = false;
        }
    }
    void fetch(Node *x)
    {
        flush(x->ch[0]);
        flush(x->ch[1]);
        x->size = 1 + x->ch[0]->size + x->ch[1]->size + x->lightSize;
        x->sum = x->value + x->ch[0]->sum + x->ch[1]->sum + x->lightSum;
    }
    bool isRoot(Node *x) const
    {
        return x->p == &none || (x->p->ch[0] != x && x->p->ch[1] != x);
    }
    void rotate(Node *x, int dir)
    {
        Node *y = x->p, *z = y->p, *w = x->ch[dir];
        flush(w);
        y->ch[!dir] = w;
        w->p = y;
        w->cancel = y->added;
        x->ch[dir] = y;
        x->p = z;
        x->cancel = z->added;
        if (z->ch[0] == y) z->ch[0] = x;
        if (z->ch[1] == y) z->ch[1] = x;
        y->p = x;
        y->cancel = x->added;
        fetch(y), fetch(x);
    }
    void splay(Node *x)
    {
        while (!isRoot(x))
        {
            Node *y = x->p;
            if (isRoot(y))
            {
                flush(y), flush(x);
                rotate(x, y->ch[0] == x);
            }
            else
            {
                Node *z = y->p;
                flush(z), flush(y), flush(x);
                bool same = (z->ch[0] == y) == (y->ch[0] == x);
                if (same) rotate(y, z->ch[0] == y), rotate(x, y->ch[0] == x);
                else rotate(x, y->ch[0] == x), rotate(x, z->ch[0] == x);
            }
        }
        flush(x);
    }
    void expose(Node *x)
    {
        Node *pre = &none;
        for (Node *cur = x; cur != &none; cur = cur->p)
        {
            splay(cur);
            if (cur->ch[1] != &none)
            {
                flush(cur->ch[1]);
                cur->lightSize += cur->ch[1]->size;
                cur->lightSum += cur->ch[1]->sum;
            }
            cur->ch[1] = pre;
            if (cur->ch[1] != &none)
            {
                flush(cur->ch[1]);
                cur->lightSize -= cur->ch[1]->size;
                cur->lightSum -= cur->ch[1]->sum;
            }
            fetch(cur);
            pre = cur;
        }
        splay(x);
    }
    void evert(Node *x)
    {
        expose(x);
        x->rev = !x->rev;
        flush(x);
    }
    void linkNode(Node *child, Node *parent)
    {
        expose(child);
        expose(parent);
        parent->ch[1] = child;
        child->p = parent;
        child->cancel = parent->added;
        fetch(parent);
    }
    void cutNode(Node *x)
    {
        expose(x);
        flush(x->ch[0]);
        x->ch[0]->p = &none;
        x->ch[0] = &none;
        fetch(x);
    }

public:
    explicit DynSubExact(const vector<Acc> &value = {}) : nodes(value.size())
    {
        none.p = &none;
        none.ch[0] = none.ch[1] = &none;
        none.size = none.lightSize = 0;
        for (int i = 0; i < (int)value.size(); i++)
        {
            nodes[i].p = &none;
            nodes[i].ch[0] = nodes[i].ch[1] = &none;
            nodes[i].value = nodes[i].sum = value[i];
            fetch(&nodes[i]);
        }
    }
    bool link(int u, int v)
    {
        evert(&nodes[u]);
        linkNode(&nodes[u], &nodes[v]);
        return true;
    }
    bool cut(int u, int v)
    {
        evert(&nodes[u]);
        expose(&nodes[v]);
        if (nodes[v].ch[0] != &nodes[u] || nodes[u].ch[1] != &none) return false;
        cutNode(&nodes[v]);
        return true;
    }
    void add(int u, Acc delta)
    {
        evert(&nodes[u]);
        nodes[u].apply(delta);
    }
    Acc componentSum(int u)
    {
        evert(&nodes[u]);
        return nodes[u].sum;
    }
};
