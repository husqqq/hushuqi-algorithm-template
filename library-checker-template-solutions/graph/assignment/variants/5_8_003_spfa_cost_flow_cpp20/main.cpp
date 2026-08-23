
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T = long long, class Cost = long long>
struct SpfaCostFlow
{
    struct E
    {
        int to, rev;  // 终点与反向边下标。
        T cap;        // 当前残量容量。
        Cost cost;     // 单位流量费用。
    };

    int n;               // 残量网络点数。
    vector<vector<E>> e; // 每个点的残量边。
    SpfaCostFlow(int n) : n(n), e(n)
    {
        // n 是点数；构造空残量网络，无返回值。
    }

    void add(int u, int v, T cap, Cost cost)
    {
        // u、v 是端点，cap 是容量，cost 是单位费用；加入正向边和零容量反向边，无返回值。
        assert(0 <= u && u < n && 0 <= v && v < n && cap >= 0); // 调试检查，可删。
        assert(cost != numeric_limits<Cost>::min()); // 调试检查，可删。
        int id = e[u].size();
        int rev = e[v].size() + (u == v);
        e[u].push_back({v, rev, cap, cost});
        e[v].push_back({u, id, 0, -cost});
    }

    pair<T, Cost> flow(int s, int t, T lim = numeric_limits<T>::max())
    {
        // s、t 是源汇点，lim 是最多发送的流量；返回实际流量与最小费用。
        if (s == t)
        {
            return {0, 0};
        }
        assert(lim >= 0); // 调试检查，可删。
        T f = 0;
        Cost cost = 0;
        vector<optional<Cost>> d(n);
        vector<int> pv(n), pe(n);
        while (f < lim)
        {
            fill(d.begin(), d.end(), nullopt);
            vector<bool> in(n);
            queue<int> q;
            d[s] = 0;
            q.push(s);
            in[s] = true;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                in[u] = false;
                for (int i = 0; i < (int)e[u].size(); i++)
                {
                    auto &a = e[u][i];
                    Cost nd = *d[u] + a.cost;
                    if (a.cap && (!d[a.to].has_value() || nd < *d[a.to]))
                    {
                        d[a.to] = nd;
                        pv[a.to] = u;
                        pe[a.to] = i;
                        if (!in[a.to])
                        {
                            q.push(a.to);
                            in[a.to] = true;
                        }
                    }
                }
            }
            if (!d[t].has_value())
            {
                break;
            }
            T x = lim - f;
            for (int v = t; v != s; v = pv[v])
            {
                x = min(x, e[pv[v]][pe[v]].cap);
            }
            for (int v = t; v != s; v = pv[v])
            {
                auto &a = e[pv[v]][pe[v]];
                a.cap -= x;
                e[v][a.rev].cap += x;
            }
            f += x;
            cost += (Cost)x * *d[t];
        }
        return {f, cost};
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; int s = 2 * n, t = s + 1; SpfaCostFlow<long long, long long> f(t + 1);
    for (int i = 0; i < n; ++i) f.add(s, i, 1, 0);
    for (int j = 0; j < n; ++j) f.add(n + j, t, 1, 0);
    vector<vector<int>> id(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
    { long long c; cin >> c; id[i][j] = f.e[i].size(); f.add(i, n + j, 1, c); }
    auto [sent, cost] = f.flow(s, t, n); assert(sent == n); cout << cost << '\n';
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (f.e[i][id[i][j]].cap == 0)
    { cout << j << " \n"[i + 1 == n]; break; }
}
