
#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}

// 区间加、区间和实例。换题时通常只需要修改这两个结构。
struct Tag
{
    // add 是尚未下传的区间增量。
    long long add = 0;

    // 先前标记后再执行 t：区间加直接累加。
    void apply(const Tag &t)
    {
        // t 是后执行的区间加标记；与当前标记复合。
        add += t.add;
    }
};

struct Info
{
    // sum 是区间和，len 是区间叶子数。
    long long sum = 0;
    int len = 0;

    Info() = default; // 查询越界时的合并单位元
    Info(long long x) : sum(x), len(1)
    {
        // x 是叶子的初始值。
    }

    // 把区间加标记作用到当前节点。
    void apply(const Tag &t)
    {
        // t 是区间加标记；更新当前区间和。
        sum += t.add * len;
    }

    friend Info operator+(Info a, Info b)
    {
        // a、b 是相邻区间信息；返回合并结果。
        Info c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        return c;
    }
};

template <class Info, class Tag> struct LazySeg
{
    // n 是叶子数；tr 保存已计入本节点修改的区间信息，tag 和 has 保存尚未下传给儿子的修改。
    int n;
    vector<Info> tr;
    vector<Tag> tag;
    vector<unsigned char> has;

    LazySeg(int n = 0)
    {
        // n 是叶子数；每个叶子由 Info(0) 构造成零值信息。
        init(n);
    }
    LazySeg(const vector<Info> &a)
    {
        // a 是各叶子的初始信息。
        init(a);
    }

    void init(int n_)
    {
        // n_ 是新的叶子数；用 n_ 个 Info(0) 重建全零数组。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        tr.assign(4 * max<int>(n, 1), Info{});
        tag.assign(4 * max<int>(n, 1), Tag{});
        has.assign(4 * max<int>(n, 1), 0);
        if (n)
        {
            vector<Info> a(n, Info(0));
            build(1, 0, n, a);
        }
    }

    void init(const vector<Info> &a)
    {
        // a 是新的叶子信息；清空旧状态并重建。
        n = a.size();
        tr.assign(4 * max<int>(n, 1), Info{});
        tag.assign(4 * max<int>(n, 1), Tag{});
        has.assign(4 * max<int>(n, 1), 0);
        if (n)
        {
            build(1, 0, n, a);
        }
    }

    void build(int p, int l, int r, const vector<Info> &a)
    {
        // p 是当前节点，[l,r) 是其区间，a 是叶子信息。
        if (r - l == 1)
        {
            tr[p] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m, a);
        build(2 * p + 1, m, r, a);
        pull(p);
    }

    void pull(int p)
    {
        // p 是内部节点编号；按左右顺序合并两个儿子。
        tr[p] = tr[2 * p] + tr[2 * p + 1];
    }

    void apply(int p, const Tag &v)
    {
        // p 是整段命中的节点，v 是本次区间修改标记。
        tr[p].apply(v);
        tag[p].apply(v);
        has[p] = 1;
    }

    void push(int p)
    {
        // p 是当前节点；仅在有待执行标记时把它下传给两个儿子。
        if (!has[p])
        {
            return;
        }
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag{};
        has[p] = 0;
    }

    void apply(int p, int l, int r, int ql, int qr, const Tag &v)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是修改区间，v 是本次标记。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            apply(p, v);
            return;
        }
        push(p);
        int m = (l + r) / 2;
        apply(2 * p, l, m, ql, qr, v);
        apply(2 * p + 1, m, r, ql, qr, v);
        pull(p);
    }

    Info query(int p, int l, int r, int ql, int qr)
    {
        // p、[l,r) 是当前节点， [ql,qr) 是询问区间；返回相交部分的信息。
        if (qr <= l || r <= ql)
        {
            return Info{};
        }
        if (ql <= l && r <= qr)
        {
            return tr[p];
        }
        push(p);
        int m = (l + r) / 2;
        return query(2 * p, l, m, ql, qr) + query(2 * p + 1, m, r, ql, qr);
    }

    Info get(int p, int l, int r, int i) const
    {
        // p、[l,r) 是当前节点，i 是目标叶子下标；只读返回该叶子的最新信息。
        if (r - l == 1)
        {
            return tr[p];
        }
        int m = (l + r) / 2;
        Info ans;
        if (i < m)
        {
            ans = get(2 * p, l, m, i);
        }
        else
        {
            ans = get(2 * p + 1, m, r, i);
        }
        if (has[p])
        {
            ans.apply(tag[p]);
        }
        return ans;
    }

    void apply(int l, int r, const Tag &v)
    {
        // l、r 是半开区间端点，v 是区间修改标记。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        apply(1, 0, n, l, r, v);
    }
    Info query(int l, int r)
    {
        // l、r 是半开区间端点；返回 [l,r) 的信息。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return query(1, 0, n, l, r);
    }
    Info get(int i) const
    {
        // i 是叶子下标；只读返回该位置在所有待下传标记生效后的信息。
        assert(0 <= i && i < n); // 调试检查，可删
        return get(1, 0, n, i);
    }
};

