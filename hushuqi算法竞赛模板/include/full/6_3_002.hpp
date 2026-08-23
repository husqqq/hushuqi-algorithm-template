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

class RerootHLD
{
    // 成员变量：n 保存树的点数；timer 保存 DFS 计时器。
    int n, timer = 0;
    // 成员变量：tree 保存树的邻接表或树状索引。
    vector<vector<int>> tree;
    // 成员变量：parent 保存父节点编号；depth 保存节点深度；size 保存元素数量；heavy 保存每个节点的重儿子；head 保存链首、邻接表首边或重链顶端；position 保存边内位置或当前下标；vertexAt 保存 DFS 序位置对应的原节点。
    vector<int> parent, depth, size, heavy, head, position, vertexAt;

    void firstDfs(int u)
    {
        // 参数 u 表示 当前树节点；计算子树大小、父亲、深度和重儿子，无返回值。
        size[u] = 1;
        for (int v : tree[u])
        {
            if (v == parent[u])
            {
                continue;
            }
            parent[v] = u;
            depth[v] = depth[u] + 1;
            firstDfs(v);
            size[u] += size[v];
            if (heavy[u] == -1 || size[v] > size[heavy[u]])
            {
                heavy[u] = v;
            }
        }
    }

    void secondDfs(int u, int top)
    {
        // 参数 u 表示 当前树节点；top 表示 当前重链顶端节点；分配重链顶端和 DFS 序，无返回值。
        head[u] = top;
        position[u] = timer++;
        vertexAt[position[u]] = u;
        if (heavy[u] != -1)
        {
            secondDfs(heavy[u], top);
        }
        for (int v : tree[u])
        {
            if (v != parent[u] && v != heavy[u])
            {
                secondDfs(v, v);
            }
        }
    }

    int childOnPath(int anc, int desc) const
    {
        // 参数 anc 表示 祖先节点编号；desc 表示 后代节点编号；返回祖先到后代路径上紧邻祖先的儿子。
        while (head[anc] != head[desc])
        {
            if (parent[head[desc]] == anc)
            {
                return head[desc];
            }
            desc = parent[head[desc]];
        }
        return vertexAt[position[anc] + 1];
    }

  public:
    RerootHLD(vector<vector<int>> tree, int root = 0)
        : n(tree.size()), tree(move(tree)), parent(n, -1), depth(n), size(n), heavy(n, -1), head(n), position(n),
          vertexAt(n)
    {
        // 参数 tree 表示 树的邻接表；root 表示 当前指定的树根；按指定根建立换根查询所需重链信息，无返回值。
        firstDfs(root);
        secondDfs(root, root);
    }

    vector<pair<int, int>> pathSegments(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回树路径对应的 DFS 序区间，并保留遍历方向。
        vector<pair<int, int>> result;
        while (head[u] != head[v])
        {
            if (depth[head[u]] < depth[head[v]])
            {
                swap(u, v);
            }
            result.push_back({position[head[u]], position[u] + 1});
            u = parent[head[u]];
        }
        if (depth[u] > depth[v])
        {
            swap(u, v);
        }
        result.push_back({position[u], position[v] + 1});
        return result;
    }

    int lca(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回两点的最近公共祖先。
        while (head[u] != head[v])
        {
            if (depth[head[u]] < depth[head[v]])
            {
                swap(u, v);
            }
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    int distance(int u, int v) const
    {
        // 参数 u 表示 当前树节点；v 表示 与 u 相邻或位于同一路径的节点；返回树上两点距离。
        int p = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[p];
    }

    // 当前根为 root 时，返回至多两个欧拉序区间表示 subtree(u)。
    vector<pair<int, int>> subtree(int u, int root) const
    {
        // 参数 u 表示 当前树节点；root 表示 当前指定的树根；返回当前根约定下节点对应的子树区间或聚合值。
        if (u == root)
        {
            return {{0, n}};
        }
        if (!(position[u] <= position[root] && position[root] < position[u] + size[u]))
        {
            return {{position[u], position[u] + size[u]}};
        }
        int child = childOnPath(u, root);
        vector<pair<int, int>> result;
        if (position[child])
        {
            result.push_back({0, position[child]});
        }
        if (position[child] + size[child] < n)
        {
            result.push_back({position[child] + size[child], n});
        }
        return result;
    }
};
