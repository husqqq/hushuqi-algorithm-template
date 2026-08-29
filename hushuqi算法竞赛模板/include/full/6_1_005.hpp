#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

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

    bool anc(int u, int v) const
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
        if (anc(u, v))
        {
            return u;
        }
        if (anc(v, u))
        {
            return v;
        }
        for (int k = lg - 1; k >= 0; k--)
        {
            if (!anc(up[k][u], v))
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

vector<int> offlineLCA(const vector<vector<int>> &g, int rt, const vector<pair<int, int>> &q)
{
    // 参数 g 表示 树的邻接表；rt 表示 有根树的根节点；q 表示 按输入顺序给出的节点对询问；按输入询问顺序返回每对节点的最近公共祖先。
    int n = g.size();
    vector<vector<pair<int, int>>> ask(n);
    for (int i = 0; i < (int)q.size(); i++)
    {
        auto [u, v] = q[i];
        ask[u].push_back({v, i});
        ask[v].push_back({u, i});
    }
    vector<int> p(n, -1), anc(n), ans(q.size(), -1), col(n);
    auto find = [&](auto &&self, int x) -> int
    {
        return p[x] < 0 ? x : p[x] = self(self, p[x]);
    };
    auto merge = [&](int x, int y)
    {
        x = find(find, x);
        y = find(find, y);
        if (x == y)
        {
            return x;
        }
        if (p[x] > p[y])
        {
            swap(x, y);
        }
        p[x] += p[y];
        p[y] = x;
        return x;
    };
    auto dfs = [&](auto &&self, int u, int f) -> void
    {
        anc[u] = u;
        col[u] = 1;
        for (int v : g[u])
        {
            if (v == f)
            {
                continue;
            }
            self(self, v, u);
            anc[merge(u, v)] = u;
        }
        col[u] = 2;
        for (auto [v, id] : ask[u])
        {
            if (col[v] == 2)
            {
                ans[id] = anc[find(find, v)];
            }
            else if (u == v)
            {
                ans[id] = u;
            }
        }
    };
    dfs(dfs, rt, -1);
    return ans;
}
