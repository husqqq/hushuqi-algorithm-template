#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_2_008.hpp"
#endif
#include TEST_HEADER

int seed = -1, points = 0;
vector<array<int, 3>> input;
vector<optional<__int128>> initial;

void check(bool ok)
{
    // ok 是验证结果；失败时保存种子与图，初值由该种子的固定构造恢复，NDEBUG 不关闭检查。
    if (ok) return;
    cout << "WA " << seed << '\n';
    cerr << points << ' ' << input.size() << '\n';
    for (auto [u, v, w] : input)
    {
        cerr << u << ' ' << v << ' ' << w << '\n';
    }
    exit(1);
}

vector<optional<__int128>> brute(vector<optional<__int128>> d)
{
    // d 是任意初值；每轮只用上一轮状态枚举一条边，返回至多 n-1 条边的最小代价。
    for (int step = 1; step < points; step++)
    {
        auto next = d;
        for (auto [u, v, w] : input)
        {
            if (!d[u]) continue;
            __int128 nd = *d[u] + w;
            if (!next[v] || nd < *next[v]) next[v] = nd;
        }
        d = move(next);
    }
    return d;
}

void verify()
{
    // 无参数；独立核对多源、单源、稠密版与最短路径树，并检查回调语义及零权父指针无环。
    vector<vector<array<int, 3>>> g(points);
    vector<vector<WEdge>> simple(points);
    vector mat(points, vector<optional<int>>(points));
    for (int u = 0; u < points; u++)
    {
        mat[u][u] = 0;
    }
    for (int id = 0; id < (int)input.size(); id++)
    {
        auto [u, v, w] = input[id];
        g[u].push_back({v, w, id});
        simple[u].push_back({v, w});
        if (!mat[u][v] || w < *mat[u][v]) mat[u][v] = w;
    }
    auto expected = brute(initial), d = initial, last = initial;
    vector<int> par(points, -1), edgeId(points, -1);
    int calls = 0;
    dijkstraInit(g, d, [&](int u, const array<int, 3> &edge)
    {
        // u 是前驱，edge 依次为终点、权值、原边编号；检查回调在距离更新后且严格改善时触发。
        auto [v, w, id] = edge;
        check((input[id] == array<int, 3>{u, v, w}));
        check(d[u] && d[v] && *d[v] == *d[u] + w);
        if (last[v]) check(*d[v] < *last[v]);
        last[v] = d[v];
        par[v] = u;
        edgeId[v] = id;
        calls++;
    });
    check(d == expected && last == d && calls <= (int)input.size());
    for (int v = 0; v < points; v++)
    {
        if (par[v] != -1)
        {
            check(d[v] && d[par[v]] && *d[v] == *d[par[v]] + input[edgeId[v]][2]);
        }
        int u = v;
        for (int step = 0; par[u] != -1; step++)
        {
            check(step < points);
            u = par[u];
        }
        if (d[v]) check(initial[u] && d[u] == initial[u]);
    }
    if (!points) return;
    int s = seed < 0 ? 0 : seed % points;
    vector<optional<__int128>> start(points);
    start[s] = 0;
    expected = brute(start);
    check(dijkstraSp<__int128>(simple, s) == expected);
    check(dijkstraDn<__int128>(mat, s) == expected);
    auto [dist, parent] = shortestTree<__int128>(simple, s);
    check(dist == expected && parent[s] == -1);
    for (int v = 0; v < points; v++)
    {
        if (!dist[v])
        {
            check(parent[v] == -1);
            continue;
        }
        int u = v;
        for (int step = 0; u != s; step++)
        {
            check(step < points && 0 <= parent[u] && parent[u] < points);
            bool found = false;
            for (auto [a, b, w] : input)
            {
                if (a == parent[u] && b == u && dist[a] && *dist[a] + w == *dist[u]) found = true;
            }
            check(found);
            u = parent[u];
        }
    }
}

signed main()
{
    // 无输入；1000 组固定种子含负初值、零权环、重边、自环、空图和全空初值。
    for (seed = 0; seed < 1000; seed++)
    {
        mt19937_64 rng(seed);
        points = rng() % 9;
        input.clear();
        initial.assign(points, nullopt);
        if (points)
        {
            for (int i = 0, m = rng() % 51; i < m; i++)
            {
                int u = rng() % points, v = rng() % points, w = rng() % 9;
                if (seed % 7 == 0) w = 0;
                input.push_back({u, v, w});
            }
            for (int u = 0; u < points; u++)
            {
                if (seed % 11 && rng() % 3 == 0) initial[u] = (int)(rng() % 41) - 20;
            }
        }
        verify();
        cout << "AC " << seed << '\n';
    }
    seed = 100000;
    points = 3;
    input = {{0, 1, LLONG_MAX}, {1, 2, LLONG_MAX}};
    initial = {0, nullopt, nullopt};
    verify();
    check(brute(initial)[2] == optional<__int128>((__int128)LLONG_MAX * 2));
    cout << "AC " << seed << '\n';
}
