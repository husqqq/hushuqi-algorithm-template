

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> tarjanSCC(const vector<vector<int>> &g)
{
    // g 是有向邻接表；返回按分量弹栈顺序递增的 SCC 编号。
    int n = g.size(), ti = 0, cc = 0;
    vector<int> dfn(n), low(n), st, bel(n, -1);
    vector<bool> on(n);
    auto dfs = [&](auto &&self, int u) -> void
    {
        dfn[u] = low[u] = ++ti;
        st.push_back(u);
        on[u] = true;
        for (int v : g[u])
        {
            if (!dfn[v])
            {
                self(self, v);
                low[u] = min(low[u], low[v]);
            }
            else if (on[v])
            {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] != dfn[u])
        {
            return;
        }
        while (true)
        {
            int v = st.back();
            st.pop_back();
            on[v] = false;
            bel[v] = cc;
            if (v == u)
            {
                break;
            }
        }
        cc++;
    };
    for (int u = 0; u < n; u++)
    {
        if (!dfn[u])
        {
            dfs(dfs, u);
        }
    }
    return bel;
}

vector<int> incrementalSCC(int n, const vector<pair<int, int>> &e)
{
    int m = e.size();
    vector<int> ans(m, m + 1), id(n, -1);
    vector<tuple<int, int, int>> all;
    for (int i = 0; i < m; i++)
    {
        auto [u, v] = e[i];
        assert(0 <= u && u < n && 0 <= v && v < n);
        all.push_back({i, u, v});
    }
    auto solve = [&](auto &&self, const vector<tuple<int, int, int>> &a, int l, int r) -> void
    {
        if (a.empty() || r == l + 1)
        {
            return;
        }
        int mid = (l + r) / 2;
        int cnt = 0;
        for (auto [i, u, v] : a)
        {
            if (id[u] == -1)
            {
                id[u] = cnt++;
            }
            if (id[v] == -1)
            {
                id[v] = cnt++;
            }
        }
        vector<vector<int>> g(cnt);
        for (auto [i, u, v] : a)
        {
            if (i < mid)
            {
                g[id[u]].push_back(id[v]);
            }
        }
        vector<int> bel = tarjanSCC(g);
        vector<tuple<int, int, int>> left, right;
        for (auto [i, u, v] : a)
        {
            int x = id[u], y = id[v];
            if (i < mid && bel[x] == bel[y])
            {
                ans[i] = min(ans[i], mid);
                left.push_back({i, x, y});
            }
            else
            {
                right.push_back({i, bel[x], bel[y]});
            }
        }
        for (auto [i, u, v] : a)
        {
            id[u] = id[v] = -1;
        }
        self(self, left, l, mid);
        self(self, right, mid, r);
    };
    solve(solve, all, 0, m + 1);
    return ans;
}

signed main()
{
    constexpr int mod = 998244353;
    int n, m; cin >> n >> m;
    vector<int> x(n);
    for (int &v : x) cin >> v;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto when = incrementalSCC(n, e);
    vector<vector<int>> add(m + 1);
    for (int i = 0; i < m; i++) if (when[i] <= m) add[when[i]].push_back(i);
    vector<int> par(n, -1);
    auto find = [&](auto &&self, int u) -> int
    {
        return par[u] < 0 ? u : par[u] = self(self, par[u]);
    };
    int ans = 0;
    for (int t = 1; t <= m; t++)
    {
        for (int id : add[t])
        {
            auto [u, v] = e[id];
            u = find(find, u);
            v = find(find, v);
            if (u == v) continue;
            ans = (ans + x[u] * x[v]) % mod;
            if (par[u] > par[v]) swap(u, v);
            par[u] += par[v];
            par[v] = u;
            x[u] = (x[u] + x[v]) % mod;
        }
        cout << ans << '\n';
    }
}
