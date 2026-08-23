#pragma once

#include "10_1_003.hpp"

vector<Z> mulMod2n(const vector<Z> &a, const vector<Z> &b)
{
    // a、b 的长度必须同为 2 的幂；返回下标乘法模数组长度的卷积。
    assert(a.size() == b.size() && !a.empty() && has_single_bit(a.size())); // 调试检查，可删。
    int n = countr_zero((unsigned long long)a.size());
    assert(n <= 22); // 调试检查，可删；该上限控制 O(n2^n) 工作量与三组分层缓冲区。
    int mask = (1LL << n) - 1;
    vector<vector<vector<Z>>> x(n + 1), y(n + 1), z(n + 1);
    auto shape = [&](int v)
    {
        int h = n - v >= 2 ? 2 : 1;
        int w = 1LL << max<int>(n - v - 2, 0);
        return pair{h, w};
    };
    for (int v = 0; v <= n; v++)
    {
        auto [h, w] = shape(v);
        x[v].assign(h, vector<Z>(w));
        y[v].assign(h, vector<Z>(w));
        z[v].assign(h, vector<Z>(w));
        int p = (1LL << v) & mask;
        for (int j = 0; j < w; j++)
        {
            x[v][0][j] = a[p];
            y[v][0][j] = b[p];
            if (h == 2)
            {
                x[v][1][j] = a[(1LL << n) - p];
                y[v][1][j] = b[(1LL << n) - p];
            }
            p = 5 * p & mask;
        }
    }
    for (int v = 0; v <= n; v++)
    {
        auto [h, w] = shape(v);
        for (int i = 0; i < h; i++)
        {
            FastNTT::dft(x[v][i]);
            FastNTT::dft(y[v][i]);
        }
        if (h == 2)
        {
            for (int j = 0; j < w; j++)
            {
                Z a0 = x[v][0][j], a1 = x[v][1][j];
                Z b0 = y[v][0][j], b1 = y[v][1][j];
                x[v][0][j] = a0 + a1;
                x[v][1][j] = a0 - a1;
                y[v][0][j] = b0 + b1;
                y[v][1][j] = b0 - b1;
            }
        }
    }
    for (int p = 0; p <= n; p++)
    {
        for (int q = 0; q <= n; q++)
        {
            int v = min(n, p + q);
            auto [h, w] = shape(v);
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    z[v][i][j] += x[p][i][j] * y[q][i][j];
                }
            }
        }
    }
    Z inv2 = Z(2).inv();
    for (int v = 0; v <= n; v++)
    {
        auto [h, w] = shape(v);
        for (int i = 0; i < h; i++)
        {
            FastNTT::idft(z[v][i]);
        }
        if (h == 2)
        {
            for (int j = 0; j < w; j++)
            {
                Z a0 = z[v][0][j], a1 = z[v][1][j];
                z[v][0][j] = (a0 + a1) * inv2;
                z[v][1][j] = (a0 - a1) * inv2;
            }
        }
    }
    vector<Z> c(1LL << n);
    for (int v = 0; v <= n; v++)
    {
        auto [h, w] = shape(v);
        int p = (1LL << v) & mask;
        for (int j = 0; j < w; j++)
        {
            c[p] = z[v][0][j];
            if (h == 2)
            {
                c[(1LL << n) - p] = z[v][1][j];
            }
            p = 5 * p & mask;
        }
    }
    return c;
}
