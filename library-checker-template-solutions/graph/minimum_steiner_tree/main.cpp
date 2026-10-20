

#include <bits/stdc++.h>
using namespace std;
#define int long long

using WEdge = array<int, 2>; // 依次为有向边终点、非负边权。

template<class E, class Dist, class F>
void dijkstraInit(const vector<vector<E>> &g, vector<optional<Dist>> &d, F relax)
{
    // g[u] 每项的 [0] 为邻点、[1] 为非负边权，其余字段交给回调；d 为等长的初始代价表。
    // 原地求 min_s(d初值[s]+最短路(s,v))，无可达初值时仍为空；所有中间和须能用 Dist 表示。
    // 每次严格改善 d[v] 后调用 relax(u,边记录)，可记录前驱，但不得修改 g、d；无返回值。
    int n = g.size();
    assert((int)d.size() == n);
    priority_queue<pair<Dist, int>, vector<pair<Dist, int>>, greater<>> q;
    for (int u = 0; u < n; u++)
    {
        if (d[u]) q.push({*d[u], u});
    }
    while (!q.empty())
    {
        auto [du, u] = q.top();
        q.pop();
        if (!d[u] || du != *d[u]) continue;
        for (const auto &edge : g[u])
        {
            int v = edge[0];
            assert(edge[1] >= 0);
            Dist w = (Dist)edge[1];
            Dist nd = du + w;
            if (d[v] && *d[v] <= nd) continue;
            d[v] = nd;
            relax(u, edge);
            q.push({nd, v});
        }
    }
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraSp(const vector<vector<WEdge>> &g, int s)
{
    // g 是非负权邻接表，s 是 [0,n) 内源点；返回最短距离，不可达点为空。
    int n = g.size();
    assert(0 <= s && s < n);
    vector<optional<Dist>> d(n);
    d[s] = 0;
    dijkstraInit(g, d, [](int, const WEdge &)
    {
        // 仅求距离，无需记录前驱。
    });
    return d;
}

template <class Dist = long long>
vector<optional<Dist>> dijkstraDn(
    const vector<vector<optional<int>>> &w, int s)
{
    // w 是非负权方阵，空值表示缺边，s 是 [0,n) 内源点；返回最短距离，不可达点为空。
    int n = w.size();
    vector<optional<Dist>> d(n);
    vector<bool> vis(n);
    d[s] = 0;
    for (int z = 0; z < n; z++)
    {
        int u = -1;
        for (int i = 0; i < n; i++)
        {
            if (!vis[i] && d[i].has_value() && (u < 0 || *d[i] < *d[u]))
            {
                u = i;
            }
        }
        if (u < 0)
        {
            break;
        }
        vis[u] = true;
        for (int v = 0; v < n; v++)
        {
            if (w[u][v].has_value())
            {
                assert(*w[u][v] >= 0); // 调试检查，可删：Dijkstra 要求非负边权。
                Dist z = (Dist)*w[u][v];
                Dist nd = *d[u] + z;
                if (!d[v].has_value() || nd < *d[v])
                {
                    d[v] = nd;
                }
            }
        }
    }
    return d;
}

struct Steiner
{
    // n 是图的点数。
    int n;
    // e[u] 每项依次为邻点、非负边权。
    vector<vector<array<int, 2>>> e;
    // ed[i][0..2] 依次是第 i 条原边的两个端点和非负边权。
    vector<array<int, 3>> ed;

    Steiner(int n = 0) : n(n), e(n)
    {
        // n>=0 是点数；构造一张没有边的图。
    }

    void add(int u, int v, int w)
    {
        // u、v 是 [0,n) 内端点，w>=0；加入无向边，原边编号为插入次序，允许重边和自环，无返回值。
        assert(0 <= u && u < n && 0 <= v && v < n && w >= 0); // 调试检查，可删。
        e[u].push_back({v, w});
        e[v].push_back({u, w});
        ed.push_back({u, v, w});
    }

    optional<int> solve(const vector<int> &key) const
    {
        // key 是 [0,n) 内的终端列表；返回最小连通边权和，空列表返回 0，终端不连通时返回空。
        // 所有合并与松弛的中间和须能用 int 表示；重复终端允许，但会重复占用状态位。
        int k = key.size();
        if (k == 0)
        {
            return 0;
        }
        assert(k < numeric_limits<int>::digits); // 调试检查，可删。
        int z = (int)1 << k;
        vector f(z, vector<optional<int>>(n)); // f[s][u]：连接终端集 s 并包含 u 的最小权值。
        for (int i = 0; i < k; i++)
        {
            assert(0 <= key[i] && key[i] < n); // 调试检查，可删。
            f[(int)1 << i][key[i]] = 0;
        }
        for (int s = 1; s < z; s++)
        {
            for (int a = (s - 1) & s; a; a = (a - 1) & s)
            {
                int b = s ^ a;
                if (!b || a > b)
                {
                    continue;
                }
                for (int u = 0; u < n; u++)
                {
                    if (f[a][u] && f[b][u])
                    {
                        int nd = *f[a][u] + *f[b][u];
                        if (!f[s][u] || nd < *f[s][u])
                        {
                            f[s][u] = nd;
                        }
                    }
                }
            }
            dijkstraInit(e, f[s], [](int, const array<int, 2> &)
            {
                // 仅求当前子集的最小代价，无需记录前驱。
            });
        }
        optional<int> ans;
        for (auto x : f.back())
        {
            if (x && (!ans || *x < *ans))
            {
                ans = *x;
            }
        }
        return ans;
    }

    optional<pair<int, vector<int>>> solveTree(const vector<int> &key) const
    {
        // key 与数值范围同 solve；返回 {最小权值, 升序且无重复的原边编号}，终端不连通时返回空。
        // 空终端列表返回 {0,{}}；图中其余连通块不影响结果。
        int k = key.size();
        if (k == 0)
        {
            return pair<int, vector<int>>{0, {}};
        }
        assert(k < numeric_limits<int>::digits); // 状态数须能用 int 表示，内存另由调用者保证。
        int lim = (int)1 << k;
        vector<vector<array<int, 3>>> g(n); // 邻接项依次为邻点、边权、原边编号。
        for (int i = 0; i < (int)ed.size(); i++)
        {
            auto [u, v, w] = ed[i];
            g[u].push_back({v, w, i});
            g[v].push_back({u, w, i});
        }
        vector dp(lim, vector<optional<int>>(n)); // 与 solve 的状态相同，另记录最优转移。
        // pre[s][u]：{子集,-1} 表示合并，{前驱点,边号} 表示松弛，{-1,-1} 表示终端基态。
        vector pre(lim, vector<array<int, 2>>(n, {-1, -1}));
        for (int i = 0; i < k; i++)
        {
            assert(0 <= key[i] && key[i] < n); // 调试检查，可删。
            dp[1 << i][key[i]] = 0;
        }
        for (int s = 1; s < lim; s++)
        {
            if (s & (s - 1))
            {
                for (int a = (s - 1) & s; a; a = (a - 1) & s)
                {
                    int b = s ^ a;
                    if (!b || a > b) continue;
                    for (int u = 0; u < n; u++)
                    {
                        if (dp[a][u] && dp[b][u])
                        {
                            int nd = *dp[a][u] + *dp[b][u];
                            if (!dp[s][u] || nd < *dp[s][u])
                            {
                                dp[s][u] = nd;
                                pre[s][u] = {a, -1};
                            }
                        }
                    }
                }
            }
            dijkstraInit(g, dp[s], [&](int u, const array<int, 3> &edge)
            {
                // u 是前驱，edge 依次为终点、边权、原边编号；保存当前最优转移。
                pre[s][edge[0]] = {u, edge[2]};
            });
        }
        int all = lim - 1, at = -1;
        for (int u = 0; u < n; u++)
        {
            if (dp[all][u] && (at == -1 || *dp[all][u] < *dp[all][at])) at = u;
        }
        if (at == -1) return nullopt;
        vector<bool> used(ed.size()); // 每条原边是否出现在恢复方案中。
        auto trace = [&](auto &&self, int s, int u) -> void
        {
            // self 是递归自身，s 是终端子集，u 是当前端点；标记该状态的最优树边集。
            auto [a, id] = pre[s][u];
            if (a == -1) return;
            if (id == -1)
            {
                self(self, a, u);
                self(self, s ^ a, u);
            }
            else
            {
                used[id] = true;
                self(self, s, a);
            }
        };
        trace(trace, all, at);
        vector<int> take;
        for (int i = 0; i < (int)ed.size(); i++)
        {
            if (used[i]) take.push_back(i);
        }
        return pair<int, vector<int>>{*dp[all][at], take};
    }
};

signed main()
{
    // 无额外参数；读入无向图与终端，输出最小权值和原边编号。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    Steiner st(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        st.add(u, v, w);
    }
    int k; cin >> k;
    vector<int> key(k);
    for (int &u : key)
    {
        cin >> u;
    }
    auto ans = st.solveTree(key);
    assert(ans.has_value());
    cout << ans->first << ' ' << ans->second.size() << '\n';
    for (int i = 0; i < (int)ans->second.size(); i++)
    {
        cout << ans->second[i] << " \n"[i + 1 == (int)ans->second.size()];
    }
    if (ans->second.empty()) cout << '\n';
}
