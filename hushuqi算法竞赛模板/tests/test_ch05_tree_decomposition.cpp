#include "../include/full/5_10_014.hpp"

bool bruteTw2(vector<unsigned long long> g, unsigned long long alive)
{
    if (!alive)
    {
        return true;
    }
    for (auto q = alive; q; q &= q - 1)
    {
        int v = countr_zero(q);
        unsigned long long nb = g[v] & alive;
        if (popcount(nb) > 2)
        {
            continue;
        }
        auto h = g;
        if (popcount(nb) == 2)
        {
            int a = countr_zero(nb);
            int b = countr_zero(nb & (nb - 1));
            h[a] |= 1ULL << b;
            h[b] |= 1ULL << a;
        }
        if (bruteTw2(h, alive & ~(1ULL << v)))
        {
            return true;
        }
    }
    return false;
}

bool valid(int n, const vector<pair<int, int>> &edge, const TreeDecomp2 &td)
{
    int k = td.bag.size();
    if (n == 0)
    {
        return k == 0 && td.edge.empty();
    }
    if (k == 0 || (int)td.edge.size() != k - 1)
    {
        return false;
    }
    vector<vector<int>> tr(k);
    for (auto [u, v] : td.edge)
    {
        if (u < 0 || u >= k || v < 0 || v >= k || u == v)
        {
            return false;
        }
        tr[u].push_back(v);
        tr[v].push_back(u);
    }
    vector<int> vis(k);
    queue<int> q;
    q.push(0);
    vis[0] = 1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : tr[u])
        {
            if (!vis[v])
            {
                vis[v] = 1;
                q.push(v);
            }
        }
    }
    if (count(vis.begin(), vis.end(), 1) != k)
    {
        return false;
    }
    vector<vector<int>> at(n);
    for (int i = 0; i < k; i++)
    {
        if (td.bag[i].empty() || td.bag[i].size() > 3)
        {
            return false;
        }
        for (int v : td.bag[i])
        {
            if (v < 0 || v >= n)
            {
                return false;
            }
            at[v].push_back(i);
        }
    }
    for (int v = 0; v < n; v++)
    {
        if (at[v].empty())
        {
            return false;
        }
        vector<int> use(k);
        for (int x : at[v])
        {
            use[x] = 1;
        }
        queue<int> bfs;
        bfs.push(at[v][0]);
        use[at[v][0]] = 2;
        int cnt = 0;
        while (!bfs.empty())
        {
            int x = bfs.front();
            bfs.pop();
            cnt++;
            for (int y : tr[x])
            {
                if (use[y] == 1)
                {
                    use[y] = 2;
                    bfs.push(y);
                }
            }
        }
        if (cnt != (int)at[v].size())
        {
            return false;
        }
    }
    for (auto [u, v] : edge)
    {
        bool found = false;
        for (const auto &bag : td.bag)
        {
            found |= find(bag.begin(), bag.end(), u) != bag.end() &&
                     find(bag.begin(), bag.end(), v) != bag.end();
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}

signed main()
{
    assert(treeDecomp2(0, {}).has_value());
    for (int n = 1; n <= 6; n++)
    {
        vector<pair<int, int>> all;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                all.push_back({u, v});
            }
        }
        for (unsigned long long mask = 0; mask < (1ULL << all.size()); mask++)
        {
            vector<pair<int, int>> edge;
            vector<unsigned long long> g(n);
            for (int i = 0; i < (int)all.size(); i++)
            {
                if (mask >> i & 1)
                {
                    auto [u, v] = all[i];
                    edge.push_back({u, v});
                    g[u] |= 1ULL << v;
                    g[v] |= 1ULL << u;
                }
            }
            bool ok = bruteTw2(g, (1ULL << n) - 1);
            auto td = treeDecomp2(n, edge);
            assert(td.has_value() == ok);
            if (td.has_value())
            {
                assert(valid(n, edge, *td));
            }
        }
    }
    puts("OK");
    return 0;
}
