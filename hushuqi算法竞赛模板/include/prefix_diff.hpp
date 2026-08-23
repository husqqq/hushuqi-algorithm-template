#pragma once

#include "base.hpp"

template <class T> vector<T> prefix(const vector<T> &a)
{
    // a 是原序列；返回长度多一位、以零开头的前缀和。
    vector<T> s(a.size() + 1);
    partial_sum(a.begin(), a.end(), s.begin() + 1);
    return s;
}

template <class T> struct Prefix2
{
    int n = 0, m = 0;    // n、m 是原矩阵行数与列数。
    vector<vector<T>> s; // s 是带额外零行零列的二维前缀和。

    explicit Prefix2(const vector<vector<T>> &a)
    {
        // a 是规则原矩阵。
        n = a.size();
        m = n ? a[0].size() : 0;
        s.assign(n + 1, vector<T>(m + 1));
        for (int i = 0; i < n; i++)
        {
            assert((int)a[i].size() == m); // 调试检查，可删
            for (int j = 0; j < m; j++)
            {
                s[i + 1][j + 1] = a[i][j] + s[i][j + 1] + s[i + 1][j] - s[i][j];
            }
        }
    }

    T sum(int x1, int y1, int x2, int y2) const
    {
        // x1、y1、x2、y2 是半开子矩形的上、左、下、右边界；返回子矩形和。
        return s[x2][y2] - s[x1][y2] - s[x2][y1] + s[x1][y1];
    }
};

template <class T> struct Diff
{
    int n = 0;   // n 是目标数组长度。
    vector<T> d; // d 保存长度为 n+1 的边界差分。

    Diff(int m = 0) : n(m), d(m + 1)
    {
        // m 是目标数组长度。
    }

    void add(int l, int r, T x)
    {
        // l、r 是半开区间端点，x 是区间增量。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        d[l] += x;
        d[r] -= x;
    }

    vector<T> build() const
    {
        // 返回执行全部区间修改后的长度 n 数组。
        vector<T> a(n);
        T cur{};
        for (int i = 0; i < n; i++)
        {
            cur += d[i];
            a[i] = cur;
        }
        return a;
    }
};

template <class T> struct Diff2
{
    int n = 0, m = 0;    // n、m 是目标矩阵行数与列数。
    vector<vector<T>> d; // d 保存带额外边界的二维差分。

    Diff2(int x = 0, int y = 0) : n(x), m(y), d(x + 1, vector<T>(y + 1))
    {
        // x、y 是目标矩阵行数与列数。
    }

    void add(int x1, int y1, int x2, int y2, T v)
    {
        // 四个坐标描述半开子矩形，v 是矩形增量。
        assert(0 <= x1 && x1 <= x2 && x2 <= n); // 调试检查，可删
        assert(0 <= y1 && y1 <= y2 && y2 <= m); // 调试检查，可删
        d[x1][y1] += v;
        d[x1][y2] -= v;
        d[x2][y1] -= v;
        d[x2][y2] += v;
    }

    vector<vector<T>> build() const
    {
        // 返回执行全部矩形修改后的 n 行 m 列矩阵。
        auto a = d;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (i)
                {
                    a[i][j] += a[i - 1][j];
                }
                if (j)
                {
                    a[i][j] += a[i][j - 1];
                }
                if (i && j)
                {
                    a[i][j] -= a[i - 1][j - 1];
                }
            }
        }
        a.resize(n);
        for (auto &row : a)
        {
            row.resize(m);
        }
        return a;
    }
};

template <class T> vector<T> kthDiff(vector<T> a, int k)
{
    // a 是按值复制的原序列，k 是差分阶数；返回 k 阶差分序列。
    assert(k >= 0); // 调试检查，可删
    auto t = min<int>(k, a.size());
    for (int z = 0; z < t; z++)
    {
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i - 1] = a[i] - a[i - 1];
        }
        a.pop_back();
    }
    return a;
}

template <class T> T maxSub(const vector<T> &a)
{
    // a 是非空序列；返回非空最大子段和。
    assert(!a.empty()); // 调试检查，可删
    T ans = a[0];
    T cur = a[0];
    for (int i = 1; i < (int)a.size(); i++)
    {
        T x = a[i];
        cur = max(x, cur + x);
        chmax(ans, cur);
    }
    return ans;
}

template <class T> T maxSubMat(const vector<vector<T>> &a)
{
    // a 是非空规则矩阵；返回非空最大子矩阵和。
    assert(!a.empty() && !a[0].empty()); // 调试检查，可删
    auto n = (int)a.size();
    auto m = (int)a[0].size();
    for (auto &row : a)
    {
        assert((int)row.size() == m); // 调试检查，可删
    }
    T ans = a[0][0];
    for (int u = 0; u < n; u++)
    {
        vector<T> s(m);
        for (int d = u; d < n; d++)
        {
            for (int j = 0; j < m; j++)
            {
                s[j] += a[d][j];
            }
            chmax(ans, maxSub(s));
        }
    }
    return ans;
}

template <class T> struct GcdRanges
{
    using U = make_unsigned_t<T>;

    vector<vector<pair<int, U>>> g; // g[r] 保存以 r 结尾的不同 GCD 及最小左端点。

    static U mag(T x)
    {
        // x 是原数组元素；返回不对最小负数取正值的无符号幅值。
        auto u = (U)x;
        if constexpr (is_signed_v<T>)
        {
            if (x < 0)
            {
                return (U)0 - u;
            }
        }
        return u;
    }

    explicit GcdRanges(const vector<T> &a) : g(a.size())
    {
        // a 是用于预处理所有右端点 GCD 分段的原数组。
        static_assert(is_integral_v<T>);
        vector<pair<int, U>> pre;
        for (int r = 0; r < (int)a.size(); r++)
        {
            auto v = mag(a[r]);
            vector<pair<int, U>> cur{{r, v}};
            for (auto [l, x] : pre)
            {
                x = gcd(x, v);
                if (cur.back().second == x)
                {
                    cur.back().first = l;
                }
                else
                {
                    cur.push_back({l, x});
                }
            }
            reverse(cur.begin(), cur.end());
            g[r] = cur;
            pre = cur;
            reverse(pre.begin(), pre.end());
        }
    }

    U query(int l, int r) const
    {
        // l、r 是闭区间端点；返回 a[l..r] 的绝对值 GCD。
        assert(0 <= l && l <= r && r < (int)g.size()); // 调试检查，可删
        auto it = upper_bound(g[r].begin(),
                              g[r].end(),
                              l,
                              [](int x, const auto &p)
                              {
                                  return x < p.first;
                              });
        return prev(it)->second;
    }
};