// a 是初始叶子信息。
// LazySeg<Info, Tag> seg(a);
// seg.apply(1, 4, Tag{2}); // 给 [1,4) 的每个数加 2。
// long long ans = seg.query(0, 4).sum; // 查询 [0,4) 的区间和。

using namespace std;
#define int long long

struct SAM
{
    // to、link、len 分别是转移、后缀链接和状态最长串长。
    vector<array<int, 26>> to;
    vector<int> link, len;
    int sz = 1, last = 1;
    SAM(int n) : to(2 * n + 2), link(2 * n + 2), len(2 * n + 2)
    {
        // n 是原串长度上界；预留 2n 个状态。
    }
    void add(int c)
    {
        // c 是 0..25 的字符；把字符加入自动机。
        int cur = ++sz, p = last;
        len[cur] = len[last] + 1;
        while (p && !to[p][c])
        {
            to[p][c] = cur;
            p = link[p];
        }
        if (!p)
        {
            link[cur] = 1;
        }
        else
        {
            int q = to[p][c];
            if (len[q] == len[p] + 1)
            {
                link[cur] = q;
            }
            else
            {
                int cp = ++sz;
                to[cp] = to[q];
                len[cp] = len[p] + 1;
                link[cp] = link[q];
                while (p && to[p][c] == q)
                {
                    to[p][c] = cp;
                    p = link[p];
                }
                link[q] = link[cur] = cp;
            }
        }
        last = cur;
    }
};

struct MaxSeg
{
    // n 是叶子数；tr 是区间最大值。
    int n, z;
    vector<int> tr;
    MaxSeg(int m) : n(m), z(1)
    {
        // m 是叶子数；初始化为 0。
        while (z < n)
        {
            z <<= 1;
        }
        tr.assign(2 * z, 0);
    }
    void set(int p, int v)
    {
        // p 是位置，v 是候选值；执行单点取最大。
        p += z;
        tr[p] = max(tr[p], v);
        for (; p >>= 1;)
        {
            tr[p] = max(tr[2 * p], tr[2 * p + 1]);
        }
    }
    int query(int l, int r) const
    {
        // [l,r) 是查询区间；返回最大值。
        int x = 0, y = 0;
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = max(x, tr[l++]);
            }
            if (r & 1)
            {
                y = max(tr[--r], y);
            }
        }
        return max(x, y);
    }
};

vector<int> rangeSubs(const string &s, const vector<pair<int, int>> &qs)
{
    // s 是小写字母串；qs 是 0 下标左闭右开区间；返回本质不同子串数。
    int n = s.size(), q = qs.size();
    assert(n > 0);
    vector<vector<pair<int, int>>> que(n + 1);
    for (int i = 0; i < q; i++)
    {
        int l = qs[i].first, r = qs[i].second;
        assert(0 <= l && l < r && r <= n);
        que[r].push_back({l + 1, i});
    }
    SAM sam(n);
    for (char c : s)
    {
        sam.add(c - 'a');
    }
    vector<vector<int>> ch(sam.sz + 1);
    for (int i = 2; i <= sam.sz; i++)
    {
        ch[sam.link[i]].push_back(i);
    }
    vector<int> in(sam.sz + 1), out(sam.sz + 1), it(sam.sz + 1), st{1};
    int tim = 0;
    while (!st.empty())
    {
        int u = st.back();
        if (!in[u])
        {
            in[u] = ++tim;
        }
        if (it[u] < (int)ch[u].size())
        {
            st.push_back(ch[u][it[u]++]);
        }
        else
        {
            out[u] = tim;
            st.pop_back();
        }
    }
    LazySeg<Info, Tag> seg(n);
    MaxSeg mx(sam.sz + 1);
    vector<int> top(n + 1, 1);
    vector<char> vis(sam.sz + 1);
    vector<int> ans(q);
    int u = 1;
    for (int r = 1; r <= n; r++)
    {
        u = sam.to[u][s[r - 1] - 'a'];
        int v = u;
        while (v != 1 && !vis[v])
        {
            vis[v] = 1;
            v = sam.link[v];
        }
        while (v != 1)
        {
            int pos = mx.query(in[v] - 1, out[v]);
            int old = top[pos];
            seg.apply(pos - sam.len[v], pos - sam.len[old], Tag{-1});
            swap(top[pos], v);
        }
        mx.set(in[u] - 1, r);
        top[r] = 1;
        for (auto [l, id] : que[r])
        {
            int z = r - l + 1;
            ans[id] = z * (z + 1) / 2 + seg.query(l - 1, r).sum;
        }
    }
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs)
    {
        cin >> l >> r;
        --l;
    }
    for (int x : rangeSubs(s, qs))
    {
        cout << x << '\n';
    }
}
