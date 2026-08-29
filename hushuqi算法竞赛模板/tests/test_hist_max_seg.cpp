#include "../include/full/3_3_003.hpp"

signed main()
{
    mt19937 rng(654321);
    for (int n = 1; n <= 25; n++)
    {
        vector<long long> a(n), hist(n);
        for (auto &x : a) x = (int)rng() % 21 - 10;
        hist = a;
        HistMaxSeg st(a);
        for (int it = 0; it < 300; it++)
        {
            int l = rng() % n, r = l + 1 + rng() % (n - l), v = (int)rng() % 21 - 10;
            bool add = rng() & 1;
            if (add) st.add(l, r, v); else st.assign(l, r, v);
            for (int i = l; i < r; i++)
            {
                if (add) a[i] += v; else a[i] = v;
                hist[i] = max(hist[i], a[i]);
            }
            int ql = rng() % n, qr = ql + 1 + rng() % (n - ql);
            auto got = st.query(ql, qr);
            assert(got.cur == *max_element(a.begin() + ql, a.begin() + qr));
            assert(got.hist == *max_element(hist.begin() + ql, hist.begin() + qr));
        }
    }
    cout << "OK\n";
}
