#include "../include/full/5_7_006.hpp"

using Edge = tuple<int, int, long long>;

long long brute(const vector<vector<long long>> &weight, int mask,
                vector<long long> &memo)
{
    int n = weight.size();
    if (mask == (1LL << n) - 1)
    {
        return 0;
    }
    if (memo[mask] != -1)
    {
        return memo[mask];
    }
    int u = 0;
    while (mask >> u & 1)
    {
        u++;
    }
    long long ans = brute(weight, mask | (1LL << u), memo);
    for (int v = u + 1; v < n; v++)
    {
        if (!(mask >> v & 1) && weight[u][v] > 0)
        {
            ans = max(ans, weight[u][v] +
                               brute(weight, mask | (1LL << u) | (1LL << v), memo));
        }
    }
    return memo[mask] = ans;
}

void check(int n, const vector<Edge> &edges)
{
    vector<vector<long long>> weight(n, vector<long long>(n));
    for (auto [u, v, w] : edges)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        weight[u][v] = weight[v][u] = max(weight[u][v], w);
    }

    WBlossom<long long> solver(n);
    auto first = solver.solve(edges);
    auto second = solver.solve(edges);
    assert(first == second);

    auto [answer, matching] = first;
    assert(is_sorted(matching.begin(), matching.end()));
    vector<int> used(n);
    long long sum = 0;
    for (auto [u, v] : matching)
    {
        assert(0 <= u && u < v && v < n);
        assert(!used[u] && !used[v]);
        assert(weight[u][v] > 0);
        used[u] = used[v] = 1;
        sum += weight[u][v];
    }
    assert(sum == answer);

    vector<long long> memo(1LL << n, -1);
    assert(answer == brute(weight, 0, memo));
}

signed main()
{
    check(0, {});
    check(1, {});
    check(4, {{0, 1, 0}, {1, 2, -7}, {2, 3, -1}});
    check(4, {{1, 0, 3}, {0, 1, 8}, {1, 0, 5}, {2, 3, 7}, {3, 2, 4}});
    check(4, {{0, 1, 1000000000000LL}, {2, 3, 999999999999LL},
              {0, 2, 1000000000001LL}, {1, 3, 999999999997LL}});

    WBlossom<long long> reused(3);
    assert(reused.solve({{0, 1, 7}, {1, 2, 3}}).first == 7);
    assert(reused.solve({{0, 1, -4}, {1, 2, -2}}).first == 0);
    assert(reused.solve({}).first == 0);

    mt19937_64 rng(20261020);
    for (int iteration = 0; iteration < 3000; iteration++)
    {
        int n = rng() % 12;
        vector<Edge> edges;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3)
                {
                    long long w = (long long)(rng() % 131) - 50;
                    edges.push_back({u, v, w});
                    if (rng() % 7 == 0)
                    {
                        edges.push_back({v, u, (long long)(rng() % 131) - 50});
                    }
                }
            }
        }
        check(n, edges);
    }
    cout << "AC 20261020\n";
}
