

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template<class T>
bool chmin(T &a, const T &b)
{
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template<class T>
bool chmax(T &a, const T &b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

int addMod(int a, int b, int mod)
{
    return a >= mod - b ? a - (mod - b) : a + b;
}

vector<int> zeroOnePack(int m, const vector<pair<int, int>> &a)
{
    assert(m >= 0);
    vector<int> f(m + 1, -INF);
    f[0] = 0;
    for (auto [w, v] : a)
    {
        assert(w > 0);
        for (int j = m; j >= w; j--)
        {
            if (f[j - w] != -INF)
            {
                chmax(f[j], f[j - w] + v);
            }
        }
    }
    return f;
}

template<class T>
vector<T> lis(const vector<T> &a)
{
    int n = a.size();
    vector<T> tail;
    vector<int> at;
    vector<int> pre(n, -1);
    for (int i = 0; i < n; i++)
    {
        int k = lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin();
        if (k > 0)
        {
            pre[i] = at[k - 1];
        }
        if (k == (int)tail.size())
        {
            tail.push_back(a[i]);
            at.push_back(i);
        }
        else
        {
            tail[k] = a[i];
            at[k] = i;
        }
    }
    if (at.empty())
    {
        return {};
    }
    vector<T> ans;
    for (int x = at.back(); x != -1; x = pre[x])
    {
        ans.push_back(a[x]);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n; vector<int> a(n);
    for (auto &x : a) cin >> x;
    auto value = lis(a);
    vector<int> answer;
    int p = 0;
    for (int x : value)
    {
        while (a[p] != x) ++p;
        answer.push_back(p++);
    }
    cout << answer.size() << '\n';
    for (int i = 0; i < (int)answer.size(); ++i)
        cout << answer[i] << " \n"[i + 1 == (int)answer.size()];
}
