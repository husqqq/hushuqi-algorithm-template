
#include <bits/stdc++.h>
using namespace std;
#define int long long



#include <limits>

using namespace std;

constexpr long long INF = numeric_limits<long long>::max() / 4;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更小则更新 a 并返回 true。
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更大则更新 a 并返回 true。
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

constexpr int inf = 1E9;
constexpr long double eps = 1E-12L;

// 区间和实例：Sum{} 是合并单位元，Sum{x} 表示一个值为 x 的叶子。
struct Sum
{
    // val 是当前区间的元素和。
    long long val = 0;

    Sum() = default;
    Sum(long long x) : val(x)
    {
        // x 是单个叶子的值。
    }

    friend Sum operator+(Sum a, Sum b)
    {
        // a、b 是相邻区间信息；返回合并后的区间和。
        return a.val + b.val;
    }
};

struct MaxInfo
{
    // 当前信息取区间最大值。改成最小值信息时，把 lowest 改为 numeric_limits<long long>::max()、max 改为 min，
    // 并同步把类型名、查询谓词中的 >= 改成 <=。
    // val 是区间最大值；空区间以 long long 最小值作为 max 的单位元。
    long long val = numeric_limits<long long>::lowest();

    MaxInfo() = default;
    MaxInfo(long long x) : val(x)
    {
        // x 是单个叶子的值。
    }

    friend MaxInfo operator+(MaxInfo a, MaxInfo b)
    {
        // a、b 是按顺序相邻的区间信息；返回两段的最大值。
        return max(a.val, b.val);
    }
};

