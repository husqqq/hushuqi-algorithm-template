
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> directedCycle(const vector<vector<int>> &g)
{
    // g 是有向邻接表；返回一个简单环，无环返回空。
    int n = g.size();
    vector<int> c(n), p(n, -1), ans;
    auto dfs = [&](auto &&self, int u) -> bool
    {
        c[u] = 1;
        for (int v : g[u])
        {
            assert(0 <= v && v < n); // 调试检查，可删。
            if (c[v] == 0)
            {
                p[v] = u;
                if (self(self, v))
                {
                    return true;
                }
            }
            else if (c[v] == 1)
            {
                ans = {v};
                for (int x = u; x != v; x = p[x])
                {
                    ans.push_back(x);
                }
                reverse(ans.begin() + 1, ans.end());
                return true;
            }
        }
        c[u] = 2;
        return false;
    };
    for (int s = 0; s < n; s++)
    {
        if (!c[s] && dfs(dfs, s))
        {
            return ans;
        }
    }
    return {};
}

vector<int> directedCycleEdges(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是有向边；返回一条环的原边编号顺序，无环返回空。
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        auto [u, v] = edges[i];
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        g[u].push_back({v, i});
    }
    vector<int> state(n), it(n), par(n, -1), pe(n, -1), st;
    for (int s = 0; s < n; s++)
    {
        if (state[s]) continue;
        st.push_back(s);
        state[s] = 1;
        while (!st.empty())
        {
            int u = st.back();
            if (it[u] == (int)g[u].size())
            {
                state[u] = 2;
                st.pop_back();
                continue;
            }
            auto [v, id] = g[u][it[u]++];
            if (!state[v])
            {
                par[v] = u;
                pe[v] = id;
                state[v] = 1;
                st.push_back(v);
            }
            else if (state[v] == 1)
            {
                vector<int> ans{id};
                for (int x = u; x != v; x = par[x]) ans.push_back(pe[x]);
                reverse(ans.begin(), ans.end());
                return ans;
            }
        }
    }
    return {};
}

vector<int> undirectedCycleEdges(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是无向边；返回一条环的原边编号顺序，无环返回空。
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        auto [u, v] = edges[i];
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        g[u].push_back({v, i});
        if (u != v) g[v].push_back({u, i});
    }
    vector<int> state(n), it(n), par(n, -1), pe(n, -1), st;
    for (int s = 0; s < n; s++)
    {
        if (state[s]) continue;
        st.push_back(s);
        state[s] = 1;
        while (!st.empty())
        {
            int u = st.back();
            if (it[u] == (int)g[u].size())
            {
                state[u] = 2;
                st.pop_back();
                continue;
            }
            auto [v, id] = g[u][it[u]++];
            if (id == pe[u]) continue;
            if (!state[v])
            {
                par[v] = u;
                pe[v] = id;
                state[v] = 1;
                st.push_back(v);
            }
            else if (state[v] == 1)
            {
                vector<int> ans{id};
                for (int x = u; x != v; x = par[x]) ans.push_back(pe[x]);
                reverse(ans.begin(), ans.end());
                return ans;
            }
        }
    }
    return {};
}

unsigned __int128 simpleCycles(const vector<unsigned long long> &g)
{
    // g 是简单无向图的对称邻接位掩码；返回全部简单环数量。
    int n = g.size();
    assert(n <= 22); // 调试检查，可删：状态数必须能承受。
    for (int u = 0; u < n; u++)
    {
        assert(!(g[u] >> u & 1)); // 调试检查，可删。
        for (int v = 0; v < n; v++)
        {
            assert((g[u] >> v & 1) == (g[v] >> u & 1)); // 调试检查，可删。
        }
    }
    unsigned __int128 ans = 0;
    for (int s = 0; s < n; s++)
    {
        int m = n - s;
        size_t states = 1ULL << m;
        vector<unsigned long long> dp(states * m);
        dp[m] = 1;
        for (size_t mask = 1; mask < states; mask += 2)
        {
            for (int u = 0; u < m; u++)
            {
                unsigned long long x = dp[mask * m + u];
                if (!x)
                {
                    continue;
                }
                for (int v = 1; v < m; v++)
                {
                    if (!(mask >> v & 1) && (g[s + u] >> (s + v) & 1))
                    {
                        dp[(mask | 1ULL << v) * m + v] += x;
                    }
                }
                if (popcount((unsigned)mask) >= 3 && (g[s + u] >> s & 1))
                {
                    ans += x;
                }
            }
        }
    }
    return ans / 2;
}

signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = directedCycleEdges(n, e);
    if (ans.empty()) cout << -1 << '\n';
    else
    {
        cout << ans.size() << '\n';
        for (int id : ans) cout << id << '\n';
    }
}
