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
    // p 是最小周期，[l,r) 是极大周期区间。
    int32_t p, l, r;
    auto operator<=>(const Run &) const = default;
};

using RunIndex = int32_t;

vector<RunIndex> runZ(const vector<RunIndex> &a)
{
    // a 是整数序列；返回带一个尾哨兵的 Z 数组，供 Runs 计算 LCE。
    RunIndex n = a.size();
    vector<RunIndex> z(n + 1);
    for (RunIndex i = 1, l = 0; i <= n; i++)
    {
        z[i] = l + z[l] <= i ? 0 : min(l + z[l] - i, z[i - l]);
        while (i + z[i] < n && a[z[i]] == a[i + z[i]])
        {
            z[i]++;
        }
        if (l + z[l] < i + z[i])
        {
            l = i;
        }
    }
    return z;
}

struct RunEnum
{
    // all[p] 收集最小周期候选为 p 的极大区间。
    vector<vector<pair<RunIndex, RunIndex>>> all;
    vector<RunIndex> a;

    vector<RunIndex> rev(vector<RunIndex> v)
    {
        reverse(v.begin(), v.end());
        return v;
    }

    vector<RunIndex> sub(RunIndex l, RunIndex r)
    {
        return {a.begin() + l, a.begin() + r};
    }

    void solve(RunIndex l, RunIndex r, RunIndex rounding)
    {
        if (r - l <= 1)
        {
            return;
        }
        RunIndex m = (l + r + rounding) / 2;
        solve(l, m, rounding);
        solve(m, r, rounding);
        auto zl = runZ(rev(sub(l, m)));
        auto y = sub(m, r), suffix = sub(l, r);
        y.insert(y.end(), suffix.begin(), suffix.end());
        auto zr = runZ(y);
        for (RunIndex i = m - 1; i >= l; i--)
        {
            RunIndex le = min(i - l, zl[m - i]);
            RunIndex ri = min(r - m, zr[r - l - (m - i)]);
            RunIndex p = m - i, ql = i - le, qr = m + ri;
            if (qr - ql >= 2 * p)
            {
                all[p].push_back({ql, qr});
            }
        }
    }

    explicit RunEnum(const string &s) : all(s.size() / 2 + 1), a(s.begin(), s.end())
    {
        RunIndex n = a.size();
        reverse(a.begin(), a.end());
        solve(0, n, 0);
        for (auto &v : all)
        {
            for (auto &[l, r] : v)
            {
                tie(l, r) = pair{n - r, n - l};
            }
        }
        reverse(a.begin(), a.end());
        solve(0, n, 1);
    }
};

vector<Run> runs(const string &s)
{
    // s 是原字符串；返回全部极大周期区间及其最小周期。
    RunEnum e(s);
    set<pair<RunIndex, RunIndex>> seen;
    vector<Run> ans;
    for (RunIndex p = 1; p < (RunIndex)e.all.size(); p++)
    {
        auto &v = e.all[p];
        sort(v.begin(), v.end(), [](auto x, auto y)
        {
            return x.first != y.first ? x.first < y.first : x.second > y.second;
        });
        vector<pair<RunIndex, RunIndex>> filtered;
        for (auto [l, r] : v)
        {
            if (filtered.empty() || filtered.back().second < r)
            {
                filtered.push_back({l, r});
            }
        }
        for (auto [l, r] : filtered)
        {
            if (seen.insert({l, r}).second)
            {
                ans.push_back({p, l, r});
            }
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    string s; cin >> s; auto a = runs(s); cout << a.size() << '\n';
    for (auto x : a) cout << x.p << ' ' << x.l << ' ' << x.r << '\n';
}
