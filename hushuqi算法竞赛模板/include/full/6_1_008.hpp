#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 参数 a 表示 按节点编号给出的初始权值；b 表示 第二个输入序列、矩阵或操作数；把指定区间内大于给定上界的值降低到该上界，无返回值。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 参数 a 表示 按节点编号给出的初始权值；b 表示 第二个输入序列、矩阵或操作数；把指定位置或区间内小于给定下界的值提高到该下界，无返回值。
    return a < b ? a = b, true : false;
}

struct TreeIso
{
    // 成员变量：mp 保存离散映射或哈希缓存。
    map<vector<int>, int> mp;

    int get(vector<int> a)
    {
        // 参数 a 表示 按节点编号给出的初始权值；返回当前位置、节点或结构保存的查询值。
        sort(a.begin(), a.end());
        auto [it, ok] = mp.emplace(move(a), mp.size() + 1);
        return it->second;
    }

    vector<int> ids(const vector<vector<int>> &g, int rt, const vector<bool> *ban = nullptr)
    {
        // 参数 g 表示 树的邻接表；rt 表示 版本根编号；ban 表示 本轮被排除的节点或边；返回当前遍历或分解选中的节点编号。
        int n = g.size();
        vector<int> p(n, -2), q{rt};
        p[rt] = -1;
        for (int k = 0; k < (int)q.size(); k++)
        {
            int u = q[k];
            for (int v : g[u])
            {
                if (v == p[u] || p[v] != -2 || (ban && (*ban)[v]))
                {
                    continue;
                }
                p[v] = u;
                q.push_back(v);
            }
        }
        vector<int> h(n);
        for (int k = (int)q.size() - 1; k >= 0; k--)
        {
            int u = q[k];
            vector<int> a;
            for (int v : g[u])
            {
                if (p[v] == u)
                {
                    a.push_back(h[v]);
                }
            }
            h[u] = get(move(a));
        }
        return h;
    }

    int rooted(const vector<vector<int>> &g, int rt = 0)
    {
        // 参数 g 表示 树的邻接表；rt 表示 版本根编号；返回换根后指定点所代表的子树根关系。
        return ids(g, rt)[rt];
    }

    vector<int> centroids(const vector<vector<int>> &g)
    {
        // 参数 g 表示 树的邻接表；返回树的一个或两个重心编号。
        int n = g.size();
        vector<int> p(n, -1), sz(n, 1), q{0};
        for (int k = 0; k < (int)q.size(); k++)
        {
            int u = q[k];
            for (int v : g[u])
            {
                if (v == p[u])
                {
                    continue;
                }
                p[v] = u;
                q.push_back(v);
            }
        }
        vector<int> ans;
        for (int k = n - 1; k >= 0; k--)
        {
            int u = q[k];
            for (int v : g[u])
            {
                if (p[v] != u)
                {
                    continue;
                }
                sz[u] += sz[v];
            }
            int mx = n - sz[u];
            for (int v : g[u])
            {
                if (p[v] != u)
                {
                    continue;
                }
                mx = max(mx, sz[v]);
            }
            if (mx * 2 <= n)
            {
                ans.push_back(u);
            }
        }
        return ans;
    }

    vector<int> unrooted(const vector<vector<int>> &g)
    {
        // 参数 g 表示 树的邻接表；返回无根树与编号无关的规范哈希。
        vector<int> ans;
        for (int r : centroids(g))
        {
            ans.push_back(rooted(g, r));
        }
        sort(ans.begin(), ans.end());
        return ans;
    }

    vector<int> minRot(const vector<int> &a)
    {
        // 参数 a 表示 按节点编号给出的初始权值；返回循环序列字典序最小表示的起点。
        int n = a.size(), i = 0, j = 1, k = 0;
        while (i < n && j < n && k < n)
        {
            int x = a[(i + k) % n], y = a[(j + k) % n];
            if (x == y)
            {
                k++;
                continue;
            }
            if (x > y)
            {
                i += k + 1;
            }
            else
            {
                j += k + 1;
            }
            if (i == j)
            {
                j++;
            }
            k = 0;
        }
        int s = min(i, j);
        vector<int> b;
        for (int x = 0; x < n; x++)
        {
            b.push_back(a[(s + x) % n]);
        }
        return b;
    }

    vector<int> unicycle(const vector<vector<int>> &g)
    {
        // 参数 g 表示 树的邻接表；返回基环树与编号无关的规范哈希。
        int n = g.size();
        vector<int> d(n);
        queue<int> q;
        for (int i = 0; i < n; i++)
        {
            d[i] = g[i].size();
            if (d[i] == 1)
            {
                q.push(i);
            }
        }
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            d[u] = 0;
            for (int v : g[u])
            {
                if (d[v] && --d[v] == 1)
                {
                    q.push(v);
                }
            }
        }
        vector<int> cyc;
        for (int i = 0; i < n; i++)
        {
            if (d[i])
            {
                cyc.push_back(i);
            }
        }
        if (cyc.empty())
        {
            return {};
        }
        vector<bool> ban(n);
        for (int x : cyc)
        {
            ban[x] = true;
        }
        vector<int> a;
        int s = cyc[0], p = -1, u = s;
        do
        {
            ban[u] = false;
            a.push_back(ids(g, u, &ban)[u]);
            ban[u] = true;
            int v = -1;
            for (int x : g[u])
            {
                if (d[x] && x != p)
                {
                    v = x;
                }
            }
            p = u;
            u = v;
        } while (u != s);
        auto x = minRot(a);
        reverse(a.begin(), a.end());
        return min(x, minRot(a));
    }
};
