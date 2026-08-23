#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

template <class T> int minRepr(const vector<T> &a)
{
    // a 是循环序列；返回其字典序最小旋转的起点。
    int n = a.size();
    if (!n)
    {
        return 0;
    }
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n)
    {
        T x = a[(i + k) % n], y = a[(j + k) % n];
        if (x == y)
        {
            k++;
            continue;
        }
        if (x > y)
        {
            i += k + 1;
        }
        else
        {
            j += k + 1;
        }
        if (i == j)
        {
            j++;
        }
        k = 0;
    }
    return min(i, j);
}

template <class T> vector<T> minRotation(const vector<T> &a)
{
    // a 是循环序列；返回其字典序最小的完整旋转。
    int p = minRepr(a);
    vector<T> b;
    for (int i = 0; i < (int)a.size(); i++)
    {
        b.push_back(a[(p + i) % a.size()]);
    }
    return b;
}
