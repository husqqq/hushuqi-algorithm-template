#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T, class F> struct AggDeque
{
    // op 按队首到队尾合并两个摘要，须可在 const 对象上调用；e 是双侧幺元。
    F op;
    T e;
    // l、r 是队首侧与队尾侧栈；每项为 {原值,按队列方向的本栈聚合}。
    vector<pair<T, T>> l, r;

    AggDeque(F combine, T identity) : op(move(combine)), e(move(identity))
    {
        // combine(a,b) 返回相邻段 a 后接 b 的摘要，须满足结合律；identity 是幺元，初始队列为空。
    }

    int size() const
    {
        // 返回队列元素数。
        return l.size() + r.size();
    }
    bool empty() const
    {
        // 返回队列是否为空。
        return l.empty() && r.empty();
    }
    void pushFront(T x)
    {
        // x 是新元素；插入队首并更新左栈聚合。
        l.push_back({x, l.empty() ? x : op(x, l.back().second)});
    }
    void pushBack(T x)
    {
        // x 是新元素；插入队尾并更新右栈聚合。
        r.push_back({x, r.empty() ? x : op(r.back().second, x)});
    }
private:
    void rebuild(int k)
    {
        // k 是重建后放入队首侧的元素数；保持整个队列顺序不变。
        assert(0 <= k && k <= size()); // 调试检查，可删
        vector<T> a;
        a.reserve(size());
        for (auto i = l.rbegin(); i != l.rend(); i++)
        {
            a.push_back(i->first);
        }
        for (const auto &x : r)
        {
            a.push_back(x.first);
        }
        l.clear();
        r.clear();
        for (int i = k - 1; i >= 0; i--)
        {
            pushFront(a[i]);
        }
        for (int i = k; i < (int)a.size(); i++)
        {
            pushBack(a[i]);
        }
    }
public:
    void popFront()
    {
        // 当前队列须非空；删除队首元素，无返回值。
        assert(!empty()); // 调试检查，可删
        if (l.empty())
        {
            rebuild(size() - size() / 2);
        }
        l.pop_back();
    }
    void popBack()
    {
        // 当前队列须非空；删除队尾元素，无返回值。
        assert(!empty()); // 调试检查，可删
        if (r.empty())
        {
            rebuild(size() / 2);
        }
        r.pop_back();
    }
    const T &front() const
    {
        // 当前队列须非空；返回队首元素的只读引用，修改队列前使用。
        assert(!empty()); // 调试检查，可删
        return l.empty() ? r.front().first : l.back().first;
    }
    const T &back() const
    {
        // 当前队列须非空；返回队尾元素的只读引用，修改队列前使用。
        assert(!empty()); // 调试检查，可删
        return r.empty() ? l.front().first : r.back().first;
    }
    T query() const
    {
        // 按队首到队尾顺序聚合全部元素；空队列返回 e。
        if (l.empty())
        {
            return r.empty() ? e : r.back().second;
        }
        if (r.empty())
        {
            return l.back().second;
        }
        return op(l.back().second, r.back().second);
    }
};
