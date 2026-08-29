#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

class DelDAG
{
    // 成员变量：n 保存图的点数。
    int n;
    // 成员变量：graph 保存输入图的邻接表。
    vector<vector<int>> graph;
    // 成员变量：edges 保存全部原图边。
    vector<pair<int, int>> edges;

  public:
    explicit DelDAG(int n) : n(n), graph(n)
    {
        // 参数 n 表示 图的点数；构造指定点数的删点 DAG 维护器，无返回值。
    }
    void addEdge(int u, int v)
    {
        // 参数 u 表示 一条边的起点或当前 DFS 点；v 表示 一条边的终点或相邻点；加入一条参数指定的图边并维护邻接表，无返回值。
        graph[u].push_back(v);
        edges.push_back({u, v});
    }

    // 依次删除每个点，返回剩余图中的最长路径边数。
    vector<int> solve() const
    {
        // 无参数；返回依次删除每个点后，剩余 DAG 的最长路径边数。
        vector<int> indegree(n);
        for (auto [u, v] : edges)
        {
            ++indegree[v];
        }
        queue<int> queue;
        for (int u = 0; u < n; ++u)
        {
            if (!indegree[u])
            {
                queue.push(u);
            }
        }
        vector<int> order;
        while (!queue.empty())
        {
            int u = queue.front();
            queue.pop();
            order.push_back(u);
            for (int v : graph[u])
            {
                if (!--indegree[v])
                {
                    queue.push(v);
                }
            }
        }
        assert(order.size() == n);
        vector<int> position(n), left(n), right(n);
        for (int i = 0; i < n; ++i)
        {
            position[order[i]] = i;
        }
        for (int u : order)
        {
            for (int v : graph[u])
            {
                left[v] = max(left[v], left[u] + 1);
            }
        }
        for (int u : order | views::reverse)
        {
            for (int v : graph[u])
            {
                right[u] = max(right[u], right[v] + 1);
            }
        }
        vector<vector<pair<int, int>>> starts(n + 1);
        for (auto [u, v] : edges)
        {
            int begin = position[u] + 1, end = position[v];
            if (begin < end)
            {
                starts[begin].push_back({left[u] + 1 + right[v], end});
            }
        }
        vector<int> prefix(n + 1), suffix(n + 1), ans(n);
        for (int i = 0; i < n; ++i)
        {
            prefix[i + 1] = max(prefix[i], left[order[i]]);
        }
        for (int i = n - 1; i >= 0; --i)
        {
            suffix[i] = max(suffix[i + 1], right[order[i]]);
        }
        priority_queue<pair<int, int>> active;
        for (int i = 0; i < n; ++i)
        {
            for (auto item : starts[i])
            {
                active.push(item);
            }
            while (!active.empty() && active.top().second <= i)
            {
                active.pop();
            }
            ans[order[i]] = max(prefix[i], suffix[i + 1]);
            if (!active.empty())
            {
                ans[order[i]] = max(ans[order[i]], active.top().first);
            }
        }
        return ans;
    }
};
