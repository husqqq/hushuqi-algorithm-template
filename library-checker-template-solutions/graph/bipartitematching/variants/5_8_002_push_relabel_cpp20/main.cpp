
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T>
struct PushRelabel
{
    struct E
    {
        // 残量边终点与反向边下标。
        int v, r;
        // 当前残量容量。
        T c;
    };

    // 点数与残量邻接表。
    int n;
    vector<vector<E>> e;
    // 每个点的超额流。
    vector<T> ex;
    // 高度与当前弧。
    vector<int> h, cur;
    // 是否已在活跃队列中。
    vector<bool> in;
    // FIFO 活跃点队列。
    queue<int> q;

    PushRelabel(int n = 0) : n(n), e(n), ex(n), h(n), cur(n), in(n)
    {
        // n 是残量网络点数；构造空网络。
    }

    void add(int u, int v, T c)
    {
        // u、v 是端点，c 是非负容量；加入一对残量边。
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0); // 调试检查，可删。
        int id = e[u].size();
        int rev = e[v].size() + (u == v);
        e[u].push_back({v, rev, c});
        e[v].push_back({u, id, 0});
    }

    void active(int u, int s, int t)
    {
        // 超额为正且不是源汇时，把 u 加入活跃队列。
        if (u != s && u != t && ex[u] > 0 && !in[u])
        {
            in[u] = true;
            q.push(u);
        }
    }

    void push(int u, E &a, int s, int t)
    {
        // 若 a 是允许边，就从 u 沿 a 推出尽量多的超额流。
        if (!a.c || h[u] != h[a.v] + 1)
        {
            return;
        }
        T f = min(ex[u], a.c);
        a.c -= f;
        e[a.v][a.r].c += f;
        ex[u] -= f;
        ex[a.v] += f;
        active(a.v, s, t);
    }

    T flow(int s, int t)
    {
        // s、t 是源汇点；从初始网络求最大流并返回流量。
        if (s == t)
        {
            return 0;
        }
        h[s] = n;
        for (auto &a : e[s])
        {
            T f = a.c;
            a.c = 0;
            e[a.v][a.r].c += f;
            ex[a.v] += f;
            ex[s] -= f;
            active(a.v, s, t);
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            in[u] = false;
            while (ex[u] > 0)
            {
                if (cur[u] == (int)e[u].size())
                {
                    h[u] = 2 * n;
                    for (auto a : e[u])
                    {
                        if (a.c)
                        {
                            h[u] = min(h[u], h[a.v] + 1);
                        }
                    }
                    cur[u] = 0;
                    continue;
                }
                push(u, e[u][cur[u]], s, t);
                if (!e[u][cur[u]].c || h[u] != h[e[u][cur[u]].v] + 1)
                {
                    cur[u]++;
                }
            }
        }
        return ex[t];
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int l, r, m; cin >> l >> r >> m; int s = l + r, t = s + 1;
    PushRelabel<long long> f(t + 1);
    for (int u = 0; u < l; ++u) f.add(s, u, 1);
    for (int v = 0; v < r; ++v) f.add(l + v, t, 1);
    vector<array<int, 3>> edge;
    while (m--) { int u, v; cin >> u >> v; int id = f.e[u].size(); f.add(u, l + v, 1); edge.push_back({u, v, id}); }
    auto value = f.flow(s, t); cout << value << '\n';
    for (auto [u, v, id] : edge) if (f.e[u][id].c == 0) cout << u << ' ' << v << '\n';
}
