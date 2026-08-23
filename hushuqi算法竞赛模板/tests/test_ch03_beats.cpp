#include "../include/full/3_3_011.hpp"

signed main()
{
    {
        vector<long long> a{
            5000000000000000000LL,
            -5000000000000000000LL,
            5000000000000000001LL,
            -5000000000000000001LL};
        SegBeats st(a);
        st.chmin(0, 1, 4500000000000000000LL);
        st.chmax(3, 4, -4500000000000000000LL);
        st.add(1, 3, 10);
        assert(st.sum(0, 4) == 21);
        assert(st.min(0, 4) == -4999999999999999990LL);
        assert(st.max(0, 4) == 5000000000000000011LL);
    }

    mt19937_64 rng(0);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        int n = 1 + rng() % 40;
        vector<long long> a(n);
        for (auto &x : a)
        {
            x = (long long)(rng() % 201) - 100;
        }
        SegBeats st(a);
        for (int q = 0; q < 200; q++)
        {
            int l = rng() % n, r = l + 1 + rng() % (n - l);
            long long x = (long long)(rng() % 201) - 100;
            int op = rng() % 4;
            if (!op)
            {
                st.add(l, r, x);
                for (int i = l; i < r; i++)
                {
                    a[i] += x;
                }
            }
            else if (op == 1)
            {
                st.chmin(l, r, x);
                for (int i = l; i < r; i++)
                {
                    a[i] = min(a[i], x);
                }
            }
            else if (op == 2)
            {
                st.chmax(l, r, x);
                for (int i = l; i < r; i++)
                {
                    a[i] = max(a[i], x);
                }
            }
            else
            {
                long long s = accumulate(a.begin() + l, a.begin() + r, 0LL);
                assert(st.sum(l, r) == s);
                assert(st.min(l, r) == *min_element(a.begin() + l, a.begin() + r));
                assert(st.max(l, r) == *max_element(a.begin() + l, a.begin() + r));
                acc = acc * 1'000'003 + (unsigned long long)s;
            }
        }
    }
    cout << acc << '\n';
    return 0;
}
