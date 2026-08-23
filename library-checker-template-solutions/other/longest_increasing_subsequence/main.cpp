#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

template <class T> vector<int> lis(const vector<T> &a)
{
    // a 是原序列；返回一条严格上升子序列的原下标。
    int n = a.size();
    vector<T> v;
    vector<int> at, pre(n, -1);
    for (int i = 0; i < n; i++)
    {
        int p = lower_bound(v.begin(), v.end(), a[i]) - v.begin();
        if (p == (int)v.size())
        {
            v.push_back(a[i]);
            at.push_back(i);
        }
        else
        {
            v[p] = a[i];
            at[p] = i;
        }
        if (p)
        {
            pre[i] = at[p - 1];
        }
    }
    vector<int> ans;
    for (int x = at.empty() ? -1 : at.back(); x >= 0; x = pre[x])
    {
        ans.push_back(x);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto ans = lis(a);
    cout << ans.size() << '\n';
    for (int i = 0; i < (int)ans.size(); i++)
    {
        cout << ans[i] << " \n"[i + 1 == (int)ans.size()];
    }
}
