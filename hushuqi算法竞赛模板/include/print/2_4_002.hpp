#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

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
    // 若要最小子段和，保留“非空、从首元素初始化”的结构，把 max 改成 min、chmax 改成 chmin；
    // 不要只改最后的答案比较，否则 cur 的“接上或重新开始”不变量会仍然求最大值。
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
    // 若要最小子矩阵和，同步把内部 maxSub、chmax 改为最小子段版本、chmin；边界仍以 a[0][0] 初始化，
    // 不需要正负无穷单位元，也不能只把最终 ans 的 max 改成 min。
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

unsigned long long prefixXor(unsigned long long n)
{
    // n 是非负端点；返回闭区间 [0,n] 的按位异或和。
    if ((n & 3) == 0)
    {
        return n;
    }
    if ((n & 3) == 1)
    {
        return 1;
    }
    if ((n & 3) == 2)
    {
        return n + 1;
    }
    return 0;
}

unsigned long long rangeXor(unsigned long long l, unsigned long long r)
{
    // l、r 是非负闭区间端点；返回 [l,r] 的按位异或和。
    assert(l <= r); // 调试检查，可删
    return prefixXor(r) ^ (l ? prefixXor(l - 1) : 0);
}
