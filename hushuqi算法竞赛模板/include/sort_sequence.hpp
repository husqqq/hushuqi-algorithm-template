#pragma once

#include "base.hpp"

template <class T, class Cmp = less<T>> void mergeSort(vector<T> &a, Cmp cmp = {})
{
    // a 是待排序数组，cmp 是严格弱序比较器；排序结果直接写回 a。
    vector<T> b;
    b.reserve(a.size());
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !cmp(a[j], a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
}

void radixSort(vector<int> &a)
{
    // a 是待排序的有符号 64 位整数数组；排序结果直接写回 a。
    static_assert(numeric_limits<int>::digits == 63);
    vector<unsigned long long> x(a.size()), b(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        x[i] = (unsigned long long)a[i] ^ (1ULL << 63);
    }
    constexpr int B = 1 << 16;
    vector<int> cnt(B);
    for (int sh = 0; sh < 64; sh += 16)
    {
        fill(cnt.begin(), cnt.end(), 0);
        for (auto v : x)
        {
            cnt[v >> sh & (B - 1)]++;
        }
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (auto it = x.rbegin(); it != x.rend(); it++)
        {
            auto p = *it >> sh & (B - 1);
            b[--cnt[p]] = *it;
        }
        x.swap(b);
    }
    for (int i = 0; i < (int)a.size(); i++)
    {
        a[i] = bit_cast<int64_t>(x[i] ^ (1ULL << 63));
    }
}

template <class T> T nthValue(vector<T> a, int k)
{
    // a 是按值复制的数组，k 是从 0 开始的排名；返回第 k 小值。
    assert(0 <= k && k < (int)a.size()); // 调试检查，可删
    nth_element(a.begin(), a.begin() + k, a.end());
    return a[k];
}

template <class T, class Rng> T quickSelect(vector<T> a, int k, Rng &rng)
{
    // a 是按值复制的数组，k 是从 0 开始的排名，rng 是随机源；返回第 k 小值。
    assert(0 <= k && k < (int)a.size()); // 调试检查，可删
    int l = 0, r = a.size();
    while (r - l > 1)
    {
        auto p = a[uniform_int_distribution<int>(l, r - 1)(rng)];
        auto m1 = partition(a.begin() + l,
                            a.begin() + r,
                            [&](const T &x)
                            {
                                return x < p;
                            });
        auto m2 = partition(m1,
                            a.begin() + r,
                            [&](const T &x)
                            {
                                return !(p < x);
                            });
        auto x = m1 - a.begin();
        auto y = m2 - a.begin();
        if (k < x)
        {
            r = x;
        }
        else if (k >= y)
        {
            l = y;
        }
        else
        {
            return p;
        }
    }
    return a[l];
}

template <class T> struct Discrete
{
    vector<T> a; // a 保存升序且去重后的原值。

    Discrete() = default;

    explicit Discrete(vector<T> v) : a(move(v))
    {
        // v 收集所有可能出现的值。
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }

    int id(const T &x) const
    {
        // x 必须已存在；返回 x 从 0 开始的离散编号。
        auto it = lower_bound(a.begin(), a.end(), x);
        assert(it != a.end() && *it == x); // 调试检查，可删
        return it - a.begin();
    }

    int lower(const T &x) const
    {
        // x 是查询值；返回第一个不小于 x 的离散位置。
        return lower_bound(a.begin(), a.end(), x) - a.begin();
    }

    const T &value(int p) const
    {
        // p 是合法离散编号；返回对应原值的常量引用。
        return a[p];
    }
};

template <class T> int invMerge(vector<T> a)
{
    // a 是按值复制的序列；返回严格逆序对数量。
    vector<T> b;
    b.reserve(a.size());
    int ans = 0;
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !(a[j] < a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                ans += m - i;
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
    return ans;
}

template <class T> int invBit(const vector<T> &a)
{
    // a 是原序列；返回用离散化和 Fenwick 统计的严格逆序对数量。
    Discrete<T> dis(a);
    vector<int> bit(dis.a.size() + 1);
    auto add = [&](int p)
    {
        for (p++; p < (int)bit.size(); p += p & -p)
        {
            bit[p]++;
        }
    };
    auto sum = [&](int p)
    {
        int ans = 0;
        for (; p > 0; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    };
    int ans = 0;
    for (auto it = a.rbegin(); it != a.rend(); it++)
    {
        auto p = dis.id(*it);
        ans += sum(p);
        add(p);
    }
    return ans;
}

struct Fenwick
{
    int n = 0;     // n 是下标范围长度。
    vector<int> a; // a 保存从 1 开始的树状数组节点和。

    Fenwick(int m = 0)
    {
        // m 是初始下标范围长度。
        init(m);
    }

    void init(int m)
    {
        // m 是新的下标范围长度；调用后全部频率清零。
        assert(m >= 0); // 调试检查，可删
        n = m;
        a.assign(n + 1, 0);
    }

    void add(int p, int v)
    {
        // p 是从 0 开始的位置，v 是增量。
        assert(0 <= p && p < n); // 调试检查，可删
        for (p++; p <= n; p += p & -p)
        {
            a[p] += v;
        }
    }

    int sum(int r) const
    {
        // r 是右端点；返回半开前缀 [0,r) 的和。
        assert(0 <= r && r <= n); // 调试检查，可删
        int ans = 0;
        for (; r > 0; r -= r & -r)
        {
            ans += a[r];
        }
        return ans;
    }

    int sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(r) - sum(l);
    }
};

// Point3[0..3] 依次表示 x、y、z、原下标。
using Point3 = array<int, 4>;

vector<int> dom3(vector<Point3> p)
{
    // p 保存 {x,y,z,id} 点集；返回每个原下标对应的三维非严格支配计数。
    auto n = (int)p.size();
    vector<int> seen(n);
    for (auto q : p)
    {
        assert(0 <= q[3] && q[3] < n && !seen[q[3]]); // 调试检查，可删
        seen[q[3]] = 1;
    }
    vector<int> zs;
    for (auto q : p)
    {
        zs.push_back(q[2]);
    }
    sort(zs.begin(), zs.end());
    zs.erase(unique(zs.begin(), zs.end()), zs.end());
    sort(p.begin(),
         p.end(),
         [](const auto &a, const auto &b)
         {
             return tie(a[0], a[1], a[2]) < tie(b[0], b[1], b[2]);
         });
    struct Node
    {
        int x, y, z;        // x、y 保留原坐标，z 是离散编号。
        int w = 1, ans = 1; // w 是重数，ans 是当前支配计数。
        vector<int> ids;    // ids 保存这一组重复点的原下标。
    };
    vector<Node> a;
    for (auto q : p)
    {
        auto [x, y, rawZ, id] = q;
        auto z = lower_bound(zs.begin(), zs.end(), rawZ) - zs.begin();
        if (!a.empty() && tie(a.back().x, a.back().y, a.back().z) == tuple(x, y, z))
        {
            a.back().w++;
            a.back().ans++;
            a.back().ids.push_back(id);
        }
        else
        {
            a.push_back({x, y, z, 1, 1, {id}});
        }
    }
    Fenwick bit(zs.size());
    vector<int> ord(a.size()), buf(a.size()), ans(n);
    iota(ord.begin(), ord.end(), 0);
    auto cdq = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l, j = m, k = l;
        while (j < r)
        {
            while (i < m && a[ord[i]].y <= a[ord[j]].y)
            {
                bit.add(a[ord[i]].z, a[ord[i]].w);
                buf[k++] = ord[i++];
            }
            a[ord[j]].ans += bit.sum(a[ord[j]].z + 1);
            buf[k++] = ord[j++];
        }
        auto used = i;
        while (i < m)
        {
            buf[k++] = ord[i++];
        }
        for (auto q = l; q < used; q++)
        {
            bit.add(a[ord[q]].z, -a[ord[q]].w);
        }
        copy(buf.begin() + l, buf.begin() + r, ord.begin() + l);
    };
    cdq(cdq, 0, a.size());
    for (auto &q : a)
    {
        for (auto id : q.ids)
        {
            ans[id] = q.ans;
        }
    }
    return ans;
}

template <class T> struct FracCascade
{
    struct Item
    {
        T x;           // x 是增强表中的键值。
        int own, down; // own 指向本层原表下界，down 指向下一层增强表下界。
    };

    vector<vector<T>> a;    // a 保存各层原始有序表。
    vector<vector<Item>> b; // b 保存采样合并后的增强表和桥指针。

    explicit FracCascade(vector<vector<T>> src) : a(move(src)), b(a.size())
    {
        // src 是若干个按同一规则升序排列的静态表。
        for (auto &x : a)
        {
            assert(is_sorted(x.begin(), x.end())); // 调试检查，可删
        }
        for (int i = (int)a.size() - 1; i >= 0; i--)
        {
            auto v = a[i];
            if (i + 1 < (int)b.size())
            {
                for (int j = 1; j < (int)b[i + 1].size(); j += 2)
                {
                    v.push_back(b[i + 1][j].x);
                }
            }
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            for (auto x : v)
            {
                auto p = lower_bound(a[i].begin(), a[i].end(), x) - a[i].begin();
                int q = 0;
                if (i + 1 < (int)b.size())
                {
                    q = lower_bound(b[i + 1].begin(),
                                    b[i + 1].end(),
                                    x,
                                    [](const Item &u, const T &rhs)
                                    {
                                        return u.x < rhs;
                                    }) -
                        b[i + 1].begin();
                }
                b[i].push_back({x, p, q});
            }
        }
    }

    vector<int> lowerBound(const T &x) const
    {
        // x 是所有表共用的查询值；返回每个原表中的 lower_bound 位置。
        vector<int> ans(a.size());
        int p = 0;
        if (!b.empty())
        {
            p = lower_bound(b[0].begin(),
                            b[0].end(),
                            x,
                            [](const Item &u, const T &rhs)
                            {
                                return u.x < rhs;
                            }) -
                b[0].begin();
        }
        for (int i = 0; i < (int)a.size(); i++)
        {
            ans[i] = p == (int)b[i].size() ? (int)a[i].size() : b[i][p].own;
            if (i + 1 == (int)a.size())
            {
                break;
            }
            p = p == (int)b[i].size() ? (int)b[i + 1].size() : b[i][p].down;
            if (p && b[i + 1][p - 1].x >= x)
            {
                p--;
            }
        }
        return ans;
    }
};

// Rect[0..3] 依次表示左横坐标、下纵坐标、右横坐标、上纵坐标。
using Rect = array<int, 4>;

int rectArea(const vector<Rect> &rs)
{
    // rs 中每项是 {左、下、右、上}；返回所有半开矩形的面积并。
    // Event[0..3] 依次表示横坐标、下纵坐标、上纵坐标、覆盖次数变化量。
    using Event = array<int, 4>;
    vector<Event> es;
    vector<int> ys;
    for (auto [xl, yl, xr, yr] : rs)
    {
        if (xl >= xr || yl >= yr)
        {
            continue;
        }
        es.push_back({xl, yl, yr, 1});
        es.push_back({xr, yl, yr, -1});
        ys.push_back(yl);
        ys.push_back(yr);
    }
    if (es.empty())
    {
        return 0;
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(es.begin(),
         es.end(),
         [](const auto &a, const auto &b)
         {
             return a[0] < b[0];
         });
    vector<int> cov(ys.size() * 4);
    vector<int> len(ys.size() * 4);
    auto add = [&](auto &&self, int p, int l, int r, int ql, int qr, int d) -> void
    {
        if (ql <= l && r <= qr)
        {
            cov[p] += d;
        }
        else
        {
            auto m = midpoint(l, r);
            if (ql < m)
            {
                self(self, p * 2, l, m, ql, qr, d);
            }
            if (m < qr)
            {
                self(self, p * 2 + 1, m, r, ql, qr, d);
            }
        }
        if (cov[p])
        {
            len[p] = ys[r] - ys[l];
        }
        else if (r - l == 1)
        {
            len[p] = 0;
        }
        else
        {
            len[p] = len[p * 2] + len[p * 2 + 1];
        }
    };
    int ans = 0;
    auto pre = es[0][0];
    for (int i = 0; i < (int)es.size();)
    {
        auto width = es[i][0] - pre;
        ans += len[1] * width;
        auto j = i;
        while (j < (int)es.size() && es[j][0] == es[i][0])
        {
            auto [x, yl, yr, d] = es[j];
            auto l = lower_bound(ys.begin(), ys.end(), yl) - ys.begin();
            auto r = lower_bound(ys.begin(), ys.end(), yr) - ys.begin();
            add(add, 1, 0, ys.size() - 1, l, r, d);
            j++;
        }
        pre = es[i][0];
        i = j;
    }
    return ans;
}

template <class T> vector<int> prevLess(const vector<T> &a)
{
    // a 是原序列；返回每个位置左侧最近严格更小元素的下标，不存在时为 -1。
    vector<int> stk, ans(a.size(), -1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!stk.empty() && a[stk.back()] >= a[i])
        {
            stk.pop_back();
        }
        if (!stk.empty())
        {
            ans[i] = stk.back();
        }
        stk.push_back(i);
    }
    return ans;
}

template <class T> vector<T> windowMin(const vector<T> &a, int k)
{
    // a 是原序列，k 是固定窗口长度；返回每个连续窗口的最小值。
    assert(1 <= k && k <= (int)a.size()); // 调试检查，可删
    deque<int> q;
    vector<T> ans;
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] >= a[i])
        {
            q.pop_back();
        }
        q.push_back(i);
        if (i + 1 >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}

