#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SlopeTrick
{
    // 维护分段线性凸函数 f(x) = mn + sum_{a in L} max(a-x,0) + sum_{b in R} max(x-b,0)，
    // 且 max L <= min R，[max L, min R] 即最小值区间。dl、dr 是 L、R 整体的平移量（懒标记）。
    priority_queue<int> l;                          // l 存 L 中各拐点减去 dl。
    priority_queue<int, vector<int>, greater<>> r;  // r 存 R 中各拐点减去 dr。
    int dl = 0, dr = 0, mn = 0;                     // mn 是 f 的最小值。

    void addLeft(int a)
    {
        // a 是折点；给 f 加上 max(a-x,0)，不返回值。
        if (!r.empty())
        {
            mn += max<int>(0, a - (r.top() + dr));
        }
        r.push(a - dr);
        int v = r.top() + dr;
        r.pop();
        l.push(v - dl);
    }

    void addRight(int a)
    {
        // a 是折点；给 f 加上 max(x-a,0)，不返回值。
        if (!l.empty())
        {
            mn += max<int>(0, l.top() + dl - a);
        }
        l.push(a - dl);
        int v = l.top() + dl;
        l.pop();
        r.push(v - dr);
    }

    void addAbs(int a)
    {
        // a 是折点；给 f 加上 |x-a|，不返回值。
        addLeft(a);
        addRight(a);
    }

    void shift(int a, int b)
    {
        // a<=b；f(x) <- min_{x-b<=y<=x-a} f(y)，即左半段右移 a、右半段右移 b；a==b 时为平移。
        assert(a <= b); // 调试检查，可删。
        dl += a;
        dr += b;
    }

    void prefixMin()
    {
        // f(x) <- min_{y<=x} f(y)：去掉右侧上升段。
        r = {};
    }

    void suffixMin()
    {
        // f(x) <- min_{y>=x} f(y)：去掉左侧下降段。
        l = {};
    }

    pair<optional<int>, optional<int>> argmin() const
    {
        // 返回最小值区间的左右端点，无界一侧为空。
        optional<int> x, y;
        if (!l.empty())
        {
            x = l.top() + dl;
        }
        if (!r.empty())
        {
            y = r.top() + dr;
        }
        return {x, y};
    }

    int eval(int x) const
    {
        // x 是查询位置；返回 f(x)。复制并逐个弹堆，s 个拐点耗时 O(s log(s+1))、额外空间 O(s)。
        auto a = l;
        auto b = r;
        int ans = mn;
        for (; !a.empty(); a.pop())
        {
            ans += max<int>(0, a.top() + dl - x);
        }
        for (; !b.empty(); b.pop())
        {
            ans += max<int>(0, x - b.top() - dr);
        }
        return ans;
    }
};
