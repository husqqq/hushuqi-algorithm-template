#pragma once

#include "12_1_001.hpp"

template <class Acc = long long>
vector<optional<Acc>> treePack(const vector<vector<int>> &g,
                                      const vector<array<long long, 3>> &a,
                                      int root,
                                      int cap)
{
    int n = g.size();
    assert(n > 0 && (int)a.size() == n && 0 <= root && root < n && cap >= 0);
    auto dfs = [&](auto &&self, int u, int p) -> vector<optional<Acc>>
    {
        int w = a[u][0];
        Acc v = (Acc)a[u][1];
        int cnt = a[u][2];
        assert(w > 0 && cnt >= 0);
        vector<optional<Acc>> pre(cap + 1), f(cap + 1);
        pre[0] = (Acc)0;
        for (int r = 0; r < w && r <= cap; r++)
        {
            deque<int> q;
            for (int j = 0, x = r; x <= cap; j++, x += w)
            {
                while (!q.empty() && q.front() < j - cnt)
                {
                    q.pop_front();
                }
                if (pre[x])
                {
                    Acc cur = *pre[x] - (Acc)j * v;
                    while (!q.empty())
                    {
                        int y = r + q.back() * w;
                        Acc old = *pre[y] - (Acc)q.back() * v;
                        if (old > cur)
                        {
                            break;
                        }
                        q.pop_back();
                    }
                    q.push_back(j);
                }
                if (!q.empty())
                {
                    int y = r + q.front() * w;
                    f[x] = *pre[y] + (Acc)(j - q.front()) * v;
                }
            }
        }
        for (int x : g[u])
        {
            if (x == p)
            {
                continue;
            }
            auto h = self(self, x, u);
            auto nf = f;
            for (int i = 0; i <= cap; i++)
            {
                for (int j = 1; i + j <= cap; j++)
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
