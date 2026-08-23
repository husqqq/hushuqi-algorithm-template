
#include <bits/stdc++.h>
using namespace std;
#define int long long

class TarjanTwoSAT
{
    // n 是布尔变量数，g 是 2n 个文字构成的蕴含图。
    int n;
    vector<vector<int>> g;

  public:
    TarjanTwoSAT(int n) : n(n), g(2 * n)
    {
        // n 是变量数；构造没有子句的 2-SAT，无返回值。
    }

    void imply(int x, bool xv, int y, bool yv)
    {
        // x=xv 是前件，y=yv 是后件；加入蕴含及其逆否边。
        int a = 2 * x + xv, b = 2 * y + yv;
        g[a].push_back(b);
        g[b ^ 1].push_back(a ^ 1);
    }

    void addOr(int x, bool xv, int y, bool yv)
    {
        // x=xv、y=yv 是两个文字；加入二元子句 `(x=xv) or (y=yv)`。
        imply(x, !xv, y, yv);
    }

    optional<vector<bool>> solve() const
    {
        // 无参数；返回一组可行变量值，不可满足时返回空。
        int ti = 0, cc = 0;
        vector<int> dfn(2 * n), low(2 * n), st, bel(2 * n, -1);
        vector<bool> on(2 * n);
        auto dfs = [&](auto &&self, int u) -> void
        {
            // u 是当前文字；执行 Tarjan SCC 并给封闭分量编号，无返回值。
            dfn[u] = low[u] = ++ti;
            st.push_back(u);
            on[u] = true;
            for (auto v : g[u])
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
        for (int u = 0; u < 2 * n; u++)
        {
            if (!dfn[u])
            {
                dfs(dfs, u);
            }
        }
        vector<bool> ans(n);
        for (int x = 0; x < n; x++)
        {
            if (bel[2 * x] == bel[2 * x + 1])
            {
                return nullopt;
            }
            ans[x] = bel[2 * x] > bel[2 * x + 1];
        }
        return ans;
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string p, cnf; int n, m; cin >> p >> cnf >> n >> m; TarjanTwoSAT sat(n);
    while (m--) { int x, y, z; cin >> x >> y >> z; sat.addOr(abs(x) - 1, x > 0, abs(y) - 1, y > 0); }
    auto ans = sat.solve(); if (!ans) { cout << "s UNSATISFIABLE\n"; return 0; }
    cout << "s SATISFIABLE\nv"; for (int i = 0; i < n; ++i) cout << ' ' << ((*ans)[i] ? i + 1 : -i - 1); cout << " 0\n";
}
