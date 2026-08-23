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

template <class T, class F> struct ST
{
    // 成员变量：n 保存树的点数。
    int n;
    // 成员变量：f 保存辅助树父节点。
    F f;
    // 成员变量：a 保存各节点当前权值。
    vector<vector<T>> a;

    ST(const vector<T> &v, F f = {}) : n(v.size()), f(f)
    {
        // 参数 v 表示 与 u 相邻或位于同一路径的节点；f 表示 父节点编号；按参数建立 `ST` 对象并初始化其不变量，无返回值。
        int m = n ? bit_width(unsigned(n)) : 0;
        a.assign(m, vector<T>(n));
        if (!n)
        {
            return;
        }
        a[0] = v;
        for (int k = 1; k < m; k++)
        {
            for (int i = 0; i + (1 << k) <= n; i++)
            {
                a[k][i] = f(a[k - 1][i], a[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // 仅适用于幂等运算（min/max/gcd/and/or），查询 [l,r)。
    T query(int l, int r) const
    {
        // 参数 l 表示 左端点；r 表示 右端点；返回指定区间、路径或状态的查询结果。
        assert(0 <= l && l < r && r <= n);
        int k = bit_width(unsigned(r - l)) - 1;
        return f(a[k][l], a[k][r - (1 << k)]);
    }
};

struct MinDepth
{
    pair<int, int> operator()(pair<int, int> a, pair<int, int> b) const
    {
        // 无参数；返回运算后的结果或当前对象引用。
        return min(a, b);
    }
};

struct LCARMQ
{
    // first[x] 是节点 x 在欧拉游走中的首次位置。
    vector<int> first;
    // st 对欧拉游走的深度序列做区间最小值查询。
    ST<pair<int, int>, MinDepth> st;

    static pair<vector<int>, vector<pair<int, int>>> tour(const vector<vector<int>> &g, int root)
    {
        // 参数 g 表示 树的邻接表；root 表示 当前指定的树根；返回从 root 开始的欧拉游走节点序列。
        vector<int> first(g.size(), -1);
        vector<pair<int, int>> a;
        auto dfs = [&](auto &&self, int x, int p, int d) -> void
        {
            first[x] = a.size();
            a.push_back({d, x});
            for (int y : g[x])
            {
                if (y == p)
                {
                    continue;
                }
                self(self, y, x, d + 1);
                a.push_back({d, x});
            }
        };
        dfs(dfs, root, -1, 0);
        return {first, a};
    }

    LCARMQ(const vector<vector<int>> &g, int root = 0) : LCARMQ(tour(g, root))
    {
        // 参数 root 表示 当前指定的树根；按欧拉序位置建立 RMQ 型 LCA 对象，无返回值。
    }

    int lca(int x, int y) const
    {
        // 参数 x 表示 要访问、修改或查询的节点；y 表示 另一个路径端点；返回两点的最近公共祖先。
        int l = first[x], r = first[y];
        if (l > r)
        {
            swap(l, r);
        }
        return st.query(l, r + 1).second;
    }

  private:
    LCARMQ(pair<vector<int>, vector<pair<int, int>>> p) : first(move(p.first)), st(p.second, MinDepth{})
    {
        // 参数 p 表示 当前辅助树节点；按欧拉序位置建立 RMQ 型 LCA 对象，无返回值。
    }
};
