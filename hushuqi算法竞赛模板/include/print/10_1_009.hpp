#pragma once

#include "10_1_003.hpp"

template <class T> struct OnlineConv
{
    vector<T> a; // 第一序列已经加入的系数。
    vector<T> b; // 第二序列已经加入的系数。
    vector<T> c; // 从第 0 项到当前项的卷积系数。

    T add(T x, T y)
    {
        // x、y 分别是两序列新到达的系数；追加二者并返回刚确定的卷积项。
        int n = a.size();
        a.push_back(x);
        b.push_back(y);
        c.push_back({});
        for (int i = 0; i <= n; i++)
        {
            c[n] += a[i] * b[n - i];
        }
        return c[n];
    }
};

struct OnlineNTT
{
    vector<Z> a, b; // 两个输入序列已经加入的系数。
    vector<Z> h; // 已由分块卷积累计出的答案系数。
    vector<Z> x, y; // 当前块的正逆变换工作区。
    vector<vector<Z>> fa, fb; // 各二进制层可复用的输入前缀频域值。
    int p = 0; // 下一次 add 要返回的卷积下标。

    Z add(Z u, Z v)
    {
        // u、v 分别是两序列新到达的系数；追加二者并返回刚确定的卷积项。
        a.push_back(u);
        b.push_back(v);
        int z = countr_zero((unsigned long long)(p + 2));
        int w = 1LL << z;
        int s;
        assert(2 * w <= (1 << 23)); // 调试检查，可删。
        if (p + 2 == w)
        {
            x = a;
            x.resize(2 * w);
            dft(x);
            fa.emplace_back(x.begin(), x.begin() + w);
            y = b;
            y.resize(2 * w);
            dft(y);
            fb.emplace_back(y.begin(), y.begin() + w);
            for (int i = 0; i < 2 * w; i++)
            {
                x[i] *= y[i];
            }
            s = w - 2;
            h.resize(2 * s + 2);
        }
        else
        {
            x.assign(a.end() - w, a.end());
            x.resize(2 * w);
            dft(x);
            y.assign(b.end() - w, b.end());
            y.resize(2 * w);
            dft(y);
            for (int i = 0; i < 2 * w; i++)
            {
                x[i] = x[i] * fb[z][i] + y[i] * fa[z][i];
            }
            s = w - 1;
        }
        idft(x);
        for (int i = 0; i <= s; i++)
        {
            h[p + i] += x[s + i];
        }
        return h[p++];
    }
};
