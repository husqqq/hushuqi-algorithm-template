#pragma once

#include <bits/stdc++.h>
using namespace std;

template <class Cap = long long, class Cost = long long, class Ans = long long>
struct MinCostBFlow
{
    struct Edge
    {
        int to, rev;
        Cap cap;
        Cost w;
    };
    struct Ref
    {
        int u, i;
        Cap low;
        Cost cost;
    };
    struct Certificate
    {
        // value 是最小费用，pot 是满足互补松弛的点势，flow 按 add 顺序给出每条边流量。
        Ans value;
        vector<Cost> pot;
        vector<Cap> flow;
    };
    int n;
    vector<vector<Edge>> g;
    vector<Cap> ex;
    vector<Cost> pot;
    vector<Ref> ref;

    MinCostBFlow(int n) : n(n), g(n), ex(n), pot(n)
    {
        assert(n >= 0);
    }

    void add(int u, int v, Cap low, Cap high, Cost cost)
    {
        assert(0 <= u && u < n && 0 <= v && v < n);
        assert(low <= high);
        Cost scale = (Cost)n + 1;
        assert((__int128)cost * scale < numeric_limits<Cost>::max());
        assert((__int128)cost * scale > numeric_limits<Cost>::min());
        Cost w = cost * scale;
        int i = g[u].size(), j = g[v].size() + (u == v);
        g[u].push_back({v, j, high - low, w});
        g[v].push_back({u, i, 0, -w});
        ex[u] -= low;
        ex[v] += low;
        ref.push_back({u, i, low, cost});
    }

    void supply(int u, Cap x)
    {
        assert(0 <= u && u < n);
        ex[u] += x;
    }

    Cost reduced(int u, const Edge &e) const
    {
        return e.w + pot[u] - pot[e.to];
    }

    void push(int u, int i, Cap f)
    {
        Edge &e = g[u][i];
        e.cap -= f;
        g[e.to][e.rev].cap += f;
        ex[u] -= f;
        ex[e.to] += f;
    }

    bool refine(Cost eps)
    {
        vector<int> cur(n);
        vector<char> in(n);
        deque<int> q;
        auto canDeficit = [&](int s)
        {
            vector<char> vis(n);
            deque<int> z;
            z.push_back(s);
            vis[s] = true;
            while (!z.empty())
            {
                int u = z.front();
                z.pop_front();
                if (ex[u] < 0)
                {
                    return true;
                }
                for (const auto &e : g[u])
                {
                    if (e.cap > 0 && !vis[e.to])
                    {
                        vis[e.to] = true;
                        z.push_back(e.to);
                    }
                }
            }
            return false;
        };
        for (int u = 0; u < n; u++)
        {
            for (int i = 0; i < (int)g[u].size(); i++)
            {
                if (g[u][i].cap > 0 && reduced(u, g[u][i]) < 0)
                {
                    push(u, i, g[u][i].cap);
                }
            }
        }
        for (int u = 0; u < n; u++)
        {
            if (ex[u] > 0)
            {
                q.push_back(u);
                in[u] = true;
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop_front();
            in[u] = false;
            while (ex[u] > 0)
            {
                if (cur[u] == (int)g[u].size())
                {
                    Cost best = numeric_limits<Cost>::max();
                    for (const auto &e : g[u])
                    {
                        if (e.cap > 0)
                        {
                            best = min(best, reduced(u, e));
                        }
                    }
                    if (best == numeric_limits<Cost>::max())
                    {
                        return false;
                    }
                    if (!canDeficit(u))
                    {
                        return false;
                    }
                    pot[u] -= best + eps;
                    cur[u] = 0;
                    continue;
                }
                Edge &e = g[u][cur[u]];
                if (e.cap == 0 || reduced(u, e) >= 0)
                {
                    cur[u]++;
                    continue;
                }
                int v = e.to;
                Cap f = min(ex[u], e.cap);
                push(u, cur[u], f);
                if (ex[v] > 0 && !in[v])
                {
                    q.push_back(v);
                    in[v] = true;
                }
            }
        }
        return true;
    }

    optional<Ans> solve()
    {
        Cap total = 0;
        for (Cap x : ex)
        {
            total += x;
        }
        if (total != 0)
        {
            return nullopt;
        }
        Cost mx = 1;
        for (int u = 0; u < n; u++)
        {
            for (const auto &e : g[u])
            {
                mx = max(mx, e.w >= 0 ? e.w : -e.w);
            }
        }
        Cost eps = 1;
        while (eps < mx)
        {
            assert(eps <= numeric_limits<Cost>::max() / 2);
            eps *= 2;
        }
        while (eps)
        {
            if (!refine(eps))
            {
                return nullopt;
            }
            eps /= 2;
        }
        for (Cap x : ex)
        {
            if (x != 0)
            {
                return nullopt;
            }
        }
        Ans ans = 0;
        for (const auto &x : ref)
        {
            const Edge &e = g[x.u][x.i];
            Cap flow = x.low + g[e.to][e.rev].cap;
            ans += (Ans)flow * x.cost;
        }
        return ans;
    }

    optional<Certificate> solveCert()
    {
        // 无参数；求最小费用并返回可直接验证的点势和每条原边流量，不可行时返回空。
        auto ans = solve();
        if (!ans) return nullopt;
        vector<Cost> d(n);
        // 最优残量网络无负环；从超级源以零边到各点做 Bellman-Ford，得到可行势。
        for (int it = 0; it + 1 < n; it++)
        {
            bool upd = false;
            for (int u = 0; u < n; u++)
            {
                for (const auto &x : g[u])
                {
                    if (x.cap > 0 && d[x.to] > d[u] + x.w / ((Cost)n + 1))
                    {
                        d[x.to] = d[u] + x.w / ((Cost)n + 1);
                        upd = true;
                    }
                }
            }
            if (!upd) break;
        }
        vector<Cap> flow;
        flow.reserve(ref.size());
        for (const auto &x : ref)
        {
            const Edge &a = g[x.u][x.i];
            flow.push_back(x.low + g[a.to][a.rev].cap);
        }
        return Certificate{*ans, d, flow};
    }
};
