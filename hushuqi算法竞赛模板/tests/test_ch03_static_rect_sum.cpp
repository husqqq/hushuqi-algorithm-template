#include "../include/full/3_2_010.hpp"

signed main()
{
    struct Weight
    {
        // v 是用于断言的封装权值。
        long long v = 0;

        Weight &operator+=(const Weight &x)
        {
            // x 是要累加到当前权值的增量，返回更新后的当前对象。
            v += x.v;
            return *this;
        }

        Weight &operator-=(const Weight &x)
        {
            // x 是要从当前权值扣除的增量，返回更新后的当前对象。
            v -= x.v;
            return *this;
        }

    };

    StaticRect<long long> st;
    st.add(0, 0, 5);
    st.add(2, 1, 7);
    st.add(2, 3, -4);
    st.add(5, 1, 9);
    int q0 = st.query(0, 0, 3, 2);
    int q1 = st.query(2, 1, 6, 4);
    int q2 = st.query(0, 0, 0, 10);
    assert(q0 == 0 && q1 == 1 && q2 == 2);
    assert(st.work() == vector<long long>({12, 12, 0}));

    StaticRect<Weight> generic;
    generic.add(0, 0, {4});
    generic.add(1, 0, {7});
    generic.query(0, 0, 1, 1);
    assert(generic.work()[0].v == 4);

    mt19937_64 rng(0);
    for (int tc = 0; tc < 400; ++tc)
    {
        int n = rng() % 30;
        int m = rng() % 30;
        vector<array<long long, 3>> a(n);
        StaticRect<long long> cur;
        for (auto &[x, y, w] : a)
        {
            x = (long long)(rng() % 31) - 15;
            y = (long long)(rng() % 31) - 15;
            w = (long long)(rng() % 41) - 20;
            cur.add(x, y, w);
        }
        vector<array<long long, 4>> qs(m);
        vector<long long> want(m);
        for (int i = 0; i < m; ++i)
        {
            auto &[l, d, r, u] = qs[i];
            l = (long long)(rng() % 31) - 15;
            r = (long long)(rng() % 31) - 15;
            d = (long long)(rng() % 31) - 15;
            u = (long long)(rng() % 31) - 15;
            if (l > r)
            {
                swap(l, r);
            }
            if (d > u)
            {
                swap(d, u);
            }
            cur.query(l, d, r, u);
            for (auto [x, y, w] : a)
            {
                if (l <= x && x < r && d <= y && y < u)
                {
                    want[i] += w;
                }
            }
        }
        assert(cur.work() == want);
    }
    cout << "OK\n";
}
