#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T> class PerArray
{
    struct Node
    {
        // left、right 是儿子编号，value 仅在叶子保存数组值。
        int left = 0, right = 0;
        T value{};
    };

    // n 是数组长度，nodes 是节点池，roots[v] 是版本 v 的根。
    int n;
    vector<Node> nodes{{}};
    vector<int> roots;

    int build(const vector<T> &a, int l, int r)
    {
        // a 是初始数组，[l,r) 是当前区间；返回建成的节点编号。
        int p = nodes.size();
        nodes.push_back({});
        if (r - l == 1)
        {
            nodes[p].value = a[l];
            return p;
        }
        int m = midpoint(l, r);
        nodes[p].left = build(a, l, m);
        nodes[p].right = build(a, m, r);
        return p;
    }

    int set(int p, int l, int r, int x, const T &value)
    {
        // p 是旧节点，[l,r) 是其区间；把位置 x 改为 value 并返回新节点。
        int q = nodes.size();
        nodes.push_back(nodes[p]);
        if (r - l == 1)
        {
            nodes[q].value = value;
            return q;
        }
        int m = midpoint(l, r);
        if (x < m)
        {
            nodes[q].left = set(nodes[p].left, l, m, x, value);
        }
        else
        {
            nodes[q].right = set(nodes[p].right, m, r, x, value);
        }
        return q;
    }

  public:
    explicit PerArray(const vector<T> &a) : n(a.size())
    {
        // a 是初始数组；建立版本 0，空数组的根为 0。
        roots.push_back(n ? build(a, 0, n) : 0);
    }

    int set(int version, int position, const T &value)
    {
        // version 是旧版本，position 是位置，value 是新值；返回新增版本编号。
        assert(0 <= version && version < (int)roots.size()); // 调试检查，可删
        assert(0 <= position && position < n); // 调试检查，可删
        roots.push_back(set(roots[version], 0, n, position, value));
        return roots.size() - 1;
    }

    T get(int version, int position) const
    {
        // version 是历史版本，position 是位置；按值返回该位置的内容。
        assert(0 <= version && version < (int)roots.size()); // 调试检查，可删
        assert(0 <= position && position < n); // 调试检查，可删
        int p = roots[version], l = 0, r = n;
        while (r - l > 1)
        {
            int m = midpoint(l, r);
            if (position < m)
            {
                p = nodes[p].left;
                r = m;
            }
            else
            {
                p = nodes[p].right;
                l = m;
            }
        }
        return nodes[p].value;
    }
};
