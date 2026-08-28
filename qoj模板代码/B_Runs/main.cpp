// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: B Runs

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

struct Run
{
    // p 是最小周期，[l,r) 是极大周期区间；32 位字段用于降低百万规模候选的内存占用。
    int32_t p, l, r;
    auto operator<=>(const Run &) const = default;
};

struct RunHash
{
    // a 是 1 下标字符串值；h、pw 用于单模哈希 LCE。
    int32_t n;
    vector<int32_t> a, h, pw;

    explicit RunHash(const string &s) : n(s.size()), a(n + 1), h(n + 1), pw(n + 1, 1)
    {
        for (int32_t i = 1; i <= n; i++)
        {
            a[i] = s[i - 1] - 'a' + 1;
        }
        build();
    }

    void build()
    {
        // 根据当前 a 建立前缀哈希。
        constexpr int32_t mod = 998244353, bs = 31;
        h[0] = 0;
        pw[0] = 1;
        for (int32_t i = 1; i <= n; i++)
        {
            pw[i] = 1LL * pw[i - 1] * bs % mod;
            h[i] = (1LL * h[i - 1] * bs + a[i]) % mod;
        }
    }

    int32_t get(int32_t l, int32_t len) const
    {
        // 返回 1 下标区间 a[l..l+len) 的哈希值。
        constexpr int32_t mod = 998244353;
        return (h[l + len - 1] - 1LL * h[l - 1] * pw[len] % mod + mod) % mod;
    }

    int32_t lcp(int32_t x, int32_t y, int32_t up) const
    {
        // 返回从 x、y 开始的最长公共前缀，长度不超过 up。
        if (x > n || y > n || a[x] != a[y])
        {
            return 0;
        }
        int32_t l = 0, r = min({up, n - x + 1, n - y + 1});
        while (l < r)
        {
            int32_t m = (l + r + 1) / 2;
            if (get(x, m) == get(y, m))
            {
                l = m;
            }
            else
            {
                r = m - 1;
            }
        }
        return l;
    }

    int32_t lcs(int32_t x, int32_t y, int32_t up) const
    {
        // 返回以 x、y 结尾的最长公共后缀，长度不超过 up。
        if (x <= 0 || y <= 0 || a[x] != a[y])
        {
            return 0;
        }
        int32_t l = 0, r = min({up, x, y});
        while (l < r)
        {
            int32_t m = (l + r + 1) / 2;
            if (get(x - m + 1, m) == get(y - m + 1, m))
            {
                l = m;
            }
            else
            {
                r = m - 1;
            }
        }
        return l;
    }

    void flip()
    {
        // 互补字符值，枚举另一种 Lyndon 方向。
        for (int32_t i = 1; i <= n; i++)
        {
            a[i] = 27 - a[i];
        }
        build();
    }

    void getRuns(vector<Run> &ans)
    {
        // 用 Lyndon 单调栈枚举当前方向的全部 Runs。
        vector<int32_t> st(n + 1);
        st[0] = n + 1;
        for (int32_t i = n, top = 0, lt = 0; i >= 1; i--)
        {
            while (top)
            {
                int32_t x = min(st[top] - i, st[top - 1] - st[top]);
                lt = lcp(i, st[top], x);
                if ((lt == x && st[top] - i < st[top - 1] - st[top]) ||
                    (lt < x && a[i + lt] < a[st[top] + lt]))
                {
                    top--;
                    lt = 0;
                }
                else
                {
                    break;
                }
            }
            int32_t j = st[top];
            st[++top] = i;
            int32_t x = lcs(i - 1, j - 1, j - i), y;
            if (x < j - i)
            {
                y = lt + lcp(i + lt, j + lt, n);
                if (x + y >= j - i)
                {
                    ans.push_back({j - i, i - x - 1, j + y - 1});
                }
            }
        }
    }
};

vector<Run> runs(const string &s)
{
    // s 是原字符串；返回全部极大周期区间及其最小周期。哈希碰撞概率极低。
    RunHash e(s);
    vector<Run> ans;
    e.getRuns(ans);
    e.flip();
    e.getRuns(ans);
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    auto ans = runs(s);
    sort(ans.begin(), ans.end(), [](const Run &x, const Run &y)
    {
        return tie(x.l, x.r, x.p) < tie(y.l, y.r, y.p);
    });
    cout << ans.size() << '\n';
    for (auto x : ans)
    {
        cout << x.l + 1 << ' ' << x.r << ' ' << x.p << '\n';
    }
}
