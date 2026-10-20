#include "../include/full/5_6_006.hpp"

void check(int nl, int nr, const vector<array<int, 2>> &e, const EdgeColor &ans, int seed = -1)
{
    // nl、nr 是左右部点数，e 按顺序保存两端点，ans 是待检查染色，seed 是构造编号。
    // 失败立即输出编号并保存输入；检查不受 NDEBUG 影响。
    auto need = [&](bool ok)
    {
        // ok 是必须成立的条件；失败保存当前图并终止，无返回值。
        if (ok) return;
        cout << "WA " << seed << endl;
        auto path = filesystem::temp_directory_path() / "bipartite_edge_coloring-failed.in";
        ofstream out(path);
        out << nl << ' ' << nr << ' ' << e.size() << '\n';
        for (auto [u, v] : e)
        {
            out << u << ' ' << v << '\n';
        }
        out.close();
        exit(1);
    };
    vector<int> dl(nl), dr(nr);
    int k = 0;
    for (auto [u, v] : e)
    {
        k = max(k, ++dl[u]);
        k = max(k, ++dr[v]);
    }
    need(ans.k == k && ans.c.size() == e.size());
    vector<set<int>> l(nl), r(nr);
    for (int i = 0; i < (int)e.size(); i++)
    {
        auto [u, v] = e[i];
        need(0 <= ans.c[i] && ans.c[i] < ans.k);
        need(l[u].insert(ans.c[i]).second);
        need(r[v].insert(ans.c[i]).second);
    }
    if (seed >= 0) cout << "AC " << seed << '\n';
}

signed main()
{
    BipEdgeColor empty(0, 0);
    check(0, 0, {}, empty.work());

    BipEdgeColor a(5, 5);
    // 每个元素依次为左、右端点。
    vector<array<int, 2>> e{{0, 0}, {0, 1}, {1, 0}, {2, 2}};
    for (auto [u, v] : e)
    {
        a.add(u, v);
    }
    check(5, 5, e, a.work());
    check(5, 5, e, a.work());

    for (int i = 0; i < 3; i++)
    {
        a.add(4, 4);
        e.push_back({4, 4});
    }
    check(5, 5, e, a.work());

    for (int mask = 0; mask < 512; mask++)
    {
        BipEdgeColor g(3, 3);
        vector<array<int, 2>> es;
        for (int i = 0; i < 9; i++)
        {
            if (!(mask >> i & 1)) continue;
            g.add(i / 3, i % 3);
            es.push_back({i / 3, i % 3});
        }
        check(3, 3, es, g.work(), mask);
    }
    for (int mask = 0; mask < 256; mask++)
    {
        BipEdgeColor g(2, 2);
        vector<array<int, 2>> es;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < (mask >> (2 * i) & 3); j++)
            {
                g.add(i / 2, i % 2);
                es.push_back({i / 2, i % 2});
            }
        }
        check(2, 2, es, g.work(), 512 + mask);
    }
    mt19937 rng(712367821);
    for (int tc = 0; tc < 400; tc++)
    {
        int nl = rng() % 7 + 1, nr = rng() % 7 + 1;
        int cnt = rng() % 31;
        BipEdgeColor g(nl, nr);
        vector<array<int, 2>> es;
        for (int i = 0; i < cnt; i++)
        {
            int u = rng() % nl, v = rng() % nr;
            g.add(u, v);
            es.push_back({u, v});
        }
        check(nl, nr, es, g.work(), 768 + tc);
    }

    BipEdgeColor star(100000, 100000);
    for (int i = 0; i < 100000; i++)
    {
        star.add(0, i);
    }
    auto y = star.work();
    if (y.k != 100000 || (int)y.c.size() != 100000)
    {
        cout << "WA 1168\n";
        return 1;
    }
    sort(y.c.begin(), y.c.end());
    for (int i = 0; i < 100000; i++)
    {
        if (y.c[i] != i)
        {
            cout << "WA 1168\n";
            return 1;
        }
    }
    cout << "AC 1168\n";
}
