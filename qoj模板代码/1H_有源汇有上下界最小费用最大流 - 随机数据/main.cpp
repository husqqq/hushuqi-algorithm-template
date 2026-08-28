// Generated from the protected candidate template.hpp and main.cpp.in.

// Candidate network-simplex implementation for QOJ 2021.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;

template <class Capacity = long long, class Cost = long long, class Answer = __int128_t>
class NetworkSimplexBFlow
{
    struct InputEdge
    {
        int from, to;
        Capacity lower, upper;
        Cost cost;
    };

    struct NetworkSimplex
    {
        struct Edge
        {
            int from, to;
            Capacity residual, capacity;
            Cost cost;
        };

        int n, edgeCount = 2, version = 0;
        Capacity flow = 0, maximumFlow;
        Cost penalty;
        Answer cost = 0;
        vector<Edge> edges{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
        vector<vector<int>> adjacent;
        vector<Cost> potential;
        vector<int> parent, parentEdge, cycle, tag;

        NetworkSimplex(int nodeCount, Capacity maximumFlow, Cost penalty)
            : n(nodeCount + 1), maximumFlow(maximumFlow), penalty(penalty), adjacent(n),
              potential(n), parent(n), parentEdge(n), cycle(n + 1), tag(n)
        {
        }

        void add(int from, int to, Capacity capacity, Cost cost)
        {
            from++;
            to++;
            edges.push_back({from, to, capacity, capacity, cost});
            edges.push_back({to, from, 0, 0, -cost});
            adjacent[from].push_back(edgeCount++);
            adjacent[to].push_back(edgeCount++);
        }

        void initializeForest(int root)
        {
            int stamp = ++version;
            vector<int> stack;
            tag[0] = stamp;
            auto visitComponent = [&](int start)
            {
                tag[start] = stamp;
                parent[start] = 0;
                parentEdge[start] = 0;
                stack.push_back(start);
                while (!stack.empty())
                {
                    int u = stack.back();
                    stack.pop_back();
                    for (int id : adjacent[u])
                    {
                        int v = edges[id].to;
                        if (edges[id].residual == 0 || tag[v] == stamp) continue;
                        tag[v] = stamp;
                        parent[v] = u;
                        parentEdge[v] = id;
                        stack.push_back(v);
                    }
                }
            };
            visitComponent(root);
            for (int u = 1; u < n; u++)
            {
                if (tag[u] != stamp) visitComponent(u);
            }
        }

        Cost pathCost(int u)
        {
            if (tag[u] == version) return potential[u];
            tag[u] = version;
            potential[u] = pathCost(parent[u]) + edges[parentEdge[u]].cost;
            return potential[u];
        }

        Answer pivot(int entering)
        {
            int root = edges[entering].from;
            int lca = edges[entering].to;
            int count = 0, leavingVertex = 0, direction = 2;
            ++version;
            while (root)
            {
                tag[root] = version;
                root = parent[root];
            }
            while (tag[lca] != version)
            {
                tag[lca] = version;
                lca = parent[lca];
            }

            Capacity pushed = edges[entering].residual;
            for (int u = edges[entering].from; u != lca; u = parent[u])
            {
                cycle[++count] = parentEdge[u];
                if (pushed > edges[parentEdge[u]].residual)
                {
                    pushed = edges[parentEdge[u]].residual;
                    leavingVertex = u;
                    direction = 0;
                }
            }
            for (int u = edges[entering].to; u != lca; u = parent[u])
            {
                cycle[++count] = parentEdge[u] ^ 1;
                if (pushed > edges[parentEdge[u] ^ 1].residual)
                {
                    pushed = edges[parentEdge[u] ^ 1].residual;
                    leavingVertex = u;
                    direction = 1;
                }
            }
            cycle[++count] = entering;
            Answer delta = 0;
            for (int i = 1; i <= count; i++)
            {
                int id = cycle[i];
                delta += (Answer)edges[id].cost * pushed;
                edges[id].residual -= pushed;
                edges[id ^ 1].residual += pushed;
            }
            if (direction == 2) return delta;

            int u = edges[entering].from, v = edges[entering].to;
            if (direction == 1) swap(u, v);
            int lastEdge = entering ^ direction, lastVertex = v;
            while (lastVertex != leavingVertex)
            {
                lastEdge ^= 1;
                tag[u]--;
                swap(parentEdge[u], lastEdge);
                int nextVertex = parent[u];
                parent[u] = lastVertex;
                lastVertex = u;
                u = nextVertex;
            }
            return delta;
        }

        pair<Capacity, Answer> solve(int source, int sink)
        {
            add(sink, source, maximumFlow, -penalty);
            source++;
            sink++;
            initializeForest(sink);
            tag[sink] = ++version;
            tag[0] = version;
            parent[sink] = 0;
            potential[sink] = 0;
            bool changed = true;
            while (changed)
            {
                changed = false;
                for (int id = 2; id < edgeCount; id++)
                {
                    const Edge &edge = edges[id];
                    if (edge.residual > 0 &&
                        edge.cost + pathCost(edge.from) - pathCost(edge.to) < 0)
                    {
                        cost += pivot(id);
                        changed = true;
                    }
                }
            }
            const Edge &artificialReverse = edges.back();
            flow = artificialReverse.residual;
            cost += (Answer)flow * penalty;
            return {flow, cost};
        }
    };

