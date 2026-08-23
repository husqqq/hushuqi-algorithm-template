#include "../include/full/6_2_001_frequency.hpp"

signed main()
{
    mt19937 rng(20260822);
    for (int n = 1; n <= 80; n++)
    {
        for (int round = 0; round < 60; round++)
        {
            vector<vector<int>> g(n);
            for (int v = 1; v < n; v++)
            {
                int p = rng() % v;
                g[p].push_back(v);
                g[v].push_back(p);
            }
            vector<long long> expected(max<int>(0, n - 1));
            for (int s = 0; s < n; s++)
            {
                vector<int> distance(n, -1), queue{s};
                distance[s] = 0;
                for (int i = 0; i < n; i++)
                {
                    int u = queue[i];
                    for (int v : g[u]) if (distance[v] < 0)
                    {
                        distance[v] = distance[u] + 1;
                        queue.push_back(v);
                    }
                }
                for (int v = s + 1; v < n; v++) expected[distance[v] - 1]++;
            }
            assert(treeDistFreq(g) == expected);
        }
    }
}
