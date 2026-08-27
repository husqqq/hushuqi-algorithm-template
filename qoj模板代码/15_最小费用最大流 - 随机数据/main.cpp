#include <bits/stdc++.h>
using namespace std;

template <class Cap = long long, class Cost = long long>
struct CostFlow
{
    struct Edge
    {
        int to;
        int reverse;
        Cap cap;
        Cost cost;
    };

    int n;
    vector<vector<Edge>> graph;
    vector<Cost> potential;
    bool potentialReady = false;

    explicit CostFlow(int vertexCount)
        : n(vertexCount), graph(n), potential(n)
    {
    }

    void addEdge(int from, int to, Cap cap, Cost cost)
    {
        int fromId = (int)graph[from].size();
        int toId = (int)graph[to].size() + (from == to);
        graph[from].push_back({to, toId, cap, cost});
        graph[to].push_back({from, fromId, 0, -cost});
        potentialReady = false;
    }

    void initializePotential(int source)
    {
        vector<optional<Cost>> distance(n);
        vector<char> inQueue(n);
        queue<int> q;

        distance[source] = 0;
        inQueue[source] = true;
        q.push(source);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            inQueue[u] = false;

            for (const auto &edge : graph[u])
            {
                if (edge.cap == 0)
                {
                    continue;
                }

                Cost nextDistance = *distance[u] + edge.cost;
                if (!distance[edge.to].has_value() ||
                    nextDistance < *distance[edge.to])
                {
                    distance[edge.to] = nextDistance;
                    if (!inQueue[edge.to])
                    {
                        inQueue[edge.to] = true;
                        q.push(edge.to);
                    }
                }
            }
        }

        fill(potential.begin(), potential.end(), 0);
        for (int u = 0; u < n; u++)
        {
            if (distance[u].has_value())
            {
                potential[u] = *distance[u];
            }
        }
        potentialReady = true;
    }

    pair<Cap, Cost> minCostMaxFlow(int source, int sink)
    {
        if (source == sink)
        {
            return {0, 0};
        }
        if (!potentialReady)
        {
            initializePotential(source);
        }

        Cap totalFlow = 0;
        Cost totalCost = 0;
        vector<int> previousVertex(n);
        vector<int> previousEdge(n);

        while (true)
        {
            vector<optional<Cost>> distance(n);
            priority_queue<pair<Cost, int>,
                           vector<pair<Cost, int>>,
                           greater<>> q;

            distance[source] = 0;
            q.push({0, source});

            while (!q.empty())
            {
                auto [currentDistance, u] = q.top();
                q.pop();
                if (!distance[u].has_value() ||
                    currentDistance != *distance[u])
                {
                    continue;
                }

                for (int edgeId = 0;
                     edgeId < (int)graph[u].size();
                     edgeId++)
                {
                    const auto &edge = graph[u][edgeId];
                    if (edge.cap == 0)
                    {
                        continue;
                    }

                    Cost nextDistance = currentDistance + edge.cost +
                                        potential[u] - potential[edge.to];
                    if (!distance[edge.to].has_value() ||
                        nextDistance < *distance[edge.to])
                    {
                        distance[edge.to] = nextDistance;
                        previousVertex[edge.to] = u;
                        previousEdge[edge.to] = edgeId;
                        q.push({nextDistance, edge.to});
                    }
                }
            }

            if (!distance[sink].has_value())
            {
                break;
            }

            for (int u = 0; u < n; u++)
            {
                if (distance[u].has_value())
                {
                    potential[u] += *distance[u];
                }
            }

            Cap pushed = numeric_limits<Cap>::max();
            for (int u = sink; u != source; u = previousVertex[u])
            {
                const auto &edge = graph[previousVertex[u]][previousEdge[u]];
                pushed = min(pushed, edge.cap);
            }

            for (int u = sink; u != source; u = previousVertex[u])
            {
                auto &edge = graph[previousVertex[u]][previousEdge[u]];
                edge.cap -= pushed;
                graph[u][edge.reverse].cap += pushed;
            }

            totalFlow += pushed;
            totalCost += (Cost)pushed *
                         (potential[sink] - potential[source]);
        }

        return {totalFlow, totalCost};
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    CostFlow flow(n);
    for (int i = 0; i < m; i++)
    {
        int from, to;
        long long cap, cost;
        cin >> from >> to >> cap >> cost;
        flow.addEdge(from - 1, to - 1, cap, cost);
    }

    auto [maxFlow, minCost] = flow.minCostMaxFlow(0, n - 1);
    cout << maxFlow << ' ' << minCost << '\n';
    return 0;
}
