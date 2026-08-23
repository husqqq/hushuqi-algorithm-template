
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct EdgeColor
{
    // 颜色数。
    int k;
    // c[i] 是第 i 条原边的颜色。
    vector<int> c;
};

struct BipEdgeColor
{
    struct E
    {
        // 左端点、右端点、原边编号；虚拟边编号为 -1。
        int u, v, id;
    };

    // 左、右部点数。
    int n, m;
    // 原图边集。
    vector<E> e;

    BipEdgeColor(int n, int m) : n(n), m(m)
    {
        // n、m 是左右部点数；构造空二分图。
    }

    void add(int u, int v)
    {
        // u、v 是左右端点；按加入顺序给边编号。
        assert(0 <= u && u < n && 0 <= v && v < m); // 调试检查，可删。
        e.push_back({u, v, (int)e.size()});
    }

    EdgeColor work() const
    {
        // 返回最少颜色数及每条原边的颜色；先合并低度点，避免孤立点使补图膨胀。
        vector<int> dl(n), dr(m);
        int k = 0;
        for (auto x : e)
        {
            k = max(k, ++dl[x.u]);
            k = max(k, ++dr[x.v]);
        }
        if (!k) return {0, {}};
        auto compress = [&](const vector<int> &deg) -> pair<vector<int>, int>
        {
            // deg 是一侧原点度数；返回原点到合并点的映射及合并后点数。
            int sz = deg.size();
            vector<int> fa(sz, -1);
            auto find = [&](auto &&self, int u) -> int
            {
                // u 是并查集节点；返回所在集合代表。
                return fa[u] < 0 ? u : fa[u] = self(self, fa[u]);
            };
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
            for (int i = 0; i < sz; i++) q.push({deg[i], i});
            while (q.size() > 1)
            {
                auto [x, u] = q.top(); q.pop();
                auto [y, v] = q.top(); q.pop();
                if (x + y > k)
                {
                    q.push({x, u});
                    q.push({y, v});
                    break;
                }
                if (fa[u] > fa[v]) swap(u, v);
                fa[u] += fa[v];
                fa[v] = u;
                q.push({x + y, u});
            }
            vector<int> id(sz), pos(sz, -1);
            int cnt = 0;
            for (int i = 0; i < sz; i++)
            {
                int u = find(find, i);
                if (pos[u] == -1) pos[u] = cnt++;
                id[i] = pos[u];
            }
            return {id, cnt};
        };
        auto [pl, nl] = compress(dl);
        auto [pr, nr] = compress(dr);
        int z = max(nl, nr);
        vector<E> es;
        es.reserve(2 * e.size() + 2 * z);
        vector<int> cl(z), cr(z);
        for (auto x : e)
        {
            x.u = pl[x.u];
            x.v = pr[x.v];
            es.push_back(x);
            cl[x.u]++;
            cr[x.v]++;
        }
        int u = 0, v = 0;
        while (u < z && v < z)
        {
            while (u < z && cl[u] == k) u++;
            while (v < z && cr[v] == k) v++;
            if (u == z || v == z) break;
            int cnt = min(k - cl[u], k - cr[v]);
            for (int i = 0; i < cnt; i++) es.push_back({u, v, -1});
            cl[u] += cnt;
            cr[v] += cnt;
        }
        vector<int> ans(e.size());
        auto perfect = [&](const vector<int> &ids) -> vector<int>
        {
            // ids 是正则二分图边编号；返回一组覆盖全部合并点的完美匹配边。
            vector<vector<int>> g(z);
            for (int id : ids) g[es[id].u].push_back(id);
            vector<int> ml(z, -1), mr(z, -1), dep(z), it(z);
            auto bfs = [&]() -> bool
            {
                // 无参数；给最短增广路分层，存在时返回 true。
                queue<int> q;
                fill(dep.begin(), dep.end(), -1);
                int lim = -1;
                for (int x = 0; x < z; x++) if (ml[x] == -1) dep[x] = 0, q.push(x);
                while (!q.empty())
                {
                    int x = q.front(); q.pop();
                    if (lim != -1 && dep[x] >= lim) continue;
                    for (int id : g[x])
                    {
                        int y = mr[es[id].v];
                        if (y == -1) lim = dep[x] + 1;
                        else if (dep[y] == -1 && (lim == -1 || dep[x] + 1 < lim)) dep[y] = dep[x] + 1, q.push(y);
                    }
                }
                return lim != -1;
            };
            auto dfs = [&](auto &&self, int x) -> bool
            {
                // x 是当前左点；沿分层交替路增广，成功时返回 true。
                for (int &i = it[x]; i < (int)g[x].size(); i++)
                {
                    int id = g[x][i], y = mr[es[id].v];
                    if (y == -1 || (dep[y] == dep[x] + 1 && self(self, y)))
                    {
                        ml[x] = id;
                        mr[es[id].v] = x;
                        return true;
                    }
                }
                dep[x] = -1;
                return false;
            };
            int got = 0;
            while (bfs())
            {
                fill(it.begin(), it.end(), 0);
                for (int x = 0; x < z; x++) if (ml[x] == -1 && dfs(dfs, x)) got++;
            }
            assert(got == z); // 调试检查，可删：正则二分图总有完美匹配。
            return ml;
        };
        auto color = [&](auto &&self, vector<int> ids, int d, int first) -> void
        {
            // ids 是 d 正则图边集，d 是度数，first 是该集合第一种可用颜色。
            if (!d) return;
            if (d == 1)
            {
                for (int id : ids) if (es[id].id != -1) ans[es[id].id] = first;
                return;
            }
            if (d & 1)
            {
                auto take = perfect(ids);
                vector<char> used(es.size());
                for (int id : take)
                {
                    used[id] = true;
                    if (es[id].id != -1) ans[es[id].id] = first;
                }
                vector<int> rest;
                rest.reserve(ids.size() - take.size());
                for (int id : ids) if (!used[id]) rest.push_back(id);
                self(self, move(rest), d - 1, first + 1);
                return;
            }
            vector<vector<pair<int, int>>> g(2 * z);
            for (int id : ids)
            {
                int a = es[id].u, b = z + es[id].v;
                g[a].push_back({b, id});
                g[b].push_back({a, id});
            }
            vector<char> used(es.size());
            vector<int> ptr(2 * z), a, b;
            for (int s = 0; s < 2 * z; s++)
            {
                while (ptr[s] < (int)g[s].size() && used[g[s][ptr[s]].second]) ptr[s]++;
                if (ptr[s] == (int)g[s].size()) continue;
                vector<pair<int, int>> stk{{s, -1}}, tour;
                while (!stk.empty())
                {
                    int x = stk.back().first;
                    while (ptr[x] < (int)g[x].size() && used[g[x][ptr[x]].second]) ptr[x]++;
                    if (ptr[x] == (int)g[x].size())
                    {
                        int id = stk.back().second;
                        stk.pop_back();
                        if (id != -1) tour.push_back({0, id});
                    }
                    else
                    {
                        auto [y, id] = g[x][ptr[x]++];
                        if (!used[id]) used[id] = true, stk.push_back({y, id});
                    }
                }
                for (int i = 0; i < (int)tour.size(); i++) (i & 1 ? b : a).push_back(tour[i].second);
            }
            self(self, move(a), d / 2, first);
            self(self, move(b), d / 2, first + d / 2);
        };
        vector<int> ids(es.size());
        iota(ids.begin(), ids.end(), 0);
        color(color, move(ids), k, 0);
        return {k, ans};
    }
};

signed main()
{
    int nl, nr, m; cin >> nl >> nr >> m;
    BipEdgeColor g(nl, nr);
    for (int i = 0; i < m; i++)
    {
        int u, v; cin >> u >> v;
        g.add(u, v);
    }
    auto ans = g.work();
    cout << ans.k << '\n';
    for (int x : ans.c) cout << x << '\n';
}
