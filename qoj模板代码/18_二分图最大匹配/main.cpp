
#include <bits/stdc++.h>
using namespace std;
#define int long long

class HopcroftKarp
{
    // nl、nr 是左右部点数，g 是从左部出发的邻接表。
    int nl, nr;
    vector<vector<int>> g;
    // ml、mr 是左右匹配点，dep 是左点层数，lim 是本轮最短增广路长度。
    vector<int> ml, mr, dep;
    int lim = -1;

    bool bfs()
    {
        // 无参数；给全部最短增广路分层，存在增广路时返回 true。
        queue<int> q;
        fill(dep.begin(), dep.end(), -1);
        lim = -1;
        for (int u = 0; u < nl; u++)
        {
            if (ml[u] == -1)
            {
                dep[u] = 0;
                q.push(u);
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            if (lim != -1 && dep[u] >= lim)
            {
                continue;
            }
            for (auto v : g[u])
            {
                int x = mr[v];
                if (x == -1)
                {
                    lim = dep[u] + 1;
                }
                else if (dep[x] == -1 && (lim == -1 || dep[u] + 1 < lim))
                {
                    dep[x] = dep[u] + 1;
                    q.push(x);
                }
            }
        }
        return lim != -1;
    }

    bool dfs(int u)
    {
        // u 是当前左点；沿 BFS 层寻找最短增广路，成功时返回 true。
        for (auto v : g[u])
        {
            int x = mr[v];
            if (x == -1 && dep[u] + 1 == lim)
            {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
            if (x != -1 && dep[x] == dep[u] + 1 && dfs(x))
            {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        dep[u] = -1;
        return false;
    }

  public:
    HopcroftKarp(int nl, int nr) : nl(nl), nr(nr), g(nl), ml(nl, -1), mr(nr, -1), dep(nl)
    {
        // nl、nr 是左右部点数；构造空二分图，无返回值。
    }

    void addEdge(int u, int v)
    {
        // u 是左点，v 是右点；加入一条边，无返回值。
        assert(0 <= u && u < nl && 0 <= v && v < nr); // 调试检查，可删。
        g[u].push_back(v);
    }

    int matching()
    {
        // 无参数；从空匹配重算并返回最大匹配边数。
        fill(ml.begin(), ml.end(), -1);
        fill(mr.begin(), mr.end(), -1);
        int ans = 0;
        while (bfs())
        {
            for (int u = 0; u < nl; u++)
            {
                if (ml[u] == -1)
                {
                    ans += dfs(u);
                }
            }
        }
        return ans;
    }

    const vector<int> &leftMatch() const
    {
        // 无参数；返回左点到右点的匹配数组，未匹配为 -1。
        return ml;
    }

    const vector<int> &rightMatch() const
    {
        // 无参数；返回右点到左点的匹配数组，未匹配为 -1。
        return mr;
    }
};

signed main()
{
    int l, r, m; cin >> l >> r >> m;
    HopcroftKarp match(l, r);
    while (m--)
    {
        int u, v; cin >> u >> v;
        --u;
        --v;
        match.addEdge(u, v);
    }
    cout << match.matching() << '\n';
    for (int u = 0; u < l; u++) if (match.leftMatch()[u] != -1)
        cout << u + 1 << ' ' << match.leftMatch()[u] + 1 << '\n';
}
