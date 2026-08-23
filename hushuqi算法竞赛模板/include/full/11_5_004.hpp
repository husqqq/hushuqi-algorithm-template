#pragma once

#include "11_1_001.hpp"

template <class T> struct Rectangle
{
    T x1, y1, x2, y2;
};

template <class T, class Acc = T> class RectUnion
{
    struct Event
    {
        T x, y1, y2;
        int d;
    };

    vector<T> ys;
    vector<int> cov, seg, lc, rc;
    vector<Acc> len;

    void pull(int p, int l, int r)
    {
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
            int ls = p * 2;
            int rs = ls + 1;
            len[p] = len[ls] + len[rs];
            seg[p] = seg[ls] + seg[rs] - (rc[ls] && lc[rs]);
            lc[p] = lc[ls];
            rc[p] = rc[rs];
        }
    }

    void add(int p, int l, int r, int ql, int qr, int d)
    {
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
        Acc ar = 0;
        Acc peri = 0;
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
        return calc(a).first;
    }

    Acc perimeter(const vector<Rectangle<T>> &a)
    {
        return calc(a).second;
    }

    Acc solve(const vector<Rectangle<T>> &a)
    {
        return area(a);
    }
};
