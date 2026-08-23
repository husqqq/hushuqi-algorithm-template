#include "../include/full/10_3_006.hpp"

Z brute(const vector<int> &a, const vector<int> &b)
{
    int n = a.size();
    int m = *max_element(b.begin(), b.end());
    vector<Z> dp(m + 1), ndp(m + 1);
    for (int x = a[0]; x < b[0]; x++)
    {
        dp[x] = 1;
    }
    for (int i = 1; i < n; i++)
    {
        Z sum = 0;
        fill(ndp.begin(), ndp.end(), Z(0));
        for (int x = 0; x <= m; x++)
        {
            sum += dp[x];
            if (a[i] <= x && x < b[i])
            {
                ndp[x] = sum;
            }
        }
        dp.swap(ndp);
    }
    return accumulate(dp.begin(), dp.end(), Z(0));
}

signed main()
{
    assert(countIncSeq({0, 0}, {200, 200}) == Z(20100));
    assert(countIncSeq({0, 600000000}, {1, 1}) == Z(0));

    for (int n = 1; n <= 4; n++)
    {
        vector<int> a(n), b(n);
        auto dfs = [&](auto &&self, int i) -> void
        {
            if (i == n)
            {
                assert(countIncSeq(a, b) == brute(a, b));
                return;
            }
            for (int l = 0; l < 4; l++)
            {
                for (int r = l + 1; r <= 4; r++)
                {
                    a[i] = l;
                    b[i] = r;
                    self(self, i + 1);
                }
            }
        };
        dfs(dfs, 0);
    }

    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 10000; tc++)
    {
        int n = rng() % 9 + 1, m = rng() % 14 + 1;
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = rng() % m;
            b[i] = rng() % (m - a[i]) + a[i] + 1;
        }
        Z got = countIncSeq(a, b);
        Z want = brute(a, b);
        assert(got == want);
        agg = agg * 1000000007ULL + got.val();
    }
    cout << agg << '\n';
    return 0;
}
