#include "../include/full/5_5_011.hpp"

struct DSU
{
    vector<int> p, sz;
    DSU(int n) : p(n), sz(n, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool merge(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        p[y] = x;
        sz[x] += sz[y];
        return true;
    }
};

signed main()
{
    IncrMSF fixed(3);
    assert(fixed.replaceId(0, 1, 5, 0) == -1);
    assert(fixed.replaceId(1, 2, 4, 1) == -1);
    assert(fixed.replaceId(0, 2, 3, 2) == 0);
    assert(fixed.replaceId(0, 1, 6, 3) == 3);
    assert(fixed.weight() == 7);

    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 9 + 1;
        IncrMSF q(n);
        vector<array<long long, 3>> e;
        for (int rep = 0; rep < 80; rep++)
        {
            int u = rng() % n, v = rng() % n;
            long long w = (long long)(rng() % 201) - 100;
            q.add(u, v, w);
            e.push_back({u, v, w});
            auto a = e;
            sort(a.begin(), a.end(), [](auto x, auto y) { return x[2] < y[2]; });
            DSU d(n);
            long long want = 0;
            for (auto [x, y, z] : a)
            {
                if (d.merge(x, y))
                {
                    want += z;
                }
            }
            assert(q.weight() == want);
            agg = agg * 1000000007ULL + want + 100000;
        }
    }
    cout << agg << '\n';
    return 0;
}
