#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_5_008.hpp"
#endif
#include TEST_HEADER

void check(bool ok)
{
    // ok 是待核对条件；失败时抛异常，NDEBUG 不会关闭检查。
    if (!ok) throw runtime_error("DMST 检查失败");
}

optional<__int128> brute(int n, int root, const vector<DMSTFastEdge> &e)
{
    // n、root、e 是小图；独立枚举每点的入边，返回最优树形图权值，无解为空。
    vector<vector<int>> in(n);
    for (int i = 0; i < (int)e.size(); i++)
    {
        auto [u, v, w] = e[i];
        if (u != v && v != root) in[v].push_back(i);
    }
    vector<int> par(n, root);
    optional<__int128> best;
    auto dfs = [&](auto &&self, int v, __int128 sum) -> void
    {
        // self 是递归自身，v 是下个待选点，sum 是已选边权和；仅保留从根可达的方案。
        if (v == n)
        {
            for (int s = 0; s < n; s++)
            {
                int u = s, step = 0;
                while (u != root && step++ < n)
                {
                    u = par[u];
                }
                if (u != root) return;
            }
#ifdef TEST_MAX
            if (!best || sum > *best) best = sum;
#else
            if (!best || sum < *best) best = sum;
#endif
            return;
        }
        if (v == root)
        {
            self(self, v + 1, sum);
            return;
        }
        for (int id : in[v])
        {
            par[v] = e[id][0];
            self(self, v + 1, sum + (__int128)e[id][2]);
        }
    };
    dfs(dfs, 0, 0);
    return best;
}

void verify(int n, int root, const vector<DMSTFastEdge> &e, int seed = 100000)
{
    // n、root、e 是待测图，seed 是可复现编号；核对权值与方案，失败保留输入并立即停止。
    auto require = [&](bool ok)
    {
        // ok 是当前检查结果；失败时把完整有向图保存到系统临时目录。
        if (ok) return;
        auto path = filesystem::temp_directory_path() / ("dmst-failure-" + to_string(seed) + ".in");
        ofstream out(path);
        out << n << ' ' << e.size() << ' ' << root << '\n';
        for (auto [u, v, w] : e)
        {
            out << u << ' ' << v << ' ' << w << '\n';
        }
        out.close();
        cout << "WA " << seed << endl;
        throw runtime_error("失败输入：" + path.string());
    };
    auto expected = brute(n, root, e);
    vector<DEdgeMST> wide;
    for (auto [u, v, w] : e)
    {
        wide.push_back({u, v, (__int128)w});
    }
    require(directedMST(n, root, wide) == expected);
    auto old = e;
    auto ans = dmstTree(n, root, e);
    require(e == old && (bool)ans == (bool)expected);
    if (!ans)
    {
        cout << "AC " << seed << '\n';
        return;
    }
    require(ans->weight == *expected);
    require((int)ans->parent.size() == n && (int)ans->edge.size() == n);
    require(ans->parent[root] == root && ans->edge[root] == -1);
    __int128 sum = 0;
    for (int v = 0; v < n; v++)
    {
        if (v == root) continue;
        int id = ans->edge[v];
        require(0 <= id && id < (int)e.size());
        auto [u, to, w] = e[id];
        require(to == v && ans->parent[v] == u);
        sum += (__int128)w;
        int at = v, step = 0;
        while (at != root && step++ < n)
        {
            require(0 <= at && at < n);
            at = ans->parent[at];
        }
        require(at == root);
    }
    require(sum == ans->weight);
    auto again = dmstTree(n, root, e);
    require(again && again->weight == ans->weight && again->parent == ans->parent && again->edge == ans->edge);
    cout << "AC " << seed << '\n';
}

signed main()
{
    // 无输入；穷举小图、固定种子多重图及极值权，检查两版接口与方案恢复。
    verify(1, 0, {});
    verify(1, 0, {{0, 0, -7}, {0, 0, 9}});
    verify(3, 0, {{0, 1, 1}});
    verify(3, 0, {{0, 1, 5}, {0, 2, 5}, {1, 2, -10}});
    verify(3, 0, {{0, 1, 10}, {0, 2, 10}, {1, 2, 1}, {2, 1, 1}});
    int lo = numeric_limits<int>::min(), hi = numeric_limits<int>::max();
    verify(3, 0, {{0, 1, hi}, {1, 2, hi}});
    verify(3, 0, {{0, 1, lo}, {1, 2, lo}});
    verify(4, 0, {{0, 1, hi}, {0, 2, hi}, {0, 3, hi},
                  {1, 2, lo}, {2, 1, lo}, {3, 1, lo + 1}, {2, 3, lo + 1}});
    __int128 huge = (__int128)numeric_limits<long long>::max() * 2;
    check(directedMST(3, 0, {{0, 1, huge}, {1, 2, huge}}) == huge * 2);
    int configs = 0;
    for (int n = 1; n <= 3; n++)
    {
        vector<array<int, 2>> pairs; // 每项依次为起点、终点。
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                if (u != v) pairs.push_back({u, v});
            }
        }
        int lim = (int)1 << (2 * (int)pairs.size());
        for (int mask = 0; mask < lim; mask++)
        {
            vector<DMSTFastEdge> e;
            int z = mask;
            for (auto [u, v] : pairs)
            {
                int type = z & 3;
                z >>= 2;
                if (type) e.push_back({u, v, type - 2});
            }
            for (int root = 0; root < n; root++)
            {
                verify(n, root, e, configs++);
            }
        }
    }
    check(configs == 12321);
    for (int seed = 0; seed < 1500; seed++)
    {
        mt19937_64 rng(seed);
        int n = rng() % 6 + 1, root = rng() % n, m = rng() % 16;
        vector<DMSTFastEdge> e;
        for (int i = 0; i < m; i++)
        {
            int u = rng() % n, v = rng() % n;
            int w = seed % 7 == 0 ? 0 : (int)(rng() % 15) - 7;
            if (seed % 13 == 0) w *= (int)1000000007;
            e.push_back({u, v, w});
        }
        verify(n, root, e, 20000 + seed * 2);
        shuffle(e.begin(), e.end(), rng);
        verify(n, root, e, 20001 + seed * 2);
    }
    int n = 50000;
    vector<DEdgeMST> chain;
    for (int v = 1; v < n; v++)
    {
        chain.push_back({v - 1, v, 1});
    }
    check(directedMST(n, 0, chain) == n - 1);
    vector<DMSTFastEdge> nested{{0, 1, 2000}};
    for (int v = 2; v < 1500; v++)
    {
        nested.push_back({1, v, 0});
        nested.push_back({v, 1, v - 2});
    }
    auto ans = dmstTree(1500, 0, nested);
    check(ans && ans->weight == 2000);
    vector<DEdgeMST> wide;
    for (auto [u, v, w] : nested)
    {
        wide.push_back({u, v, w});
    }
    check(directedMST(1500, 0, wide) == 2000);
    cout << "AC 200000\n";
}
