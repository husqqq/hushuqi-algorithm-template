#include <bits/stdc++.h>
using namespace std;

class LinkCutTree
{
    struct Node
    {
        int ch[2]{};
        int fa = 0;
        int val = 0;
        int sum = 0;
        bool rev = false;
    };

    vector<Node> t;

    bool isRoot(int x) const
    {
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        t[x].sum = t[t[x].ch[0]].sum ^ t[x].val ^ t[t[x].ch[1]].sum;
    }

    void reverseNode(int x)
    {
        if (!x)
        {
            return;
        }
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }

    void push(int x)
    {
        if (!t[x].rev)
        {
            return;
        }
        reverseNode(t[x].ch[0]);
        reverseNode(t[x].ch[1]);
        t[x].rev = false;
    }

    void rotate(int x)
    {
        int y = t[x].fa, z = t[y].fa;
        int k = t[y].ch[1] == x, w = t[x].ch[k ^ 1];
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
        vector<int> stk{x};
        for (int y = x; !isRoot(y); y = t[y].fa)
        {
            stk.push_back(t[y].fa);
        }
        while (!stk.empty())
        {
            push(stk.back());
            stk.pop_back();
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
        for (int y = 0; x; x = t[y = x].fa)
        {
            splay(x);
            t[x].ch[1] = y;
            pull(x);
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
        while (push(x), t[x].ch[0])
        {
            x = t[x].ch[0];
        }
        splay(x);
        return x;
    }

  public:
    explicit LinkCutTree(const vector<int> &a) : t(a.size() + 1)
    {
        for (int i = 0; i < (int)a.size(); i++)
        {
            t[i + 1].val = t[i + 1].sum = a[i];
        }
    }

    bool connected(int u, int v)
    {
        return findRoot(u + 1) == findRoot(v + 1);
    }

    void link(int u, int v)
    {
        u++, v++;
        makeRoot(u);
        if (findRoot(v) != u)
        {
            t[u].fa = v;
        }
    }

    void cut(int u, int v)
    {
        u++, v++;
        makeRoot(u);
        access(v);
        splay(v);
        if (t[v].ch[0] == u && !t[u].ch[1])
        {
            t[v].ch[0] = t[u].fa = 0;
            pull(v);
        }
    }

    void setValue(int u, int value)
    {
        u++;
        access(u);
        splay(u);
        t[u].val = value;
        pull(u);
    }

    int pathXor(int u, int v)
    {
        if (!connected(u, v))
        {
            return -1;
        }
        u++, v++;
        makeRoot(u);
        access(v);
        splay(v);
        return t[v].sum;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    LinkCutTree lct(a);
    while (m--)
    {
        int op, x, y;
        cin >> op >> x >> y;
        x--;
        if (op != 3)
        {
            y--;
        }
        if (op == 0)
        {
            cout << lct.pathXor(x, y) << '\n';
        }
        else if (op == 1)
        {
            lct.link(x, y);
        }
        else if (op == 2)
        {
            lct.cut(x, y);
        }
        else
        {
            lct.setValue(x, y);
        }
    }
}
