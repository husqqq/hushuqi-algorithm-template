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

class KthAncestor
{
    // parent[k][u] 是 u 的 2^k 级祖先，越过根记 -1。
    vector<vector<int>> parent;
    // depth[u] 是 u 的深度。
    vector<int> depth;

  public:
    KthAncestor(const vector<vector<int>> &tree, int root = 0) : depth(tree.size())
    {
        // tree 是非空树，root 是根。
        assert(!tree.empty() && 0 <= root && root < (int)tree.size()); // 调试检查，可删
        int levels = bit_width((unsigned long long)tree.size());
        parent.assign(levels, vector<int>(tree.size(), -1));
        vector<int> order{root};
        for (int i = 0; i < (int)order.size(); ++i)
        {
            int u = order[i];
            for (int v : tree[u])
            {
                if (v == parent[0][u])
                {
                    continue;
                }
                parent[0][v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }
        for (int level = 1; level < levels; ++level)
        {
            for (int u = 0; u < (int)tree.size(); ++u)
            {
                int p = parent[level - 1][u];
                parent[level][u] = p == -1 ? -1 : parent[level - 1][p];
            }
        }
    }

    int query(int u, int distance) const
    {
        // 返回 u 的第 distance 级祖先；越过根返回 -1。
        assert(distance >= 0); // 调试检查，可删
        if (distance > depth[u])
        {
            return -1;
        }
        for (int bit = 0; distance; ++bit, distance >>= 1)
        {
            if (distance & 1)
            {
                u = parent[bit][u];
            }
        }
        return u;
    }
};