    int n;
    vector<InputEdge> inputEdges;
    vector<Capacity> excess;

public:
    explicit NetworkSimplexBFlow(int n) : n(n), excess(n)
    {
        assert(n >= 0);
    }

    void add(int from, int to, Capacity lower, Capacity upper, Cost cost)
    {
        assert(0 <= from && from < n && 0 <= to && to < n && lower <= upper);
        inputEdges.push_back({from, to, lower, upper, cost});
        excess[from] -= lower;
        excess[to] += lower;
    }

    void supply(int vertex, Capacity value)
    {
        assert(0 <= vertex && vertex < n);
        excess[vertex] += value;
    }

    optional<Answer> solve() const
    {
        if (accumulate(excess.begin(), excess.end(), Capacity{}) != 0) return nullopt;
        int superSource = n, superSink = n + 1;
        Capacity required = 0;
        __int128 penaltyWide = 1;
        Answer baseCost = 0;
        for (const auto &edge : inputEdges)
        {
            penaltyWide += edge.cost >= 0 ? (__int128)edge.cost : -(__int128)edge.cost;
            baseCost += (Answer)edge.lower * edge.cost;
        }
        assert(penaltyWide <= numeric_limits<Cost>::max());
        NetworkSimplex simplex(n + 2, Capacity{}, (Cost)penaltyWide);
        for (const auto &edge : inputEdges)
        {
            simplex.add(edge.from, edge.to, edge.upper - edge.lower, edge.cost);
        }
        for (int u = 0; u < n; u++)
        {
            if (excess[u] > 0)
            {
                simplex.add(superSource, u, excess[u], 0);
                required += excess[u];
            }
            else if (excess[u] < 0)
            {
                simplex.add(u, superSink, -excess[u], 0);
            }
        }
        simplex.maximumFlow = required;
        auto [sent, extraCost] = simplex.solve(superSource, superSink);
        if (sent != required) return nullopt;
        return baseCost + extraCost;
    }
};

template <class Capacity = long long, class Cost = long long>
struct BoundedCostEdge
{
    int from, to;
    Capacity lower, upper;
    Cost cost;
};

template <class Capacity = long long, class Answer = __int128_t>
struct MinCostMaxFlowResult
{
    Capacity flow;
    Answer cost;
};

namespace lower_bound_mcmf_detail
{
template <class Capacity>
struct Dinic
{
    struct Edge
    {
        int to, reverse;
        Capacity capacity;
    };
    vector<vector<Edge>> graph;
    vector<int> level, current;

    explicit Dinic(int n) : graph(n), level(n), current(n) {}

    int add(int from, int to, Capacity capacity)
    {
        int index = graph[from].size();
        graph[from].push_back({to, (int)graph[to].size(), capacity});
        graph[to].push_back({from, index, 0});
        return index;
    }

    bool bfs(int source, int sink)
    {
        fill(level.begin(), level.end(), -1);
        vector<int> queue(graph.size());
        int begin = 0, end = 0;
        level[source] = 0;
        queue[end++] = source;
        while (begin < end)
        {
            int u = queue[begin++];
            for (const Edge &edge : graph[u])
            {
                if (edge.capacity > 0 && level[edge.to] == -1)
                {
                    level[edge.to] = level[u] + 1;
                    queue[end++] = edge.to;
                }
            }
        }
        return level[sink] != -1;
    }

