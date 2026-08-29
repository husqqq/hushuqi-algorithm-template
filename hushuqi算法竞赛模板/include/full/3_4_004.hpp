#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

struct Cart
{
    // root 是树根；parent、left、right 分别保存父亲与左右儿子编号。
    int root = -1;
    vector<int> parent, left, right;
};

// 最小笛卡尔树；中序遍历为原下标，相等时靠前元素在上方。
template <class T> Cart cartesian(const vector<T> &a)
{
    // a 是静态数组；返回其最小笛卡尔树的根、父亲和左右儿子。
    int n = a.size();
    Cart c{-1, vector<int>(n, -1), vector<int>(n, -1), vector<int>(n, -1)};
    vector<int> s;
    for (int i = 0; i < n; i++)
    {
        int last = -1;
        while (!s.empty() && a[i] < a[s.back()])
        {
            last = s.back();
            s.pop_back();
        }
        if (!s.empty())
        {
            c.parent[i] = s.back();
            c.right[s.back()] = i;
        }
        if (last != -1)
        {
            c.parent[last] = i;
            c.left[i] = last;
        }
        s.push_back(i);
    }
    if (n)
    {
        c.root = s[0];
    }
    return c;
}
