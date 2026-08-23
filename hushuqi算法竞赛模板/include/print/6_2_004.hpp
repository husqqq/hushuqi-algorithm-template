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

class DsuOnTree
{
    // 成员变量：n 保存树的点数。
    int n;
    // 成员变量：tree 保存树的邻接表或树状索引。
    vector<vector<int>> tree;
    // 成员变量：parent 保存父节点编号；size 保存元素数量；heavy 保存每个节点的重儿子。
    vector<int> parent, size, heavy;

    void prepare(int u, int p)
    {
        // 参数 u 表示 当前树节点；p 表示 当前辅助树节点；按上述参数建立后续查询或修改所需的索引，无返回值。
        parent[u] = p;
        size[u] = 1;
        for (int v : tree[u])
        {
            if (v == p)
            {
                continue;
            }
            prepare(v, u);
            size[u] += size[v];
            if (heavy[u] == -1 || size[v] > size[heavy[u]])
            {
                heavy[u] = v;
            }
        }
    }

    template <class Callback> void visitSubtree(int u, int p, Callback &&callback) const
    {
        // 参数 u 表示 当前树节点；p 表示 当前辅助树节点；callback 表示 枚举到一个合法对象时执行的回调；枚举 u 的子树中除父节点 p 外的节点并调用 callback。
        callback(u);
        for (int v : tree[u])
        {
            if (v != p)
            {
                visitSubtree(v, u, callback);
            }
        }
    }

    template <class Add, class Remove, class Answer>
    void dfs(int u, bool keep, Add &&add, Remove &&remove, Answer &&answer)
    {
        // 参数 u 表示 当前树节点；keep 表示 是否保留当前子树贡献；add 表示 定义状态加法或加入贡献的回调；remove 表示 撤销一个元素贡献的回调；answer 表示 由调用者提供的答案数组；处理 u 子树；按 keep 决定是否保留频率贡献，并把该点答案写入 answer。
        for (int v : tree[u])
        {
            if (v != parent[u] && v != heavy[u])
            {
                dfs(v, false, add, remove, answer);
            }
        }
        if (heavy[u] != -1)
        {
            dfs(heavy[u], true, add, remove, answer);
        }
        for (int v : tree[u])
        {
            if (v != parent[u] && v != heavy[u])
            {
                visitSubtree(v, u, add);
            }
        }
        add(u);
        answer(u);
        if (!keep)
        {
            visitSubtree(u, parent[u], remove);
        }
    }

  public:
    explicit DsuOnTree(vector<vector<int>> tree) : n(tree.size()), tree(move(tree)), parent(n), size(n), heavy(n, -1)
    {
        // 参数 tree 表示 树的邻接表；按邻接表构造 DSU on Tree 预处理对象，无返回值。
    }

    template <class Add, class Remove, class Answer> void run(Add &&add, Remove &&remove, Answer &&answer, int root = 0)
    {
        // 参数 add 表示 定义状态加法或加入贡献的回调；remove 表示 撤销一个元素贡献的回调；answer 表示 由调用者提供的答案数组；root 表示 当前指定的树根；执行 DSU on Tree；用 add、remove 维护贡献并用 answer 写回每点答案。
        assert(n > 0 && 0 <= root && root < n); // 调试检查，可删
        fill(heavy.begin(), heavy.end(), -1);
        prepare(root, -1);
        dfs(root, false, add, remove, answer);
    }
};
