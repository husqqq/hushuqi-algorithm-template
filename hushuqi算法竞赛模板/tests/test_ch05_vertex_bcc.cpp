#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_3_005.hpp"
#endif
#include TEST_HEADER

int seed = -1, points = 0;
vector<array<int, 2>> input;

void check(bool ok)
{
    // ok 是验证结果；失败时保留种子和图，NDEBUG 不关闭测试检查。
    if (ok) return;
    cout << "WA " << seed << '\n';
    cerr << points << ' ' << input.size() << '\n';
    for (auto [u, v] : input)
    {
        cerr << u << ' ' << v << '\n';
    }
    exit(1);
}

int countCC(int mask)
{
    // mask 是保留的点集；用普通 BFS 返回诱导子图连通块数。
    int ans = 0;
    while (mask)
    {
        int u = __builtin_ctzll(mask);
        queue<int> q;
        q.push(u);
        mask ^= 1LL << u;
        ans++;
        while (!q.empty())
        {
            u = q.front();
            q.pop();
            for (auto [a, b] : input)
            {
                int v = a == u ? b : b == u ? a : -1;
                if (v != -1 && (mask >> v & 1))
                {
                    mask ^= 1LL << v;
                    q.push(v);
                }
            }
        }
    }
    return ans;
}

vector<vector<int>> brute()
{
    // 无参数；枚举无割点的连通诱导子图，返回其中按包含关系极大的点集。
    vector<int> good;
    for (int mask = 1; mask < (1LL << points); mask++)
    {
        if (countCC(mask) != 1) continue;
        bool ok = true;
        for (int u = 0; u < points; u++)
        {
            if ((mask >> u & 1) && countCC(mask ^ (1LL << u)) > 1) ok = false;
        }
        if (ok) good.push_back(mask);
    }
    vector<vector<int>> ans;
    for (int mask : good)
    {
        bool maximal = true;
        for (int other : good)
        {
            if (mask != other && (mask & other) == mask) maximal = false;
        }
        if (!maximal) continue;
        vector<int> cur;
        for (int u = 0; u < points; u++)
        {
            if (mask >> u & 1) cur.push_back(u);
        }
        ans.push_back(cur);
    }
    sort(ans.begin(), ans.end());
    return ans;
}

void verify(VertexBCC &q)
{
    // q 保存 input 中的图；核对点双、点号顺序、割点与重复构建稳定性。
    auto expected = brute();
    vector<vector<int>> prev;
    for (int rep = 0; rep < 2; rep++)
    {
        q.build();
        auto got = q.comps();
        if (rep) check(got == prev);
        prev = got;
        for (const auto &v : got)
        {
            check(!v.empty() && is_sorted(v.begin(), v.end()));
            check(adjacent_find(v.begin(), v.end()) == v.end());
        }
        sort(got.begin(), got.end());
        check(got == expected);
        int all = (1LL << points) - 1, base = countCC(all);
        check((int)q.cutVertices().size() == points);
        for (int u = 0; u < points; u++)
        {
            check(q.cutVertices()[u] == (countCC(all ^ (1LL << u)) > base));
        }
    }
}

signed main()
{
    // 无输入；穷举点数 0..5 的简单图，再核对 500 组可复现多重图与追加边。
    int serial = 0;
    for (points = 0; points <= 5; points++)
    {
        vector<array<int, 2>> all;
        for (int u = 0; u < points; u++)
        {
            for (int v = u + 1; v < points; v++)
            {
                all.push_back({u, v});
            }
        }
        for (int mask = 0; mask < (1LL << all.size()); mask++)
        {
            seed = serial++;
            input.clear();
            VertexBCC q(points);
            for (int i = 0; i < (int)all.size(); i++)
            {
                if (!(mask >> i & 1)) continue;
                input.push_back(all[i]);
                check(q.addEdge(all[i][0], all[i][1]) == (int)input.size() - 1);
            }
            verify(q);
            cout << "AC " << seed << '\n';
        }
    }
    for (seed = 100000; seed < 100500; seed++)
    {
        mt19937_64 rng(seed);
        points = 1 + rng() % 7;
        input.clear();
        VertexBCC q(points);
        for (int i = 0, m = rng() % 24; i < m; i++)
        {
            int u = rng() % points, v = rng() % points;
            input.push_back({u, v});
            check(q.addEdge(u, v) == i);
        }
        verify(q);
        int u = rng() % points, v = rng() % points;
        input.push_back({u, v});
        check(q.addEdge(u, v) == (int)input.size() - 1);
        verify(q);
        cout << "AC " << seed << '\n';
    }
}