template <class Info> struct Seg
{
    // n 是真实叶子数，size 是不小于 n 的二次幂；tr 保存区间信息。
    int n;
    int size;
    vector<Info> tr;

    Seg(int n = 0)
    {
        // n 是叶子数；初始信息均为合并单位元。
        init(n);
    }
    Seg(const vector<Info> &a)
    {
        // a 是各叶子的初始信息。
        init(a);
    }

    void init(int n_)
    {
        // n_ 是新的叶子数；幂次二基址让每个内部节点对应连续区间。
        assert(n_ >= 0); // 调试检查，可删
        n = n_;
        size = 1;
        while (size < n)
        {
            size <<= 1;
        }
        tr.assign(2 * size, Info{});
    }

    void init(const vector<Info> &a)
    {
        // a 是新的叶子信息；线性重建整棵树。
        init(a.size());
        copy(a.begin(), a.end(), tr.begin() + size);
        for (int i = size - 1; i > 0; --i)
        {
            pull(i);
        }
    }

    void pull(int p)
    {
        // p 是内部节点编号；按左右顺序合并两个儿子。
        tr[p] = tr[2 * p] + tr[2 * p + 1];
    }

    void set(int p, Info v)
    {
        // p 是 0 下标位置，v 是新叶子信息；修改后重算祖先。
        assert(0 <= p && p < n); // 调试检查，可删
        for (tr[p += size] = v; p >>= 1;)
        {
            pull(p);
        }
    }

    Info query(int l, int r) const
    {
        // l、r 是半开区间端点；按原顺序返回 [l,r) 的合并结果。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        Info x{}, y{};
        for (l += size, r += size; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                x = x + tr[l++];
            }
            if (r & 1)
            {
                y = tr[--r] + y;
            }
        }
        return x + y;
    }

    template <class F>
    int findFirst(int p, int l, int r, int ql, int qr, F &ok) const
    {
        // p 管辖 [l,r)，查询 [ql,qr)；ok(Info) 为假须保证该段没有满足条件的叶子。
        // 返回最左满足 ok 的叶子下标，无解返回 -1；摘要有假阳性时可能遍历整段。
        if (r <= ql || qr <= l)
        {
            return -1;
        }
        if (ql <= l && r <= qr && !ok(tr[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l < n ? l : -1;
        }
        int m = l + (r - l) / 2;
        int x = findFirst(2 * p, l, m, ql, qr, ok);
        return x == -1 ? findFirst(2 * p + 1, m, r, ql, qr, ok) : x;
    }

    template <class F>
    int findFirst(int l, int r, F ok) const
    {
        // [l,r) 是查询区间；ok 为假须整段无解，返回最左满足 ok 的叶子下标或 -1。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findFirst(1, 0, size, l, r, ok);
    }

    template <class F>
    int findLast(int p, int l, int r, int ql, int qr, F &ok) const
    {
        // p 管辖 [l,r)，查询 [ql,qr)；ok(Info) 为假须保证该段没有满足条件的叶子。
        // 返回最右满足 ok 的叶子下标，无解返回 -1；摘要有假阳性时可能遍历整段。
        if (r <= ql || qr <= l)
        {
            return -1;
        }
        if (ql <= l && r <= qr && !ok(tr[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l < n ? l : -1;
        }
        int m = l + (r - l) / 2;
        int x = findLast(2 * p + 1, m, r, ql, qr, ok);
        return x == -1 ? findLast(2 * p, l, m, ql, qr, ok) : x;
    }

    template <class F>
    int findLast(int l, int r, F ok) const
    {
        // [l,r) 是查询区间；ok 为假须整段无解，返回最右满足 ok 的叶子下标或 -1。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return l == r ? -1 : findLast(1, 0, size, l, r, ok);
    }
};

#include <bits/stdc++.h>
using namespace std;
#define int long long


vector<int> saNaive(const vector<int> &s)
{
    // s 是整数序列；返回按字典序排列的全部后缀起点。
    int n = s.size();
    vector<int> sa(n);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(),
         sa.end(),
         [&](int x, int y)
         {
             while (x < n && y < n && s[x] == s[y])
             {
                 x++;
                 y++;
             }
             return y < n && (x == n || s[x] < s[y]);
         });
    return sa;
}

vector<int> saDouble(const vector<int> &s)
{
    // s 是整数序列；返回倍增法求得的后缀数组。
    int n = s.size();
    vector<int> sa(n), rk(n), nrk(n), v = s;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 0; i < n; i++)
    {
        rk[i] = lower_bound(v.begin(), v.end(), s[i]) - v.begin();
    }
    iota(sa.begin(), sa.end(), 0);
    for (int k = 1; k < n; k *= 2)
    {
        auto cmp = [&](int x, int y)
        {
            if (rk[x] != rk[y])
            {
                return rk[x] < rk[y];
            }
            int a = x + k < n ? rk[x + k] : -1;
            int b = y + k < n ? rk[y + k] : -1;
            return a < b;
        };
        sort(sa.begin(), sa.end(), cmp);
        nrk[sa[0]] = 0;
        for (int i = 1; i < n; i++)
        {
            nrk[sa[i]] = nrk[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
        }
        swap(rk, nrk);
    }
    return sa;
}

vector<int> saIs(const vector<int> &s, int m)
{
    // s 是整数序列，元素须在 [0,m] 内，m 是最大符号值；返回后缀数组。
    int n = s.size();
    if (n == 0)
    {
        return {};
    }
    if (n == 1)
    {
        return {0};
    }
    if (n == 2)
    {
        return s[0] < s[1] ? vector<int>{0, 1} : vector<int>{1, 0};
    }
    if (n < 10)
    {
        return saNaive(s);
    }
    if (n < 40)
    {
        return saDouble(s);
    }

    vector<int> sa(n, -1), sl(m + 1), ss(m + 1);
    vector<bool> ls(n);
    for (int i = n - 2; i >= 0; i--)
    {
        ls[i] = s[i] == s[i + 1] ? ls[i + 1] : s[i] < s[i + 1];
    }
    for (int i = 0; i < n; i++)
    {
        if (ls[i])
        {
            sl[s[i] + 1]++;
        }
        else
        {
            ss[s[i]]++;
        }
    }
    for (int i = 0; i <= m; i++)
    {
        ss[i] += sl[i];
        if (i < m)
        {
            sl[i + 1] += ss[i];
        }
    }

    auto induce = [&](const vector<int> &lms)
    {
        fill(sa.begin(), sa.end(), -1);
        vector<int> b = ss;
        for (int x : lms)
        {
            if (x < n)
            {
                sa[b[s[x]]++] = x;
            }
        }
        b = sl;
        sa[b[s[n - 1]]++] = n - 1;
        for (int i = 0; i < n; i++)
        {
            int x = sa[i];
            if (x > 0 && !ls[x - 1])
            {
                sa[b[s[x - 1]]++] = x - 1;
            }
        }
        b = sl;
        for (int i = n - 1; i >= 0; i--)
        {
            int x = sa[i];
            if (x > 0 && ls[x - 1])
            {
                sa[--b[s[x - 1] + 1]] = x - 1;
            }
        }
    };

    vector<int> id(n + 1, -1), lms;
    for (int i = 1; i < n; i++)
    {
        if (!ls[i - 1] && ls[i])
        {
            id[i] = lms.size();
            lms.push_back(i);
        }
    }
    induce(lms);
    int z = lms.size();
    if (z)
    {
        vector<int> ord;
        for (int x : sa)
        {
            if (x >= 0 && id[x] != -1)
            {
                ord.push_back(x);
            }
        }
        vector<int> ns(z);
        int nm = 0;
        ns[id[ord[0]]] = 0;
        for (int i = 1; i < z; i++)
        {
            int x = ord[i - 1], y = ord[i];
            int ex = id[x] + 1 < z ? lms[id[x] + 1] : n;
            int ey = id[y] + 1 < z ? lms[id[y] + 1] : n;
            bool same = ex - x == ey - y;
            while (same && x < ex)
            {
                if (s[x] != s[y])
                {
                    same = false;
                }
                x++;
                y++;
            }
            if (same && (x == n || s[x] != s[y]))
            {
                same = false;
            }
            if (!same)
            {
                nm++;
            }
            ns[id[ord[i]]] = nm;
        }
        vector<int> rsa = saIs(ns, nm);
        for (int i = 0; i < z; i++)
        {
            ord[i] = lms[rsa[i]];
        }
        induce(ord);
    }
    return sa;
}

template <class T> vector<int> suffixArray(const vector<T> &s)
{
    // s 是可比较序列；先离散化，再返回按字典序排列的全部后缀起点。
    int n = s.size();
    vector<int> p(n), a(n);
    iota(p.begin(), p.end(), 0);
    sort(p.begin(),
         p.end(),
         [&](int x, int y)
         {
             return s[x] < s[y];
         });
    int m = 0;
    for (int i = 0; i < n; i++)
    {
        if (i && s[p[i - 1]] != s[p[i]])
        {
            m++;
        }
        a[p[i]] = m;
    }
    return saIs(a, m);
}

vector<int> suffixArray(const string &s)
{
    // s 按无符号字节比较；返回按后缀字典序排列的起点下标。
    vector<int> a;
    a.reserve(s.size());
    for (unsigned char c : s)
    {
        a.push_back(c);
    }
    return saIs(a, 255);
}

template <class T> vector<int> lcpArray(const vector<T> &s, const vector<int> &sa)
{
    // s 是原序列，sa 是后缀数组；返回相邻后缀 LCP，ans[i]=lcp(sa[i],sa[i+1])。
    int n = s.size();
    vector<int> rk(n), lcp(max<int>(0, n - 1));
    for (int i = 0; i < n; i++)
    {
        rk[sa[i]] = i;
    }
    for (int i = 0, h = 0; i < n; i++)
    {
        if (rk[i] == 0)
        {
            continue;
        }
        int j = sa[rk[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h])
        {
            h++;
        }
        lcp[rk[i] - 1] = h;
        if (h)
        {
            h--;
        }
    }
    return lcp;
}

vector<int> lcpArray(const string &s, const vector<int> &sa)
{
    // s 是原字符串，sa 是后缀数组；返回相邻后缀的 LCP 数组。
    vector<int> a;
    a.reserve(s.size());
    for (unsigned char c : s)
    {
        a.push_back(c);
    }
    return lcpArray(a, sa);
}

#include <bits/stdc++.h>
using namespace std;
#define int long long

// 知识点 4.3.003：LCP、Height 与 RMQ。
namespace suffix_lcp
{
vector<int> lcpArray(const string &s, const vector<int> &sa)
{
    // s 是原字符串，sa 是全部后缀按字典序排列的起点；返回相邻后缀 LCP，ans[i]=lcp(sa[i],sa[i+1])。
    auto n = (int)s.size();
    assert((int)sa.size() == n);
    vector<int> rk(n), ht(max<int>(0, n - 1));
    vector<char> seen(n);
    for (int i = 0; i < n; i++)
    {
        assert(0 <= sa[i] && sa[i] < n && !seen[sa[i]]);
        seen[sa[i]] = true;
        rk[sa[i]] = i;
    }
    int h = 0;
    for (int i = 0; i < n; i++)
    {
        if (rk[i] == n - 1)
        {
            h = 0;
            continue;
        }
        auto j = sa[rk[i] + 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h])
        {
            h++;
        }
        ht[rk[i]] = h;
        if (h)
        {
            h--;
        }
    }
    return ht;
}

struct LcpRMQ
{
    int n = 0;              // n 是字符串长度。
    vector<int> sa;         // sa[r] 是排名 r 的后缀起点。
    vector<int> rk;         // rk[i] 是后缀 s[i..n) 的排名。
    vector<int> ht;         // ht[i] 是 sa[i] 与 sa[i+1] 的最长公共前缀长度。
    vector<int> lg;         // lg[len] 是不超过 len 的最大二次幂指数。
    vector<vector<int>> st; // st[k][i] 是 ht[i..i+2^k) 的最小值。

    LcpRMQ(const string &s, vector<int> order)
        : n((int)s.size()), sa(move(order)), rk(n), ht(lcpArray(s, sa)), lg(n + 1)
    {
        // s 是原字符串，order 是已构造好的后缀数组；建立 Height 的 ST 表，无返回值。
        for (int i = 0; i < n; i++)
        {
            rk[sa[i]] = i;
        }
        for (int i = 2; i <= n; i++)
        {
            lg[i] = lg[i / 2] + 1;
        }
        if (n <= 1)
        {
            return;
        }
        st.assign(lg[n - 1] + 1, {});
        st[0] = ht;
        for (int k = 1; k < (int)st.size(); k++)
        {
            auto len = 1LL << k;
            st[k].resize(n - len);
            for (int i = 0; i + len <= n - 1; i++)
            {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + len / 2]);
            }
        }
    }

    int queryRank(int l, int r) const
    {
        // l、r 是后缀排名且 l<r；返回 sa[l] 与 sa[r] 的最长公共前缀长度。
        assert(0 <= l && l < r && r < n);
        auto k = lg[r - l];
        return min(st[k][l], st[k][r - (1LL << k)]);
    }

    int query(int i, int j) const
    {
        // i、j 是两个后缀起点；返回 s[i..n) 与 s[j..n) 的最长公共前缀长度。
        assert(0 <= i && i < n && 0 <= j && j < n);
        if (i == j)
        {
            return n - i;
        }
        auto x = rk[i];
        auto y = rk[j];
        if (x > y)
        {
            swap(x, y);
        }
        return queryRank(x, y);
    }
};
}

