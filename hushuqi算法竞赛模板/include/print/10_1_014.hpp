#pragma once

#include "10_1_003.hpp"
#include "10_1_004.hpp"
#include "7_1_008.hpp"
#include "7_4_003.hpp"

vector<Z> multiConv(const vector<int> &base, const vector<Z> &f, const vector<Z> &g)
{
    // base 依次给出各维长度，f、g 按最低维在前展平；返回各维无进位卷积。
    int k = base.size();
    int n = 1;
    for (int x : base)
    {
        assert(x > 0 && n <= (1 << 22) / x); // 调试检查，可删；同时保证 NTT 长度可用。
        n *= x;
    }
    assert((int)f.size() == n && (int)g.size() == n); // 调试检查，可删。
    if (n == 1)
    {
        return {f[0] * g[0]};
    }
    auto cls = [&](int x)
    {
        int c = 0;
        for (int b : base)
        {
            x /= b;
            c += x;
        }
        return c % k;
    };
    int len = bit_ceil((unsigned long long)(2 * n - 1));
    vector a(k, vector<Z>(len));
    vector b(k, vector<Z>(len));
    for (int i = 0; i < n; i++)
    {
        a[cls(i)][i] = f[i];
        b[cls(i)][i] = g[i];
    }
    for (int i = 0; i < k; i++)
    {
        FastNTT::dft(a[i]);
        FastNTT::dft(b[i]);
    }
    vector c(k, vector<Z>(len));
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            for (int p = 0; p < len; p++)
            {
                c[(i + j) % k][p] += a[i][p] * b[j][p];
            }
        }
    }
    for (auto &v : c)
    {
        FastNTT::idft(v);
    }
    vector<Z> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = c[cls(i)][i];
    }
    return ans;
}

vector<DynMod> geomDft(vector<DynMod> f, DynMod r)
{
    // f 是低次在前的系数，r 是非零单位根；返回 f(1),f(r),...,f(r^(n-1))。
    int n = f.size();
    if (n <= 100)
    {
        vector<DynMod> ans(n);
        DynMod x = 1;
        for (int i = 0; i < n; i++)
        {
            DynMod pw = 1;
            for (int j = 0; j < n; j++)
            {
                ans[i] += f[j] * pw;
                pw *= x;
            }
            x *= r;
        }
        return ans;
    }
    vector<DynMod> a(2 * n - 1), b(n);
    DynMod pw = 1;
    a[0] = 1;
    for (int i = 1; i < 2 * n - 1; i++)
    {
        a[i] = a[i - 1] * pw;
        pw *= r;
    }
    DynMod ir = r.pow(DynMod::mod() - 2);
    pw = 1;
    b[0] = 1;
    for (int i = 1; i < n; i++)
    {
        b[i] = b[i - 1] * pw;
        pw *= ir;
    }
    for (int i = 0; i < n; i++)
    {
        f[i] *= b[i];
    }
    reverse(f.begin(), f.end());
    vector<long long> x(n), y(2 * n - 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = f[i].x;
    }
    for (int i = 0; i < 2 * n - 1; i++)
    {
        y[i] = a[i].x;
    }
    auto c = convMod(move(x), move(y), DynMod::mod());
    vector<DynMod> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = DynMod(c[n - 1 + i]) * b[i];
    }
    return ans;
}

vector<long long> multiCycConv(long long p, const vector<int> &base,
                                  const vector<long long> &f, const vector<long long> &g)
{
    // p 是不超过 1E9 的素数，各维长度整除 p-1；f、g 最低维在前，返回逐维循环卷积。
    assert(2 <= p && p <= 1000000000); // 调试检查，可删。
    DynMod::setMod(p);
    int n = 1;
    for (int x : base)
    {
        assert(x >= 2 && (p - 1) % x == 0 && n <= 262144 / x); // 调试检查，可删。
        n *= x;
    }
    assert((int)f.size() == n && (int)g.size() == n); // 调试检查，可删。
    vector<DynMod> a(n), b(n);
    for (int i = 0; i < n; i++)
    {
        a[i] = f[i];
        b[i] = g[i];
    }
    DynMod gen = primRoot(p);
    auto transform = [&](vector<DynMod> &v, bool inv)
    {
        int step = 1;
        for (int len : base)
        {
            DynMod root = gen.pow((p - 1) / len);
            if (inv)
            {
                root = root.pow(p - 2);
            }
            for (int block = 0; block < n; block += step * len)
            {
                for (int off = 0; off < step; off++)
                {
                    vector<DynMod> row(len);
                    for (int j = 0; j < len; j++)
                    {
                        row[j] = v[block + off + step * j];
                    }
                    row = geomDft(move(row), root);
                    for (int j = 0; j < len; j++)
                    {
                        v[block + off + step * j] = row[j];
                    }
                }
            }
            step *= len;
        }
    };
    transform(a, false);
    transform(b, false);
    for (int i = 0; i < n; i++)
    {
        a[i] *= b[i];
    }
    transform(a, true);
    DynMod scale = DynMod(n).pow(p - 2);
    vector<long long> ans(n);
    for (int i = 0; i < n; i++)
    {
        ans[i] = (a[i] * scale).x;
    }
    return ans;
}
