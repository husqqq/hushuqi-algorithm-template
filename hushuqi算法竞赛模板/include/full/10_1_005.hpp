#pragma once

#include "10_1_003.hpp"

namespace poly2
{
struct Mat
{
    int n = 0; // 二维系数表的行数。
    int m = 0; // 二维系数表的列数。
    vector<Z> a; // 按行连续保存的 n*m 个系数。

    Mat(int r = 0, int c = 0) : n(r), m(c), a(r * c)
    {
        // r、c 是二维系数表的行数与列数；构造全零系数表。
    }

    Z &operator()(int i, int j)
    {
        // i、j 是行列下标；返回对应系数的可修改引用。
        return a[i * m + j];
    }

    const Z &operator()(int i, int j) const
    {
        // i、j 是行列下标；返回对应系数的只读引用。
        return a[i * m + j];
    }
};

vector<Z> pack(const Mat &a, int w)
{
    // a 是行主序二维系数表，w 不小于其列数；返回以 w 为行跨度的 Kronecker 打包结果。
    if (a.n == 0 || a.m == 0)
    {
        return {};
    }
    assert(w >= a.m); // 调试检查，可删。
    vector<Z> x((a.n - 1) * w + a.m);
    for (int i = 0; i < a.n; i++)
    {
        for (int j = 0; j < a.m; j++)
        {
            x[i * w + j] = a(i, j);
        }
    }
    return x;
}

Mat unpack(const vector<Z> &a, int n, int m)
{
    // a 是行跨度为 m 的 Kronecker 系数，n、m 是目标形状；返回对应行主序二维系数表。
    if (n == 0 || m == 0)
    {
        return {};
    }
    assert((int)a.size() >= n * m); // 调试检查，可删。
    Mat c(n, m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            c(i, j) = a[i * m + j];
        }
    }
    return c;
}

Mat conv(const Mat &a, const Mat &b)
{
    // a、b 是行主序二维系数表；用 Kronecker 打包返回二维线性卷积。
    if (a.n == 0 || a.m == 0 || b.n == 0 || b.m == 0)
    {
        return {};
    }
    int n = a.n + b.n - 1;
    int m = a.m + b.m - 1;
    auto c = ::conv(pack(a, m), pack(b, m));
    c.resize(n * m);
    return unpack(c, n, m);
}

array<Mat, 2> conv(const Mat &a, const Mat &b, const Mat &c)
{
    // a、b 是同形左因子，c 是公共右因子；返回 {a*c,b*c}，两次卷积共享 c 的正变换。
    assert(a.n == b.n && a.m == b.m); // 调试检查，可删。
    if (a.n == 0 || a.m == 0 || c.n == 0 || c.m == 0)
    {
        return {};
    }
    int n = a.n + c.n - 1;
    int m = a.m + c.m - 1;
    vector<Z> x = pack(a, m);
    vector<Z> y = pack(b, m);
    vector<Z> z = pack(c, m);
    if (min({x.size(), y.size(), z.size()}) <= 64)
    {
        return {unpack(::conv(x, z), n, m), unpack(::conv(y, z), n, m)};
    }
    int s = max(x.size() + z.size() - 1, y.size() + z.size() - 1);
    assert(nttFit(x.size(), z.size()) && nttFit(y.size(), z.size())); // 调试检查，可删。
    int len = bit_ceil((unsigned long long)s);
    x.resize(len);
    y.resize(len);
    z.resize(len);
    dft(x);
    dft(y);
    dft(z);
    for (int i = 0; i < len; i++)
    {
        x[i] *= z[i];
        y[i] *= z[i];
    }
    idft(x);
    idft(y);
    x.resize(n * m);
    y.resize(n * m);
    return {unpack(x, n, m), unpack(y, n, m)};
}
}

vector<vector<Z>> conv2(const vector<vector<Z>> &a, const vector<vector<Z>> &b)
{
    // a、b 是矩形二维系数表；返回二维线性卷积，任一维为空时返回空。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int m1 = a[0].size();
    int m2 = b[0].size();
    for (const auto &r : a)
    {
        assert((int)r.size() == m1); // 调试检查，可删。
    }
    for (const auto &r : b)
    {
        assert((int)r.size() == m2); // 调试检查，可删。
    }
    if (m1 == 0 || m2 == 0)
    {
        return {};
    }
    poly2::Mat x(a.size(), m1), y(b.size(), m2);
    for (int i = 0; i < x.n; i++)
    {
        for (int j = 0; j < x.m; j++)
        {
            x(i, j) = a[i][j];
        }
    }
    for (int i = 0; i < y.n; i++)
    {
        for (int j = 0; j < y.m; j++)
        {
            y(i, j) = b[i][j];
        }
    }
    auto z = poly2::conv(x, y);
    vector<vector<Z>> ans(z.n, vector<Z>(z.m));
    for (int i = 0; i < z.n; i++)
    {
        for (int j = 0; j < z.m; j++)
        {
            ans[i][j] = z(i, j);
        }
    }
    return ans;
}