template <class T> vector<T> windowMax(const vector<T> &a, int k)
{
    // a 是原序列，k 是固定窗口长度；返回每个连续窗口的最大值。
    assert(1 <= k && k <= (int)a.size()); // 调试检查，可删
    deque<int> q;
    vector<T> ans;
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] <= a[i])
        {
            q.pop_back();
        }
        q.push_back(i);
        if (i + 1 >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}

vector<int> lis(const vector<int> &a)
{
    // a 是原序列；返回一组严格上升子序列的原数组下标。
    vector<int> val, pos, pre(a.size(), -1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        auto p = lower_bound(val.begin(), val.end(), a[i]) - val.begin();
        if (p == (int)val.size())
        {
            val.push_back(a[i]);
            pos.push_back(i);
        }
        else
        {
            val[p] = a[i];
            pos[p] = i;
        }
        if (p)
        {
            pre[i] = pos[p - 1];
        }
    }
    vector<int> ans;
    for (auto x = pos.empty() ? -1 : pos.back(); x >= 0; x = pre[x])
    {
        ans.push_back(x);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

int lis2D(vector<pair<int, int>> a)
{
    // a 保存二维点并按值复制；返回两维都严格增加的最长链长度。
    sort(a.begin(),
         a.end(),
         [](const auto &x, const auto &y)
         {
             if (x.first != y.first)
             {
                 return x.first < y.first;
             }
             return x.second > y.second;
         });
    vector<int> d;
    for (auto [x, y] : a)
    {
        auto it = lower_bound(d.begin(), d.end(), y);
        if (it == d.end())
        {
            d.push_back(y);
        }
        else
        {
            *it = y;
        }
    }
    return d.size();
}

int lcis(const vector<int> &a, const vector<int> &b)
{
    // a、b 是两个原序列；返回最长公共严格上升子序列长度。
    vector<int> f(b.size());
    for (auto x : a)
    {
        int best = 0;
        for (int j = 0; j < (int)b.size(); j++)
        {
            if (b[j] < x)
            {
                chmax(best, f[j]);
            }
            else if (b[j] == x)
            {
                chmax(f[j], best + 1);
            }
        }
    }
    return f.empty() ? 0 : *max_element(f.begin(), f.end());
}

int maxRect(const vector<int> &h)
{
    // h 是非负直方图柱高；返回由 Acc 可表示的最大矩形面积。
    vector<int> stk;
    int ans = 0;
    for (int i = 0; i <= (int)h.size(); i++)
    {
        auto x = i == (int)h.size() ? 0 : h[i];
        while (!stk.empty() && h[stk.back()] >= x)
        {
            auto p = stk.back();
            stk.pop_back();
            auto l = stk.empty() ? 0 : stk.back() + 1;
            chmax(ans, h[p] * (i - l));
        }
        stk.push_back(i);
    }
    return ans;
}

int maxRect(const vector<vector<int>> &a)
{
    // a 是规则矩阵，非零元素视为 1；返回由 Acc 可表示的最大全 1 子矩形面积。
    if (a.empty() || a[0].empty())
    {
        return 0;
    }
    vector<int> h(a[0].size());
    int ans = 0;
    for (auto &row : a)
    {
        assert(row.size() == h.size()); // 调试检查，可删
        for (int j = 0; j < (int)h.size(); j++)
        {
            h[j] = row[j] ? h[j] + 1 : 0;
        }
        chmax(ans, maxRect(h));
    }
    return ans;
}
