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

optional<pair<int, int>> pathIsect(int a, int b, int c, int d, const LCA &l)
{
    // 参数 a 表示 第一条树路径的起点；b 表示 第一条树路径的终点；c 表示 第二条树路径的起点；d 表示 第二条树路径的终点；l 表示 已经建立的最近公共祖先结构；返回两条树路径的公共路径端点；交集为空时返回空。
    vector<int> p = {a, b, c, d};
    for (int x : vector<int>{a, b})
    {
        for (int y : vector<int>{c, d})
        {
            p.push_back(l.lca(x, y));
        }
    }
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    vector<int> q;
    for (int x : p)
    {
        bool u = l.dist(a, x) + l.dist(x, b) == l.dist(a, b);
        bool v = l.dist(c, x) + l.dist(x, d) == l.dist(c, d);
        if (u && v)
        {
            q.push_back(x);
        }
    }
    if (q.empty())
    {
        return nullopt;
    }
    pair<int, int> ans{q[0], q[0]};
    for (int x : q)
    {
        for (int y : q)
        {
            if (l.dist(x, y) > l.dist(ans.first, ans.second))
            {
                ans = {x, y};
            }
        }
    }
    return ans;
}

vector<int> pruferCode(const vector<vector<int>> &g)
{
    // 参数 g 表示 树的邻接表；返回带标号树对应的 Prüfer 序列。
    int n = g.size();
    if (n <= 2)
    {
        return {};
    }
    vector<int> d(n);
    priority_queue<int, vector<int>, greater<>> q;
    for (int i = 0; i < n; i++)
    {
        d[i] = g[i].size();
        if (d[i] == 1)
        {
            q.push(i);
        }
    }
    vector<int> a;
    for (int k = 0; k < n - 2; k++)
    {
        int u = q.top();
        q.pop();
        int v = -1;
        for (int x : g[u])
        {
            if (d[x])
            {
                v = x;
                break;
            }
        }
        a.push_back(v);
        d[u]--;
        if (--d[v] == 1)
        {
            q.push(v);
        }
    }
    return a;
}

vector<pair<int, int>> pruferTree(const vector<int> &a)
{
    // 参数 a 表示 按节点编号给出的初始权值；返回 Prüfer 序列还原出的无向树邻接表。
    int n = a.size() + 2;
    vector<int> d(n, 1);
    for (int x : a)
    {
        assert(0 <= x && x < n);
        d[x]++;
    }
    priority_queue<int, vector<int>, greater<>> q;
    for (int i = 0; i < n; i++)
    {
        if (d[i] == 1)
        {
            q.push(i);
        }
    }
    vector<pair<int, int>> e;
    for (int v : a)
    {
        int u = q.top();
        q.pop();
        e.push_back({u, v});
        if (--d[v] == 1)
        {
            q.push(v);
        }
    }
    int u = q.top();
    q.pop();
    e.push_back({u, q.top()});
    return e;
}
