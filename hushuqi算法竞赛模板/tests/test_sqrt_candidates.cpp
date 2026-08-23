#include "../include/full/3_10_001.hpp"
#include "../include/full/3_10_002.hpp"
#include "../include/full/3_10_003.hpp"
#include "../include/full/3_10_004.hpp"

signed main()
{
    mt19937_64 rng(0);
    for (int tc = 0; tc < 400; tc++)
    {
        int n = rng() % 40;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = (int)(rng() % 101) - 50;
        }
        SqrtBlock x(a);
        OrderBlock y(a);
        for (int z = 0; z < 200; z++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            if (rng() & 1)
            {
                int v = (int)(rng() % 21) - 10;
                x.add(l, r, v);
                y.add(l, r, v);
                for (int i = l; i < r; i++)
                {
                    a[i] += v;
                }
                continue;
            }
            int lim = (int)(rng() % 121) - 60;
            int cnt = 0;
            for (int i = l; i < r; i++)
            {
                cnt += a[i] < lim;
            }
            assert(y.count(l, r, lim) == cnt);
            if (l < r)
            {
                int sm = 0, mn = a[l], mx = a[l];
                vector<int> b(a.begin() + l, a.begin() + r);
                for (int v : b)
                {
                    sm += v;
                    mn = min(mn, v);
                    mx = max(mx, v);
                }
                assert((x.query(l, r) == array<int, 3>{sm, mn, mx}));
                sort(b.begin(), b.end());
                int k = rng() % b.size();
                assert(y.kth(l, r, k, -300, 300) == b[k]);
                auto p = lower_bound(b.begin(), b.end(), lim);
                optional<int> pre = p == b.begin() ? nullopt : optional<int>(*prev(p));
                optional<int> nxt = p == b.end() ? nullopt : optional<int>(*p);
                assert(y.prev(l, r, lim) == pre);
                assert(y.next(l, r, lim) == nxt);
            }
        }
    }

    vector<vector<int>> g(13);
    for (int i = 1; i < 13; i++)
    {
        int p = (i - 1) / 2;
        g[p].push_back(i);
        g[i].push_back(p);
    }
    TreeBlock z(g, 0, 3);
    assert((int)z.bel.size() == 13 && !z.cap.empty());
    vector<int> cnt(z.cap.size());
    for (int k : z.bel)
    {
        assert(0 <= k && k < (int)z.cap.size());
        cnt[k]++;
    }
    for (int k = 0; k + 1 < (int)cnt.size(); k++)
    {
        assert(3 <= cnt[k] && cnt[k] < 6);
    }

    vector<array<int, 2>> edge;
    for (int i = 1; i < 20; i++)
    {
        edge.push_back({0, i});
    }
    for (int i = 1; i + 1 < 20; i++)
    {
        edge.push_back({i, i + 1});
    }
    vector<int> w(20);
    iota(w.begin(), w.end(), 0);
    DegreeBlock d(20, edge, w);
    assert(d.query(0) == 190);
    assert(d.query(7) == 6 + 8);
    d.add(7, 100);
    assert(d.query(0) == 290);
    assert(d.query(6) == 5 + 107);
    cout << "OK\n";
}