vector<array<int, 3>> runs(const string &s)
{
    // s 可含任意字节；返回按字典序排列的 {最小周期 p, 左端点 l, 右端点 r}，区间为 [l,r)。
    int n = s.size();
    if (n < 2) return {};
    auto make = [&](const string &text)
    {
        // text 是原串或反串；返回后缀排名及负 LCP 的最大值线段树。
        auto sa = suffixArray(text), h = suffix_lcp::lcpArray(text, sa);
        vector<int> rank(n);
        vector<MaxInfo> v(n - 1);
        for (int i = 0; i < n; i++)
        {
            rank[sa[i]] = i;
            if (i < n - 1) v[i] = MaxInfo{-h[i]};
        }
        return pair{move(rank), Seg<MaxInfo>(v)};
    };
    string rev(s.rbegin(), s.rend());
    auto [rk, pre] = make(s);
    auto [rr, suf] = make(rev);
    auto lce = [&](bool back, int i, int j) -> int
    {
        // back 选反串，否则选原串；i、j 是合法后缀起点，返回其 LCP 长度。
        const auto &text = back ? rev : s;
        if (i == j) return n - i;
        if (text[i] != text[j]) return 0;
        const auto &rank = back ? rr : rk;
        const auto &seg = back ? suf : pre;
        int l = rank[i], r = rank[j];
        if (l > r) swap(l, r);
        // 非空区间上 max(-h) 取反即 min(h)；不对空区间的负无穷单位元取反。
        return -seg.query(l, r).val;
    };
    // 每项依次为 {最小周期, 左端点, 右端点}。
    vector<array<int, 3>> ans;
    for (int dir = 0; dir < 2; dir++)
    {
        vector<int> rank = rk, st;
        st.reserve(n);
        if (dir)
        {
            string opp = s;
            for (char &c : opp)
            {
                c = (char)(255 - (unsigned char)c);
            }
            auto sa = suffixArray(opp);
            for (int i = 0; i < n; i++)
            {
                rank[sa[i]] = i;
            }
        }
        // st 顶端给出右侧第一个后缀排名更小的位置，即当前最长 Lyndon 根的右端点。
        for (int i = n - 1; i >= 0; i--)
        {
            while (!st.empty() && rank[st.back()] > rank[i])
            {
                st.pop_back();
            }
            int j = st.empty() ? n : st.back();
            st.push_back(i);
            if (j == n) continue;
            int p = j - i, x = i ? lce(true, n - i, n - j) : 0;
            if (x >= p) continue;
            int y = lce(false, i, j);
            if (x + y >= p) ans.push_back({p, i - x, j + y});
        }
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

signed main()
{
    // 无参数；按最小周期、左端点、右端点的字典序输出全部极大重复，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto a = runs(s);
    cout << a.size() << '\n';
    for (auto [p, l, r] : a)
    {
        cout << p << ' ' << l << ' ' << r << '\n';
    }
}
