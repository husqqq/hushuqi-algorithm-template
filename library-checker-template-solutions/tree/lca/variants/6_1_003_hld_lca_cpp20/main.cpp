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

struct HLD
{
    // n 是点数，tim 是 DFS 序计数器。
    int n, tim = 0;
    // fa、dep、sz、son、top、in 依次是父亲、深度、子树大小、重儿子、链顶和 DFS 序位置。
    // rev[i] 是 DFS 序位置 i 对应的原节点。
    vector<int> fa, dep, sz, son, top, in, rev;

    HLD(const vector<vector<int>> &g, int root = 0)
        : n(g.size()), fa(n, -1), dep(n), sz(n, 1), son(n, -1), top(n), in(n), rev(n)
    {
        // g 是非空树，root 是根。
        assert(n > 0 && 0 <= root && root < n); // 调试检查，可删
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
                sz[u] += sz[v];
                if (son[u] < 0 || sz[v] > sz[son[u]])
                {
                    son[u] = v;
                }
            }
        };
        auto dfs2 = [&](auto &&self, int u, int h) -> void
        {
            top[u] = h;
            rev[tim] = u;
            in[u] = tim++;
            if (son[u] >= 0)
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

    int lca(int u, int v) const
    {
        // 返回 u、v 的最近公共祖先。
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // op(l,r,rev)：区间为 [l,r)，rev 表示沿路径应逆序读取。
    template <class F> void path(int u, int v, F op, bool edge = false) const
    {
        // 依次回调 u 到 v 的路径区间；edge 表示点权还是边权。
        vector<pair<int, int>> right;
        while (top[u] != top[v])
        {
            if (dep[top[u]] >= dep[top[v]])
            {
                op(in[top[u]], in[u] + 1, true);
                u = fa[top[u]];
            }
            else
            {
                right.push_back({in[top[v]], in[v] + 1});
                v = fa[top[v]];
            }
        }
        if (dep[u] >= dep[v])
        {
            int l = in[v] + edge, r = in[u] + 1;
            if (l < r)
            {
                op(l, r, true);
            }
        }
        else
        {
            int l = in[u] + edge, r = in[v] + 1;
            if (l < r)
            {
                right.push_back({l, r});
            }
        }
        reverse(right.begin(), right.end());
        for (auto [l, r] : right)
        {
            op(l, r, false);
        }
    }

    pair<int, int> subtree(int u) const
    {
        // 返回 u 子树对应的半开区间。
        return {in[u], in[u] + sz[u]};
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; vector<vector<int>> g(n);
    for (int v = 1; v < n; ++v) { int p; cin >> p; g[p].push_back(v); g[v].push_back(p); }
    HLD lca(g); while (q--) { int u, v; cin >> u >> v; cout << lca.lca(u, v) << '\n'; }
}
