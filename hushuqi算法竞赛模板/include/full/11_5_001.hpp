#pragma once

#include "11_1_003.hpp"

template <class T> struct ClosestPair
{
    struct Q
    {
        Point<T> p;
        int id;
    };

    vector<Q> a;
    vector<Q> b;
    T ans;
    pair<int, int> ids;

    void upd(Q x, Q y)
    {
        T d = norm(x.p - y.p);
        if (d < ans)
        {
            ans = d;
            ids = {x.id, y.id};
        }
    }

    void solve(int l, int r)
    {
        if (r - l <= 3)
        {
            for (int i = l; i < r; i++)
            {
                for (int j = l; j < i; j++)
                {
                    upd(a[i], a[j]);
                }
            }
            sort(a.begin() + l, a.begin() + r, [](Q x, Q y)
                 {
                     return x.p.y < y.p.y;
                 });
            return;
        }
        int m = (l + r) / 2;
        T x = a[m].p.x;
        solve(l, m);
        solve(m, r);
        merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r,
              b.begin() + l, [](Q x, Q y)
              {
                  return x.p.y < y.p.y;
              });
        copy(b.begin() + l, b.begin() + r, a.begin() + l);
        vector<Q> s;
        for (int i = l; i < r; i++)
        {
            if ((a[i].p.x - x) * (a[i].p.x - x) > ans)
            {
                continue;
            }
            for (int j = (int)s.size() - 1; j >= 0; j--)
            {
                if ((a[i].p.y - s[j].p.y) * (a[i].p.y - s[j].p.y) > ans)
                {
                    break;
                }
                upd(a[i], s[j]);
            }
            s.push_back(a[i]);
        }
    }

    optional<pair<T, pair<int, int>>> work(const vector<Point<T>> &p)
    {
        if (p.size() < 2)
        {
            return nullopt;
        }
        a.resize(p.size());
        b.resize(p.size());
        for (int i = 0; i < (int)p.size(); i++)
        {
            a[i] = {p[i], i};
        }
        sort(a.begin(), a.end(), [](Q x, Q y)
             {
                 return x.p.x != y.p.x ? x.p.x < y.p.x : x.p.y < y.p.y;
             });
        for (int i = 1; i < (int)a.size(); i++)
        {
            if (a[i - 1].p == a[i].p)
            {
                return pair<T, pair<int, int>>{0, {a[i - 1].id, a[i].id}};
            }
        }
        ans = norm(a[0].p - a[1].p);
        ids = {a[0].id, a[1].id};
        solve(0, (int)a.size());
        return pair<T, pair<int, int>>{ans, ids};
    }
};
