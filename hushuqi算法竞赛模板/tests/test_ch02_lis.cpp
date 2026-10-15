#include "../include/full/2_2_010.hpp"

template <class T> int bestLisLength(const vector<T> &a)
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

template <class T> void checkLis(const vector<T> &a)
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
    checkLis(vector<int>{});
    checkLis(vector<int>{7});
    checkLis(vector<int>{5, 5, 5, 5});
    checkLis(vector<int>{5, 4, 3, 2, 1});
    checkLis(vector<int>{1, 2, 3, 4, 5});
    checkLis(vector<int>{3, 1, 2, 2, 4, 3, 5});
    checkLis(vector<int>{LLONG_MIN, LLONG_MAX, LLONG_MIN, 0, LLONG_MAX});
    checkLis(vector<string>{"b", "a", "c", "c", "d"});
    struct Item
    {
        int x; // 比较键。
        Item() = delete;
        explicit Item(int x) : x(x) {}
        bool operator<(const Item &other) const
        {
            // other 是另一元素；返回当前键是否更小。
            return x < other.x;
        }
    };
    checkLis(vector<Item>{Item(3), Item(1), Item(2), Item(2), Item(4)});

    mt19937 rng(20260821);
    for (int tc = 0; tc < 2000; tc++)
    {
        int n = rng() % 25 + 1;
        vector<int> a(n);
        for (int &x : a)
        {
            x = rng() % 11;
        }
        checkLis(a);
    }
    cout << "OK\n";
}
