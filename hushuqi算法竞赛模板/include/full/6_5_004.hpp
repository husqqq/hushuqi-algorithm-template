#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Acc = long long>
vector<optional<Acc>> treeDepPack(const vector<int> &w,
                                  const vector<Acc> &val,
                                  const vector<int> &par,
                                  int cap)
{
    // w、val、par 是体积、价值和父节点，根的父节点为 -1；返回恰用容量 c 的最大值，c 不可达时为 nullopt。
    int n = w.size(), rt = n;
    assert((int)val.size() == n && (int)par.size() == n && cap >= 0); // 调试检查，可删
    vector<vector<int>> son(n + 1);
    for (int u = 0; u < n; u++)
    {
        assert(w[u] > 0 && (-1 <= par[u] && par[u] < n)); // 调试检查，可删
        son[par[u] < 0 ? rt : par[u]].push_back(u);
    }
    int seen = 0;
    auto dfs = [&](auto &&self, int u) -> vector<optional<Acc>>
    {
        // u 是虚根或实际物品；返回其子树满足父子依赖的容量状态。
        seen++;
        vector<optional<Acc>> f(cap + 1);
        if (u == rt)
        {
            f[0] = (Acc)0;
        }
        else if (w[u] <= cap)
        {
            f[w[u]] = val[u];
        }
        for (int v : son[u])
        {
            auto h = self(self, v);
            auto nf = f;
            for (int i = 0; i <= cap; i++)
            {
                if (!f[i] || (u != rt && i == 0))
                {
                    continue;
                }
                for (int j = 1; i + j <= cap; j++)
                {
                    if (h[j])
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
    auto ans = dfs(dfs, rt);
    assert(seen == n + 1); // 调试检查，可删：par 必须构成森林。
    return ans;
}
