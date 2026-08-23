#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct IncrMSF
{
    struct Node
    {
        array<int, 2> ch{};
        int fa = 0;
        bool rev = false, edge = false;
        long long val = 0, mx = 0;
        // who 是子树最大边节点，u、v 是该边端点，tag 是调用者传入的边编号。
        int who = 0, u = 0, v = 0, tag = -1;
    };

    int n, cc;
    int sum = 0;
    vector<Node> t;
    // buf 是 splay 下推懒标记时复用的祖先链缓冲，避免每次操作重新分配。
    vector<int> buf;

    IncrMSF(int n) : n(n), cc(n), t(n + 1)
    {
        assert(n >= 0);
    }

    void reserveEdges(int m)
    {
        // m 是后续最多插入的边数；预留边节点容量，避免在线替换时反复扩容。
        assert(m >= 0); // 调试检查，可删。
        t.reserve(n + 1 + m);
        buf.reserve(64);
    }

    bool isRoot(int x)
    {
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        t[x].who = t[x].edge ? x : 0;
        t[x].mx = t[x].val;
        for (int c : t[x].ch)
        {
            if (t[c].who && (!t[x].who || t[c].mx > t[x].mx))
            {
                t[x].who = t[c].who;
                t[x].mx = t[c].mx;
            }
        }
    }

    void reverseNode(int x)
    {
        if (x)
        {
            swap(t[x].ch[0], t[x].ch[1]);
            t[x].rev ^= 1;
        }
    }

    void push(int x)
    {
        if (t[x].rev)
        {
            reverseNode(t[x].ch[0]);
            reverseNode(t[x].ch[1]);
            t[x].rev = false;
        }
    }

    void rotate(int x)
    {
        int y = t[x].fa, z = t[y].fa, k = t[y].ch[1] == x, w = t[x].ch[k ^ 1];
        if (!isRoot(y))
        {
            t[z].ch[t[z].ch[1] == y] = x;
        }
        t[x].fa = z;
        t[x].ch[k ^ 1] = y;
        t[y].fa = x;
        t[y].ch[k] = w;
        if (w)
        {
            t[w].fa = y;
        }
        pull(y);
        pull(x);
    }

    void splay(int x)
    {
        buf.clear();
        buf.push_back(x);
        for (int y = x; !isRoot(y); y = t[y].fa)
        {
            buf.push_back(t[y].fa);
        }
        while (!buf.empty())
        {
            push(buf.back());
            buf.pop_back();
        }
        while (!isRoot(x))
        {
            int y = t[x].fa, z = t[y].fa;
            if (!isRoot(y))
            {
                rotate((t[y].ch[1] == x) == (t[z].ch[1] == y) ? y : x);
            }
            rotate(x);
        }
    }

    void access(int x)
    {
        int y = 0;
        while (x)
        {
            splay(x);
            int p = t[x].fa;
            t[x].ch[1] = y;
            pull(x);
            y = x;
            x = p;
        }
    }

    void makeRoot(int x)
    {
        access(x);
        splay(x);
        reverseNode(x);
    }

    int findRoot(int x)
    {
        access(x);
        splay(x);
        while (true)
        {
            push(x);
            if (!t[x].ch[0])
            {
                break;
            }
            x = t[x].ch[0];
        }
        splay(x);
        return x;
    }

    void linkNode(int x, int y)
    {
        makeRoot(x);
        t[x].fa = y;
    }

    void cutNode(int x, int y)
    {
        makeRoot(x);
        access(y);
        splay(y);
        assert(t[y].ch[0] == x && !t[x].ch[1]);
        t[y].ch[0] = t[x].fa = 0;
        pull(y);
    }

    int pathMax(int x, int y)
    {
        makeRoot(x);
        access(y);
        splay(y);
        return t[y].who;
    }

    int newEdge(int u, int v, long long w, int tag = -1)
    {
        // u、v 是端点，w 是边权，tag 是外部边编号；创建边节点并返回内部编号。
        Node x;
        x.edge = true;
        x.val = x.mx = w;
        x.u = u;
        x.v = v;
        x.tag = tag;
        t.push_back(x);
        int id = t.size() - 1;
        t[id].who = id;
        return id;
    }

    bool add(int u, int v, long long w)
    {
        assert(0 <= u && u < n && 0 <= v && v < n);
        if (u == v)
        {
            return false;
        }
        u++;
        v++;
        if (findRoot(u) != findRoot(v))
        {
            int e = newEdge(u, v, w);
            linkNode(u, e);
            linkNode(e, v);
            sum += w;
            cc--;
            return true;
        }
        int e = pathMax(u, v);
        if (t[e].val <= w)
        {
            return false;
        }
        long long old = t[e].val;
        cutNode(e, t[e].u);
        cutNode(e, t[e].v);
        int ne = newEdge(u, v, w);
        linkNode(u, ne);
        linkNode(ne, v);
        sum += w - old;
        return true;
    }

    int replaceId(int u, int v, long long w, int id)
    {
        // u、v 是端点，w 是互异边权，id 是当前输入边编号；返回被 MSF 删除的输入编号，未删除时返回 -1，拒绝当前边时返回 id。
        assert(0 <= u && u < n && 0 <= v && v < n && id >= 0); // 调试检查，可删。
        if (u == v)
        {
            return id;
        }
        u++;
        v++;
        if (findRoot(u) != findRoot(v))
        {
            int e = newEdge(u, v, w, id);
            linkNode(u, e);
            linkNode(e, v);
            sum += w;
            cc--;
            return -1;
        }
        int e = pathMax(u, v);
        if (t[e].val <= w)
        {
            return id;
        }
        int old = t[e].tag;
        assert(old >= 0); // 调试检查，可删：在线接口不能与未编号 add 混用。
        long long pre = t[e].val;
        cutNode(e, t[e].u);
        cutNode(e, t[e].v);
        int ne = newEdge(u, v, w, id);
        linkNode(u, ne);
        linkNode(ne, v);
        sum += w - pre;
        return old;
    }

    int weight() const
    {
        return sum;
    }

    int comps() const
    {
        return cc;
    }
};
