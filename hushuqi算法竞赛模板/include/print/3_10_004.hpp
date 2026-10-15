#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DegreeBlock
{
    int B; // B 是重点度数阈值，约为 sqrt(2m)。
    vector<vector<int>> g, big; // g 是邻接表，big[u] 是 u 的重点邻居编号（id 值）。
    vector<int> id, w, sum; // id[u] 是重点编号（轻点为 -1），w 是点权，sum[k] 是重点 k 的邻点权值和。

    DegreeBlock(int n, const vector<array<int, 2>> &e, vector<int> init = {})
        : g(n), big(n), id(n, -1), w(move(init))
    {
        // n 是点数，e 是无向边 {u,v}（0 起、无自环），init 是初始点权（空表示全 0）。
        // 建邻接表，把度数不小于 B≈√(2m) 的点设为重点并预存其邻点权值和。
        assert(n >= 0 && (w.empty() || (int)w.size() == n)); // 调试检查，可删。
        w.resize(n);
        for (auto [u, v] : e)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && u != v); // 调试检查，可删。
            g[u].push_back(v);
            g[v].push_back(u);
        }
        B = max<int>(1, (int)sqrtl(2 * (int)e.size() + 1));
        for (int u = 0; u < n; u++)
        {
            if ((int)g[u].size() >= B)
            {
                id[u] = sum.size();
                sum.push_back(0);
            }
        }
        for (int u = 0; u < n; u++)
        {
            for (int v : g[u])
            {
                if (id[v] >= 0)
                {
                    big[u].push_back(id[v]);
                }
                if (id[u] >= 0)
                {
                    sum[id[u]] += w[v];
                }
            }
        }
    }

    void add(int u, int v)
    {
        // u 是点编号，v 是增量；令 w[u]+=v，并同步更新与 u 相邻的所有重点的邻点和。
        w[u] += v;
        for (int k : big[u])
        {
            sum[k] += v;
        }
    }

    int query(int u) const
    {
        // u 是点编号；返回 u 所有邻点的权值和（重边按重数计），重点直接取预存值。
        if (id[u] >= 0)
        {
            return sum[id[u]];
        }
        int ans = 0;
        for (int v : g[u])
        {
            ans += w[v];
        }
        return ans;
    }
};
