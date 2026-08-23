#include "../include/full/3_4_009.hpp"

int slowLIS(const vector<int> &a, int l, int r)
{
    // a 是排列，l、r 是半开查询端点；用朴素 patience sorting 返回区间严格 LIS 长度。
    vector<int> tail;
    for (int i = l; i < r; i++)
    {
        auto it = lower_bound(tail.begin(), tail.end(), a[i]);
        if (it == tail.end())
        {
            tail.push_back(a[i]);
        }
        else
        {
            *it = a[i];
        }
    }
    return tail.size();
}

signed main()
{
    StaticRngLIS<5> empty;
    assert(empty.query(0, 0) == 0);

    vector<int> sample{0, 2, 1, 3};
    StaticRngLIS<5> fixed(sample);
    assert(fixed.query(0, 2) == 2);
    assert(fixed.query(0, 4) == 3);
    assert(fixed.query(1, 3) == 1);
    assert(fixed.query(2, 2) == 0);
    fixed.init({3, 2, 1, 0});
    assert(fixed.query(0, 4) == 1 && fixed.query(4, 4) == 0);

    mt19937_64 rng(20260819);
    for (int tc = 0; tc < 500; tc++)
    {
        int n = 1 + rng() % 31;
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        StaticRngLIS<5> st(p);
        for (int l = 0; l <= n; l++)
        {
            for (int r = l; r <= n; r++)
            {
                assert(st.query(l, r) == slowLIS(p, l, r));
            }
        }
    }

    int n = 100000;
    vector<int> monotone(n);
    iota(monotone.begin(), monotone.end(), 0);
    StaticRngLIS up(monotone);
    assert(up.query(0, n) == n);
    assert(up.query(123, 98765) == 98642);
    reverse(monotone.begin(), monotone.end());
    StaticRngLIS down(monotone);
    assert(down.query(0, n) == 1);
    assert(down.query(123, 98765) == 1);
    assert(down.query(n, n) == 0);

    n = 2000;
    vector<int> medium(n);
    iota(medium.begin(), medium.end(), 0);
    shuffle(medium.begin(), medium.end(), rng);
    StaticRngLIS mediumQuery(medium);
    for (int q = 0; q < 2000; q++)
    {
        int l = rng() % (n + 1), r = rng() % (n + 1);
        if (l > r)
        {
            swap(l, r);
        }
        assert(mediumQuery.query(l, r) == slowLIS(medium, l, r));
    }
    cout << "OK\n";
}
