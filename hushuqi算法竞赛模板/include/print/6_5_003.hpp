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

// ans[u]：u 子树中结点数最多的深度差；并列取较小深度。
vector<int> depthMode(const vector<vector<int>> &g, int root = 0)
{
    // 参数 g 表示 树的邻接表；root 表示 当前指定的树根；返回每个子树中出现次数最多的相对深度。
    int n = g.size();
    if (!n)
    {
        return {};
    }
    assert(0 <= root && root < n); // 调试检查，可删
    vector<int> len(n, 1), son(n, -1), ans(n), at(n), f(n + 1);
    auto dfs1 = [&](auto &&self, int u, int p) -> void
    {
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            self(self, v, u);
            if (len[v] + 1 > len[u])
            {
                len[u] = len[v] + 1;
                son[u] = v;
            }
        }
    };
    dfs1(dfs1, root, -1);
    int ptr = len[root];
    auto dfs2 = [&](auto &&self, int u, int p) -> void
    {
        int h = son[u];
        if (h != -1)
        {
            at[h] = at[u] + 1;
            self(self, h, u);
            ans[u] = ans[h] + 1;
        }
        f[at[u]] = 1;
        if (f[at[u] + ans[u]] <= 1)
        {
            ans[u] = 0;
        }
        for (int v : g[u])
        {
            if (v == p || v == h)
            {
                continue;
            }
            at[v] = ptr;
            ptr += len[v];
            self(self, v, u);
            for (int d = 0; d < len[v]; d++)
            {
                f[at[u] + d + 1] += f[at[v] + d];
                int q = f[at[u] + d + 1], z = f[at[u] + ans[u]];
                if (q > z || (q == z && d + 1 < ans[u]))
                {
                    ans[u] = d + 1;
                }
            }
        }
    };
    dfs2(dfs2, root, -1);
    return ans;
}
