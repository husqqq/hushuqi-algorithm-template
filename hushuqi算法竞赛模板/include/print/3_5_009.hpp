#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则更新 a，返回是否更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则更新 a，返回是否更新。
    return a < b ? a = b, true : false;
}

template <class T, class Cmp = less<T>> struct ErasableHeap
{
    // a 保存全部插入，b 保存待抵消删除，n 是当前有效元素数。
    priority_queue<T, vector<T>, Cmp> a, b;
    int n = 0;

    void clean()
    {
        // 抵消已经同时暴露在两个堆顶的失效元素。
        while (!a.empty() && !b.empty() && a.top() == b.top())
        {
            a.pop();
            b.pop();
        }
    }
    void push(const T &x)
    {
        // x 是新键；插入一份。
        a.push(x);
        n++;
    }
    void erase(const T &x)
    {
        // x 是当前确实存在的键；登记删除一份。
        b.push(x);
        n--;
        clean();
    }
    const T &top()
    {
        // 返回当前非空堆的最优键。
        assert(n > 0); // 调试检查，可删
        clean();
        return a.top();
    }
    void pop()
    {
        // 删除当前非空堆的最优键。
        assert(n > 0); // 调试检查，可删
        clean();
        a.pop();
        n--;
        clean();
    }
    int size() const
    {
        // 返回有效元素数。
        return n;
    }
    bool empty() const
    {
        // 返回是否没有有效元素。
        return n == 0;
    }
};
