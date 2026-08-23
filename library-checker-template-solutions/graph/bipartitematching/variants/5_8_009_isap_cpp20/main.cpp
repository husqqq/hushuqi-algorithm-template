
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T>
struct ISAP
{
    struct E
    {
        int to, rev; // 终点与反向边下标。
        T cap;       // 当前残量容量。
    };

    int n, src = 0;      // n 是点数，src 是本次增广的源点。
    vector<vector<E>> e; // e[u] 是点 u 发出的全部残量边。
    vector<int> h, gap;  // h 是到汇点的距离标号，gap[d] 是标号为 d 的点数。
    vector<int> cur;     // cur[u] 是点 u 尚未尝试的第一条残量边下标。

    ISAP(int n = 0) : n(n), e(n), h(n), gap(n + 1), cur(n)
    {
        // n 是点数；构造空残量网络。
    }

    void add(int x, int y, T c)
    {
        // x、y 是端点，c 是非负容量；加入一对残量边。
        assert(0 <= x && x < n && 0 <= y && y < n && c >= 0); // 调试检查，可删。
        int id = e[x].size();
        int rev = e[y].size() + (x == y);
        e[x].push_back({y, rev, c});
        e[y].push_back({x, id, 0});
    }

    void bfs(int t)
    {
        // t 是汇点；在反向残量意义下初始化距离标号和 gap 计数，无返回值。
        fill(h.begin(), h.end(), n);
        fill(gap.begin(), gap.end(), 0);
        queue<int> q;
        h[t] = 0;
        q.push(t);
        while (!q.empty())
        {
            auto x = q.front();
            q.pop();
            for (auto [y, rev, cap] : e[x])
            {
                if (h[y] != n || e[y][rev].cap == 0)
                {
                    continue;
                }
                h[y] = h[x] + 1;
                q.push(y);
            }
        }
        for (auto d : h)
        {
            gap[d]++;
        }
    }

    T dfs(int x, int t, T lim)
    {
        // x、t、lim 是当前点、汇点和流量上限；返回本次实际增广量。
        if (x == t)
        {
            return lim;
        }
        T used = 0;
        for (auto &i = cur[x]; i < (int)e[x].size(); i++)
        {
            auto &a = e[x][i];
            if (a.cap == 0 || h[x] != h[a.to] + 1)
            {
                continue;
            }
            T d = dfs(a.to, t, min(lim - used, a.cap));
            a.cap -= d;
            e[a.to][a.rev].cap += d;
            used += d;
            if (used == lim || h[x] == n)
            {
                return used;
            }
        }
        if (--gap[h[x]] == 0)
        {
            h[src] = n;
            return used;
        }
        int nh = n - 1;
        for (const auto &a : e[x])
        {
            if (a.cap > 0)
            {
                nh = min(nh, h[a.to]);
            }
        }
        h[x] = nh + 1;
        gap[h[x]]++;
        cur[x] = 0;
        return used;
    }

    T flow(int s, int t)
    {
        // s、t 是源汇点；返回现有残量上新增的最大流。
        if (s == t)
        {
            return 0;
        }
        src = s;
        bfs(t);
        fill(cur.begin(), cur.end(), 0);
        T ans = 0;
        while (h[s] < n && ans < numeric_limits<T>::max())
        {
            ans += dfs(s, t, numeric_limits<T>::max() - ans);
        }
        return ans;
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int l, r, m; cin >> l >> r >> m; int s = l + r, t = s + 1;
    ISAP<long long> f(t + 1);
    for (int u = 0; u < l; ++u) f.add(s, u, 1);
    for (int v = 0; v < r; ++v) f.add(l + v, t, 1);
    vector<array<int, 3>> edge;
    while (m--) { int u, v; cin >> u >> v; int id = f.e[u].size(); f.add(u, l + v, 1); edge.push_back({u, v, id}); }
    auto value = f.flow(s, t); cout << value << '\n';
    for (auto [u, v, id] : edge) if (f.e[u][id].cap == 0) cout << u << ' ' << v << '\n';
}
