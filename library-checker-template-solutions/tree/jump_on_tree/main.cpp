#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 参数 a 表示 按节点编号给出的初始权值；b 表示 第二个输入序列、矩阵或操作数；把指定区间内大于给定上界的值降低到该上界，无返回值。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 参数 a 表示 按节点编号给出的初始权值；b 表示 第二个输入序列、矩阵或操作数；把指定位置或区间内小于给定下界的值提高到该下界，无返回值。
    return a < b ? a = b, true : false;
}

struct LCA
{
    // 成员变量：n 保存树的点数；lg 保存二进制提升层数；tim 保存 DFS 计时器。
    int n, lg, tim = 0;
    // 成员变量：dep 保存深度；in 保存 DFS 进入时间；out 保存 DFS 离开时间或输出边集合。
    vector<int> dep, in, out;
    // 成员变量：up 保存二进制祖先表。
    vector<vector<int>> up;

    LCA(const vector<vector<int>> &g, int root = 0)
        : n(g.size()), lg(bit_width((unsigned)(max<int>(1, n)))), dep(n), in(n), out(n), up(lg, vector<int>(n))
    {
        // 参数 g 表示 树的邻接表；root 表示 当前指定的树根；按参数建立 `LCA` 对象并初始化其不变量，无返回值。
        auto dfs = [&](auto &&self, int u, int p) -> void
        {
            in[u] = tim++;
            up[0][u] = p;
            for (int k = 1; k < lg; k++)
            {
                up[k][u] = up[k - 1][up[k - 1][u]];
            }
            for (int v : g[u])
            {
                if (v == p)
                {
                    continue;
                }
                dep[v] = dep[u] + 1;
                self(self, v, u);
            }
            out[u] = tim;
        };
        if (n)
        {
            dfs(dfs, root, root);
        }
    }

    bool ancestor(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回 u 是否为 v 的祖先。
        return in[u] <= in[v] && out[v] <= out[u];
    }

    int jump(int u, int d) const
    {
        // 参数 u 表示 当前树节点；d 表示 方向、维度或二进制层级；返回从指定节点向上跳 k 层后的节点。
        if (d > dep[u])
        {
            return -1;
        }
        for (int k = 0; d; k++, d >>= 1)
        {
            if (d & 1)
            {
                u = up[k][u];
            }
        }
        return u;
    }

    int lca(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回两点的最近公共祖先。
        if (ancestor(u, v))
        {
            return u;
        }
        if (ancestor(v, u))
        {
            return v;
        }
        for (int k = lg - 1; k >= 0; k--)
        {
            if (!ancestor(up[k][u], v))
            {
                u = up[k][u];
            }
        }
        return up[0][u];
    }

    int dist(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回树上两点间边数或带权距离。
        int p = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[p];
    }

    int pathKth(int u, int v, int k) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；k 表示 从路径起点算起、从 0 开始的位置；返回树路径上的第 k 个节点。
        int p = lca(u, v), a = dep[u] - dep[p], b = dep[v] - dep[p];
        if (k < 0 || k > a + b)
        {
            return -1;
        }
        return k <= a ? jump(u, k) : jump(v, a + b - k);
    }
};

signed main()
{
    int n, q; cin >> n >> q;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    LCA a(g);
    while (q--)
    {
        int s, t, k; cin >> s >> t >> k;
        cout << a.pathKth(s, t, k) << '\n';
    }
}
