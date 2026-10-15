#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DegreeBlock
{
    int B;
    vector<vector<int>> g, big;
    vector<int> id, w, sum;

    DegreeBlock(int n, const vector<array<int, 2>> &e, vector<int> init = {})
        : g(n), big(n), id(n, -1), w(move(init))
    {
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
        w[u] += v;
        for (int k : big[u])
        {
            sum[k] += v;
        }
    }

    int query(int u) const
    {
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
