#pragma once

#include "6_1_002.hpp"

template <class T = long long> class TreeDiff
{
    // n 是点数，root 是固定树根。
    int n, root;
    // lca 保存倍增祖先、深度与 DFS 序。
    LCA lca;
    // order 按父亲先于儿子的 DFS 序保存节点。
    vector<int> order;
    // dv 与 edgeDelta 分别保存路径点加、路径边加的树上差分。
    vector<T> dv, edgeDelta;

    vector<T> collect(vector<T> value) const
    {
        // value 是一份树上差分；返回自底向上累加后的每点结果。
        for (int i = n - 1; i > 0; i--)
        {
            int u = order[i];
            value[lca.up[0][u]] += value[u];
        }
        return value;
    }

  public:
    TreeDiff(const vector<vector<int>> &g, int root = 0)
        : n(g.size()), root(root), lca(g, root), order(n), dv(n), edgeDelta(n)
    {
        // g 是非空无向树，root 是固定根；预处理祖先与汇总顺序。
        assert(n > 0 && 0 <= root && root < n); // 调试检查，可删
        for (int u = 0; u < n; u++)
        {
            order[lca.in[u]] = u;
        }
    }

    void clear()
    {
        // 无参数；清空已经记录的点路径与边路径增量，无返回值。
        fill(dv.begin(), dv.end(), T{});
        fill(edgeDelta.begin(), edgeDelta.end(), T{});
    }

    void addVtxPath(int u, int v, const T &delta)
    {
        // u、v 是路径两端，delta 是增量；给简单路径上的每个点增加 delta。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        int p = lca.lca(u, v);
        dv[u] += delta;
        dv[v] += delta;
        dv[p] -= delta;
        if (p != root)
        {
            dv[lca.up[0][p]] -= delta;
        }
    }

    void addEdgePath(int u, int v, const T &delta)
    {
        // u、v 是路径两端，delta 是增量；给简单路径上的每条边增加 delta。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        int p = lca.lca(u, v);
        edgeDelta[u] += delta;
        edgeDelta[v] += delta;
        edgeDelta[p] -= delta;
        edgeDelta[p] -= delta;
    }

    vector<T> val() const
    {
        // 无参数；返回全部点累计的路径点增量，按节点编号排列。
        return collect(dv);
    }

    vector<T> edgeValues() const
    {
        // 无参数；返回每点到父亲边的累计增量，根位置为零。
        auto ans = collect(edgeDelta);
        ans[root] = T{};
        return ans;
    }
};
