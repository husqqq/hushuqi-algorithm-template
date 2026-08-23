#pragma once

#include "base.hpp"

template <class Acc = long long>
class TreePathSum
{
    // n 保存点数，ti 保存 DFS 序计数器。
    int n, ti = 0;
    // g 保存静态树邻接表。
    vector<vector<int>> g;
    // fa、dep、siz、son、top、in 依次保存父节点、深度、子树大小、重儿子、链顶和 DFS 序。
    vector<int> fa, dep, siz, son, top, in;
    // val 保存各点当前权值，bit 保存 DFS 序上的前缀和。
    vector<Acc> val, bit;

    void build(int root)
    {
        // 递归建立父亲、重儿子、链顶与 DFS 序。
        auto dfs1 = [&](auto &&self, int u, int p) -> void
        {
            fa[u] = p;
            for (int v : g[u])
            {
                if (v == p)
                {
                    continue;
                }
                dep[v] = dep[u] + 1;
                self(self, v, u);
                siz[u] += siz[v];
                if (son[u] == -1 || siz[v] > siz[son[u]])
                {
                    son[u] = v;
                }
            }
        };
        auto dfs2 = [&](auto &&self, int u, int h) -> void
        {
            top[u] = h;
            in[u] = ti++;
            if (son[u] != -1)
            {
                self(self, son[u], h);
            }
            for (int v : g[u])
            {
                if (v != fa[u] && v != son[u])
                {
                    self(self, v, v);
                }
            }
        };
        dfs1(dfs1, root, -1);
        dfs2(dfs2, root, root);
    }

    void addBit(int p, Acc v)
    {
        // p 是 0 下标 DFS 序位置，v 是增量；更新树状数组，无返回值。
        for (p++; p <= n; p += p & -p)
        {
            bit[p] += v;
        }
    }

    Acc sumBit(int p) const
    {
        // p 是右端点；返回半开前缀 [0,p) 的点权和。
        Acc ans = 0;
        for (; p; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    }

  public:
    TreePathSum(const vector<vector<int>> &g, const vector<Acc> &a, int root = 0)
        : n(g.size()), g(g), fa(n, -1), dep(n), siz(n, 1), son(n, -1), top(n), in(n), val(a), bit(n + 1)
    {
        // g 是静态树，a 是各点初值，root 是根；完成重链剖分与树状数组初始化，无返回值。
        assert(n > 0 && (int)a.size() == n && 0 <= root && root < n); // 调试检查，可删
        build(root);
        for (int u = 0; u < n; u++)
        {
            addBit(in[u], val[u]);
        }
    }

    void set(int u, Acc v)
    {
        // u 是节点编号，v 是新点权；执行单点赋值，无返回值。
        assert(0 <= u && u < n); // 调试检查，可删
        addBit(in[u], v - val[u]);
        val[u] = v;
    }

    Acc pathSum(int u, int v) const
    {
        // u、v 是路径两端；返回简单路径上全部点权之和。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        Acc ans = 0;
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }
            ans += sumBit(in[u] + 1) - sumBit(in[top[u]]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v])
        {
            swap(u, v);
        }
        return ans + sumBit(in[v] + 1) - sumBit(in[u]);
    }

    Acc subtreeSum(int u) const
    {
        // u 是子树根；返回固定构造根下 u 的整棵子树点权和。
        assert(0 <= u && u < n); // 调试检查，可删
        return sumBit(in[u] + siz[u]) - sumBit(in[u]);
    }
};

template <class Acc = long long>
class LinkCutTree
{
    struct Node
    {
        // ch[0..1] 依次保存辅助树左右儿子编号。
        array<int, 2> ch{};
        // fa 保存辅助树父节点，rev 标记辅助子树翻转。
        int fa = 0;
        bool rev = false;
        // val 保存当前点权，sum 保存辅助树中路径节点的聚合和。
        Acc val = 0, sum = 0;
    };

    // t[0] 是空节点，t[u+1] 对应外部 0 下标节点 u。
    vector<Node> t;

    bool isRoot(int x) const
    {
        // x 是内部节点编号；返回 x 是否为当前辅助树根。
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        // x 是内部节点编号；由两个儿子重新计算路径聚合和，无返回值。
        t[x].sum = t[t[x].ch[0]].sum + t[x].val + t[t[x].ch[1]].sum;
    }

