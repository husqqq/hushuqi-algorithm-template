#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Acc = long long>
vector<optional<Acc>> treePack(const vector<vector<int>> &g, const vector<Acc> &val, int root, int k)
{
    // g 是无向树，val 是各点权，root 是根，k 是恰选点数；返回恰选 j 点的最大值，j 不可达时为 nullopt。
    int n = g.size();
    assert(n > 0 && (int)val.size() == n && 0 <= root && root < n && k >= 0); // 调试检查，可删
    auto dfs = [&](auto &&self, int u, int p) -> vector<optional<Acc>>
    {
        // u、p 是当前点及其父亲；返回 u 子树内各选点数的最优值。
        vector<optional<Acc>> f(min<int>(1, k) + 1);
        f[0] = 0;
        if (k)
        {
            f[1] = val[u];
        }
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            auto h = self(self, v, u);
            vector<optional<Acc>> nf(min<int>(k, (int)f.size() + (int)h.size() - 2) + 1);
            for (int i = 0; i < (int)f.size(); i++)
            {
                for (int j = 0; j < (int)h.size() && i + j <= k; j++)
                {
                    if (f[i] && h[j])
                    {
                        Acc cur = *f[i] + *h[j];
                        if (!nf[i + j] || *nf[i + j] < cur)
                        {
                            nf[i + j] = cur;
                        }
                    }
                }
            }
            f.swap(nf);
        }
        return f;
    };
    return dfs(dfs, root, -1);
}
