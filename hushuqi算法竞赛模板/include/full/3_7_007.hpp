#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}

template <class T, class F> struct AggDeque
{
    // op 是结合律运算，e 是幺元。
    F op;
    T e;
    // l、r 是队首侧与队尾侧栈；每项为 {原值,按队列方向的本栈聚合}。
    vector<pair<T, T>> l, r;

    AggDeque(F combine, T identity) : op(combine), e(identity)
    {
        // combine 是结合律二元运算，identity 是其幺元；初始队列为空。
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
    void rebuild(int k)
    {
        // k 是重建后放入队首侧的元素数；保持整个队列顺序不变。
        assert(0 <= k && k <= size()); // 调试检查，可删
        vector<T> a;
        for (auto i = l.rbegin(); i != l.rend(); i++)
        {
            a.push_back(i->first);
        }
        for (auto x : r)
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
    void popFront()
    {
        // 删除队首元素。
        assert(!empty()); // 调试检查，可删
        if (l.empty())
        {
            rebuild((size() + 1) / 2);
        }
        l.pop_back();
    }
    void popBack()
    {
        // 删除队尾元素。
        assert(!empty()); // 调试检查，可删
        if (r.empty())
        {
            rebuild(size() / 2);
        }
        r.pop_back();
    }
    T front() const
    {
        // 返回队首元素；只读端点，不为读取重建两侧栈。
        assert(!empty()); // 调试检查，可删
        return l.empty() ? r.front().first : l.back().first;
    }
    T back() const
    {
        // 返回队尾元素，不为读取重建两侧栈。
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
