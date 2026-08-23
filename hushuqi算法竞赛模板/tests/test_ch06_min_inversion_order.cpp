#include "../include/full/6_5_005.hpp"

__int128 value(const vector<int> &p, const vector<long long> &c, const vector<long long> &d)
{
    __int128 ans = 0, sum = 0;
    for (int u : p)
    {
        ans += sum * c[u];
        sum += d[u];
    }
    return ans;
}

__int128 brute(const vector<int> &par, const vector<long long> &c, const vector<long long> &d)
{
    int n = par.size();
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    __int128 ans = -1;
    do
    {
        vector<int> at(n);
        for (int i = 0; i < n; i++)
        {
            at[p[i]] = i;
        }
        bool ok = true;
        for (int u = 0; u < n; u++)
        {
            if (par[u] != -1 && at[par[u]] > at[u])
            {
                ok = false;
            }
        }
        if (ok)
        {
            __int128 x = value(p, c, d);
            if (ans == -1 || x < ans)
            {
                ans = x;
            }
        }
    }
    while (next_permutation(p.begin(), p.end()));
    return ans;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 1200; tc++)
    {
        int n = rng() % 8 + 1;
        vector<int> par(n, -1);
        vector<long long> c(n), d(n);
        for (int u = 1; u < n; u++)
        {
            par[u] = rng() % u;
        }
        for (int u = 0; u < n; u++)
        {
            c[u] = rng() % 5;
            d[u] = rng() % 5;
        }
        auto p = minInvOrder(par, c, d);
        assert(value(p, c, d) == brute(par, c, d));
        agg = agg * 1000000007ULL + (unsigned long long)value(p, c, d);
    }
    long long big = numeric_limits<long long>::max() / 2 + 10;
    vector<int> par{-1, 0, 1, 0};
    vector<long long> c{0, big, big, 1}, d{1, 1, 1, 1};
    auto p = minInvOrder(par, c, d);
    assert(value(p, c, d) == brute(par, c, d));
    cout << agg << '\n';
    return 0;
}
