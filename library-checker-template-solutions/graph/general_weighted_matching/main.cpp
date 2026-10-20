
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class W = long long>
struct WBlossom
{
    static_assert(numeric_limits<W>::is_integer && numeric_limits<W>::is_signed);

    struct Edge
    {
        int u = 0, v = 0; // u、v 是边在当前收缩图中的两个端点。
        W w = 0;          // w 是边权。
    };

    int n, m, stamp = 0, head = 0, tail = 0; // n 是点数，m 是已用的点与花朵编号上界，stamp 是找 LCA 的时间戳，head、tail 是 BFS 队列首尾。
    W inf = numeric_limits<W>::max() / 4; // inf 是不可达权值。
    vector<vector<Edge>> e;          // e 是原点与花朵之间的最优代表边矩阵。
    vector<W> lab;                   // lab 是原点和花朵的对偶顶标。
    vector<int> link, slack, state;  // link 是匹配点，slack 是最松边，state 是交替树状态。
    vector<int> rep, fa, seen, q;    // rep 是当前花朵代表，fa 是交替树父边端点。
    vector<vector<int>> blossom;     // blossom[x] 按环序保存花朵 x 的子块。
    vector<vector<int>> base;        // base[x][v] 是花朵 x 内含原点 v 的直接子块。
    vector<vector<int>> adj;         // 原图正权邻接表；BFS 只扫描实际存在的边。

    explicit WBlossom(int n)
        : n(n), m(n), e(2 * n + 5, vector<Edge>(2 * n + 5)),
          lab(2 * n + 5), link(2 * n + 5), slack(2 * n + 5),
          state(2 * n + 5), rep(2 * n + 5), fa(2 * n + 5), seen(2 * n + 5),
          q(2 * n + 5), blossom(2 * n + 5), base(2 * n + 5, vector<int>(2 * n + 5)), adj(n + 1)
    {
        // n 是原图点数；构造最多含 n 个收缩花朵的工作区。
        assert(n >= 0);
        for (int i = 1; i < (int)e.size(); i++)
        {
            for (int j = 1; j < (int)e.size(); j++)
            {
                e[i][j] = {i, j, 0};
            }
        }
    }

    W delta(const Edge &x) const
    {
        // x 是边；返回它相对当前顶标的二倍松弛量。
        return lab[x.u] + lab[x.v] - x.w * 2;
    }

    void updateSlack(int u, int v)
    {
        // u 是外点，v 是待更新块；保留连接 v 的最松边起点。
        if (!slack[v] || delta(e[u][v]) < delta(e[slack[v]][v]))
        {
            slack[v] = u;
        }
    }

    void setSlack(int v)
    {
        // v 是当前块；重新寻找从外层交替树连接到 v 的最松边。
        slack[v] = 0;
        for (int u = 1; u <= n; u++)
        {
            if (e[u][v].w > 0 && rep[u] != v && !state[rep[u]])
            {
                updateSlack(u, v);
            }
        }
    }

    void enqueue(int u)
    {
        // u 是外层块；把其中全部原点加入交替树扫描队列。
        if (u <= n)
        {
            q[++tail] = u;
        }
        else
        {
            for (int v : blossom[u])
            {
                enqueue(v);
            }
        }
    }

    void modify(int u, int w)
    {
        // u 是块，w 是新代表；递归修改 u 内全部原点的代表。
        rep[u] = w;
        if (u > n)
        {
            for (int v : blossom[u])
            {
                modify(v, w);
            }
        }
    }

    int position(int u, int v)
    {
        // u 是花朵，v 是其中一个直接子块；调整环向后返回 v 的偶数下标。
        auto it = find(blossom[u].begin(), blossom[u].end(), v);
        int x = it - blossom[u].begin();
        if (x & 1)
        {
            reverse(blossom[u].begin() + 1, blossom[u].end());
            return (int)blossom[u].size() - x;
        }
        return x;
    }

    void setMatch(int u, int v)
    {
        // u、v 是两个待配对块；展开花朵 u 内部并建立对应匹配。
        link[u] = e[u][v].v;
        if (u <= n)
        {
            return;
        }
        Edge x = e[u][v];
        int z = base[u][x.u], k = position(u, z);
        for (int i = 0; i < k; i++)
        {
            setMatch(blossom[u][i], blossom[u][i ^ 1]);
        }
        setMatch(z, v);
        rotate(blossom[u].begin(), blossom[u].begin() + k, blossom[u].end());
    }

