
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

signed main()
{
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
    }
    auto bel = tarjanSCC(g);
    int cnt = 0;
    for (int x : bel) cnt = max(cnt, x + 1);
    vector<vector<int>> part(cnt);
    for (int u = 0; u < n; u++) part[cnt - 1 - bel[u]].push_back(u);
    cout << cnt << '\n';
    for (auto &v : part)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
