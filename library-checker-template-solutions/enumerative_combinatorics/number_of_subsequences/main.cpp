

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

int distinctSub(const vector<long long> &a, int mod = 998244353)
{
    assert(mod > 0);
    unordered_map<long long, int> last;
    int sum = 1 % mod;
    for (long long x : a)
    {
        int old = last[x];
        int twice = sum >= mod - sum ? sum - (mod - sum) : sum + sum;
        int next = twice - old;
        if (next < 0)
        {
            next += mod;
        }
        last[x] = sum;
        sum = next;
    }
    return sum == 0 ? mod - 1 : sum - 1;
}

signed main()
{
    int n; cin >> n;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    cout << distinctSub(a) << '\n';
}
