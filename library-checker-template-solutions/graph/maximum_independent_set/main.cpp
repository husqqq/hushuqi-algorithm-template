
#include <bits/stdc++.h>
using namespace std;
#define int long long

// 这是补图最大团的精确分支限界，只适合 n<=63 的一般图。
inline vector<int> maxIS(int n, const vector<pair<int, int>> &edges)
{
    // n 是点数，edges 是无向边；返回一组最大独立集顶点，允许重边和自环。
    assert(0 <= n && n <= 63); // 调试检查，可删：位集使用 unsigned long long。
    vector<unsigned long long> adj(n);
    vector<bool> loop(n);
    unsigned long long all = n ? (1ULL << n) - 1 : 0;
    for (int u = 0; u < n; u++) adj[u] = all ^ (1ULL << u);
    for (auto [u, v] : edges)
    {
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删。
        if (u == v)
        {
            loop[u] = true;
        }
        else
        {
            adj[u] &= ~(1ULL << v);
            adj[v] &= ~(1ULL << u);
        }
    }
    vector<int> best, cur;
    auto dfs = [&](auto &&self, unsigned long long can) -> void
    {
        // can 是仍与当前选点全相邻的候选点集合；更新 best，无返回值。
        vector<int> ord, col;
        unsigned long long rem = can;
        int cnt = 0;
        while (rem)
        {
            cnt++;
            unsigned long long able = rem;
            while (able)
            {
                int u = countr_zero(able);
                ord.push_back(u);
                col.push_back(cnt);
                rem ^= 1ULL << u;
                able &= ~(1ULL << u);
                able &= ~adj[u];
                able &= rem;
            }
        }
        for (int i = (int)ord.size() - 1; i >= 0; i--)
        {
            if ((int)cur.size() + col[i] <= (int)best.size()) return;
            int u = ord[i];
            cur.push_back(u);
            unsigned long long nxt = 0;
            for (int j = 0; j < i; j++)
            {
                if (adj[u] >> ord[j] & 1ULL) nxt |= 1ULL << ord[j];
            }
            if (!nxt)
            {
                if (cur.size() > best.size()) best = cur;
            }
            else self(self, nxt);
            cur.pop_back();
        }
    };
    unsigned long long can = all;
    for (int u = 0; u < n; u++) if (loop[u]) can &= ~(1ULL << u);
    dfs(dfs, can);
    return best;
}

signed main()
{
    int n, m; cin >> n >> m;
    vector<pair<int, int>> e(m);
    for (auto &[u, v] : e) cin >> u >> v;
    auto ans = maxIS(n, e);
    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++) cout << ans[i] << " \n"[i + 1 == (int)ans.size()];
}
