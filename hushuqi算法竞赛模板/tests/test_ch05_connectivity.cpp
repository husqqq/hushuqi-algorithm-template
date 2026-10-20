#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_3_008.hpp"
#endif
#include TEST_HEADER

int seed = -1;
int points = 0;
vector<tuple<int, int, long long>> input;

void check(bool ok)
{
    // ok 是验证结果；失败时输出种子与当前图并停止，NDEBUG 不关闭本检查。
    if (ok) return;
    cout << "WA " << seed << '\n';
    cerr << points << ' ' << input.size() << '\n';
    for (auto [u, v, w] : input)
    {
        cerr << u << ' ' << v << ' ' << w << '\n';
    }
    exit(1);
}

int bruteEdge(int n, const vector<tuple<int, int, long long>> &e)
{
    // n 是点数，e 是非负权边；枚举非平凡点集，返回最小跨集合边权和。
    if (n <= 1) return 0;
    int ans = LLONG_MAX;
    for (int mask = 1; mask < (1LL << n) - 1; mask++)
    {
        int sum = 0;
        for (auto [u, v, w] : e)
        {
            if (((mask >> u) & 1) != ((mask >> v) & 1)) sum += w;
        }
        ans = min(ans, sum);
    }
    return ans;
}

int bruteVertex(const vector<vector<int>> &g)
{
    // g 是简单无向图；枚举删点集，以普通 BFS 检查剩余图是否断连。
    int n = g.size(), ans = max(0LL, n - 1);
    for (int mask = 0; mask < (1LL << n); mask++)
    {
        vector<char> vis(n);
        int root = -1, left = 0;
        for (int u = 0; u < n; u++)
        {
            if (!(mask >> u & 1)) root = u, left++;
        }
        if (left < 2) continue;
        queue<int> q;
        q.push(root);
        vis[root] = true;
        int seen = 0;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            seen++;
            for (int v : g[u])
            {
                if (!(mask >> v & 1) && !vis[v])
                {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }
        if (seen < left) ans = min(ans, (int)popcount((unsigned long long)mask));
    }
    return ans;
}

signed main()
{
    check(edgeConn(0, {}) == 0);
    check(edgeConn(1, {{0, 0, 7}}) == 0);
    check(edgeConn(3, {{0, 1, 2}}) == 0);
    check(edgeConn(3, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}}) == 2);
    check(edgeConn(2, {{0, 1, 3}, {0, 1, 4}}) == 7);
    check(edgeConn(2, {{0, 1, 3000000000LL}, {0, 1, 4000000000LL}}) == 7000000000LL);

    check(vertexConn({}) == 0);
    check(vertexConn({{}}) == 0);
    check(vertexConn({{1}, {0}, {}}) == 0);
    check(vertexConn({{1}, {0, 2}, {1}}) == 1);
    check(vertexConn({{1, 3}, {0, 2}, {1, 3}, {0, 2}}) == 2);
    check(vertexConn({{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}}) == 3);

    for (int n = 0; n <= 5; n++)
    {
        points = n;
        vector<pair<int, int>> e;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                e.push_back({u, v});
            }
        }
        for (int mask = 0; mask < (1LL << e.size()); mask++)
        {
            seed++;
            input.clear();
            vector<vector<int>> g(n);
            for (int i = 0; i < (int)e.size(); i++)
            {
                if (!(mask >> i & 1)) continue;
                auto [u, v] = e[i];
                input.push_back({u, v, 1});
                g[u].push_back(v);
                g[v].push_back(u);
            }
            int k = vertexConn(g), l = edgeConn(n, input), deg = n ? n - 1 : 0;
            for (auto &a : g)
            {
                deg = min(deg, (int)a.size());
            }
            check(k == bruteVertex(g) && l == bruteEdge(n, input));
            check(k <= l && l <= deg);
            cout << "AC " << seed << '\n';
        }
    }
    for (int r = 0; r < 300; r++)
    {
        seed++;
        mt19937_64 rng(seed);
        int n = rng() % 8 + 1;
        points = n;
        input.clear();
        int m = rng() % 30;
        for (int i = 0; i < m; i++)
        {
            input.push_back({(int)(rng() % n), (int)(rng() % n), (long long)(rng() % 9)});
        }
        check(edgeConn(n, input) == bruteEdge(n, input));
        cout << "AC " << seed << '\n';
    }
}
