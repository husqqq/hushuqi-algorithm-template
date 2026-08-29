#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <class T> struct Rectangle
{
    // x1、y1、x2、y2 依次是矩形的左、下、右、上边界。
    T x1, y1, x2, y2;
};

template <class T, class Acc = T> class RectUnion
{
    struct Event
    {
        // x 是扫描线横坐标，y1、y2 是修改的纵坐标区间，d 为加入 1 或删除 -1。
        T x, y1, y2;
        int d;
    };

    // ys 保存离散后的纵坐标；下标区间 [l,r) 对应实际区间 [ys[l],ys[r])。
    vector<T> ys;
    // cov、seg、lc、rc 分别保存覆盖标记、覆盖段数和左右端覆盖状态。
    vector<int> cov, seg, lc, rc;
    // len 保存每个节点的实际覆盖长度，类型由 Acc 指定。
    vector<Acc> len;

    void pull(int p, int l, int r)
    {
        // p 是节点编号，[l,r) 是纵坐标下标区间；由覆盖标记和儿子信息更新节点。
        if (cov[p])
        {
            len[p] = (Acc)ys[r] - (Acc)ys[l];
            seg[p] = lc[p] = rc[p] = 1;
        }
        else if (r - l == 1)
        {
            len[p] = 0;
            seg[p] = lc[p] = rc[p] = 0;
        }
        else
        {
            int ls = p * 2, rs = ls + 1;
            len[p] = len[ls] + len[rs];
            seg[p] = seg[ls] + seg[rs] - (rc[ls] && lc[rs]);
            lc[p] = lc[ls];
            rc[p] = rc[rs];
        }
    }

    void add(int p, int l, int r, int ql, int qr, int d)
    {
        // p、[l,r) 是当前节点，[ql,qr) 是修改区间，d 是覆盖次数增量。
        if (ql <= l && r <= qr)
        {
            cov[p] += d;
            pull(p, l, r);
            return;
        }
        int m = midpoint(l, r);
        if (ql < m)
        {
            add(p * 2, l, m, ql, qr, d);
        }
        if (m < qr)
        {
            add(p * 2 + 1, m, r, ql, qr, d);
        }
        pull(p, l, r);
    }

    pair<Acc, Acc> calc(const vector<Rectangle<T>> &a)
    {
        // a 是输入矩形集合；返回值 first、second 依次为面积并和周长并。
        vector<Event> ev;
        ys.clear();
        for (auto [x1, y1, x2, y2] : a)
        {
            if (x1 >= x2 || y1 >= y2)
            {
                continue;
            }
            ev.push_back({x1, y1, y2, 1});
            ev.push_back({x2, y1, y2, -1});
            ys.push_back(y1);
            ys.push_back(y2);
        }
        if (ev.empty())
        {
            return {};
        }
        ranges::sort(ys);
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        ranges::sort(ev, {}, &Event::x);
        int n = (int)ys.size() * 4;
        cov.assign(n, 0);
        seg.assign(n, 0);
        lc.assign(n, 0);
        rc.assign(n, 0);
        len.assign(n, 0);

        Acc ar = 0, peri = 0;
        T px = ev[0].x;
        for (int i = 0; i < (int)ev.size();)
        {
            int j = i;
            while (j < (int)ev.size() && ev[j].x == ev[i].x)
            {
                j++;
            }
            Acc dx = (Acc)ev[i].x - (Acc)px;
            ar += len[1] * dx;
            peri += (Acc)2 * seg[1] * dx;

            // 同一横坐标先加入再删除，逐次长度变化之和恰为新旧覆盖集合的对称差长度。
            for (int d : {1, -1})
            {
                for (int k = i; k < j; k++)
                {
                    if (ev[k].d != d)
                    {
                        continue;
                    }
                    Acc old = len[1];
                    int l = lower_bound(ys.begin(), ys.end(), ev[k].y1) - ys.begin();
                    int r = lower_bound(ys.begin(), ys.end(), ev[k].y2) - ys.begin();
                    add(1, 0, (int)ys.size() - 1, l, r, d);
                    Acc delta = len[1] - old;
                    peri += delta < 0 ? -delta : delta;
                }
            }
            px = ev[i].x;
            i = j;
        }
        return {ar, peri};
    }

  public:
    Acc area(const vector<Rectangle<T>> &a)
    {
        // a 是输入矩形集合；返回全部轴平行矩形并集的面积。
        return calc(a).first;
    }

    Acc perimeter(const vector<Rectangle<T>> &a)
    {
        // a 是输入矩形集合；返回全部轴平行矩形并集的周长。
        return calc(a).second;
    }

    Acc solve(const vector<Rectangle<T>> &a)
    {
        // a 是输入矩形集合；兼容旧接口并返回矩形并集面积。
        return area(a);
    }
};
