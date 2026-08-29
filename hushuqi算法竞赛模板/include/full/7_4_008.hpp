#pragma once

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

struct DLogHash
{
    vector<unsigned long long> tab;

    DLogHash(int n) : tab(max<long long>(3, n * 10 / 7 + 3))
    {
    }

    unsigned long long hash(unsigned long long x) const
    {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void insert(long long key, long long val)
    {
        int p = hash(key) % tab.size();
        while (tab[p] && (tab[p] >> 32) != (unsigned long long)key)
        {
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        if (!tab[p] || val < (long long)(tab[p] & 0xffffffffULL))
        {
            tab[p] = ((unsigned long long)key << 32) | val;
        }
    }

    int find(long long key) const
    {
        int p = hash(key) % tab.size();
        while (tab[p])
        {
            if ((tab[p] >> 32) == (unsigned long long)key)
            {
                return (int)(tab[p] & 0xffffffffULL);
            }
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        return -1;
    }
};

vector<int> batchDLog(int p, int g, const vector<int> &a)
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
    DLogHash tab(lim);
    long long step = powMod64(g, B, p), cur = step;
    for (int x = 1; x <= lim; x++)
    {
        tab.insert(cur, x);
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
            int x = tab.find(a[i] * gy % p);
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
