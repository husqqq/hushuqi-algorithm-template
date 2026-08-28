#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int mod = 998244353;

int power(int a, int b)
{
    // a、b 是模幂底数和非负指数；返回 a^b mod mod。
    int r = 1;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            r = r * a % mod;
        }
        a = a * a % mod;
        b >>= 1;
    }
    return r;
}

vector<int> detAz(vector<vector<int>> a, vector<vector<int>> b)
{
    // a、b 是同阶矩阵；返回 det(a+bz) 的低次到高次系数。
    int n = a.size();
    assert(n == (int)b.size());
    for (auto &v : a)
    {
        assert((int)v.size() == n);
        for (int &x : v)
        {
            x = (x % mod + mod) % mod;
        }
    }
    for (auto &v : b)
    {
        assert((int)v.size() == n);
        for (int &x : v)
        {
            x = (x % mod + mod) % mod;
        }
    }

    int z = 0, sc = 1;
    for (int c = 0; c < n; c++)
    {
        while (1)
        {
            for (int p = 0; p < c; p++)
            {
                int w = b[p][c];
                for (int i = 0; i < n; i++)
                {
                    a[i][c] = (a[i][c] - w * a[i][p]) % mod;
                }
                b[p][c] = 0;
            }
            int p = c;
            while (p < n && !b[p][c])
            {
                p++;
            }
            if (p < n)
            {
                break;
            }
            if (++z > n)
            {
                return vector<int>(n + 1);
            }
            for (int i = 0; i < n; i++)
            {
                b[i][c] = a[i][c];
                a[i][c] = 0;
            }
        }
        int p = c;
        while (p < n && !b[p][c])
        {
            p++;
        }
        if (p != c)
        {
            sc = mod - sc;
            swap(a[p], a[c]);
            swap(b[p], b[c]);
        }
        sc = sc * b[c][c] % mod;
        int iv = power(b[c][c], mod - 2);
        for (int j = 0; j < n; j++)
        {
            a[c][j] = a[c][j] * iv % mod;
            b[c][j] = b[c][j] * iv % mod;
        }
        for (int i = c + 1; i < n; i++)
        {
            int w = b[i][c];
            for (int j = 0; j < n; j++)
            {
                a[i][j] = (a[i][j] - w * a[c][j]) % mod;
                b[i][j] = (b[i][j] - w * b[c][j]) % mod;
            }
        }
    }
    for (auto &v : a)
    {
        for (int &x : v)
        {
            x = (-x) % mod;
        }
    }

    for (int c = 0; c + 1 < n; c++)
    {
        int p = c + 1;
        while (p < n && !a[p][c])
        {
            p++;
        }
        if (p == n)
        {
            continue;
        }
        swap(a[p], a[c + 1]);
        for (int i = 0; i < n; i++)
        {
            swap(a[i][p], a[i][c + 1]);
        }
        int iv = power(a[c + 1][c], mod - 2);
        for (int i = c + 2; i < n; i++)
        {
            int w = a[i][c] * iv % mod;
            for (int j = 0; j < n; j++)
            {
                a[i][j] = (a[i][j] - w * a[c + 1][j]) % mod;
                a[j][c + 1] = (a[j][c + 1] + w * a[j][i]) % mod;
            }
        }
    }

    vector<vector<int>> f(n + 1);
    f[0] = {1};
    for (int m = 1; m <= n; m++)
    {
        f[m].assign(m + 1, 0);
        for (int i = 0; i < m; i++)
        {
            int w = a[i][m - 1];
            for (int k = i + 1; k < m; k++)
            {
                w = w * a[k][k - 1] % mod;
            }
            for (int d = 0; d <= i; d++)
            {
                f[m][d] = (f[m][d] - w * f[i][d]) % mod;
            }
        }
        for (int d = 0; d < m; d++)
        {
            f[m][d + 1] = (f[m][d + 1] + f[m - 1][d]) % mod;
        }
    }
    vector<int> ans(n + 1);
    for (int d = 0; d + z <= n; d++)
    {
        ans[d] = f[n][d + z] * sc % mod;
    }
    for (int &x : ans)
    {
        x = (x + mod) % mod;
    }
    return ans;
}