    void reverseNode(int x)
    {
        // x 是内部节点编号；翻转辅助子树并记录懒标记，无返回值。
        if (!x)
        {
            return;
        }
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }

    void push(int x)
    {
        // x 是内部节点编号；下传翻转标记，无返回值。
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
        // x 是要上旋的内部节点；执行一次 Splay 旋转并维护聚合，无返回值。
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
        // x 是内部节点；把它旋到当前辅助树根，无返回值。
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
        // x 是内部节点；把动态树根到 x 的路径改为首选路径，无返回值。
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

    void makeRootNode(int x)
    {
        // x 是内部节点；把它改为所在动态树的根，无返回值。
        access(x);
        splay(x);
        reverseNode(x);
    }

    int findRootNode(int x)
    {
        // x 是内部节点；返回所在动态树根的内部编号。
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

  public:
    LinkCutTree(const vector<Acc> &a = {}) : t(a.size() + 1)
    {
        // a 是各节点初始点权；建立 n 个互不连通的节点，无返回值。
        for (int i = 0; i < (int)a.size(); i++)
        {
            t[i + 1].val = a[i];
            t[i + 1].sum = a[i];
        }
    }

    bool same(int u, int v)
    {
        // u、v 是外部节点编号；返回两点是否连通。
        return connected(u, v);
    }

    bool link(int u, int v)
    {
        // u、v 是外部节点编号；不连通时连边并返回 true，否则不修改并返回 false。
        u++;
        v++;
        makeRootNode(u);
        if (findRootNode(v) == u)
        {
            return false;
        }
        t[u].fa = v;
        return true;
    }

    bool cut(int u, int v)
    {
        // u、v 是外部节点编号；边存在时删除并返回 true，否则返回 false。
        u++;
        v++;
        makeRootNode(u);
        access(v);
        splay(v);
        if (t[v].ch[0] != u || t[u].ch[1])
        {
            return false;
        }
        t[v].ch[0] = 0;
        t[u].fa = 0;
        pull(v);
        return true;
    }

    bool connected(int u, int v)
    {
        // u、v 是外部节点编号；返回两点当前是否属于同一棵动态树。
        return findRootNode(u + 1) == findRootNode(v + 1);
    }

    void set(int u, Acc v)
    {
        // u 是外部节点编号，v 是新点权；执行单点赋值，无返回值。
        u++;
        access(u);
        splay(u);
        t[u].val = v;
        pull(u);
    }

    Acc pathSum(int u, int v)
    {
        // u、v 必须连通；返回两点简单路径上的点权和。
        assert(connected(u, v)); // 调试检查，可删
        u++;
        v++;
        makeRootNode(u);
        access(v);
        splay(v);
        return t[v].sum;
    }
};

template <class Acc = long long>
class SubtreeLCT
{
    struct Node
    {
        // ch[0..1] 依次保存辅助树左右儿子编号。
        array<int, 2> ch{};
        // fa 保存辅助树父节点，rev 标记辅助子树翻转。
        int fa = 0;
        bool rev = false;
        // val、path、vir、sum 依次保存点权、首选路径和、虚儿子贡献、整棵辅助子树贡献。
        Acc val = 0, path = 0, vir = 0, sum = 0;
    };

    // t[0] 是空节点，t[u+1] 对应外部节点 u。
    vector<Node> t;

    bool isRoot(int x) const
    {
        // x 是内部节点编号；返回 x 是否为当前辅助树根。
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        // x 是内部节点；重算首选路径和与包含虚儿子的总和，无返回值。
        auto [l, r] = t[x].ch;
        t[x].path = t[l].path + t[x].val + t[r].path;
        t[x].sum = t[l].sum + t[x].val + t[r].sum + t[x].vir;
    }

    void reverseNode(int x)
    {
        // x 是内部节点；翻转辅助子树并记录标记，无返回值。
        if (!x)
        {
            return;
        }
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }

    void push(int x)
    {
        // x 是内部节点；下传翻转标记，无返回值。
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
        // x 是要上旋的内部节点；执行一次 Splay 旋转并维护信息，无返回值。
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
        // x 是内部节点；把它旋到辅助树根，无返回值。
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
        // x 是内部节点；切换首选儿子并同步虚儿子贡献，无返回值。
        int y = 0, z = x;
        while (z)
        {
            splay(z);
            int p = t[z].fa;
            t[z].vir += t[t[z].ch[1]].sum - t[y].sum;
            t[z].ch[1] = y;
            pull(z);
            y = z;
            z = p;
        }
        splay(x);
    }

    void makeRootNode(int x)
    {
        // x 是内部节点；把它改为所在动态树的根，无返回值。
        access(x);
        reverseNode(x);
    }

    int findRootNode(int x)
    {
        // x 是内部节点；返回所在动态树根的内部编号。
        access(x);
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

  public:
    SubtreeLCT(const vector<Acc> &a = {}) : t(a.size() + 1)
    {
        // a 是各点初始权值；建立 n 个互不连通节点，无返回值。
        for (int i = 0; i < (int)a.size(); i++)
        {
            t[i + 1].val = a[i];
            t[i + 1].path = a[i];
            t[i + 1].sum = a[i];
        }
    }

    bool link(int u, int v)
    {
        // u、v 是外部节点编号；不连通时连边并返回 true，否则返回 false。
        u++;
        v++;
        makeRootNode(u);
        if (findRootNode(v) == u)
        {
            return false;
        }
        access(v);
        t[u].fa = v;
        t[v].vir += t[u].sum;
        pull(v);
        return true;
    }

    bool cut(int u, int v)
    {
        // u、v 是外部节点编号；边存在时删除并返回 true，否则返回 false。
        u++;
        v++;
        makeRootNode(u);
        access(v);
        if (t[v].ch[0] != u || t[u].ch[1])
        {
            return false;
        }
        t[v].ch[0] = 0;
        t[u].fa = 0;
        pull(v);
        return true;
    }

    bool connected(int u, int v)
    {
        // u、v 是外部节点编号；返回两点当前是否属于同一棵动态树。
        return findRootNode(u + 1) == findRootNode(v + 1);
    }

    void set(int u, Acc v)
    {
        // u 是外部节点编号，v 是新点权；执行单点赋值，无返回值。
        u++;
        access(u);
        t[u].val = v;
        pull(u);
    }

    Acc pathSum(int u, int v)
    {
        // u、v 必须连通；返回两点简单路径上的点权和。
        assert(connected(u, v)); // 调试检查，可删
        u++;
        v++;
        makeRootNode(u);
        access(v);
        return t[v].path;
    }

    Acc subtreeSum(int root, int u)
    {
        // root 是有根树根，u 是子树根；返回该根方向下 u 的子树点权和。
        assert(connected(root, u)); // 调试检查，可删
        root++;
        u++;
        makeRootNode(root);
        access(u);
        return t[u].val + t[u].vir;
    }
};

class OnlineBridge
{
    // d2、dc 分别保存边双代表元和连通块代表元。
    vector<int> d2, dc;
    // fa 保存当前生成森林父节点，siz 保存连通块大小。
    vector<int> fa, siz;
    // vis 保存最近一次 mergePath 的访问轮次。
    vector<int> vis;
    // ti 是访问轮次，cnt 是当前桥数。
    int ti = 0, cnt = 0;

    int find2(int x)
    {
        // x 是节点编号；返回其当前边双连通分量代表元。
        if (x == -1)
        {
            return -1;
        }
        return d2[x] == x ? x : d2[x] = find2(d2[x]);
    }

    int findC(int x)
    {
        // x 是节点编号；返回其当前连通块代表元。
        if (x == -1)
        {
            return -1;
        }
        x = find2(x);
        return dc[x] == x ? x : dc[x] = findC(dc[x]);
    }

    void makeRoot(int x)
    {
        // x 是节点编号；翻转生成森林中 x 到原根的父边，无返回值。
        x = find2(x);
        int root = x, pre = -1;
        while (x != -1)
        {
            int p = find2(fa[x]);
            fa[x] = pre;
            dc[x] = root;
            pre = x;
            x = p;
        }
        siz[root] = siz[pre];
    }

    void mergePath(int a, int b)
    {
        // a、b 已在同一连通块；压缩新环上的全部桥，无返回值。
        ti++;
        vector<int> pa, pb;
        int lca = -1;
        while (lca == -1)
        {
            if (a != -1)
            {
                a = find2(a);
                pa.push_back(a);
                if (vis[a] == ti)
                {
                    lca = a;
                    break;
                }
                vis[a] = ti;
                a = fa[a];
            }
            swap(a, b);
            swap(pa, pb);
        }
        for (auto x : pa)
        {
            d2[x] = lca;
            if (x == lca)
            {
                break;
            }
            cnt--;
        }
        for (auto x : pb)
        {
            d2[x] = lca;
            if (x == lca)
            {
                break;
            }
            cnt--;
        }
    }

  public:
    OnlineBridge(int n) : d2(n), dc(n), fa(n, -1), siz(n, 1), vis(n)
    {
        // n 是固定点数；建立没有边的无向图，无返回值。
        iota(d2.begin(), d2.end(), 0);
        iota(dc.begin(), dc.end(), 0);
    }

    void addEdge(int a, int b)
    {
        // a、b 是无向边两端；加入边并更新当前桥数，无返回值，允许重边和自环。
        a = find2(a);
        b = find2(b);
        if (a == b)
        {
            return;
        }
        int ca = findC(a), cb = findC(b);
        if (ca != cb)
        {
            cnt++;
            if (siz[ca] > siz[cb])
            {
                swap(a, b);
                swap(ca, cb);
            }
            makeRoot(a);
            fa[a] = b;
            dc[a] = b;
            siz[cb] += siz[a];
        }
        else
        {
            mergePath(a, b);
        }
    }

    int bridges() const
    {
        // 无参数；返回当前无向图中的桥数。
        return cnt;
    }
};

class DynamicCut
{
    // n 保存固定点数。
    int n;
    // e[id][0..1] 依次保存第 id 条边的两个端点，on 标记边当前是否存在。
    vector<array<int, 2>> e;
    vector<char> on;

  public:
    DynamicCut(int n) : n(n)
    {
        // n 是固定点数；建立空无向图，无返回值。
    }

    int addEdge(int u, int v)
    {
        // u、v 是边的两个端点；加入边并返回稳定边编号。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        e.push_back({u, v});
        on.push_back(1);
        return e.size() - 1;
    }

    bool eraseEdge(int id)
    {
        // id 是 addEdge 返回的边编号；边存在时删除并返回 true，否则返回 false。
        if (id < 0 || id >= (int)e.size() || !on[id])
        {
            return false;
        }
        on[id] = 0;
        return true;
    }

    pair<vector<int>, vector<int>> analyze() const
    {
        // 无参数；返回 {全部桥的边编号,全部割点编号}，两部分都按编号升序排列。
        vector<vector<array<int, 2>>> g(n);
        for (int id = 0; id < (int)e.size(); id++)
        {
            if (!on[id])
            {
                continue;
            }
            auto [u, v] = e[id];
            g[u].push_back({v, id});
            g[v].push_back({u, id});
        }
        vector<int> dfn(n), low(n), cut(n), bridge;
        int ti = 0;
        auto dfs = [&](auto &&self, int u, int pe) -> void
        {
            dfn[u] = low[u] = ++ti;
            int child = 0;
            for (auto [v, id] : g[u])
            {
                if (id == pe)
                {
                    continue;
                }
                if (!dfn[v])
                {
                    child++;
                    self(self, v, id);
                    chmin(low[u], low[v]);
                    if (low[v] > dfn[u])
                    {
                        bridge.push_back(id);
                    }
                    if (pe != -1 && low[v] >= dfn[u])
                    {
                        cut[u] = 1;
                    }
                }
                else
                {
                    chmin(low[u], dfn[v]);
                }
            }
            if (pe == -1)
            {
                cut[u] = child > 1;
            }
        };
        for (int s = 0; s < n; s++)
        {
            if (!dfn[s])
            {
                dfs(dfs, s, -1);
            }
        }
        vector<int> point;
        for (int u = 0; u < n; u++)
        {
            if (cut[u])
            {
                point.push_back(u);
            }
        }
        sort(bridge.begin(), bridge.end());
        return {bridge, point};
    }
};

class GlobalTree
{
    // n 保存点数，root 保存全局平衡树根。
    int n, root;
    // g 保存原树邻接表。
    vector<vector<int>> g;
    // pa、siz、son 依次保存原树父节点、子树大小和重儿子。
    vector<int> pa, siz, son;
    // ch[u][0..1] 保存全局平衡树左右儿子，fa 保存平衡树父边或轻边父亲。
    vector<array<int, 2>> ch;
    vector<int> fa;

    void dfs(int root)
    {
        // 递归计算原树父亲、子树大小和重儿子。
        auto work = [&](auto &&self, int u) -> void
        {
            siz[u] = 1;
            for (int v : g[u])
            {
                if (v == pa[u])
                {
                    continue;
                }
                pa[v] = u;
                self(self, v);
                siz[u] += siz[v];
                if (son[u] == -1 || siz[v] > siz[son[u]])
                {
                    son[u] = v;
                }
            }
        };
        work(work, root);
    }

    int buildSeq(const vector<int> &v, const vector<int> &pre, int l, int r)
    {
        // v 是一条重链，pre 是轻子树质量前缀和，l、r 是当前闭区间；返回加权中点根。
        if (l > r)
        {
            return -1;
        }
        int need = (pre[l] + pre[r + 1]) / 2;
        int m = lower_bound(pre.begin() + l + 1, pre.begin() + r + 2, need) - pre.begin() - 1;
        m = clamp(m, l, r);
        int u = v[m];
        ch[u][0] = buildSeq(v, pre, l, m - 1);
        ch[u][1] = buildSeq(v, pre, m + 1, r);
        if (ch[u][0] != -1)
        {
            fa[ch[u][0]] = u;
        }
        if (ch[u][1] != -1)
        {
            fa[ch[u][1]] = u;
        }
        return u;
    }

    int buildChain(int x)
    {
        // x 是一条重链链顶；递归建立轻链并返回该链的平衡树根。
        vector<int> v;
        for (int u = x; u != -1; u = son[u])
        {
            v.push_back(u);
        }
        for (auto u : v)
        {
            for (auto w : g[u])
            {
                if (w == pa[u] || w == son[u])
                {
                    continue;
                }
                int z = buildChain(w);
                fa[z] = u;
            }
        }
        vector<int> pre(v.size() + 1);
        for (int i = 0; i < (int)v.size(); i++)
        {
            pre[i + 1] = pre[i] + siz[v[i]] - (son[v[i]] == -1 ? 0 : siz[son[v[i]]]);
        }
        return buildSeq(v, pre, 0, v.size() - 1);
    }

  public:
    GlobalTree(const vector<vector<int>> &g, int rt = 0)
        : n(g.size()), root(-1), g(g), pa(n, -1), siz(n), son(n, -1), ch(n, array<int, 2>{-1, -1}), fa(n, -1)
    {
        // g 是静态树，rt 是原树根；建立全局平衡二叉树，无返回值。
        assert(n > 0 && 0 <= rt && rt < n); // 调试检查，可删
        dfs(rt);
        root = buildChain(rt);
    }

    int getRoot() const
    {
        // 无参数；返回全局平衡二叉树根节点编号。
        return root;
    }

    const vector<array<int, 2>> &children() const
    {
        // 无参数；返回每个节点在平衡二叉树中的左右儿子。
        return ch;
    }

    const vector<int> &parents() const
    {
        // 无参数；返回平衡树父边与轻边共同组成的父指针。
        return fa;
    }

    const vector<int> &heavy() const
    {
        // 无参数；返回原树中每个节点的重儿子。
        return son;
    }

    const vector<int> &parTree() const
    {
        // 无参数；返回原树父节点数组。
        return pa;
    }
};

template <class Acc = long long>
class DynamicMWIS
{
    struct Mat
    {
        // nullopt 表示不可达，其余项保存 128 位 max-plus 值。
        array<array<optional<Acc>, 2>, 2> a{};
    };

    // tr 保存全局平衡二叉树拓扑。
    GlobalTree tr;
    // ch、fa、son、pa 分别引用平衡树儿子、混合父指针、重儿子和原树父节点。
    vector<array<int, 2>> ch;
    vector<int> fa, son, pa;
    // w 是 64 位点权，g[u] 是不含重儿子的“不选/选 u”贡献。
    vector<Acc> w;
    vector<array<Acc, 2>> g;
    // s 保存每个平衡树子树合并后的 max-plus 矩阵。
    vector<Mat> s;
    // tree 保存原树邻接表。
    vector<vector<int>> tree;

    static Mat mul(const Mat &x, const Mat &y)
    {
        // x、y 是两个 max-plus 矩阵；返回按顺序合并后的矩阵。
        Mat z;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    if (!x.a[i][k] || !y.a[k][j])
                    {
                        continue;
                    }
                    Acc v = *x.a[i][k] + *y.a[k][j];
                    if (!z.a[i][j] || *z.a[i][j] < v)
                    {
                        z.a[i][j] = v;
                    }
                }
            }
        }
        return z;
    }

