#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T, class C = T> class SegLiChao
{
  public:
    static_assert(is_floating_point_v<T> || (is_integral_v<T> && is_signed_v<T>));

    struct Line
    {
        // k、b 是一次函数 y=kx+b 的斜率与截距。
        T k{}, b{};

        C get(T x) const
        {
            // x 是查询横坐标；用比较类型 C 计算函数值。
            return (C)k * x + b;
        }
    };

  private:
    // xs 是全部允许查询的横坐标，tr[p] 是节点 p 上在中点占优且整段有效的直线。
    vector<T> xs;
    vector<optional<Line>> tr;

    void addLine(int p, int l, int r, Line f)
    {
        // p 是节点编号，[l,r) 是横坐标下标段，f 已在整段内有效；把 f 加入该段。
        if (!tr[p])
        {
            tr[p] = f;
            return;
        }
        int m = midpoint(l, r);
        bool lef = f.get(xs[l]) < tr[p]->get(xs[l]);
        bool mid = f.get(xs[m]) < tr[p]->get(xs[m]);
        if (mid)
        {
            swap(f, *tr[p]);
        }
        if (r - l == 1)
        {
            return;
        }
        if (lef != mid)
        {
            addLine(p << 1, l, m, f);
        }
        else
        {
            addLine(p << 1 | 1, m, r, f);
        }
    }

    void addSegment(int p, int l, int r, int ql, int qr, Line f)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是 f 有效的离散下标段；插入二者交集。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            addLine(p, l, r, f);
            return;
        }
        int m = midpoint(l, r);
        addSegment(p << 1, l, m, ql, qr, f);
        addSegment(p << 1 | 1, m, r, ql, qr, f);
    }

  public:
    SegLiChao(vector<T> xs) : xs(move(xs))
    {
        // xs 是全部可能查询的横坐标；排序去重并建立最小值线段李超树。
        sort(this->xs.begin(), this->xs.end());
        this->xs.erase(unique(this->xs.begin(), this->xs.end()), this->xs.end());
        assert(!this->xs.empty()); // 调试检查，可删
        tr.resize(4 * this->xs.size());
    }

    void add(Line f, T l, T r)
    {
        // f 是待插入直线，[l,r) 是其按真实横坐标定义的有效半开区间。
        assert(l < r); // 调试检查，可删
        int ql = lower_bound(xs.begin(), xs.end(), l) - xs.begin();
        int qr = lower_bound(xs.begin(), xs.end(), r) - xs.begin();
        if (ql < qr)
        {
            addSegment(1, 0, xs.size(), ql, qr, f);
        }
    }

    optional<C> query(T x) const
    {
        // x 必须在构造坐标中；返回所有在 x 有效直线的最小值，无线时返回空。
        int q = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        assert(q < (int)xs.size() && xs[q] == x); // 调试检查，可删
        optional<C> ans;
        int p = 1, l = 0, r = xs.size();
        while (true)
        {
            if (tr[p])
            {
                C y = tr[p]->get(x);
                if (!ans || y < *ans)
                {
                    ans = y;
                }
            }
            if (r - l == 1)
            {
                break;
            }
            int m = midpoint(l, r);
            if (q < m)
            {
                p <<= 1;
                r = m;
            }
            else
            {
                p = p << 1 | 1;
                l = m;
            }
        }
        return ans;
    }
};
