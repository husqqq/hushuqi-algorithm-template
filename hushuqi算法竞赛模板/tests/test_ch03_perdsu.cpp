#include "../include/full/3_1_005.hpp"

signed main()
{
    PerDSU empty(0);
    assert(empty.build() == 0);

    PerDSU d(5);
    int r0 = d.build();
    int r1 = d.merge(r0, 0, 1);
    int r2 = d.merge(r1, 1, 2);
    int r3 = d.merge(r0, 3, 4);

    assert(d.size(r0, 0) == 1);
    assert(d.same(r1, 0, 1));
    assert(!d.same(r1, 0, 2));
    assert(d.same(r2, 0, 2));
    assert(d.size(r2, 1) == 3);
    assert(!d.same(r3, 0, 1));
    assert(d.same(r3, 3, 4));
    assert(d.merge(r2, 0, 2) == r2);
    assert(d.t.size() < 50);

    // 每次选择历史版本分叉，独立保存小规模连通块编号作参照。
    for (int seed = 0; seed < 80; seed++)
    {
        mt19937_64 rng(seed);
        int n = 1 + rng() % 40;
        PerDSU st(n);
        vector<int> root{st.build()};
        vector<vector<int>> state(1, vector<int>(n));
        iota(state[0].begin(), state[0].end(), 0);
        auto check = [&](int v, int x, int y)
        {
            bool same = state[v][x] == state[v][y];
            int size = count(state[v].begin(), state[v].end(), state[v][x]);
            return st.same(root[v], x, y) == same && st.size(root[v], x) == size;
        };
        for (int q = 0; q < 1200; q++)
        {
            int v = rng() % root.size();
            int x = rng() % n, y = rng() % n;
            auto next = state[v];
            int from = next[y], to = next[x];
            for (auto &z : next)
            {
                if (z == from) z = to;
            }
            auto old = st.t.size();
            int rt = st.merge(root[v], x, y);
            if (from == to)
            {
                assert(rt == root[v] && st.t.size() == old);
            }
            state.push_back(move(next));
            root.push_back(rt);
            int w = rng() % root.size();
            if (!check(v, x, y) || !check(w, x, y) || !check((int)root.size() - 1, x, y))
            {
                cout << "WA " << seed << '\n';
                return 1;
            }
        }
        cout << "AC " << seed << '\n';
    }
}