    bool isChild(int x) const
    {
        // x 是节点编号；返回 x 到 fa[x] 是否为平衡二叉树实儿子边。
        return fa[x] != -1 && (ch[fa[x]][0] == x || ch[fa[x]][1] == x);
    }

    void pull(int x)
    {
        // x 是节点编号；由自身转移和左右儿子重算矩阵，无返回值。
        Mat cur;
        cur.a[0][0] = g[x][0];
        cur.a[0][1] = g[x][0];
        cur.a[1][0] = g[x][1];
        if (ch[x][0] != -1)
        {
            cur = mul(s[ch[x][0]], cur);
        }
        if (ch[x][1] != -1)
        {
            cur = mul(cur, s[ch[x][1]]);
        }
        s[x] = cur;
    }

    void init(int root)
    {
        // 递归求静态 DP，并初始化各点的轻儿子贡献。
        vector<array<Acc, 2>> f(w.size());
        auto dfs = [&](auto &&self, int u) -> void
        {
            f[u] = {0, w[u]};
            for (int v : tree[u])
            {
                if (pa[v] != u)
                {
                    continue;
                }
                self(self, v);
                f[u][0] += max(f[v][0], f[v][1]);
                f[u][1] += f[v][0];
                if (v != son[u])
                {
                    g[u][0] += max(f[v][0], f[v][1]);
                    g[u][1] += f[v][0];
                }
            }
            g[u][1] += w[u];
        };
        dfs(dfs, root);
    }

