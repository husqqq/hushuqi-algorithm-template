
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<vector<int>> complementCC(const vector<vector<int>> &g)
{
    // g 是原图邻接表；返回补图的全部连通块，每个点恰出现一次。
    int n = g.size();
    set<int> left;
    for (int u = 0; u < n; u++)
    {
        left.insert(u);
    }
    vector<int> mark(n);
    int tim = 0;
    vector<vector<int>> ans;
    while (!left.empty())
    {
        int s = *left.begin();
        left.erase(s);
        ans.push_back({});
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            ans.back().push_back(u);
            tim++;
            for (int v : g[u])
            {
                mark[v] = tim;
            }
            for (auto it = left.begin(); it != left.end();)
            {
                int v = *it;
                if (mark[v] != tim)
                {
                    it = left.erase(it);
                    q.push(v);
                }
                else
                {
                    it++;
                }
            }
        }
    }
    return ans;
}

signed main()
{
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    while (m--)
    {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto ans = complementCC(g);
    cout << ans.size() << '\n';
    for (auto &v : ans)
    {
        cout << v.size();
        for (int u : v) cout << ' ' << u;
        cout << '\n';
    }
}
