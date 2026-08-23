#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Coord = long long, class T = long long>
struct KDRectAffine
{
    struct Node
    {
        Coord xl{}, xr{}, yl{}, yr{};
        int cnt = 0;
        T sum = 0, mul = 1, add = 0;
    };

    int n;
    vector<Coord> x, y;
    vector<int> ord, pos;
    vector<Node> tr;

    KDRectAffine(vector<Coord> x, vector<Coord> y, const vector<optional<T>> &w)
        : n(x.size()), x(move(x)), y(move(y)), ord(n), pos(n), tr(max<int>(1, 4 * n))
    {
        assert((int)this->y.size() == n && (int)w.size() == n);
        iota(ord.begin(), ord.end(), 0);
        if (n)
        {
            build(1, 0, n, 0, w);
        }
    }

    void pull(int k)
    {
        auto &a = tr[2 * k];
        auto &b = tr[2 * k + 1];
        tr[k].xl = min(a.xl, b.xl);
        tr[k].xr = max(a.xr, b.xr);
        tr[k].yl = min(a.yl, b.yl);
        tr[k].yr = max(a.yr, b.yr);
        tr[k].cnt = a.cnt + b.cnt;
        tr[k].sum = a.sum + b.sum;
    }

    void build(int k, int l, int r, int d, const vector<optional<T>> &w)
    {
        if (r - l == 1)
        {
            int id = ord[l];
            pos[id] = k;
            tr[k].xl = tr[k].xr = x[id];
            tr[k].yl = tr[k].yr = y[id];
            if (w[id])
            {
                tr[k].cnt = 1;
                tr[k].sum = *w[id];
            }
            return;
        }
        int m = (l + r) / 2;
        nth_element(ord.begin() + l, ord.begin() + m, ord.begin() + r, [&](int a, int b)
        {
            return d == 0 ? x[a] < x[b] : y[a] < y[b];
        });
        build(2 * k, l, m, d ^ 1, w);
        build(2 * k + 1, m, r, d ^ 1, w);
        pull(k);
    }

    void applyAt(int k, T a, T b)
    {
        tr[k].sum = a * tr[k].sum + b * T(tr[k].cnt);
        tr[k].mul = a * tr[k].mul;
        tr[k].add = a * tr[k].add + b;
    }

    void push(int k)
    {
        if (tr[k].mul == T(1) && tr[k].add == T(0))
        {
            return;
        }
        applyAt(2 * k, tr[k].mul, tr[k].add);
        applyAt(2 * k + 1, tr[k].mul, tr[k].add);
        tr[k].mul = T(1);
        tr[k].add = T(0);
    }

    bool disjoint(int k, Coord xl, Coord xr, Coord yl, Coord yr) const
    {
        return xr <= tr[k].xl || tr[k].xr < xl || yr <= tr[k].yl || tr[k].yr < yl;
    }

    bool covered(int k, Coord xl, Coord xr, Coord yl, Coord yr) const
    {
        return xl <= tr[k].xl && tr[k].xr < xr && yl <= tr[k].yl && tr[k].yr < yr;
    }

    T query(int k, Coord xl, Coord xr, Coord yl, Coord yr)
    {
        if (disjoint(k, xl, xr, yl, yr))
        {
            return T(0);
        }
        if (covered(k, xl, xr, yl, yr))
        {
            return tr[k].sum;
        }
        push(k);
        return query(2 * k, xl, xr, yl, yr) + query(2 * k + 1, xl, xr, yl, yr);
    }

    void apply(int k, Coord xl, Coord xr, Coord yl, Coord yr, T a, T b)
    {
        if (disjoint(k, xl, xr, yl, yr))
        {
            return;
        }
        if (covered(k, xl, xr, yl, yr))
        {
            applyAt(k, a, b);
            return;
        }
        push(k);
        apply(2 * k, xl, xr, yl, yr, a, b);
        apply(2 * k + 1, xl, xr, yl, yr, a, b);
        pull(k);
    }

    void set(int id, T w)
    {
        assert(0 <= id && id < n);
        int k = pos[id];
        vector<int> path;
        for (int z = k / 2; z; z /= 2)
        {
            path.push_back(z);
        }
        reverse(path.begin(), path.end());
        for (int z : path)
        {
            push(z);
        }
        tr[k].cnt = 1;
        tr[k].sum = w;
        tr[k].mul = T(1);
        tr[k].add = T(0);
        for (k /= 2; k; k /= 2)
        {
            pull(k);
        }
    }

    T query(Coord xl, Coord xr, Coord yl, Coord yr)
    {
        assert(xl <= xr && yl <= yr);
        return n ? query(1, xl, xr, yl, yr) : T(0);
    }

    void apply(Coord xl, Coord xr, Coord yl, Coord yr, T a, T b)
    {
        assert(xl <= xr && yl <= yr);
        if (n)
        {
            apply(1, xl, xr, yl, yr, a, b);
        }
    }
};