    void buildMat()
    {
        // 对平衡二叉树森林作递归后序，建立全部矩阵。
        auto dfs = [&](auto &&self, int u) -> void
        {
            for (int v : ch[u])
            {
                if (v != -1)
                {
                    self(self, v);
                }
            }
            pull(u);
        };
        for (int u = 0; u < (int)w.size(); u++)
        {
            if (!isChild(u))
            {
                dfs(dfs, u);
            }
        }
    }

  public:
    DynamicMWIS(const vector<vector<int>> &tree, const vector<Acc> &w, int root = 0)
        : tr(tree, root), ch(tr.children()), fa(tr.parents()), son(tr.heavy()), pa(tr.parTree()), w(w),
          g(w.size()), s(w.size()), tree(tree)
    {
        // tree 是静态树，w 是可修改点权，root 是固定根；初始化动态最大权独立集，无返回值。
        assert(tree.size() == w.size()); // 调试检查，可删
        init(root);
        buildMat();
    }

    void set(int x, Acc v)
    {
        // x 是节点编号，v 是新点权；更新点权并维护全树最大权独立集，无返回值。
        assert(0 <= x && x < (int)w.size()); // 调试检查，可删
        g[x][1] += v - w[x];
        w[x] = v;
        while (fa[x] != -1)
        {
            if (isChild(x))
            {
                pull(x);
            }
            else
            {
                int p = fa[x];
                Acc old0 = max(*s[x].a[0][0], *s[x].a[1][0]);
                Acc old1 = *s[x].a[0][0];
                g[p][0] -= old0;
                g[p][1] -= old1;
                pull(x);
                g[p][0] += max(*s[x].a[0][0], *s[x].a[1][0]);
                g[p][1] += *s[x].a[0][0];
            }
            x = fa[x];
        }
        pull(x);
    }

    Acc answer() const
    {
        // 无参数；返回当前整棵树最大权独立集的权值，允许选择空集。
        int r = tr.getRoot();
        return max(*s[r].a[0][0], *s[r].a[1][0]);
    }
};
