#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

template <class T> struct Diff
{
    // d 是长度多一位的一维差分数组。
    vector<T> d;

    Diff(int n = 0) : d(n + 1)
    {
        // n 是原数组长度；初值全为零。
    }

    void add(int l, int r, T x)
    {
        // 给半开区间 [l,r) 的每项增加 x。
        d[l] += x;
        d[r] -= x;
    }

    vector<T> build()
    {
        // 还原并返回原数组；本对象不应再次调用 build。
        d.pop_back();
        partial_sum(d.begin(), d.end(), d.begin());
        return d;
    }
};

template <class T> struct Diff2
{
    // n、m 是原矩阵的行数和列数。
    int n, m;
    // d 是各维多一位的二维差分数组。
    vector<vector<T>> d;

    Diff2(int n = 0, int m = 0) : n(n), m(m), d(n + 1, vector<T>(m + 1))
    {
        // n、m 是原矩阵大小；初值全为零。
    }

    void add(int u, int l, int dwn, int r, T x)
    {
        // 给半开矩形 [u,dwn) × [l,r) 的每项增加 x。
        d[u][l] += x;
        d[u][r] -= x;
        d[dwn][l] -= x;
        d[dwn][r] += x;
    }

    vector<vector<T>> build()
    {
        // 做二维前缀和并返回还原后的矩阵。
        vector<vector<T>> a(n, vector<T>(m));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                a[i][j] = d[i][j];
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
        return a;
    }
};

template <class T> vector<T> kthDiff(vector<T> a, int k)
{
    // 返回序列 a 做 k 次相邻差分后的结果。
    while (k--)
    {
        for (int i = 1; i < a.size(); i++)
        {
            a[i - 1] = a[i] - a[i - 1];
        }
        if (!a.empty())
        {
            a.pop_back();
        }
    }
    return a;
}

template <class T> T maxSubarray(const vector<T> &a)
{
    // a 非空；返回最大非空连续子段和。
    // 改求最小子段和时，保留首元素初始化，把 max 改成 min、chmax 改成 chmin；
    // 必须同时反向“接上/重新开始”的比较，不能只改最终 ans。
    assert(!a.empty());
    T ans = a[0], cur = a[0];
    for (int i = 1; i < a.size(); i++)
    {
        cur = max(a[i], cur + a[i]);
        chmax(ans, cur);
    }
    return ans;
}

template <class T> T maxSubmatrix(const vector<vector<T>> &a)
{
    // a 是非空规则矩阵；返回最大非空子矩形和。
    // 改求最小子矩形和时，同步使用最小子段版本并把 chmax 改为 chmin；边界仍以 a[0][0] 初始化。
    assert(!a.empty() && !a[0].empty());
    int n = a.size(), m = a[0].size();
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
            chmax(ans, maxSubarray(s));
        }
    }
    return ans;
}

unsigned long long prefixXor(unsigned long long n)
{
    // 返回 0 xor 1 xor ... xor n。
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
    // 返回闭区间 [l,r] 内所有整数的异或和。
    return prefixXor(r) ^ (l ? prefixXor(l - 1) : 0);
}
