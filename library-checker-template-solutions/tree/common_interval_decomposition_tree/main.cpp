
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct CommonIntervalNode
{
    int l, r, parent; // l、r 是闭区间端点，parent 是父节点编号，根为 -1。
    bool linear; // linear 为真表示线性节点，为假表示素节点。
};

vector<CommonIntervalNode> commonIntervalTree(const vector<int> &p)
{
    // p 是 0..n-1 的排列；返回先序编号的公共区间分解树，父节点编号严格更小。
    struct MaxAddSeg
    {
        int n;
        vector<int> mx, tag; // mx 是区间最大值，tag 是整段加法懒标记。
        MaxAddSeg(int n) : n(n), mx(n * 4), tag(n * 4)
        {
            // n 是叶子数；初始每个叶子为 0。
        }
        void add(int ql, int qr, int x, int u, int l, int r)
        {
            // [ql,qr) 加 x，u 覆盖 [l,r)。
            if (qr <= l || r <= ql)
            {
                return;
            }
            if (ql <= l && r <= qr)
            {
                mx[u] += x;
                tag[u] += x;
                return;
            }
            int m = (l + r) / 2;
            add(ql, qr, x, u * 2, l, m);
            add(ql, qr, x, u * 2 + 1, m, r);
            mx[u] = tag[u] + max(mx[u * 2], mx[u * 2 + 1]);
        }
        void set(int p, int x, int u, int l, int r)
        {
            // 把位置 p 赋为 x，u 覆盖 [l,r)。
            if (r - l == 1)
            {
                mx[u] = x;
                tag[u] = 0;
                return;
            }
            int m = (l + r) / 2;
            if (p < m)
            {
                set(p, x - tag[u], u * 2, l, m);
            }
            else
            {
                set(p, x - tag[u], u * 2 + 1, m, r);
            }
            mx[u] = tag[u] + max(mx[u * 2], mx[u * 2 + 1]);
        }
        int prod(int ql, int qr, int u, int l, int r) const
        {
            // 返回 [ql,qr) 的最大值，u 覆盖 [l,r)。
            if (qr <= l || r <= ql)
            {
                return numeric_limits<int>::min() / 4;
            }
            if (ql <= l && r <= qr)
            {
                return mx[u];
            }
            int m = (l + r) / 2;
            return tag[u] + max(prod(ql, qr, u * 2, l, m), prod(ql, qr, u * 2 + 1, m, r));
        }
        void add(int l, int r, int x) { add(l, r, x, 1, 0, n); }
        void set(int p, int x) { set(p, x, 1, 0, n); }
        int prod(int l, int r) const { return prod(l, r, 1, 0, n); }
    };
    struct Node
    {
        int l, r, mn, mx; // l、r 是半开位置区间，mn、mx 是对应值半开区间。
        bool linear; // 是否为线性节点。
        vector<int> ch; // 从左到右的子节点编号。
    };
    int n = p.size();
    if (n == 0)
    {
        return {};
    }
    MaxAddSeg seg(n);
    vector<int> mn, mx, stk;
    vector<Node> tr;
    auto makeLeaf = [&](int i)
    {
        tr.push_back({i, i + 1, p[i], p[i] + 1, true, {}});
        return (int)tr.size() - 1;
    };
    auto merge = [&](vector<int> ch, bool linear)
    {
        int l = tr[ch[0]].l, r = tr[ch.back()].r;
        int lo = tr[ch[0]].mn, hi = tr[ch[0]].mx;
        for (int x : ch)
        {
            lo = min(lo, tr[x].mn);
            hi = max(hi, tr[x].mx);
        }
        tr.push_back({l, r, lo, hi, linear, move(ch)});
        return (int)tr.size() - 1;
    };
    for (int i = 0; i < n; i++)
    {
        int r = i;
        while (!mn.empty() && p[mn.back()] > p[i])
        {
            mn.pop_back();
            int l = mn.empty() ? 0 : mn.back() + 1;
            seg.add(l, r, p[i] - p[r - 1]);
            r = l;
        }
        mn.push_back(i);
        r = i;
        while (!mx.empty() && p[mx.back()] < p[i])
        {
            mx.pop_back();
            int l = mx.empty() ? 0 : mx.back() + 1;
            seg.add(l, r, -p[i] + p[r - 1]);
            r = l;
        }
        mx.push_back(i);
        seg.add(0, i, 1);
        int cur = makeLeaf(i);
        while (true)
        {
            if (!stk.empty() && tr[stk.back()].linear && !tr[stk.back()].ch.empty() &&
                (tr[cur].mn == tr[tr[stk.back()].ch.back()].mx || tr[cur].mx == tr[tr[stk.back()].ch.back()].mn))
            {
                tr[stk.back()].ch.push_back(cur);
                tr[stk.back()].r = tr[cur].r;
                tr[stk.back()].mn = min(tr[stk.back()].mn, tr[cur].mn);
                tr[stk.back()].mx = max(tr[stk.back()].mx, tr[cur].mx);
                cur = stk.back();
                stk.pop_back();
                continue;
            }
            if (!stk.empty() && (tr[stk.back()].mx == tr[cur].mn || tr[stk.back()].mn == tr[cur].mx))
            {
                cur = merge({stk.back(), cur}, true);
                stk.pop_back();
                continue;
            }
            stk.push_back(cur);
            if (stk.size() == 1)
            {
                break;
            }
            if (seg.prod(0, tr[stk.back()].l) != 0)
            {
                break;
            }
            vector<int> ch{stk.back()};
            stk.pop_back();
            int l = tr[ch[0]].l, r2 = tr[ch[0]].r, lo = tr[ch[0]].mn, hi = tr[ch[0]].mx;
            while (true)
            {
                int x = stk.back();
                stk.pop_back();
                ch.push_back(x);
                l = min(l, tr[x].l);
                r2 = max(r2, tr[x].r);
                lo = min(lo, tr[x].mn);
                hi = max(hi, tr[x].mx);
                if (r2 - l == hi - lo)
                {
                    break;
                }
            }
            reverse(ch.begin(), ch.end());
            tr.push_back({l, r2, lo, hi, false, move(ch)});
            cur = tr.size() - 1;
        }
        seg.set(i, 0);
    }
    assert(stk.size() == 1);
    vector<CommonIntervalNode> ans;
    auto dfs = [&](auto &&self, int u, int fa) -> void
    {
        int id = ans.size();
        ans.push_back({tr[u].l, tr[u].r - 1, fa, tr[u].linear});
        for (int v : tr[u].ch)
        {
            self(self, v, id);
        }
    };
    dfs(dfs, stk.back(), -1);
    return ans;
}

signed main()
{
    int n; cin >> n;
    vector<int> p(n);
    for (int &x : p) cin >> x;
    auto a = commonIntervalTree(p);
    cout << a.size() << '\n';
    for (auto x : a)
    {
        cout << x.parent << ' ' << x.l << ' ' << x.r << ' ' << (x.linear ? "linear" : "prime") << '\n';
    }
}
