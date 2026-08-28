#include <bits/stdc++.h>
using namespace std;

class PathLengthQueries
{
    int n;
    int maxQuery;
    vector<vector<pair<int, int>>> g;
    vector<int> size;
    vector<char> removed;
    vector<char> seen;
    vector<int> query;
    vector<char> answer;

    int calcSize(int u, int parent)
    {
        size[u] = 1;
        for (auto [v, w] : g[u])
        {
            if (v != parent && !removed[v])
            {
                size[u] += calcSize(v, u);
            }
        }
        return size[u];
    }

    int findCentroid(int u, int parent, int total)
    {
        for (auto [v, w] : g[u])
        {
            if (v != parent && !removed[v] && size[v] > total / 2)
            {
                return findCentroid(v, u, total);
            }
        }
        return u;
    }

    void collect(int u, int parent, int distance, vector<int> &distances)
    {
        if (distance > maxQuery)
        {
            return;
        }
        distances.push_back(distance);
        for (auto [v, w] : g[u])
        {
            if (v != parent && !removed[v])
            {
                collect(v, u, distance + w, distances);
            }
        }
    }

    void decompose(int start)
    {
        int total = calcSize(start, -1);
        int centroid = findCentroid(start, -1, total);
        removed[centroid] = true;

        vector<int> touched{0};
        seen[0] = true;
        for (auto [v, w] : g[centroid])
        {
            if (removed[v])
            {
                continue;
            }
            vector<int> distances;
            collect(v, centroid, w, distances);
            for (int distance : distances)
            {
                for (int i = 0; i < (int)query.size(); i++)
                {
                    if (!answer[i] && query[i] >= distance && seen[query[i] - distance])
                    {
                        answer[i] = true;
                    }
                }
            }
            for (int distance : distances)
            {
                if (!seen[distance])
                {
                    seen[distance] = true;
                    touched.push_back(distance);
                }
            }
        }
        for (int distance : touched)
        {
            seen[distance] = false;
        }

        for (auto [v, w] : g[centroid])
        {
            if (!removed[v])
            {
                decompose(v);
            }
        }
    }

  public:
    PathLengthQueries(vector<vector<pair<int, int>>> graph, vector<int> queries)
        : n(graph.size()), maxQuery(*max_element(queries.begin(), queries.end())), g(move(graph)), size(n),
          removed(n), seen(maxQuery + 1), query(move(queries)), answer(query.size())
    {
    }

    vector<char> solve()
    {
        if (n)
        {
            decompose(0);
        }
        return answer;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> g(n);
    for (int i = 1; i < n; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    vector<int> query(m);
    for (auto &x : query)
    {
        cin >> x;
    }
    for (char ok : PathLengthQueries(move(g), move(query)).solve())
    {
        cout << (ok ? "Yes" : "No") << '\n';
    }
}
