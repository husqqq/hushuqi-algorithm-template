#ifndef LIS_HEADER
#define LIS_HEADER "../include/full/3_4_009.hpp"
#endif
#include LIS_HEADER

vector<int> input;
int ql = 0, qr = 0;
bool mode = true;

void check(bool ok)
{
    // ok 表示结果是否符合独立预期；失败时抛出异常，关闭 assert 后仍执行。
    if (!ok)
    {
        throw runtime_error("区间 LIS 结果不符");
    }
}

template <int LOG> void verify(StaticRngLIS<LOG> &st, const vector<int> &a, bool strict)
{
    // st 是复用的查询结构，a 是任意整数序列，strict 选择模式；用二次 DP 逐区间核对。
    input = a;
    mode = strict;
    st.init(a, strict);
    const auto &cs = st;
    int n = a.size();
    for (int l = 0; l <= n; l++)
    {
        ql = qr = l;
        check(cs.query(l, l) == 0);
        vector<int> dp(n);
        int best = 0;
        for (int i = l; i < n; i++)
        {
            dp[i] = 1;
            for (int j = l; j < i; j++)
            {
                if (strict ? a[j] < a[i] : a[j] <= a[i])
                {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            best = max(best, dp[i]);
            qr = i + 1;
            check(cs.query(l, i + 1) == best);
        }
    }
}

signed main()
{
    for (int seed = 0; seed < 16; seed++)
    {
        try
        {
            mt19937_64 rng(seed);
            StaticRngLIS<6> st;
            check(st.query(0, 0) == 0);
            for (int n : {0, 1, 2, 3, 7, 8, 15, 16, 31, 32, 63})
            {
                vector<int> a(n);
                iota(a.begin(), a.end(), 0);
                for (int shape = 0; shape < 5; shape++)
                {
                    if (shape == 1)
                    {
                        reverse(a.begin(), a.end());
                    }
                    else if (shape == 2)
                    {
                        shuffle(a.begin(), a.end(), rng);
                    }
                    else if (shape == 3)
                    {
                        fill(a.begin(), a.end(), 7);
                    }
                    else if (shape == 4)
                    {
                        for (int &x : a)
                        {
                            x = (int)(rng() % 9) - 4;
                        }
                    }
                    verify(st, a, true);
                    verify(st, a, false);
                }
            }
            vector<int> edge{numeric_limits<int>::max(), numeric_limits<int>::min(),
                             0, numeric_limits<int>::min(), 0, numeric_limits<int>::max()};
            verify(st, edge, true);
            verify(st, edge, false);
            verify(st, {}, false);
            verify(st, {2, 2, 1}, true);
            // init 省略模式必须恢复默认严格递增，不继承旧模式。
            st.init({1, 1}, false);
            check(st.query(0, 2) == 2);
            st.init({1, 1});
            check(st.query(0, 2) == 1);
            StaticRngLIS<1> one({numeric_limits<int>::min()});
            check(one.query(0, 1) == 1);
            StaticRngLIS<62> wide({3, 1, 1, 2}, false);
            check(wide.query(0, 4) == 3);
            if (!seed)
            {
                // 穷举短排列与三值序列，两种模式分别核对 DP。
                for (int n = 0; n <= 7; n++)
                {
                    vector<int> a(n);
                    iota(a.begin(), a.end(), 0);
                    do
                    {
                        verify(st, a, true);
                        verify(st, a, false);
                    } while (next_permutation(a.begin(), a.end()));
                    int total = 1;
                    for (int i = 0; i < n; i++)
                    {
                        total *= 3;
                    }
                    for (int mask = 0; mask < total; mask++)
                    {
                        int cur = mask;
                        for (int &x : a)
                        {
                            x = cur % 3 - 1;
                            cur /= 3;
                        }
                        verify(st, a, true);
                        verify(st, a, false);
                    }
                }
                int n = 100000;
                vector<int> a(n);
                iota(a.begin(), a.end(), 0);
                StaticRngLIS<> large(a);
                check(large.query(0, n) == n);
                reverse(a.begin(), a.end());
                large.init(a);
                check(large.query(0, n) == 1);
                // 官方 issue_1223 型：两段各自递增但整体有一次大幅下降。
                iota(a.begin(), a.end(), 0);
                rotate(a.begin(), a.begin() + n / 2, a.end());
                large.init(a);
                check(large.query(0, n) == n / 2);
                check(large.query(n / 2 - 10, n / 2 + 20) == 20);
                fill(a.begin(), a.end(), numeric_limits<int>::min());
                large.init(a, true);
                check(large.query(0, n) == 1);
                large.init(a, false);
                check(large.query(123, 98765) == 98642);
            }
            cout << "AC " << seed << '\n';
        }
        catch (const exception &)
        {
            cout << "WA " << seed << '\n';
            cerr << "strict=" << mode << " l=" << ql << " r=" << qr << '\n';
            for (int x : input)
            {
                cerr << x << ' ';
            }
            cerr << '\n';
            return 1;
        }
    }
}
