#include "../include/full/5_3_013.hpp"
#include "../include/full/5_8_012.hpp"
#include "../include/full/6_1_011.hpp"

int bruteCut(int n, const vector<array<int, 3>> &e, const vector<int> &a)
{
    int ans = numeric_limits<int>::max();
    for (int s = 1; s < (1 << (n + 1)) - 1; s++)
    {
        int cur = 0;
        for (auto [u, v, w] : e) if (((s >> u) & 1) != ((s >> v) & 1)) cur += w;
        for (int i = 0; i < n; i++) if (((s >> i) & 1) != ((s >> n) & 1)) cur += a[i];
        ans = min(ans, cur);
    }
    return ans;
}

signed main()
{
    mt19937 rng(4);
    for (int tc = 0; tc < 100; tc++)
    {
        int n = rng() % 5 + 1;
        vector<array<int, 3>> e;
        for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) if (rng() & 1) e.push_back({i, j, (int)rng() % 7});
        vector<int> a(n);
        for (int &x : a) x = rng() % 7;
        DynStarCut cut(n, e, a);
        for (int z = 0; z < 10; z++)
        {
            int u = rng() % n, x = rng() % 7;
            a[u] = x;
            assert(cut.update(u, x) == bruteCut(n, e, a));
        }
    }
    auto t = commonIntTr({0, 1, 2, 3});
    assert(t.size() == 5 && t[0].l == 0 && t[0].r == 3 && t[0].linear);
    auto r = commonIntTr({3, 2, 1, 0});
    assert(r.size() == 5 && r[0].linear);
    for (int n = 1; n <= 8; n++)
    {
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        for (int it = 0; it < 300; it++)
        {
            shuffle(p.begin(), p.end(), rng);
            auto tr = commonIntTr(p);
            set<pair<int, int>> all, want, got;
            for (int l = 0; l < n; l++)
            {
                int mn = p[l], mx = p[l];
                for (int r = l; r < n; r++)
                {
                    mn = min(mn, p[r]);
                    mx = max(mx, p[r]);
                    if (mx - mn == r - l)
                    {
                        all.insert({l, r});
                    }
                }
            }
            for (auto x : all)
            {
                bool ok = true;
                for (auto y : all)
                {
                    bool cross = x.first < y.first && y.first <= x.second && x.second < y.second;
                    cross |= y.first < x.first && x.first <= y.second && y.second < x.second;
                    ok &= !cross;
                }
                if (ok)
                {
                    want.insert(x);
                }
            }
            for (auto x : tr)
            {
                got.insert({x.l, x.r});
            }
            assert(got == want && tr[0].l == 0 && tr[0].r == n - 1 && tr[0].parent == -1);
            for (int u = 0; u < (int)tr.size(); u++)
            {
                if (tr[u].parent != -1)
                {
                    assert(tr[u].parent < u);
                }
            }
        }
    }
    vector<vector<int>> g{{1, 1, 2, 2}, {0, 0, 2, 2}, {0, 0, 1, 1}};
    auto cc = threeEdgeCC(g);
    assert(cc.size() == 1 && cc[0].size() == 3);
}
