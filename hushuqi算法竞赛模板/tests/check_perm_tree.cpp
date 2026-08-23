#include "../include/full/3_7_008.hpp"

signed main()
{
    mt19937 rng(7);
    for (int n = 1; n <= 9; n++)
    {
        vector<int> a(n);
        iota(a.begin(), a.end(), 0);
        for (int it = 0; it < 1000; it++)
        {
            shuffle(a.begin(), a.end(), rng);
            PermTree tr(a);
            set<pair<int, int>> all, want, got;
            for (int l = 0; l < n; l++)
            {
                int mn = a[l], mx = a[l];
                for (int r = l + 1; r <= n; r++)
                {
                    mn = min(mn, a[r - 1]);
                    mx = max(mx, a[r - 1]);
                    if (mx - mn == r - l - 1)
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
                    ok &= !(x.first < y.first && y.first < x.second && x.second < y.second);
                    ok &= !(y.first < x.first && x.first < y.second && y.second < x.second);
                }
                if (ok)
                {
                    want.insert(x);
                }
            }
            for (auto &x : tr.t)
            {
                got.insert({x.l, x.r});
            }
            assert(got == want);
            assert(tr.t[tr.root].l == 0 && tr.t[tr.root].r == n);
        }
    }
}
