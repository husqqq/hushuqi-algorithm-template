#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_5_009.hpp"
#endif
#include TEST_HEADER

int seed = -1, points = 0;
vector<array<int, 3>> input;
vector<int> keys;

void check(bool ok)
{
    // ok 是验证结果；失败时保留种子、图和终端，NDEBUG 不关闭测试检查。
    if (ok) return;
    cout << "WA " << seed << '\n';
    cerr << points << ' ' << input.size() << '\n';
    for (auto [u, v, w] : input)
    {
        cerr << u << ' ' << v << ' ' << w << '\n';
    }
    cerr << keys.size() << '\n';
    for (int u : keys)
    {
        cerr << u << ' ';
    }
    cerr << '\n';
    exit(1);
}

optional<int> brute()
{
    // 无参数；枚举所有原边子集，用传递闭包检查终端连通，返回最优边权和。
    if (keys.empty()) return 0;
    optional<int> best;
    for (int mask = 0; mask < (1LL << input.size()); mask++)
    {
        vector<int> reach(points);
        for (int u = 0; u < points; u++)
        {
            reach[u] = 1LL << u;
        }
        int sum = 0;
        for (int i = 0; i < (int)input.size(); i++)
        {
            if (!(mask >> i & 1)) continue;
            auto [u, v, w] = input[i];
            reach[u] |= 1LL << v;
            reach[v] |= 1LL << u;
            sum += w;
        }
        if (best && sum >= *best) continue;
        for (int v = 0; v < points; v++)
        {
            for (int u = 0; u < points; u++)
            {
                if (reach[u] >> v & 1) reach[u] |= reach[v];
            }
        }
        bool ok = true;
        for (int u : keys)
        {
            if (!(reach[keys[0]] >> u & 1)) ok = false;
        }
        if (ok) best = sum;
    }
    return best;
}

void verify(const Steiner &q)
{
    // q 保存 input 中的图；独立核对两个接口的权值，并检查恢复边集的连通性与无环性。
    auto expected = brute();
    check(q.solve(keys) == expected);
    auto ans = q.solveTree(keys);
    check(ans.has_value() == expected.has_value());
    if (!ans) return;
    check(ans->first == *expected);
    const auto &take = ans->second;
    check(is_sorted(take.begin(), take.end()));
    check(adjacent_find(take.begin(), take.end()) == take.end());
    vector<int> reach(points);
    for (int u = 0; u < points; u++)
    {
        reach[u] = 1LL << u;
    }
    int sum = 0;
    for (int id : take)
    {
        check(0 <= id && id < (int)input.size());
        auto [u, v, w] = input[id];
        check(!(reach[u] >> v & 1));
        int merged = reach[u] | reach[v];
        for (int a = 0; a < points; a++)
        {
            if (merged >> a & 1) reach[a] = merged;
        }
        sum += w;
    }
    check(sum == ans->first);
    if (keys.empty()) check(take.empty());
    else
    {
        for (int u : keys)
        {
            check(reach[keys[0]] >> u & 1);
        }
        for (int id : take)
        {
            auto [u, v, w] = input[id];
            check(reach[keys[0]] >> u & 1);
        }
    }
}

signed main()
{
    // 无输入；穷举 0..4 点的缺边/零权/单位权图及其所有终端集，再测固定种子多重图。
    int serial = 0;
    for (points = 0; points <= 4; points++)
    {
        vector<array<int, 2>> all;
        for (int u = 0; u < points; u++)
        {
            for (int v = u + 1; v < points; v++)
            {
                all.push_back({u, v});
            }
        }
        int total = 1;
        for (int i = 0; i < (int)all.size(); i++)
        {
            total *= 3;
        }
        for (int code = 0; code < total; code++)
        {
            seed = serial++;
            input.clear();
            Steiner q(points);
            int state = code;
            for (auto [u, v] : all)
            {
                int x = state % 3;
                state /= 3;
                if (!x) continue;
                input.push_back({u, v, x - 1});
                q.add(u, v, x - 1);
            }
            for (int mask = 0; mask < (1LL << points); mask++)
            {
                keys.clear();
                for (int u = 0; u < points; u++)
                {
                    if (mask >> u & 1) keys.push_back(u);
                }
                verify(q);
            }
            cout << "AC " << seed << '\n';
        }
    }
    for (seed = 100000; seed < 100500; seed++)
    {
        mt19937_64 rng(seed);
        points = 1 + rng() % 7;
        input.clear();
        keys.clear();
        Steiner q(points);
        for (int i = 0, m = rng() % 13; i < m; i++)
        {
            int u = rng() % points, v = rng() % points, w = rng() % 8;
            if (seed % 4 == 0) w *= 1000000000LL;
            if (seed % 7 == 0) w = 0;
            input.push_back({u, v, w});
            q.add(u, v, w);
        }
        for (int u = 0; u < points; u++)
        {
            if (rng() & 1) keys.push_back(u);
        }
        shuffle(keys.begin(), keys.end(), rng);
        verify(q);
        verify(q);
        if (!keys.empty() && seed % 5 == 0)
        {
            keys.push_back(keys[0]);
            verify(q);
        }
        if (seed % 10 == 0)
        {
            int u = rng() % points, v = rng() % points, w = rng() % 8;
            input.push_back({u, v, w});
            q.add(u, v, w);
            verify(q);
        }
        cout << "AC " << seed << '\n';
    }
    seed = 200000;
    points = 4;
    input = {{0, 3, 1}, {1, 3, 1}, {2, 3, 1}, {0, 1, 10}, {1, 2, 10}};
    keys = {0, 1, 2};
    Steiner q(points);
    for (auto [u, v, w] : input)
    {
        q.add(u, v, w);
    }
    verify(q);
    check(q.solveTree(keys) == make_optional(pair<int, vector<int>>{3, {0, 1, 2}}));
    cout << "AC " << seed << '\n';
}
