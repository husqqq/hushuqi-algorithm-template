#include "../include/full/5_10_013.hpp"

bool valid(const vector<vector<int>> &g, int s, int t, const vector<int> &rank)
{
    int n = g.size();
    if ((int)rank.size() != n || rank[s] != 0 || rank[t] != n - 1)
    {
        return false;
    }
    vector<int> seen(n);
    for (int x : rank)
    {
        if (x < 0 || x >= n || seen[x]++)
        {
            return false;
        }
    }
    for (int u = 0; u < n; u++)
    {
        bool left = false, right = false;
        for (int v : g[u])
        {
            left |= rank[v] < rank[u];
            right |= rank[u] < rank[v];
        }
        if ((u != s && !left) || (u != t && !right))
        {
            return false;
        }
    }
    return true;
}

bool brute(const vector<vector<int>> &g, int s, int t)
{
    int n = g.size();
    vector<int> mid;
    for (int u = 0; u < n; u++)
    {
        if (u != s && u != t)
        {
            mid.push_back(u);
        }
    }
    do
    {
        vector<int> rank(n);
        rank[s] = 0;
        rank[t] = n - 1;
        for (int i = 0; i < (int)mid.size(); i++)
        {
            rank[mid[i]] = i + 1;
        }
        if (valid(g, s, t, rank))
        {
            return true;
        }
    } while (next_permutation(mid.begin(), mid.end()));
    return false;
}

signed main()
{
    for (int n = 2; n <= 6; n++)
    {
        vector<pair<int, int>> edge;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                edge.push_back({u, v});
            }
        }
        for (unsigned long long mask = 0; mask < (1ULL << edge.size()); mask++)
        {
            vector<vector<int>> g(n);
            for (int i = 0; i < (int)edge.size(); i++)
            {
                if (mask >> i & 1)
                {
                    auto [u, v] = edge[i];
                    g[u].push_back(v);
                    g[v].push_back(u);
                }
            }
            auto rank = stNumbering(g, 0, n - 1);
            assert(rank.has_value() == brute(g, 0, n - 1));
            if (rank.has_value())
            {
                assert(valid(g, 0, n - 1, *rank));
            }
        }
    }
    puts("OK");
    return 0;
}
