#include "../include/full/6_3_004_affine_sum.hpp"

constexpr long long MOD_AFFINE_SUM = 998244353;

vector<long long> bruteAffineSum(const vector<long long> &value,
                                 const vector<TreeAffEdge<MOD_AFFINE_SUM>> &edges)
{
    int n = value.size();
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        g[edges[i].u].push_back({edges[i].v, i});
        g[edges[i].v].push_back({edges[i].u, i});
    }
    vector<long long> answer(n);
    for (int root = 0; root < n; root++)
    {
        auto dfs = [&](auto &&self, int u, int p) -> pair<long long, int>
        {
            long long sum = value[u] % MOD_AFFINE_SUM;
            int count = 1;
            for (auto [v, id] : g[u]) if (v != p)
            {
                auto [childSum, childCount] = self(self, v, u);
                auto e = edges[id];
                sum = (sum + e.a * childSum + e.b * childCount) % MOD_AFFINE_SUM;
                count += childCount;
            }
            return {sum, count};
        };
        answer[root] = dfs(dfs, root, -1).first;
    }
    return answer;
}

signed main()
{
    mt19937 rng(20260822);
    for (int n = 1; n <= 25; n++)
    {
        for (int round = 0; round < 80; round++)
        {
            vector<long long> value(n);
            for (auto &x : value) x = rng() % 100;
            vector<TreeAffEdge<MOD_AFFINE_SUM>> edges;
            for (int v = 1; v < n; v++)
            {
                int p = rng() % v;
                edges.push_back({p, v, rng() % 100, rng() % 100});
            }
            assert(treeAffRoots<MOD_AFFINE_SUM>(value, edges) ==
                   bruteAffineSum(value, edges));

            vector<DynTreeAff<MOD_AFFINE_SUM>::Info> info(2 * n - 1);
            for (int u = 0; u < n; u++) info[u] = {true, value[u], 0};
            for (int i = 0; i < n - 1; i++) info[n + i] = {false, edges[i].a, edges[i].b};
            DynTreeAff<MOD_AFFINE_SUM> dynamic(info);
            for (int i = 0; i < n - 1; i++)
            {
                assert(dynamic.link(n + i, edges[i].u));
                assert(dynamic.link(n + i, edges[i].v));
            }
            for (int step = 0; step < 100; step++)
            {
                if (rng() & 1)
                {
                    int u = rng() % n;
                    value[u] = rng() % 100;
                    dynamic.setVertex(u, value[u]);
                }
                else if (n > 1)
                {
                    int i = rng() % (n - 1);
                    edges[i].a = rng() % 100;
                    edges[i].b = rng() % 100;
                    dynamic.setEdge(n + i, edges[i].a, edges[i].b);
                }
                auto expected = bruteAffineSum(value, edges);
                int root = rng() % n;
                assert(dynamic.query(root) == expected[root]);
            }
        }
    }
}
