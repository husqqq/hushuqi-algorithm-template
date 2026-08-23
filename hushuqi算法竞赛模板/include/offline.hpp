#pragma once

#include "sort_sequence.hpp"

// MoQuery[0..2] 依次表示左端点、右端点、答案编号。
using MoQuery = array<int, 3>;

template <class Ans, class AddL, class AddR, class DelL, class DelR, class Get>
vector<Ans> mo(int n, vector<MoQuery> qs, AddL addL, AddR addR, DelL delL, DelR delR, Get get)
{
    // n 是数组长度，qs 是询问；四个端点回调维护状态，get 返回当前答案。
    auto z = max<int>(1, (int)(n / sqrtl(max<int>(1, qs.size()))));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto x = a[0] / z, y = b[0] / z;
             if (x != y)
             {
                 return x < y;
             }
             return x & 1 ? a[1] > b[1] : a[1] < b[1];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0;
    for (auto q : qs)
    {
        while (l > q[0])
        {
            addL(--l);
        }
        while (r < q[1])
        {
            addR(r++);
        }
        while (l < q[0])
        {
            delL(l++);
        }
        while (r > q[1])
        {
            delR(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}

// ModifyQuery[0..3] 依次表示左端点、右端点、答案编号、修改时间。
using ModifyQuery = array<int, 4>;

template <class Ans, class Add, class Del, class Change, class Get>
vector<Ans> moModify(int n, vector<ModifyQuery> qs, Add add, Del del, Change change, Get get)
{
    // n 是数组长度，qs 含修改时间；add/del 维护端点，change(p,forward,l,r) 切换第 p 次修改，get 返回答案。
    auto z = max<int>(1, (int)powl(max<int>(1, n), 2.0L / 3));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto al = a[0] / z, bl = b[0] / z;
             if (al != bl)
             {
                 return al < bl;
             }
             auto ar = a[1] / z, br = b[1] / z;
             if (ar != br)
             {
                 return al & 1 ? ar > br : ar < br;
             }
             return ar & 1 ? a[3] > b[3] : a[3] < b[3];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0, t = 0;
    for (auto q : qs)
    {
        while (t < q[3])
        {
            change(t++, true, l, r);
        }
        while (t > q[3])
        {
            change(--t, false, l, r);
        }
        while (l > q[0])
        {
            add(--l);
        }
        while (r < q[1])
        {
            add(r++);
        }
        while (l < q[0])
        {
            del(l++);
        }
        while (r > q[1])
        {
            del(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}

template <class Ans, class Add, class Snap, class Undo, class Get>
vector<Ans> rollbackMo(int n, vector<MoQuery> qs, Add add, Snap snap, Undo undo, Get get)
{
    // n 是数组长度，qs 是询问；add 加入位置，snap/undo 管理快照，get 返回当前答案。
    auto z = max<int>(1, (int)sqrtl(max<int>(1, n)));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto x = a[0] / z, y = b[0] / z;
             return x != y ? x < y : a[1] < b[1];
         });
    vector<Ans> ans(qs.size());
    auto empty = snap();
    for (int i = 0; i < (int)qs.size();)
    {
        auto k = qs[i][0] / z;
        auto j = i;
        while (j < (int)qs.size() && qs[j][0] / z == k)
        {
            j++;
        }
        auto mid = min<int>(n, (k + 1) * z);
        auto r = mid;
        for (auto p = i; p < j; p++)
        {
            auto q = qs[p];
            if (q[1] <= mid)
            {
                auto s = snap();
                for (auto x = q[0]; x < q[1]; x++)
                {
                    add(x);
                }
                ans[q[2]] = get();
                undo(s);
                continue;
            }
            while (r < q[1])
            {
                add(r++);
            }
            auto s = snap();
            for (auto x = mid - 1; x >= q[0]; x--)
            {
                add(x);
            }
            ans[q[2]] = get();
            undo(s);
        }
        undo(empty);
        i = j;
    }
    return ans;
}

vector<int> xorPairs(const vector<int> &a, const vector<pair<int, int>> &qs, int k, int bits)
{
    // a 是 bits 位非负整数，qs 是半开区间，k 是目标异或位数；返回各区间点对数。
    assert(0 <= bits && bits <= 20); // 调试检查，可删
    auto n = (int)a.size();
    auto lim = 1LL << bits;
    for (auto x : a)
    {
        assert(0 <= x && x < lim); // 调试检查，可删
        (void)x;
    }
    vector<int> ans(qs.size());
    if (k < 0 || k > bits)
    {
        return ans;
    }

    // Query[0..2] 依次表示从 1 开始的左端点、右端点、原询问编号。
    using Query = array<int, 3>;
    vector<Query> ord;
    for (int id = 0; id < (int)qs.size(); id++)
    {
        auto [l, r] = qs[id];
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l < r)
        {
            ord.push_back({l + 1, r, id});
        }
    }
    if (ord.empty())
    {
        return ans;
    }

    auto z = max<int>(1, (int)(n / sqrtl((long double)ord.size())));
    sort(ord.begin(),
         ord.end(),
         [&](const auto &x, const auto &y)
         {
             auto bx = (x[0] - 1) / z, by = (y[0] - 1) / z;
             if (bx != by)
             {
                 return bx < by;
             }
             return bx & 1 ? x[1] > y[1] : x[1] < y[1];
         });

    vector<int> masks;
    for (int s = 0; s < lim; s++)
    {
        if (popcount((unsigned long long)s) == k)
        {
            masks.push_back(s);
        }
    }
    vector<int> v(n + 1), cnt(lim), pre(n + 1);
    copy(a.begin(), a.end(), v.begin() + 1);
    for (int i = 1; i <= n; i++)
    {
        pre[i] = pre[i - 1];
        for (auto s : masks)
        {
            pre[i] += cnt[v[i] ^ s];
        }
        cnt[v[i]]++;
    }

    // Event[0..3] 依次表示左端点、右端点、贡献符号、排序后询问编号。
    using Event = array<int, 4>;
    vector<vector<Event>> ev(n + 1);
    vector<int> cur(ord.size());
    int l = 1, r = 0;
    for (int id = 0; id < (int)ord.size(); id++)
    {
        auto q = ord[id];
        cur[id] = pre[q[1]] - pre[r] + pre[q[0] - 1] - pre[l - 1];
        if (!k)
        {
            cur[id] += q[0] - l;
        }
        if (r < q[1])
        {
            ev[l - 1].push_back({r + 1, q[1], -1, id});
            r = q[1];
        }
        if (l > q[0])
        {
            ev[r].push_back({q[0], l - 1, 1, id});
            l = q[0];
        }
        if (r > q[1])
        {
            ev[l - 1].push_back({q[1] + 1, r, 1, id});
            r = q[1];
        }
        if (l < q[0])
        {
            ev[r].push_back({l, q[0] - 1, -1, id});
            l = q[0];
        }
    }

    fill(cnt.begin(), cnt.end(), 0);
    for (int i = 1; i <= n; i++)
    {
        for (auto s : masks)
        {
            cnt[v[i] ^ s]++;
        }
        for (auto e : ev[i])
        {
            int val = 0;
            for (int j = e[0]; j <= e[1]; j++)
            {
                val += cnt[v[j]];
            }
            cur[e[3]] += e[2] * val;
        }
    }
    for (int i = 1; i < (int)cur.size(); i++)
    {
        cur[i] += cur[i - 1];
    }
    for (int i = 0; i < (int)ord.size(); i++)
    {
        ans[ord[i][2]] = cur[i];
    }
    return ans;
}

vector<int> distinct(const vector<int> &a, const vector<pair<int, int>> &qs)
{
    // a 是静态数组，qs 是半开区间；返回每个区间中的不同值数量。
    // 只有下标、排名和频次的热点数据固定用 32 位，避免全局 int 宏加倍工作集。
    vector<int> val = a;
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());
    vector<int32_t> rank(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        rank[i] = (int32_t)(lower_bound(val.begin(), val.end(), a[i]) - val.begin());
    }
    // Ask[0..1] 依次表示左端点与答案编号，按右端点分桶。
    using Ask = array<int32_t, 2>;
    vector<vector<Ask>> ask(a.size() + 1);
    for (int i = 0; i < (int)qs.size(); i++)
    {
        auto [l, r] = qs[i];
        assert(0 <= l && l <= r && r <= (int)a.size()); // 调试检查，可删
        assert(r <= numeric_limits<int32_t>::max());    // 调试检查，可删
        ask[r].push_back({(int32_t)l, (int32_t)i});
    }
    vector<int32_t> bit(a.size() + 1), last(val.size(), -1);
    auto add = [&](int p, int delta)
    {
        // p 是从 0 开始的位置，delta 是出现位置标记的变化。
        for (p++; p < (int)bit.size(); p += p & -p)
        {
            bit[p] += (int32_t)delta;
        }
    };
    auto sum = [&](int p)
    {
        // p 是半开前缀右端点；返回 [0,p) 的最后出现位置标记数。
        int ans = 0;
        for (; p; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    };
    vector<int> ans(qs.size());
    for (int r = 0; r <= (int)a.size(); r++)
    {
        if (r)
        {
            auto x = rank[r - 1];
            if (last[x] >= 0)
            {
                add(last[x], -1);
            }
            add(r - 1, 1);
            last[x] = r - 1;
        }
        for (auto [l, id] : ask[r])
        {
            ans[id] = sum(r) - sum(l);
        }
    }
    return ans;
}

// KthQuery[0..2] 依次表示左端点、右端点、从 1 开始的排名。
using KthQuery = array<int, 3>;

vector<int> rangeKth(const vector<int> &a, vector<KthQuery> qs)
{
    // a 是静态数组，qs 中每项是 {l,r,k}；返回各半开区间第 k 小值。
    if (qs.empty())
    {
        return {};
    }
    assert(!a.empty()); // 调试检查，可删
    for (auto q : qs)
    {
        assert(0 <= q[0] && q[0] < q[1] && q[1] <= (int)a.size()); // 调试检查，可删
        assert(1 <= q[2] && q[2] <= q[1] - q[0]);                  // 调试检查，可删
    }
    Discrete<int> dis(a);
    vector<int> rk(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        rk[i] = dis.id(a[i]);
    }
    vector<int> es(a.size()), ids(qs.size()), ans(qs.size());
    iota(es.begin(), es.end(), 0);
    iota(ids.begin(), ids.end(), 0);
    Fenwick bit(a.size());
    auto solve = [&](auto &&self, int vl, int vr, vector<int> ps, vector<int> qi) -> void
    {
        if (qi.empty())
        {
            return;
        }
        if (vr - vl == 1)
        {
            for (auto id : qi)
            {
                ans[id] = dis.value(vl);
            }
            return;
        }
        auto vm = midpoint(vl, vr);
        vector<int> pl, pr, ql, qr;
        for (auto p : ps)
        {
            if (rk[p] < vm)
            {
                bit.add(p, 1);
                pl.push_back(p);
            }
            else
            {
                pr.push_back(p);
            }
        }
        for (auto id : qi)
        {
            auto &q = qs[id];
            auto c = bit.sum(q[0], q[1]);
            if (q[2] <= c)
            {
                ql.push_back(id);
            }
            else
            {
                q[2] -= c;
                qr.push_back(id);
            }
        }
        for (auto p : pl)
        {
            bit.add(p, -1);
        }
        self(self, vl, vm, move(pl), move(ql));
        self(self, vm, vr, move(pr), move(qr));
    };
    solve(solve, 0, dis.a.size(), move(es), move(ids));
    return ans;
}

struct TreeMo
{
    int n = 0;                     // n 是树的顶点数。
    int lg = 1;                    // lg 是倍增祖先表的层数。
    int blk = 1;                   // blk 是当前询问排序的块长。
    vector<vector<int>> g;         // g 是树的邻接表。
    vector<int> col;               // col[u] 是顶点 u 离散到 [0,n) 的权值。
    vector<int> st;                // st[u] 是顶点 u 进入欧拉序的位置。
    vector<int> ed;                // ed[u] 是顶点 u 离开欧拉序的位置。
    vector<int> seq;               // seq 是每个顶点进出各出现一次的欧拉序。
    vector<int> dep;               // dep[u] 是顶点 u 到当前根的边数。
    vector<vector<int>> up;        // up[k][u] 是顶点 u 的第 2^k 级祖先。
    vector<array<int, 4>> qs;      // 每项是 {欧拉左端、欧拉右端、额外 LCA、答案编号}。

    explicit TreeMo(int m) : n(m), g(m), col(m), st(m), ed(m), dep(m)
    {
        // m 是非空树的顶点数。
        assert(m > 0); // 调试检查，可删
    }

    void addTreeEdge(int u, int v)
    {
        // u、v 是无向树边的两个端点；调用后把边加入邻接表。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 0)
    {
        // root 是本次选定的根；重建欧拉序和祖先表，并清空依赖旧序的询问。
        assert(0 <= root && root < n); // 调试检查，可删
        lg = 1;
        while ((1ULL << lg) < (unsigned long long)n)
        {
            lg++;
        }
        up.assign(lg, vector<int>(n, root));
        fill(dep.begin(), dep.end(), 0);
        seq.clear();
        seq.reserve((size_t)n * 2);
        qs.clear();

        st[root] = 0;
        seq.push_back(root);
        auto dfs = [&](auto &&self, int u, int p) -> void
        {
            for (auto v : g[u])
            {
                if (v == p)
                {
                    continue;
                }
                up[0][v] = u;
                dep[v] = dep[u] + 1;
                st[v] = seq.size();
                seq.push_back(v);
                self(self, v, u);
            }
            ed[u] = seq.size();
            seq.push_back(u);
        };
        dfs(dfs, root, -1);
        for (int k = 1; k < lg; k++)
        {
            for (int u = 0; u < n; u++)
            {
                up[k][u] = up[k - 1][up[k - 1][u]];
            }
        }
    }

    int lca(int u, int v) const
    {
        // u、v 是树上顶点；返回它们在当前根下的最近公共祖先。
        assert(!up.empty() && 0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        auto d = dep[u] - dep[v];
        for (int k = 0; k < lg; k++)
        {
            if (d >> k & 1)
            {
                u = up[k][u];
            }
        }
        if (u == v)
        {
            return u;
        }
        for (int k = lg - 1; k >= 0; k--)
        {
            if (up[k][u] != up[k][v])
            {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }

    void addQuery(int u, int v)
    {
        // u、v 是路径两端；把该路径转成半开欧拉区间并按加入顺序编号。
        assert(!up.empty() && 0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        if (st[u] > st[v])
        {
            swap(u, v);
        }
        auto w = lca(u, v);
        auto id = (int)qs.size();
        if (w == u)
        {
            qs.push_back({st[u], st[v] + 1, -1, id});
        }
        else
        {
            qs.push_back({ed[u], st[v] + 1, w, id});
        }
    }

    vector<int> solve()
    {
        // 返回每条已加入路径上的不同权值数，顺序与 addQuery 一致。
        if (qs.empty())
        {
            return {};
        }
        for (auto x : col)
        {
            assert(0 <= x && x < n); // 调试检查，可删
        }
        auto m = (int)qs.size();
        blk = max<int>(1, (int)((long double)seq.size() / sqrtl(m)));
        sort(qs.begin(),
             qs.end(),
             [&](const auto &a, const auto &b)
             {
                 auto x = a[0] / blk;
                 auto y = b[0] / blk;
                 if (x != y)
                 {
                     return x < y;
                 }
                 return x & 1 ? a[1] > b[1] : a[1] < b[1];
             });
        vector<int> cnt(n);
        vector<int> used(n);
        vector<int> ans(m);
        int cur = 0;
        auto work = [&](int u)
        {
            // u 是本次切换的顶点；奇数次出现计入路径，偶数次出现移出路径。
            used[u] ^= 1;
            if (used[u])
            {
                if (cnt[col[u]]++ == 0)
                {
                    cur++;
                }
            }
            else
            {
                if (--cnt[col[u]] == 0)
                {
                    cur--;
                }
            }
        };
        int l = 0;
        int r = 0;
        for (auto q : qs)
        {
            auto ql = q[0];
            auto qr = q[1];
            auto w = q[2];
            auto id = q[3];
            while (l > ql)
            {
                work(seq[--l]);
            }
            while (r < qr)
            {
                work(seq[r++]);
            }
            while (l < ql)
            {
                work(seq[l++]);
            }
            while (r > qr)
            {
                work(seq[--r]);
            }
            if (w >= 0)
            {
                work(w);
            }
            ans[id] = cur;
            if (w >= 0)
            {
                work(w);
            }
        }
        return ans;
    }
};
