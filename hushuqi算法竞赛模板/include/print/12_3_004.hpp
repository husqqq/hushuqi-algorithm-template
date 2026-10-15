#pragma once

#include "12_1_001.hpp"

template <class Acc = long long>
struct MonoHull
{
    // 当前凸壳按最小函数值维护。改求最大值可在 k、b 与函数值都能安全取反时同时取反，
    // 继续调用本结构，最后把非空 query 结果取反；否则必须完整反向交点与比较判定。
    using U128 = unsigned __int128;

    struct Line
    {
        // k、b 是直线 y=kx+b 的斜率和截距。
        int k;
        int b;

        Acc get(int x) const
        {
            // x 是查询横坐标；返回直线在 x 处的函数值。
            return (Acc)k * x + b;
        }
    };
    // q 是按交点顺序排列的下凸壳，last 是上一次查询横坐标。
    deque<Line> q;
    optional<int> last;

    static pair<bool, U128> product(int a, int b, int c, int d)
    {
        // 返回 (a-b)(c-d) 的符号与绝对值，true 表示负数。
        auto diff = [](int x, int y)
        {
            if (x >= y)
            {
                return pair{false, (unsigned long long)x - (unsigned long long)y};
            }
            return pair{true, (unsigned long long)y - (unsigned long long)x};
        };
        auto x = diff(a, b), y = diff(c, d);
        return {x.first != y.first, (U128)x.second * y.second};
    }

    static bool less(pair<bool, U128> a, pair<bool, U128> b)
    {
        // 比较两个符号-绝对值表示的整数。
        if (a.first != b.first)
        {
            return a.first;
        }
        return a.first ? a.second > b.second : a.second < b.second;
    }

    void add(int k, int b)
    {
        // k、b 是新直线的斜率和截距；要求斜率按不增顺序加入。
        assert(q.empty() || k <= q.back().k); // 调试检查，可删。
        if (!q.empty() && k == q.back().k)
        {
            if (q.back().b <= b)
            {
                return;
            }
            q.pop_back();
        }
        Line c{k, b};
        while (q.size() >= 2)
        {
            Line a = q[q.size() - 2];
            Line d = q.back();
            auto l = product(d.b, a.b, d.k, c.k);
            auto r = product(c.b, d.b, a.k, d.k);
            if (less(l, r))
            {
                break;
            }
            q.pop_back();
        }
        q.push_back(c);
    }

    optional<Acc> query(int x)
    {
        // x 是不小于上次查询的横坐标；无直线时返回空，否则返回最小函数值。
        assert(!last || *last <= x); // 调试检查，可删。
        last = x;
        while (q.size() >= 2 && q[1].get(x) <= q[0].get(x))
        {
            q.pop_front();
        }
        if (q.empty())
        {
            return nullopt;
        }
        return q.front().get(x);
    }
};
