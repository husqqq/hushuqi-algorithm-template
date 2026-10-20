

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DSU
{
    // f 是并查集父亲，s 仅在根处保存连通块大小。
    vector<int> f, s;

    DSU(int n = 0)
    {
        // n 是元素个数；建立 n 个单点集合。
        init(n);
    }

    void init(int n)
    {
        // n 是元素个数；清空旧状态并建立 n 个单点集合，无返回值。
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
    }

    int find(int x)
    {
        // x 是元素编号；返回所在集合的根并压缩路径。
        return x == f[x] ? x : f[x] = find(f[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是元素编号；合并所在集合，原本不连通时返回 true。
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return false;
        }
        if (s[x] < s[y])
        {
            swap(x, y);
        }
        f[y] = x;
        s[x] += s[y];
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否属于同一集合。
        return find(x) == find(y);
    }

    int size(int x)
    {
        // x 是元素编号；返回所在集合的元素个数。
        return s[find(x)];
    }
};

struct MEdge
{
    // x、y 是无向边端点，w 是边权。
    int x, y, w;
};

optional<pair<int, vector<MEdge>>> kruskal(int n, vector<MEdge> e)
{
    // n 是点数，e 是无向边集；返回 MST 总权与所选边，不连通时返回空。
    sort(e.begin(), e.end(), [](const MEdge &a, const MEdge &b)
         { return a.w < b.w; });
    DSU d(n);
    int ans = 0;
    vector<MEdge> use;
    for (auto v : e)
    {
        if (!d.merge(v.x, v.y))
        {
            continue;
        }
        ans += v.w;
        use.push_back(v);
    }
    if ((int)use.size() + 1 != n && n)
    {
        return nullopt;
    }
    return pair{ans, use};
}

struct DEdgeMST
{
    // u、v 是有向边的起点和终点。
    int u, v;
    // w 是边权；收缩时在函数内副本上改写，须用 __int128 承载中间差值。
    __int128 w;
};

optional<__int128> directedMST(int n, int root, vector<DEdgeMST> e)
{
    // n 是点数，root 是树根，e 是有向边集；返回最小树形图权值，不存在时返回空。
    // 点号为 [0,n)，允许负权、重边、自环；所有中间和与差须能用 __int128 表示。
    // 改求最大树形图：将下面选入边的 w < *in[v] 改为 >，其余步骤不变，返回最大总权。
    assert(n >= 1 && 0 <= root && root < n); // 调试检查，可删。
    for (auto [u, v, w] : e)
    {
        assert(0 <= u && u < n && 0 <= v && v < n);
    }
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
            while (vis[u] < 0 && id[u] < 0 && u != root)
            {
                vis[u] = s;
                u = pre[u];
            }
            if (u != root && id[u] < 0 && vis[u] == s)
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
    __int128 weight; // 最小总权；显式加宽以承载多条当前 int 边权的累加。
    vector<int> parent; // parent[v] 是父点，parent[root]=root。
    vector<int> edge; // edge[v] 是进入 v 的原边下标，edge[root]=-1，可区分重边。
};

using DMSTFastEdge = array<int, 3>; // 依次为有向边起点、终点、边权，允许负权。

optional<DMSTResult> dmstTree(int n, int root, const vector<DMSTFastEdge> &e)
{
    // n>=1 是点数，root 是 [0,n) 内根，e 的下标是原边编号；允许重边、自环、负权。
    // 返回 {最小总权,各点父点,各点入边编号}，根无法到达全部点时为空；不修改 e。
    // 堆偏移与总权显式使用 __int128，所有中间和与差须可表示；原边权用当前 int。
    // 改求最大树形图：将 meld 中根键值比较的 > 改为 <，其余步骤不变，返回最大总权。
    assert(n >= 1 && 0 <= root && root < n);
    int m = e.size();
    // 公共 LeftistHeap 没有整堆加法及下传接口，PBDS 堆也没有；此处只保留所需惰性配对堆。
    vector<array<int, 3>> t(m + 1); // 每项依次为原边编号、首个儿子、下个兄弟，节点 0 为空。
    vector<__int128> lazy(m + 1); // 相对父节点的代价偏移，根上为整堆偏移；须承载边权差。
    vector<int> heap(2 * n); // 每个收缩点的入边堆根，0 为空。
    auto meld = [&](int a, int b) -> int
    {
        // a、b 是节点不共享的两堆根；破坏性合并后返回新根，原边权不变。
        if (!a || !b) return a | b;
        if ((__int128)e[t[a][0]][2] + lazy[a] > (__int128)e[t[b][0]][2] + lazy[b]) swap(a, b);
        lazy[b] -= lazy[a];
        t[b][2] = t[a][1];
        t[a][1] = b;
        return a;
    };
    vector<int> buf; // 删顶时第一遍相邻配对的结果，整次调用复用容量。
    auto pop = [&](int x) -> int
    {
        // x 是非空堆根；将其儿子相邻配对、再从右向左合并，返回删顶后的根。
        buf.clear();
        int p = t[x][1];
        while (p)
        {
            int a = p;
            p = t[a][2];
            t[a][2] = 0;
            lazy[a] += lazy[x];
            int b = 0;
            if (p)
            {
                b = p;
                p = t[b][2];
                t[b][2] = 0;
                lazy[b] += lazy[x];
            }
            buf.push_back(meld(a, b));
        }
        int rt = 0;
        for (int i = (int)buf.size() - 1; i >= 0; i--)
        {
            rt = meld(rt, buf[i]);
        }
        return rt;
    };
    for (int i = 0; i < m; i++)
    {
        auto [u, v, w] = e[i];
        assert(0 <= u && u < n && 0 <= v && v < n);
        if (u == v || v == root) continue;
        t[i + 1][0] = i;
        heap[v] = meld(heap[v], i + 1);
    }
    vector<int> up(2 * n, -1); // 收缩树父点，原始点为叶子，-1 表示没有父点。
    DSU d(2 * n); // 复用 find 与公开父表 f；收缩必须挂到新编号，不调用按大小合并或 size。
    vector<int> vis(2 * n), pick(2 * n, -1); // 搜索批次、选中的原边编号。
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
                    // 减去已选入边的当前代价，原权与旧偏移相消，故直接设置新偏移。
                    lazy[heap[v]] = -(__int128)e[pick[v]][2];
                    heap[c] = meld(heap[c], heap[v]);
                    heap[v] = 0;
                    up[v] = d.f[v] = c;
                    v = d.find(e[pick[v]][0]);
                } while (v != c);
                u = c;
            }
            vis[u] = stamp;
            while (heap[u] && d.find(e[t[heap[u]][0]][0]) == u)
            {
                heap[u] = pop(heap[u]);
            }
            if (!heap[u]) return nullopt;
            pick[u] = t[heap[u]][0];
            u = d.find(e[pick[u]][0]);
        }
    }
    vector<bool> used(tot); // 已被更外层入边替换的收缩树路径。
    vector<int> parent(n, root), edge(n, -1);
    __int128 sum = 0; // 只累加最终恢复的原边权，避免混入约化代价。
    for (int i = tot - 1; i >= 0; i--)
    {
        if (i == root || used[i]) continue;
        int id = pick[i], u = e[id][1];
        while (u != -1 && !used[u])
        {
            used[u] = true;
            u = up[u];
        }
        sum += (__int128)e[id][2];
        parent[e[id][1]] = e[id][0];
        edge[e[id][1]] = id;
    }
    parent[root] = root;
    return DMSTResult{sum, move(parent), move(edge)};
}

signed main()
{
    // 无额外参数；读入有向图，输出最小总权与各点父点，题目保证有解。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, root; cin >> n >> m >> root;
    vector<DMSTFastEdge> e(m);
    for (auto &[u, v, w] : e)
    {
        cin >> u >> v >> w;
    }
    auto ans = dmstTree(n, root, e);
    assert(ans.has_value());
    cout << (long long)ans->weight << '\n'; // 题目保证总权不超过 2e14，输出时可缩回 64 位。
    for (int i = 0; i < n; i++)
    {
        cout << ans->parent[i] << " \n"[i + 1 == n];
    }
}
