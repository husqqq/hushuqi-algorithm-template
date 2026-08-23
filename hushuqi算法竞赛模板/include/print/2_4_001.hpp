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

template <class T> vector<T> prefix(const vector<T> &a)
{
    // a 是原序列；返回长度多一位、以零开头的前缀和。
    vector<T> s(a.size() + 1);
    partial_sum(a.begin(), a.end(), s.begin() + 1);
    return s;
}

template <class T> struct Prefix2
{
    // n、m 是原矩阵的行数和列数。
    int n, m;
    // s[i][j] 是前 i 行、前 j 列的二维前缀和。
    vector<vector<T>> s;

    Prefix2(const vector<vector<T>> &a) : n(a.size()), m(n ? a[0].size() : 0), s(n + 1, vector<T>(m + 1))
    {
        // a 是规则矩阵；据此建立带零行零列的前缀和表。
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                s[i + 1][j + 1] = a[i][j] + s[i][j + 1] + s[i + 1][j] - s[i][j];
            }
        }
    }

    T sum(int x1, int y1, int x2, int y2) const
    {
        // 返回半开矩形 [x1,x2) × [y1,y2) 的元素和。
        return s[x2][y2] - s[x1][y2] - s[x2][y1] + s[x1][y1];
    }
};