    void augment(int u, int v)
    {
        // u、v 是增广路首条边两端；沿交替树翻转匹配直到自由根。
        int x = rep[link[u]];
        setMatch(u, v);
        if (!x)
        {
            return;
        }
        setMatch(x, rep[fa[x]]);
        augment(rep[fa[x]], x);
    }

    int lca(int u, int v)
    {
        // u、v 是两个外层块；返回其交替树最低公共祖先，不相交时返回 0。
        ++stamp;
        while (u || v)
        {
            if (u)
            {
                if (seen[u] == stamp)
                {
                    return u;
                }
                seen[u] = stamp;
                u = rep[link[u]];
                if (u)
                {
                    u = rep[fa[u]];
                }
            }
            swap(u, v);
        }
        return 0;
    }

    void contract(int u, int a, int v)
    {
        // u、v 是零紧边两端，a 是交替树公共祖先；收缩对应奇环。
        int x = n + 1, i, j;
        while (x <= m && rep[x])
        {
            x++;
        }
        if (x > m)
        {
            m++;
        }
        lab[x] = state[x] = rep[x] = 0;
        link[x] = link[a];
        blossom[x].clear();
        blossom[x].push_back(a);
        for (i = u; i != a; i = rep[fa[j]])
        {
            blossom[x].push_back(i);
            blossom[x].push_back(j = rep[link[i]]);
            enqueue(j);
        }
        reverse(blossom[x].begin() + 1, blossom[x].end());
        for (i = v; i != a; i = rep[fa[j]])
        {
            blossom[x].push_back(i);
            blossom[x].push_back(j = rep[link[i]]);
            enqueue(j);
        }
        modify(x, x);
        for (i = 1; i <= m; i++)
        {
            e[x][i].w = e[i][x].w = 0;
        }
        fill(base[x].begin() + 1, base[x].begin() + n + 1, 0);
        for (int u0 : blossom[x])
        {
            for (int v0 = 1; v0 <= m; v0++)
            {
                if (!e[x][v0].w || delta(e[u0][v0]) < delta(e[x][v0]))
                {
                    e[x][v0] = e[u0][v0];
                    e[v0][x] = e[v0][u0];
                }
            }
            for (int v0 = 1; v0 <= n; v0++)
            {
                if (base[u0][v0])
                {
                    base[x][v0] = u0;
                }
            }
        }
        setSlack(x);
    }

    void expand(int u)
    {
        // u 是顶标降到零的内层花朵；按交替树方向展开。
        for (int x : blossom[u])
        {
            modify(x, x);
        }
        int a = base[u][e[u][fa[u]].u], r = position(u, a);
        for (int i = 0; i < r; i += 2)
        {
            int x = blossom[u][i], y = blossom[u][i + 1];
            fa[x] = e[y][x].u;
            state[x] = 1;
            state[y] = slack[x] = 0;
            setSlack(y);
            enqueue(y);
        }
        state[a] = 1;
        fa[a] = fa[u];
        for (int i = r + 1; i < (int)blossom[u].size(); i++)
        {
            state[blossom[u][i]] = -1;
            setSlack(blossom[u][i]);
        }
        rep[u] = 0;
    }

    bool onEdge(const Edge &x)
    {
        // x 是新出现的零紧边；扩展交替树、收花或增广，增广成功返回 true。
        int u = rep[x.u], v = rep[x.v];
        if (state[v] == -1)
        {
            fa[v] = x.u;
            state[v] = 1;
            int a = rep[link[v]];
            slack[v] = slack[a] = state[a] = 0;
            enqueue(a);
        }
        else if (!state[v])
        {
            int a = lca(u, v);
            if (!a)
            {
                augment(u, v);
                augment(v, u);
                return true;
            }
            contract(u, a, v);
        }
        return false;
    }

