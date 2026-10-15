#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class Coord = long long, class T = long long>
struct KDRectAffine
{
    struct Node
    {
        Coord xl{}, xr{}, yl{}, yr{}; // 子树内全部点的坐标包围盒（闭区间）。
        int cnt = 0; // cnt 是子树内已激活点数。
        T sum = 0, mul = 1, add = 0; // sum 是激活点权值和，mul、add 是未下传的 w->mul*w+add 标记。
    };

    int n; // n 是预收集的点数。
    vector<Coord> x, y; // x[id]、y[id] 是第 id 个点的坐标。
    vector<int> ord, pos; // ord 是建树时的点序，pos[id] 是点 id 所在叶子的节点编号。
    vector<Node> tr; // tr 是按堆式编号存放的 KD 树节点。

    KDRectAffine(vector<Coord> x, vector<Coord> y, const vector<optional<T>> &w)
        : n(x.size()), x(move(x)), y(move(y)), ord(n), pos(n), tr(max<int>(1, 4 * n))
    {
        // x、y 是全部已知点坐标，w[id] 为空表示该点尚未激活。
        assert((int)this->y.size() == n && (int)w.size() == n); // 调试检查，可删。
        iota(ord.begin(), ord.end(), 0);
        if (n)
        {
            build(1, 0, n, 0, w);
        }
    }

    void pull(int k)
    {
        // k 是内部节点；由两个儿子重算包围盒、激活点数与权值和。
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
        // k 管辖 ord[l..r)，d 是切分维度（0 按 x、1 按 y），w 是初始权值；递归建 KD 树。
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
        // k 是节点；对其子树内激活点执行 w=a*w+b，并复合到延迟标记。
        tr[k].sum = a * tr[k].sum + b * T(tr[k].cnt);
        tr[k].mul = a * tr[k].mul;
        tr[k].add = a * tr[k].add + b;
    }

    void push(int k)
    {
        // k 是内部节点；把延迟标记下传给两个儿子。
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
        // 返回节点 k 的包围盒是否与半开矩形 [xl,xr)x[yl,yr) 不相交。
        return xr <= tr[k].xl || tr[k].xr < xl || yr <= tr[k].yl || tr[k].yr < yl;
    }

    bool covered(int k, Coord xl, Coord xr, Coord yl, Coord yr) const
    {
        // 返回节点 k 的包围盒是否完全落在半开矩形 [xl,xr)x[yl,yr) 内。
        return xl <= tr[k].xl && tr[k].xr < xr && yl <= tr[k].yl && tr[k].yr < yr;
    }

    T query(int k, Coord xl, Coord xr, Coord yl, Coord yr)
    {
        // k 是当前节点；返回其子树内落在半开矩形中的激活点权值和。
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
        // k 是当前节点；对其子树内落在半开矩形中的激活点执行 w=a*w+b。
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
        // id 是预收集点编号，w 是当前新权值；激活或覆盖该点。
        assert(0 <= id && id < n); // 调试检查，可删。
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
        // 四个边界给出半开矩形；返回全部激活点权值和。
        assert(xl <= xr && yl <= yr); // 调试检查，可删。
        return n ? query(1, xl, xr, yl, yr) : T(0);
    }

    void apply(Coord xl, Coord xr, Coord yl, Coord yr, T a, T b)
    {
        // 四个边界给出半开矩形；对其中激活点执行 w=a*w+b。
        assert(xl <= xr && yl <= yr); // 调试检查，可删。
        if (n)
        {
            apply(1, xl, xr, yl, yr, a, b);
        }
    }
};
