#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct NegEdge
{
    // from、to 是有向边的起点和终点，weight 是边权。
    int from, to, weight;
};

struct NegPath
{
    // dist[v] 是源点到 v 的有限最短距离；空值表示不可达。
    vector<optional<int>> dist;
    // negInf[v] 为真表示 v 可由某个源点可达负环到达，答案为负无穷。
    vector<char> negInf;
};

class NegPathSolver
{
private:
    struct TreeNode
    {
        // parent、depth 是当前最短路树中的父结点和深度。
        int parent = -1, depth = 0;
        // active 表示该结点仍可能参与松弛，children 保存树孩子。
        char active = 0;
        vector<int> children;
    };

    int n;
    vector<NegEdge> edges;
    vector<vector<int>> graph;
    vector<TreeNode> tree;
    vector<int> dist;
    vector<char> reachable, negInf;

    bool change(int x, int stop, char value)
    {
        if (negInf[x])
        {
            return false;
        }
        if (x == stop)
        {
            return true;
        }
        tree[x].active = value;
        for (int y : tree[x].children)
        {
            if (!negInf[y] && change(y, stop, value))
            {
                return true;
            }
        }
        return false;
    }

    void maintain(int x)
    {
        if (negInf[x])
        {
            return;
        }
        for (int y : tree[x].children)
        {
            if (!negInf[y])
            {
                tree[y].depth = tree[x].depth + 1;
                maintain(y);
            }
        }
    }

    void cut(int x)
    {
        int p = tree[x].parent;
        if (p == -1)
        {
            return;
        }
        tree[x].depth = 0;
        maintain(x);
        auto &child = tree[p].children;
        child.erase(find(child.begin(), child.end(), x));
        tree[x].parent = -1;
    }

    void link(int x, int p)
    {
        tree[x].parent = p;
        tree[x].depth = tree[p].depth + 1;
        maintain(x);
        tree[p].children.push_back(x);
    }

    void markBad(int x, queue<int> &pending)
    {
        if (x < 0 || negInf[x])
        {
            return;
        }
        negInf[x] = true;
        tree[x].active = 0;
        pending.push(x);
    }

    void markPath(int x, int ancestor, queue<int> &pending)
    {
        while (x != -1 && x != ancestor)
        {
            markBad(x, pending);
            x = tree[x].parent;
        }
        markBad(ancestor, pending);
    }

    void propagate(queue<int> &pending)
    {
        while (!pending.empty())
        {
            int x = pending.front();
            pending.pop();
            for (int id : graph[x])
            {
                markBad(edges[id].to, pending);
            }
        }
    }

public:
    // n 是顶点数，edges 是 0 下标有向边，source 是源点；返回有限距离及负无穷标记。
    NegPath solve(int n_, const vector<NegEdge> &edges_, int source)
    {
        n = n_;
        edges = edges_;
        graph.assign(n, {});
        for (int i = 0; i < (int)edges.size(); i++)
        {
            graph[edges[i].from].push_back(i);
        }
        tree.assign(n, {});
        dist.assign(n, 0);
        reachable.assign(n, false);
        negInf.assign(n, false);

        queue<int> todo;
        todo.push(source);
        reachable[source] = true;
        tree[source].active = 1;
        tree[source].depth = 1;
        change(source, -1, 1);

        while (!todo.empty())
        {
            int x = todo.front();
            todo.pop();
            if (negInf[x] || !tree[x].active)
            {
                continue;
            }
            for (int id : graph[x])
            {
                const auto &edge = edges[id];
                int y = edge.to;
                int nd = dist[x] + edge.weight;
                if (reachable[y] && nd >= dist[y])
                {
                    continue;
                }
                dist[y] = nd;
                reachable[y] = true;
                todo.push(y);
                queue<int> pending;
                if (change(y, x, 0))
                {
                    markPath(x, y, pending);
                    propagate(pending);
                    continue;
                }
                tree[y].active = 1;
                cut(y);
                link(y, x);
            }
        }

        vector<optional<int>> finite(n);
        for (int i = 0; i < n; i++)
        {
            if (reachable[i] && !negInf[i])
            {
                finite[i] = dist[i];
            }
        }
        return {finite, negInf};
    }
};

// edges 使用 0 下标端点；返回负无穷点、不可达点和有限距离三种状态。
inline NegPath shortestPathWithNegInf(int n, const vector<NegEdge> &edges, int source)
{
    return NegPathSolver().solve(n, edges, source);
}