    Capacity dfs(int u, int sink, Capacity pushed)
    {
        if (u == sink) return pushed;
        for (int &i = current[u]; i < (int)graph[u].size(); i++)
        {
            Edge &edge = graph[u][i];
            if (edge.capacity == 0 || level[edge.to] != level[u] + 1) continue;
            Capacity sent = dfs(edge.to, sink, min(pushed, edge.capacity));
            if (sent == 0) continue;
            edge.capacity -= sent;
            graph[edge.to][edge.reverse].capacity += sent;
            return sent;
        }
        return 0;
    }

    Capacity flow(int source, int sink)
    {
        Capacity answer = 0;
        constexpr Capacity infinity = numeric_limits<Capacity>::max() / 4;
        while (bfs(source, sink))
        {
            fill(current.begin(), current.end(), 0);
            while (Capacity sent = dfs(source, sink, infinity)) answer += sent;
        }
        return answer;
    }
};
}

template <class Capacity = long long, class Cost = long long, class Answer = __int128_t>
optional<MinCostMaxFlowResult<Capacity, Answer>> lowerBoundMinCostMaxFlow(
    int n,
    int source,
    int sink,
    const vector<BoundedCostEdge<Capacity, Cost>> &edges)
{
    assert(0 <= source && source < n && 0 <= sink && sink < n && source != sink);
    int superSource = n, superSink = n + 1;
    lower_bound_mcmf_detail::Dinic<Capacity> feasibility(n + 2);
    vector<Capacity> balance(n);
    for (const auto &edge : edges)
    {
        assert(0 <= edge.from && edge.from < n && 0 <= edge.to && edge.to < n);
        assert(0 <= edge.lower && edge.lower <= edge.upper);
        feasibility.add(edge.from, edge.to, edge.upper - edge.lower);
        balance[edge.from] -= edge.lower;
        balance[edge.to] += edge.lower;
    }
    constexpr Capacity infinity = numeric_limits<Capacity>::max() / 4;
    int returnIndex = feasibility.add(sink, source, infinity);
    Capacity required = 0;
    for (int u = 0; u < n; u++)
    {
        if (balance[u] > 0)
        {
            feasibility.add(superSource, u, balance[u]);
            required += balance[u];
        }
        else if (balance[u] < 0)
        {
            feasibility.add(u, superSink, -balance[u]);
        }
    }
    if (feasibility.flow(superSource, superSink) != required) return nullopt;

    auto &returnEdge = feasibility.graph[sink][returnIndex];
    Capacity maximumFlow = feasibility.graph[source][returnEdge.reverse].capacity;
    returnEdge.capacity = 0;
    feasibility.graph[source][returnEdge.reverse].capacity = 0;
    for (int u = 0; u < n + 2; u++)
    {
        for (auto &edge : feasibility.graph[u])
        {
            if (u >= n || edge.to >= n) edge.capacity = 0;
        }
    }
    maximumFlow += feasibility.flow(source, sink);

    NetworkSimplexBFlow<Capacity, Cost, Answer> minimumCost(n);
    for (const auto &edge : edges)
    {
        minimumCost.add(edge.from, edge.to, edge.lower, edge.upper, edge.cost);
    }
    minimumCost.supply(source, maximumFlow);
    minimumCost.supply(sink, -maximumFlow);
    optional<Answer> cost = minimumCost.solve();
    if (!cost) return nullopt;
    return MinCostMaxFlowResult<Capacity, Answer>{maximumFlow, *cost};
}
string toString(__int128 value)
{
    if (value == 0) return "0";
    bool negative = value < 0;
    if (negative) value = -value;
    string result;
    while (value)
    {
        result.push_back((char)('0' + value % 10));
        value /= 10;
    }
    if (negative) result.push_back('-');
    reverse(result.begin(), result.end());
    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source, sink;
    cin >> n >> m >> source >> sink;
    source--;
    sink--;
    vector<BoundedCostEdge<>> edges(m);
    for (auto &[from, to, lower, upper, cost] : edges)
    {
        cin >> from >> to >> lower >> upper >> cost;
        from--;
        to--;
    }
    auto answer = lowerBoundMinCostMaxFlow(n, source, sink, edges);
    if (!answer) cout << -1 << '\n';
    else cout << answer->flow << ' ' << toString(answer->cost) << '\n';
}
