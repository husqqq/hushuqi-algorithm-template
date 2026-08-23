#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Coloring
{
    int k;             // k 是使用的颜色数。
    vector<int> color; // color[u] 是点 u 的颜色，编号从 0 开始。
};

class Dsatur
{
    int n, best;
    vector<unsigned long long> e;
    vector<int> col, ans;

    unsigned long long forbidden(int u) const
    {
        // u 是待染点；返回其已染邻居使用的颜色位集。
        unsigned long long mask = 0;
        unsigned long long q = e[u];
        while (q)
        {
            int v = countr_zero(q);
            q &= q - 1;
            if (col[v] >= 0)
            {
                mask |= 1ULL << col[v];
            }
        }
        return mask;
    }

    int pick() const
    {
        // 无参数；返回饱和度最大、再按未染邻居数最大的未染点。
        int at = -1;
        int sat = -1;
        int deg = -1;
        unsigned long long left = 0;
        for (int u = 0; u < n; u++)
        {
            if (col[u] < 0)
            {
                left |= 1ULL << u;
            }
        }
        for (int u = 0; u < n; u++)
        {
            if (col[u] >= 0)
            {
                continue;
            }
            int s = popcount(forbidden(u));
            int d = popcount(e[u] & left);
            if (s > sat || (s == sat && d > deg))
            {
                at = u;
                sat = s;
                deg = d;
            }
        }
        return at;
    }

    void dfs(int done, int used)
    {
        // done 是已染点数，used 是当前颜色数；搜索并更新最优方案，无返回值。
        if (used >= best)
        {
            return;
        }
        if (done == n)
        {
            best = used;
            ans = col;
            return;
        }
        int u = pick();
        unsigned long long ban = forbidden(u);
        for (int c = 0; c < used; c++)
        {
            if (ban >> c & 1)
            {
                continue;
            }
            col[u] = c;
            dfs(done + 1, used);
            col[u] = -1;
        }
        if (used + 1 < best)
        {
            col[u] = used;
            dfs(done + 1, used + 1);
            col[u] = -1;
        }
    }

  public:
    Dsatur(vector<unsigned long long> e) : n(e.size()), best(n + 1), e(move(e)), col(n, -1)
    {
        // e 是简单无向图邻接位集；构造精确染色器。
        assert(n <= 64); // 调试检查，可删。
        for (int u = 0; u < n; u++)
        {
            assert(!(this->e[u] >> u & 1)); // 调试检查，可删。
            for (int v = 0; v < n; v++)
            {
                assert((this->e[u] >> v & 1) == (this->e[v] >> u & 1)); // 调试检查，可删。
            }
        }
    }

    Coloring solve()
    {
        // 无参数；返回最少颜色数和一组染色方案。
        fill(col.begin(), col.end(), -1);
        int used = 0;
        for (int step = 0; step < n; step++)
        {
            int u = pick();
            unsigned long long ban = forbidden(u);
            int c = 0;
            while (ban >> c & 1)
            {
                c++;
            }
            col[u] = c;
            used = max(used, c + 1);
        }
        best = used;
        ans = col;
        fill(col.begin(), col.end(), -1);
        dfs(0, 0);
        return {best, ans};
    }
};
