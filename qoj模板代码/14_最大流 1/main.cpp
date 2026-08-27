#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

struct Dinic
{
    struct Edge
    {
        int to;
        int rev;
        i64 cap;
    };

    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> current;

    explicit Dinic(int n) : graph(n), level(n), current(n) {}

    void addEdge(int from, int to, i64 cap)
    {
        int fromId = (int)graph[from].size();
        int toId = (int)graph[to].size() + (from == to);
        graph[from].push_back({to, toId, cap});
        graph[to].push_back({from, fromId, 0});
    }

    bool buildLevelGraph(int source, int sink)
    {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (const auto &edge : graph[u])
            {
                if (edge.cap > 0 && level[edge.to] == -1)
                {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[sink] != -1;
    }

    i64 sendFlow(int u, int sink, i64 limit)
    {
        if (u == sink)
        {
            return limit;
        }

        i64 sent = 0;
        for (int &id = current[u]; id < (int)graph[u].size(); id++)
        {
            auto &edge = graph[u][id];
            if (edge.cap == 0 || level[edge.to] != level[u] + 1)
            {
                continue;
            }

            i64 pushed = sendFlow(edge.to, sink, min(limit - sent, edge.cap));
            edge.cap -= pushed;
            graph[edge.to][edge.rev].cap += pushed;
            sent += pushed;
            if (sent == limit)
            {
                break;
            }
        }
        return sent;
    }

    i64 maxFlow(int source, int sink)
    {
        i64 answer = 0;
        while (buildLevelGraph(source, sink))
        {
            fill(current.begin(), current.end(), 0);
            answer += sendFlow(source, sink, numeric_limits<i64>::max());
        }
        return answer;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source, sink;
    cin >> n >> m >> source >> sink;
    source--;
    sink--;

    Dinic flow(n);
    for (int i = 0; i < m; i++)
    {
        int from, to;
        i64 cap;
        cin >> from >> to >> cap;
        flow.addEdge(from - 1, to - 1, cap);
    }

    cout << flow.maxFlow(source, sink) << '\n';
    return 0;
}
