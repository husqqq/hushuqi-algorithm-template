// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 14 最大流 1


#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T>
struct Flow
{
    struct E
    {
        // to 是终点，rev 是反向边在 e[to] 中的下标。
        int to, rev;
        // cap 是当前残量容量。
        T cap;
    };

    // n 是残量网络点数。
    int n;
    // e[u] 是点 u 发出的全部残量边。
    vector<vector<E>> e;
    // h 是层次图距离，cur[u] 是点 u 尚未尝试的第一条残量边下标。
    vector<int> h, cur;

    Flow(int n = 0) : n(n), e(n), h(n), cur(n)
    {
        // n 是残量网络点数；构造空网络。
    }

    int add(int x, int y, T c)
    {
        // x、y 是端点，c 是非负容量；返回正向边在 e[x] 中的下标。
        assert(0 <= x && x < n && 0 <= y && y < n && c >= 0); // 调试检查，可删。
        int id = e[x].size();
        int rev = e[y].size() + (x == y);
        e[x].push_back({y, rev, c});
        e[y].push_back({x, id, 0});
        return id;
    }

    bool bfs(int s, int t)
    {
        // s、t 是源汇点；重建层次图，存在增广路时返回 true。
        fill(h.begin(), h.end(), -1);
        queue<int> q;
        h[s] = 0;
        q.push(s);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            for (auto [y, r, c] : e[x])
            {
                if (c > 0 && h[y] == -1)
                {
                    h[y] = h[x] + 1;
                    q.push(y);
                }
            }
        }
        return h[t] != -1;
    }

    T dfs(int x, int t, T f)
    {
        // x 是当前点，t 是汇点，f 是流量上限；返回沿层次图实际送出的流量。
        if (x == t)
        {
            return f;
        }
        T ans = 0;
        for (int &i = cur[x]; i < (int)e[x].size(); ++i)
        {
            auto &a = e[x][i];
            if (a.cap == 0 || h[a.to] != h[x] + 1)
            {
                continue;
            }
            T d = dfs(a.to, t, min(f - ans, a.cap));
            a.cap -= d;
            e[a.to][a.rev].cap += d;
            ans += d;
            if (ans == f)
            {
                break;
            }
        }
        return ans;
    }

    T flow(int s, int t, T lim = numeric_limits<T>::max())
    {
        // s、t 是源汇点，lim 是本次流量上限；返回现有残量网络上的新增流量。
        if (s == t)
        {
            return 0;
        }
        T ans = 0;
        while (ans < lim && bfs(s, t))
        {
            fill(cur.begin(), cur.end(), 0);
            ans += dfs(s, t, lim - ans);
        }
        return ans;
    }

    vector<bool> cut(int s) const
    {
        // s 是残量搜索起点；返回残量图中从 s 可达的点集。
        vector<bool> vis(n);
        queue<int> q;
        vis[s] = true;
        q.push(s);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            for (auto [y, r, c] : e[x])
            {
                if (c > 0 && !vis[y])
                {
                    vis[y] = true;
                    q.push(y);
                }
            }
        }
        return vis;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Flow<long long> g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        g.add(u - 1, v - 1, c);
    }
    cout << g.flow(s - 1, t - 1) << '\n';
}
