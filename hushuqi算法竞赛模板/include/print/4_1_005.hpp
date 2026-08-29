#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

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
