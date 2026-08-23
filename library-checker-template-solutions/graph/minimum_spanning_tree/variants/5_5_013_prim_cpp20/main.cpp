
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct PrimResult
{
    int sum;        // sum 保存最小生成树边权和。
    vector<int> fa; // fa[v] 保存 v 的父亲，根为 -1。
};

optional<PrimResult> primSparse(const vector<vector<pair<int, int>>> &g,
                                int s = 0)
{
    // g[u] 的每项是 {邻点,边权}，s 是根；连通时返回 MST，否则返回空。
    int n = g.size();
    assert(n > 0 && 0 <= s && s < n);
    const int inf = numeric_limits<int>::max() / 4;
    vector<int> d(n, inf), fa(n, -1);
    vector<char> vis(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
    d[s] = 0;
    q.push({0, s});
    int sum = 0, cnt = 0;
    while (!q.empty())
    {
        auto [w, u] = q.top();
        q.pop();
        if (vis[u])
        {
            continue;
        }
        vis[u] = true;
        sum += w;
        cnt++;
        for (auto [v, c] : g[u])
        {
            assert(0 <= v && v < n);
            if (!vis[v] && c < d[v])
            {
                d[v] = c;
                fa[v] = u;
                q.push({c, v});
            }
        }
    }
    if (cnt != n)
    {
        return nullopt;
    }
    return PrimResult{sum, move(fa)};
}

optional<PrimResult> primDense(const vector<vector<int>> &w, int s = 0)
{
    // w 是对称邻接矩阵，缺边填较大的 inf；s 是根，连通时返回 MST，否则返回空。
    int n = w.size();
    assert(n > 0 && 0 <= s && s < n);
    for (const auto &row : w)
    {
        assert((int)row.size() == n);
    }
    const int inf = numeric_limits<int>::max() / 4;
    vector<int> d(n, inf), fa(n, -1);
    vector<char> vis(n);
    d[s] = 0;
    int sum = 0;
    for (int t = 0; t < n; t++)
    {
        int u = -1;
        for (int v = 0; v < n; v++)
        {
            if (!vis[v] && (u < 0 || d[v] < d[u]))
            {
                u = v;
            }
        }
        if (u < 0 || d[u] == inf)
        {
            return nullopt;
        }
        vis[u] = true;
        sum += d[u];
        for (int v = 0; v < n; v++)
        {
            if (!vis[v] && w[u][v] < d[v])
            {
                d[v] = w[u][v];
                fa[v] = u;
            }
        }
    }
    return PrimResult{sum, move(fa)};
}

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<vector<pair<int,int>>>g(n);map<tuple<int,int,int>,vector<int>>ids;for(int i=0;i<m;i++){int u,v,w;cin>>u>>v>>w;g[u].push_back({v,w});g[v].push_back({u,w});ids[{min(u,v),max(u,v),w}].push_back(i);}auto z=primSparse(g);assert(z);cout<<z->sum<<'\n';for(int v=0;v<n;v++)if(z->fa[v]!=-1){int u=z->fa[v],w=LLONG_MAX;for(auto[x,c]:g[v])if(x==u)w=min(w,c);auto&a=ids[{min(u,v),max(u,v),w}];cout<<a.back()<<' ';a.pop_back();}cout<<'\n';}