    bool bfs()
    {
        // 在当前匹配上寻找并执行一条增广路；不存在时返回 false。
        fill(state.begin() + 1, state.begin() + m + 1, -1);
        fill(slack.begin() + 1, slack.begin() + m + 1, 0);
        head = 1;
        tail = 0;
        for (int i = 1; i <= m; i++)
        {
            if (rep[i] == i && !link[i])
            {
                fa[i] = state[i] = 0;
                enqueue(i);
            }
        }
        if (head > tail)
        {
            return false;
        }
        while (true)
        {
            while (head <= tail)
            {
                int u = q[head++];
                if (state[rep[u]] == 1)
                {
                    continue;
                }
                for (int v : adj[u])
                {
                    if (rep[u] != rep[v])
                    {
                        if (delta(e[u][v]))
                        {
                            updateSlack(u, rep[v]);
                        }
                        else if (onEdge(e[u][v]))
                        {
                            return true;
                        }
                    }
                }
            }
            W x = inf;
            for (int i = n + 1; i <= m; i++)
            {
                if (rep[i] == i && state[i] == 1)
                {
                    x = min(x, lab[i] / 2);
                }
            }
            for (int i = 1; i <= m; i++)
            {
                if (rep[i] == i && slack[i] && rep[slack[i]] != i && state[i] != 1)
                {
                    x = min(x, delta(e[slack[i]][i]) >> (state[i] + 1));
                }
            }
            for (int i = 1; i <= n; i++)
            {
                if (state[rep[i]] != -1)
                {
                    lab[i] += (state[rep[i]] * 2 - 1) * x;
                    if (lab[i] <= 0)
                    {
                        return false;
                    }
                }
            }
            for (int i = n + 1; i <= m; i++)
            {
                if (rep[i] == i && state[i] != -1)
                {
                    lab[i] += (2 - state[i] * 4) * x;
                }
            }
            head = 1;
            tail = 0;
            for (int i = 1; i <= m; i++)
            {
                if (rep[i] == i && slack[i] && rep[slack[i]] != i &&
                    !delta(e[slack[i]][i]) && onEdge(e[slack[i]][i]))
                {
                    return true;
                }
            }
            for (int i = n + 1; i <= m; i++)
            {
                if (rep[i] == i && state[i] == 1 && !lab[i])
                {
                    expand(i);
                }
            }
        }
    }

    pair<W, vector<pair<int, int>>> solve(const vector<tuple<int, int, W>> &edges)
    {
        // 输入 0 下标无向边；忽略非正权边，平行边只保留最大权。
        assert((int)e.size() >= 2 * n + 5);
        m = n;
        stamp = 0;
        fill(link.begin(), link.end(), 0);
        fill(seen.begin(), seen.end(), 0);
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                e[i][j].w = 0;
            }
        }
        for (int i = 1; i <= n; i++)
        {
            rep[i] = i;
            blossom[i].clear();
        }
        W wm = 0;
        for (auto [u, v, w] : edges)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && u != v);
            u++, v++;
            if (w > 0)
            {
                e[u][v].w = e[v][u].w = max(e[u][v].w, w);
                wm = max(wm, w);
            }
        }
        for (int i = 1; i <= n; i++)
        {
            adj[i].clear();
            for (int j = 1; j <= n; j++)
            {
                base[i][j] = i == j ? i : 0;
                if (e[i][j].w > 0)
                {
                    adj[i].push_back(j);
                }
                if (i != j)
                {
                    e[i][j].u = i;
                    e[i][j].v = j;
                }
            }
        }
        fill(lab.begin() + 1, lab.begin() + n + 1, wm);
        while (bfs())
        {
        }
        W ans = 0;
        vector<pair<int, int>> match;
        for (int i = 1; i <= n; i++)
        {
            if (link[i])
            {
                ans += e[i][link[i]].w;
                if (i < link[i])
                {
                    match.push_back({i - 1, link[i] - 1});
                }
            }
        }
        return {ans / 2, match};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, long long>> e(m);
    for (auto &[u, v, w] : e)
    {
        cin >> u >> v >> w;
    }
    WBlossom<long long> g(n);
    auto [sum, ans] = g.solve(e);
    cout << ans.size() << ' ' << sum << '\n';
    for (auto [u, v] : ans)
    {
        cout << u << ' ' << v << '\n';
    }
}
