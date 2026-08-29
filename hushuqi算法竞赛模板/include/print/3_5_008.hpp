#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T, class Compare = less<T>> class LeftistHeap
{
    struct Node
    {
        // x 是键值；l、r 是儿子；d 是到空儿子的最短距离。
        T x{};
        int l = 0, r = 0, d = 1;
    };

    // cmp 决定堆序，t 是节点池，0 表示空节点且 d=0。
    Compare cmp;
    vector<Node> t{{T{}, 0, 0, 0}};

  public:
    int makeHeap(const T &x)
    {
        // x 是键值；新建零路径长为 1 的单点堆并返回根编号。
        t.push_back({x});
        return t.size() - 1;
    }

    int merge(int x, int y)
    {
        // x、y 是节点不共享的两个堆根；破坏性合并并返回新根，两个旧堆句柄随即失效。
        if (!x || !y)
        {
            return x | y;
        }
        if (cmp(t[y].x, t[x].x))
        {
            swap(x, y);
        }
        t[x].r = merge(t[x].r, y);
        if (t[t[x].l].d < t[t[x].r].d)
        {
            swap(t[x].l, t[x].r);
        }
        t[x].d = t[t[x].r].d + 1;
        return x;
    }

    const T &top(int root) const
    {
        // root 是非空堆根；返回堆顶键值。
        assert(root); // 调试检查，可删
        return t[root].x;
    }

    int pop(int root)
    {
        // root 是非空堆根；删除堆顶并返回新根。
        assert(root); // 调试检查，可删
        return merge(t[root].l, t[root].r);
    }
};
