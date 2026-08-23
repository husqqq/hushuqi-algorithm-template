#include "../include/full/2_2_010.hpp"

int bestLisLength(const vector<int> &a)
{
    // a 是待验证序列；返回朴素动态规划求得的严格 LIS 最优长度。
    vector<int> dp(a.size(), 1);
    int ans = 0;
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (a[j] < a[i]) chmax(dp[i], dp[j] + 1);
        }
        chmax(ans, dp[i]);
    }
    return ans;
}

void checkLis(const vector<int> &a)
{
    // a 是待验证序列；断言 lis 返回一条下标递增、值严格递增的最优方案。
    auto ids = lis(a);
    assert((int)ids.size() == bestLisLength(a));
    for (int i = 0; i < (int)ids.size(); i++)
    {
        assert(0 <= ids[i] && ids[i] < (int)a.size());
        if (i) assert(ids[i - 1] < ids[i] && a[ids[i - 1]] < a[ids[i]]);
    }
}

signed main()
{
    checkLis({7});
    checkLis({5, 5, 5, 5});
    checkLis({5, 4, 3, 2, 1});
    checkLis({1, 2, 3, 4, 5});
    checkLis({3, 1, 2, 2, 4, 3, 5});

    mt19937 rng(20260821);
    for (int tc = 0; tc < 2000; tc++)
    {
        int n = rng() % 25 + 1;
        vector<int> a(n);
        for (int &x : a) x = rng() % 11;
        checkLis(a);
    }
    cout << "OK\n";
}
