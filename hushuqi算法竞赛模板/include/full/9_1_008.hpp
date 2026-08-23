#pragma once
#include <bits/stdc++.h>
#include "../linear_algebra_field.hpp"
using namespace std;

template <class T> struct SparseMat
{
    int n, m;
    vector<vector<pair<int, T>>> a;

    SparseMat(int n, int m) : n(n), m(m), a(n)
    {
    }

    void add(int i, int j, T x)
    {
        // 调试检查，可删。
        assert(0 <= i && i < n && 0 <= j && j < m);
        a[i].push_back({j, x});
    }

    vector<T> operator*(const vector<T> &x) const
    {
        // 调试检查，可删。
        assert((int)x.size() == m);
        vector<T> y(n);
        for (int i = 0; i < n; i++)
        {
            for (auto [j, v] : a[i])
            {
                y[i] += v * x[j];
            }
        }
        return y;
    }
};

template <class T> vector<T> BM(const vector<T> &s)
{
    // s 是域上的标量序列；返回首项为 1 的最短线性递推系数。
    vector<T> c{T(1)}, b{T(1)};
    int l = 0, m = 1;
    T last = 1;
    for (int n = 0; n < (int)s.size(); n++)
    {
        T d = s[n];
        for (int i = 1; i <= l; i++) d += c[i] * s[n - i];
        if (d == T(0))
        {
            m++;
            continue;
        }
        vector<T> old = c;
        T q = d / last;
        if ((int)c.size() < (int)b.size() + m) c.resize(b.size() + m, T(0));
        for (int i = 0; i < (int)b.size(); i++) c[i + m] -= q * b[i];
        if (2 * l <= n)
        {
            l = n + 1 - l;
            b = move(old);
            last = d;
            m = 1;
        }
        else m++;
    }
    c.resize(l + 1);
    return c;
}

template <class T> T sparseDet(const SparseMat<T> &a)
{
    // a 是域上的 n 阶稀疏矩阵；返回 Las Vegas 随机预条件 Wiedemann 行列式。
    assert(a.n == a.m);
    int n = a.n;
    mt19937_64 rng(712367821);
    for (;;)
    {
        vector<T> left(n), right(n), diag(n);
        T diagProd = 1;
        for (int i = 0; i < n; i++)
        {
            left[i] = T((long long)(rng() % 998244352 + 1));
            right[i] = T((long long)(rng() % 998244352 + 1));
            diag[i] = T((long long)(rng() % 998244352 + 1));
            diagProd *= diag[i];
        }
        vector<T> seq(2 * n);
        for (int k = 0; k < 2 * n; k++)
        {
            for (int i = 0; i < n; i++) seq[k] += left[i] * right[i];
            auto next = a * right;
            for (int i = 0; i < n; i++) right[i] = next[i] * diag[i];
        }
        auto c = BM(seq);
        if (c.back() == T(0)) return T(0);
        if ((int)c.size() != n + 1) continue;
        T ans = c[n] / diagProd;
        if (n & 1) ans = -ans;
        return ans;
    }
}
