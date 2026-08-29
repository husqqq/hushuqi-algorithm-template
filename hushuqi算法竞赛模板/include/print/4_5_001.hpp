#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> vector<T> lcs(const vector<T> &a, const vector<T> &b)
{
    // a、b 是两个序列；返回二者的一条最长公共子序列。
    int n = a.size(), m = b.size();
    vector<vector<int>> f(n + 1, vector<int>(m + 1));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (a[i] == b[j])
            {
                f[i + 1][j + 1] = f[i][j] + 1;
            }
            else
            {
                f[i + 1][j + 1] = max(f[i][j + 1], f[i + 1][j]);
            }
        }
    }
    vector<T> c;
    for (int i = n, j = m; i && j;)
    {
        if (a[i - 1] == b[j - 1])
        {
            i--;
            j--;
            c.push_back(a[i]);
        }
        else if (f[i - 1][j] >= f[i][j - 1])
        {
            i--;
        }
        else
        {
            j--;
        }
    }
    reverse(c.begin(), c.end());
    return c;
}

int lcsBitset(const string &a, const string &b)
{
    // a、b 是两个字节串；返回二者的最长公共子序列长度。
    int w = (b.size() + 63) / 64;
    vector<vector<unsigned long long>> mask(256, vector<unsigned long long>(w));
    vector<unsigned long long> s(w), x(w), z(w);
    for (int i = 0; i < (int)b.size(); i++)
    {
        mask[(unsigned char)b[i]][i / 64] |= 1ULL << (i % 64);
    }
    for (unsigned char c : a)
    {
        unsigned long long shift = 1, borrow = 0;
        for (int i = 0; i < w; i++)
        {
            x[i] = mask[c][i] | s[i];
            unsigned long long y = s[i] << 1 | shift;
            shift = s[i] >> 63;
            unsigned long long q = y + borrow;
            unsigned long long borrow = q < y || x[i] < q;
            z[i] = x[i] - q;
            borrow = borrow;
        }
        for (int i = 0; i < w; i++)
        {
            s[i] = x[i] & ~z[i];
        }
    }
    int ans = 0;
    for (unsigned long long x : s)
    {
        ans += popcount(x);
    }
    return ans;
}

string scs(const string &a, const string &b)
{
    // a、b 是两个字符串；返回二者的一条最短公共超序列。
    vector<char> x(a.begin(), a.end()), y(b.begin(), b.end());
    auto c = lcs(x, y);
    string ans;
    int i = 0, j = 0;
    for (char z : c)
    {
        while (a[i] != z)
        {
            ans += a[i++];
        }
        while (b[j] != z)
        {
            ans += b[j++];
        }
        ans += z;
        i++;
        j++;
    }
    return ans + a.substr(i) + b.substr(j);
}
