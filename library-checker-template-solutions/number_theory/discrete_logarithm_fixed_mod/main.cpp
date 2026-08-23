
#include <bits/stdc++.h>
using namespace std;
#define int long long

long long powMod64(long long a, long long n, long long mod)
{
    long long r = 1;
    while (n)
    {
        if (n & 1)
        {
            r = r * a % mod;
        }
        a = a * a % mod;
        n >>= 1;
    }
    return r;
}

struct DiscreteLogHash
{
    vector<unsigned long long> tab;

    DiscreteLogHash(int n) : tab(max<long long>(3, n * 10 / 7 + 3))
    {
    }

    unsigned long long hash(unsigned long long x) const
    {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void insert(uint32_t key, uint32_t val)
    {
        int p = hash(key) % tab.size();
        while (tab[p] && (uint32_t)(tab[p] >> 32) != key)
        {
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        if (!tab[p] || val < (uint32_t)tab[p])
        {
            tab[p] = ((unsigned long long)key << 32) | val;
        }
    }

    int find(uint32_t key) const
    {
        int p = hash(key) % tab.size();
        while (tab[p])
        {
            if ((uint32_t)(tab[p] >> 32) == key)
            {
                return (uint32_t)tab[p];
            }
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        return -1;
    }
};

vector<int> batchDiscreteLog(int p, int g, const vector<int> &a)
{
    assert(p >= 2 && 1 <= g && g < p);
    int qn = a.size();
    vector<int> ans(qn, -1);
    if (qn == 0)
    {
        return ans;
    }
    if (p == 2)
    {
        fill(ans.begin(), ans.end(), 0);
        return ans;
    }
    int B = sqrtl((long double)(p - 1) / max<long long>(1, qn)) + 1;
    int lim = (p - 2 + B - 1) / B;
    DiscreteLogHash tab(lim);
    long long step = powMod64(g, B, p), cur = step;
    for (int x = 1; x <= lim; x++)
    {
        tab.insert((uint32_t)cur, (uint32_t)x);
        cur = cur * step % p;
    }
    long long gy = 1;
    for (int y = 0; y < B; y++)
    {
        for (int i = 0; i < qn; i++)
        {
            assert(1 <= a[i] && a[i] < p);
            if (a[i] == 1)
            {
                ans[i] = 0;
                continue;
            }
            int x = tab.find((uint32_t)(a[i] * gy % p));
            if (x != -1)
            {
                int e = (x * B - y) % (p - 1);
                if (e < 0)
                {
                    e += p - 1;
                }
                if (ans[i] == -1 || e < ans[i])
                {
                    ans[i] = e;
                }
            }
        }
        gy = gy * g % p;
    }
    return ans;
}

signed main()
{
    int p, g, n; cin >> p >> g >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto ans = batchDiscreteLog(p, g, a);
    for (int x : ans) cout << x << '\n';
}
