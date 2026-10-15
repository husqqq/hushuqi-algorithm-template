#include "../include/full/13_1_007.hpp"
#include "../include/full/13_1_011.hpp"
#include "../include/full/13_1_012.hpp"

void checkEdge(int n, const vector<array<int, 2>> &ed)
{
    // n 是点数，ed 是逐条列出的无向边；用删边状态枚举检查每个根的 SG。
    for (int root = 0; root < n; root++)
    {
        vector<int> memo(1LL << ed.size(), -1);
        auto solve = [&](auto &&self, int mask) -> int
        {
            // self 是递归自身，mask 是仍存在的边集；先清除不接地边，再独立取 mex。
            vector<bool> seen(n);
            seen[root] = true;
            bool changed = true;
            while (changed)
            {
                changed = false;
                for (int i = 0; i < (int)ed.size(); i++)
                {
                    auto [u, v] = ed[i];
                    if ((mask >> i & 1) && seen[u] != seen[v])
                    {
                        seen[u] = seen[v] = true;
                        changed = true;
                    }
                }
            }
            int keep = 0;
            for (int i = 0; i < (int)ed.size(); i++)
            {
                if ((mask >> i & 1) && seen[ed[i][0]]) keep |= 1LL << i;
            }
            if (memo[keep] != -1) return memo[keep];
            set<int> next;
            for (int i = 0; i < (int)ed.size(); i++)
            {
                if (keep >> i & 1) next.insert(self(self, keep ^ (1LL << i)));
            }
            int ans = 0;
            while (next.contains(ans))
            {
                ans++;
            }
            return memo[keep] = ans;
        };
        assert(edge_del::graphSg(n, ed, root) == solve(solve, (1LL << ed.size()) - 1));
    }
}

void checkVertex(int n, const vector<array<int, 2>> &ed, int nl = -1)
{
    // n、ed 是无自环无向图；nl>=0 时前 nl 点为左部，同时核对全部二分图必胜起点。
    vector<vector<int>> g(n);
    for (auto [u, v] : ed)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<vector<int>> memo(1LL << n, vector<int>(n, -1));
    auto solve = [&](auto &&self, int mask, int u) -> bool
    {
        // self 是递归自身，mask 是尚未删除点集，u 是当前点；直接枚举返回是否必胜。
        if (memo[mask][u] != -1) return memo[mask][u];
        bool ans = false;
        for (int v : g[u])
        {
            if ((mask >> v & 1) && !self(self, mask ^ (1LL << u), v)) ans = true;
        }
        return memo[mask][u] = ans;
    };
    pair<vector<int>, vector<int>> want;
    int mask = (1LL << n) - 1;
    for (int u = 0; u < n; u++)
    {
        bool win = solve(solve, mask, u);
        auto move = vertexGame(n, ed, u);
        assert(move.has_value() == win);
        if (move)
        {
            assert(find(g[u].begin(), g[u].end(), *move) != g[u].end());
            assert(!solve(solve, mask ^ (1LL << u), *move));
        }
        if (win && nl >= 0)
        {
            if (u < nl) want.first.push_back(u);
            else want.second.push_back(u - nl);
        }
    }
    if (nl >= 0)
    {
        vector<vector<int>> left(nl);
        for (auto [u, v] : ed)
        {
            assert(u < nl && v >= nl);
            left[u].push_back(v - nl);
        }
        assert(bipGame(left, n - nl) == want);
    }
}

signed main()
{
    // 无参数；穷尽小图并随机覆盖自环、重边、孤立点、非连通图和返回首步，成功返回 0。
    for (int n = 1; n <= 5; n++)
    {
        vector<array<int, 2>> all;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                all.push_back({u, v});
            }
        }
        for (int mask = 0; mask < (1LL << all.size()); mask++)
        {
            vector<array<int, 2>> ed;
            for (int i = 0; i < (int)all.size(); i++)
            {
                if (mask >> i & 1) ed.push_back(all[i]);
            }
            checkEdge(n, ed);
            checkVertex(n, ed);
        }
    }
    mt19937_64 rng(20261017);
    for (int tc = 0; tc < 500; tc++)
    {
        int n = 1 + rng() % 6, m = rng() % 11;
        vector<array<int, 2>> ed;
        for (int i = 0; i < m; i++)
        {
            ed.push_back({(int)(rng() % n), (int)(rng() % n)});
        }
        checkEdge(n, ed);
        vector<vector<int>> tr(n);
        ed.clear();
        for (int v = 1; v < n; v++)
        {
            int u = rng() % v;
            ed.push_back({u, v});
            tr[u].push_back(v);
            tr[v].push_back(u);
        }
        for (int root = 0; root < n; root++)
        {
            assert(edge_del::treeSg(tr, root) == edge_del::graphSg(n, ed, root));
        }
        checkEdge(n, ed);
    }
    for (int nl = 0; nl <= 3; nl++)
    {
        for (int nr = 0; nr <= 3; nr++)
        {
            for (int mask = 0; mask < (1LL << (nl * nr)); mask++)
            {
                vector<array<int, 2>> ed;
                for (int u = 0; u < nl; u++)
                {
                    for (int v = 0; v < nr; v++)
                    {
                        if (mask >> (u * nr + v) & 1) ed.push_back({u, nl + v});
                    }
                }
                checkVertex(nl + nr, ed, nl);
                if (!ed.empty()) ed.push_back(ed.back());
                checkVertex(nl + nr, ed, nl);
            }
        }
    }
    for (int tc = 0; tc < 500; tc++)
    {
        int n = 6 + rng() % 4;
        vector<array<int, 2>> ed;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0) ed.push_back({u, v});
            }
        }
        checkVertex(n, ed);
    }
    cout << "OK\n";
}
