#include "../include/full/5_8_003.hpp"
#include "../include/full/5_8_004.hpp"
#include "../include/full/5_8_013.hpp"

using Clock = chrono::steady_clock;

struct FlowCase
{
    int n, s, t, lim;
    // edge[i][0..3] 依次是起点、终点、容量和单位费用。
    vector<array<int, 4>> edge;
};

template <class Flow>
pair<pair<int, int>, long long> runCase(const FlowCase &a)
{
    // a 是一张费用流测试图；返回答案与本次运行的微秒数。
    auto st = Clock::now();
    Flow g(a.n);
    for (auto [u, v, cap, cost] : a.edge)
    {
        g.add(u, v, cap, cost);
    }
    auto ans = g.flow(a.s, a.t, a.lim);
    auto us = chrono::duration_cast<chrono::microseconds>(Clock::now() - st).count();
    return {ans, us};
}

template <class Flow>
pair<pair<int, int>, long long> bench(const FlowCase &a, int rep)
{
    // a 是测试图，rep 是正式重复次数；预热一次后返回答案与耗时中位数。
    auto warm = runCase<Flow>(a).first;
    vector<long long> tm;
    for (int z = 0; z < rep; z++)
    {
        auto [ans, us] = runCase<Flow>(a);
        assert(ans == warm);
        tm.push_back(us);
    }
    ranges::sort(tm);
    return {warm, tm[rep / 2]};
}

FlowCase assignmentCase(int n, bool same, mt19937_64 &rng)
{
    // n 是方阵边长，same 表示费用是否全零；返回对应的完全二分图费用流网络。
    int s = 2 * n, t = s + 1;
    FlowCase a{t + 1, s, t, n, {}};
    for (int i = 0; i < n; i++)
    {
        a.edge.push_back({s, i, 1, 0});
        a.edge.push_back({n + i, t, 1, 0});
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int cost = same ? 0 : (int)(rng() % 1000000);
            a.edge.push_back({i, n + j, 1, cost});
        }
    }
    return a;
}

FlowCase layeredCase(int width)
{
    // width 是相邻层宽度；返回有大量同费用最短路的三层网络。
    int s = 2 * width, t = s + 1;
    FlowCase a{t + 1, s, t, width, {}};
    for (int i = 0; i < width; i++)
    {
        a.edge.push_back({s, i, 1, 0});
        a.edge.push_back({width + i, t, 1, 0});
        for (int z = 0; z < 8; z++)
        {
            a.edge.push_back({i, width + (i + z) % width, 1, 7});
        }
    }
    return a;
}

void report(const string &name, const FlowCase &a, int rep)
{
    // name 是场景名，a 是测试图，rep 是重复次数；核对答案并打印三版耗时中位数。
    auto [spfaAns, spfaUs] = bench<SpfaCostFlow<>>(a, rep);
    auto [dualAns, dualUs] = bench<CostFlow<>>(a, rep);
    auto [fastAns, fastUs] = bench<FastCostFlow<>>(a, rep);
    assert(spfaAns == dualAns && dualAns == fastAns);
    cout << name << " V=" << a.n << " E=" << a.edge.size()
         << " flow=" << spfaAns.first << " cost=" << spfaAns.second
         << " spfa_us=" << spfaUs << " dual_us=" << dualUs
         << " fast_us=" << fastUs << '\n';
}

signed main()
{
    mt19937_64 rng(0);
    report("equal_assignment", assignmentCase(120, true, rng), 5);
    report("random_assignment", assignmentCase(120, false, rng), 5);
    report("equal_layered", layeredCase(500), 7);
    return 0;
}
