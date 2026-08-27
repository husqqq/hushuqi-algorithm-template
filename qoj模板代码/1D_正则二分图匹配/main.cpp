#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, degree;
    cin >> n >> degree;

    vector<vector<int>> graph(n, vector<int>(degree));
    for (auto &edges : graph)
    {
        for (int &v : edges)
        {
            cin >> v;
            v--;
        }
    }

    vector<int> leftMatch(n, -1);
    vector<int> rightMatch(n, -1);
    vector<int> distance(n);

    auto bfs = [&]()
    {
        queue<int> q;
        for (int u = 0; u < n; u++)
        {
            if (leftMatch[u] == -1)
            {
                distance[u] = 0;
                q.push(u);
            }
            else
            {
                distance[u] = -1;
            }
        }

        bool found = false;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int v : graph[u])
            {
                if (rightMatch[v] == -1)
                {
                    found = true;
                }
                else if (distance[rightMatch[v]] == -1)
                {
                    distance[rightMatch[v]] = distance[u] + 1;
                    q.push(rightMatch[v]);
                }
            }
        }
        return found;
    };

    auto dfs = [&](auto &&self, int u) -> bool
    {
        for (int v : graph[u])
        {
            int next = rightMatch[v];
            if (next == -1 ||
                (distance[next] == distance[u] + 1 && self(self, next)))
            {
                leftMatch[u] = v;
                rightMatch[v] = u;
                return true;
            }
        }

        distance[u] = -1;
        return false;
    };

    while (bfs())
    {
        for (int u = 0; u < n; u++)
        {
            if (leftMatch[u] == -1)
            {
                dfs(dfs, u);
            }
        }
    }

    for (int u = 0; u < n; u++)
    {
        cout << leftMatch[u] + 1 << " \n"[u + 1 == n];
    }
    return 0;
}
