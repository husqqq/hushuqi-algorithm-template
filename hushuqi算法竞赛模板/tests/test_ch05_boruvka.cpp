#include "../include/full/5_5_014.hpp"

signed main()
{
    assert(boruvka(0, {})->first == 0);
    assert(boruvka(1, {})->first == 0);
    assert(!boruvka(3, {{0, 1, 2}}).has_value());
    auto fixed = boruvka(4, {{0, 1, 4}, {1, 2, -2}, {2, 3, 5},
                             {0, 3, 10}, {0, 2, 1}});
    assert(fixed.has_value() && fixed->first == 4 && fixed->second.size() == 3);

    mt19937_64 rng(20260903);
    for (int t = 0; t < 3000; t++)
    {
        int n = rng() % 20 + 1;
        vector<MEdge> e;
        for (int i = 1; i < n; i++)
        {
            e.push_back({i, (int)(rng() % i), (int)(rng() % 101) - 50});
        }
        for (int i = 0; i < n * n / 2; i++)
        {
            int x = rng() % n, y = rng() % n;
            e.push_back({x, y, (int)(rng() % 101) - 50});
        }
        auto x = boruvka(n, e);
        auto y = kruskal(n, e);
        assert(x.has_value() && y.has_value() && x->first == y->first);
        assert(x->second.size() == n - 1);
    }
    cout << "OK\n";
}
