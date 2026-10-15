#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct FuncGraph
{
    int n; // n 是点数，点编号为 [0,n)。
    // to[v] 是 v 的唯一后继；comp[v] 是所在连通块（即所在环）的编号。
    vector<int> to, comp;
    // pos[v] 是环上点在 cycles[comp[v]] 中的下标，树上点为 -1；entry[v] 是 v 第一次到达的环点。
    vector<int> pos, entry;
    // depth[v] 是 v 走到环上所需的步数（环上点为 0）；len[v] 是所在环的长度。
    vector<int> depth, len;
    vector<vector<int>> cycles; // cycles[i] 按后继方向列出第 i 个环。
    int L; // L 是倍增层数，满足 2^L > n。
    // up[j][v] 是 v 走 2^j 步后的点；sum、mn 是这 2^j 步离开的各点权值之和与最小值。
    vector<vector<int>> up, sum, mn;

    FuncGraph(vector<int> f, vector<int> w = {}) : n(f.size()), to(move(f)), comp(n, -1), pos(n, -1),
        entry(n), depth(n), len(n), L(bit_width((unsigned long long)n)), up(L, vector<int>(n)),
        sum(L, vector<int>(n)), mn(L, vector<int>(n))
    {
        // f[v] 是 v 的后继，w[v] 是点权（为空时全为 0），须满足 n*max|w| 不超过 LLONG_MAX；
        // 建立环分解与 L 层倍增表。
        if (w.empty())
        {
            w.assign(n, 0);
        }
        assert(w.size() == to.size()); // 调试检查，可删。
        // 反复删入度为 0 的点，剩下的恰是环；order 是被删点的顺序。
        vector<int> deg(n), order;
        for (int v : to)
        {
            deg[v]++;
        }
        queue<int> q;
        for (int i = 0; i < n; i++)
        {
            if (!deg[i])
            {
                q.push(i);
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            order.push_back(u);
            if (!--deg[to[u]])
            {
                q.push(to[u]);
            }
        }
        for (int s = 0; s < n; s++)
        {
            if (deg[s] && comp[s] < 0)
            {
                int id = cycles.size(), u = s;
                vector<int> c;
                do
                {
                    comp[u] = id;
                    pos[u] = c.size();
                    entry[u] = u;
                    c.push_back(u);
                    u = to[u];
                } while (u != s);
                for (int v : c)
                {
                    len[v] = c.size();
                }
                cycles.push_back(move(c));
            }
        }
        // 逆删点顺序处理树上点：后继总是先被处理，可直接继承其信息。
        reverse(order.begin(), order.end());
        for (int u : order)
        {
            int v = to[u];
            comp[u] = comp[v];
            entry[u] = entry[v];
            depth[u] = depth[v] + 1;
            len[u] = len[v];
        }
        for (int i = 0; i < n; i++)
        {
            up[0][i] = to[i];
            sum[0][i] = w[i];
            mn[0][i] = w[i];
        }
        for (int j = 1; j < L; j++)
        {
            for (int i = 0; i < n; i++)
            {
                int v = up[j - 1][i];
                up[j][i] = up[j - 1][v];
                sum[j][i] = sum[j - 1][i] + sum[j - 1][v];
                mn[j][i] = min(mn[j - 1][i], mn[j - 1][v]);
            }
        }
    }

    struct Walk
    {
        int end;          // end 是走完后到达的点。
        __int128 total;   // total 是途中离开的各点权值之和。
        optional<int> minimum; // minimum 是途中离开的各点权值最小值，不走时为空。
    };

    Walk lift(int v, int k) const
    {
        // 0<=k<2^L；用倍增表从 v 出发走 k 步。
        Walk r{v, 0, nullopt};
        for (int j = 0; j < L; j++)
        {
            if (k >> j & 1)
            {
                r.total += sum[j][r.end];
                r.minimum = r.minimum ? min(*r.minimum, mn[j][r.end]) : mn[j][r.end];
                r.end = up[j][r.end];
            }
        }
        return r;
    }

    Walk walk(int v, unsigned long long k) const
    {
        // 从 v 出发走 k 步；返回终点以及离开的 k 个点（含 v，不含终点）的权值和与最小值。
        if (k <= (unsigned long long)n)
        {
            return lift(v, k);
        }
        // k>n 时必然绕环至少一整圈：先走到入环点 e，再把整圈的和乘上圈数。
        Walk a = lift(v, depth[v]);
        int e = a.end;
        unsigned long long rest = k - depth[v], q = rest / len[e];
        Walk c = lift(e, len[e]), b = lift(e, rest % len[e]);
        a.total += (__int128)q * c.total + b.total;
        a.minimum = a.minimum ? min(*a.minimum, *c.minimum) : *c.minimum;
        a.end = b.end;
        return a;
    }

    int jump(int v, unsigned long long k) const
    {
        // 返回 v 走 k 步后的点。
        return walk(v, k).end;
    }

    optional<unsigned long long> meet(int u, int v) const
    {
        // u、v 同时每步各走一条边；返回第一次落在同一点时的步数，永不相遇时返回空。
        if (comp[u] != comp[v])
        {
            return nullopt;
        }
        // 都走到环上之后位置差不再改变，所以相遇只可能发生在前 max(depth) 步内。
        unsigned long long hi = max(depth[u], depth[v]);
        if (jump(u, hi) != jump(v, hi))
        {
            return nullopt;
        }
        unsigned long long lo = 0;
        while (lo < hi)
        {
            unsigned long long m = lo + (hi - lo) / 2;
            if (jump(u, m) == jump(v, m))
            {
                hi = m;
            }
            else
            {
                lo = m + 1;
            }
        }
        return lo;
    }
};
