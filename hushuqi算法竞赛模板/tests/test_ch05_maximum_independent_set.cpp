#include "../include/full/5_6_008.hpp"

void need(bool ok, int id)
{
    // ok 是必须成立的条件，id 是检查编号；失败时不依赖 assert。
    if (ok) return;
    cout << "WA " << id << '\n';
    exit(1);
}

int brute(int n, const vector<pair<int, int>> &edges)
{
    // n<=10，edges 允许重边和自环；返回最大独立集大小。
    vector<unsigned long long> ban(n);
    for (auto [u, v] : edges)
    {
        ban[u] |= 1ULL << v;
        ban[v] |= 1ULL << u;
    }
    int best = 0;
    for (unsigned long long s = 0; s < (1ULL << n); s++)
    {
        if (popcount(s) <= best) continue;
        bool ok = true;
        for (int u = 0; u < n; u++)
        {
            if ((s >> u & 1ULL) && (ban[u] & s))
            {
                ok = false;
                break;
            }
        }
        if (ok) best = popcount(s);
    }
    return best;
}

void check(int n, const vector<pair<int, int>> &edges, int optimum, int id)
{
    // 检查返回点集的范围、互异性、独立性和最优大小。
    auto ans = maxIS(n, edges);
    vector<bool> used(n);
    need((int)ans.size() == optimum, id);
    for (int u : ans)
    {
        need(0 <= u && u < n && !used[u], id);
        used[u] = true;
    }
    for (auto [u, v] : edges)
    {
        need(!(used[u] && used[v]), id);
    }
}

signed main()
{
    int id = 0;
    check(0, {}, 0, id++);

    // 穷举至多 6 点的全部简单图，与子集枚举对拍。
    for (int n = 1; n <= 6; n++)
    {
        vector<pair<int, int>> all;
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++) all.push_back({u, v});
        }
        for (unsigned long long mask = 0; mask < (1ULL << all.size()); mask++)
        {
            vector<pair<int, int>> edges;
            for (int i = 0; i < (int)all.size(); i++)
            {
                if (mask >> i & 1ULL) edges.push_back(all[i]);
            }
            check(n, edges, brute(n, edges), id++);
        }
    }

    // 穷举 4 点图的自环子集，并加入反向平行边。
    vector<pair<int, int>> pairs;
    for (int u = 0; u < 4; u++)
    {
        for (int v = u + 1; v < 4; v++) pairs.push_back({u, v});
    }
    for (int graph = 0; graph < (1 << pairs.size()); graph++)
    {
        for (int loops = 0; loops < 16; loops++)
        {
            vector<pair<int, int>> edges;
            for (int i = 0; i < (int)pairs.size(); i++)
            {
                if (!(graph >> i & 1)) continue;
                auto [u, v] = pairs[i];
                edges.push_back({u, v});
                edges.push_back({v, u});
            }
            for (int u = 0; u < 4; u++)
            {
                if (loops >> u & 1) edges.push_back({u, u});
            }
            check(4, edges, brute(4, edges), id++);
        }
    }

    mt19937 rng(712367821);
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 10 + 1;
        vector<pair<int, int>> edges;
        for (int u = 0; u < n; u++)
        {
            if (rng() % 7 == 0) edges.push_back({u, u});
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3) continue;
                edges.push_back({u, v});
                if (rng() % 4 == 0) edges.push_back({v, u});
            }
        }
        check(n, edges, brute(n, edges), id++);
        need(maxIS(n, edges) == maxIS(n, edges), id++);
    }

    check(64, {}, 64, id++);
    vector<pair<int, int>> complete, path, loops;
    for (int u = 0; u < 64; u++)
    {
        loops.push_back({u, u});
        if (u + 1 < 64) path.push_back({u, u + 1});
        for (int v = u + 1; v < 64; v++) complete.push_back({u, v});
    }
    check(64, complete, 1, id++);
    check(64, path, 32, id++);
    check(64, loops, 0, id++);
    cout << "AC " << id << '\n';
}
