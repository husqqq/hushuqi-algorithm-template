
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T = long long, class Cost = long long>
struct CostFlow
{
    struct E
    {
        // 终点与反向边下标。
        int to, rev;
        // 当前残量容量。
        T cap;
        // 单位流量费用。
        Cost cost;
    };

    // 点数、残量邻接表与跨调用保留的势能。
    int n;
    vector<vector<E>> e;
    vector<Cost> h;
    bool ready = false;
    int source = -1, sink = -1;

    CostFlow(int n = 0) : n(n), e(n), h(n)
    {
        // n 是残量网络点数；构造空网络。
    }

    void add(int x, int y, T cap, Cost cost)
    {
        // x、y 是端点，cap 是非负容量，cost 是单位费用；加入一对残量边。
        assert(0 <= x && x < n && 0 <= y && y < n && cap >= 0); // 调试检查，可删。
        assert(source == -1); // 调试检查，可删：所有边须在首次 flow 前加入。
        assert(cost != numeric_limits<Cost>::min()); // 调试检查，可删。
        int id = e[x].size();
        int rev = e[y].size() + (x == y);
        e[x].push_back({y, rev, cap, cost});
        e[y].push_back({x, id, 0, -cost});
        ready = false;
    }

    void init(int s)
    {
        // s 是源点；用 SPFA 求初始势能，要求不存在 s 可达的负费用环。
        vector<optional<Cost>> d(n);
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
            for (const auto &a : e[u])
            {
                Cost nd = *d[u] + a.cost;
                if (a.cap > 0 &&
                    (!d[a.to].has_value() || nd < *d[a.to]))
                {
                    d[a.to] = nd;
                    if (!in[a.to])
                    {
                        in[a.to] = true;
                        q.push(a.to);
                    }
                }
            }
        }
        fill(h.begin(), h.end(), 0);
        for (int u = 0; u < n; u++)
        {
            if (d[u].has_value())
            {
                h[u] = *d[u];
            }
        }
        ready = true;
    }

    pair<T, Cost> flow(int s, int t, T lim = numeric_limits<T>::max())
    {
        // s、t 是固定源汇，lim 是流量上限；返回本次新增流量与费用。
        if (s == t)
        {
            return {0, 0};
        }
        assert(lim >= 0); // 调试检查，可删。
        if (source == -1)
        {
            source = s;
            sink = t;
        }
        assert(source == s && sink == t); // 调试检查，可删：分段调用不能更换源汇。
        if (!ready)
        {
            init(s);
        }

        vector<int> pv(n), pe(n);
        T f = 0;
        Cost cost = 0;
        while (f < lim)
        {
            vector<optional<Cost>> dis(n);
            priority_queue<pair<Cost, int>, vector<pair<Cost, int>>, greater<>> q;
            dis[s] = 0;
            q.emplace(0, s);
            while (!q.empty())
            {
                auto [d, x] = q.top();
                q.pop();
                if (!dis[x].has_value() || d != *dis[x])
                {
                    continue;
                }
                for (int i = 0; i < (int)e[x].size(); ++i)
                {
                    auto &a = e[x][i];
                    if (a.cap == 0)
                    {
                        continue;
                    }
                    Cost nd = d + a.cost + h[x] - h[a.to];
                    assert(nd >= 0); // 调试检查，可删：势能应保证约化费用非负。
                    if (!dis[a.to].has_value() || nd < *dis[a.to])
                    {
                        dis[a.to] = nd;
                        pv[a.to] = x;
                        pe[a.to] = i;
                        q.emplace(nd, a.to);
                    }
                }
            }
            if (!dis[t].has_value())
            {
                break;
            }
            for (int i = 0; i < n; ++i)
            {
                if (dis[i].has_value())
                {
                    h[i] += *dis[i];
                }
            }
            T d = lim - f;
            for (int x = t; x != s; x = pv[x])
            {
                d = min(d, e[pv[x]][pe[x]].cap);
            }
            for (int x = t; x != s; x = pv[x])
            {
                auto &a = e[pv[x]][pe[x]];
                a.cap -= d;
                e[x][a.rev].cap += d;
            }
            f += d;
            cost += (Cost)d * (h[t] - h[s]);
        }
        return {f, cost};
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; int s = 2 * n, t = s + 1; CostFlow<long long, long long> f(t + 1);
    for (int i = 0; i < n; ++i) f.add(s, i, 1, 0);
    for (int j = 0; j < n; ++j) f.add(n + j, t, 1, 0);
    vector<vector<int>> id(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
    { long long c; cin >> c; id[i][j] = f.e[i].size(); f.add(i, n + j, 1, c); }
    auto [sent, cost] = f.flow(s, t, n); assert(sent == n); cout << cost << '\n';
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (f.e[i][id[i][j]].cap == 0)
    { cout << j << " \n"[i + 1 == n]; break; }
}
