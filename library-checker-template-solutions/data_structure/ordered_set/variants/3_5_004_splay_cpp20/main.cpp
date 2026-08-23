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

template <class T> struct Splay
{
    struct Node
    {
        // x 是键值，ch[0]、ch[1] 是左右儿子。
        T x{};
        array<int, 2> ch{};
        // fa 是父亲，cnt 是本键重数，sz 是子树元素总数。
        int fa = 0, cnt = 0, sz = 0;
    };

    // t 是节点池，root 是当前根编号。
    vector<Node> t{{}};
    int root = 0;

    int size(int u) const
    {
        // u 是节点编号；返回其子树元素数，空节点返回 0。
        return u ? t[u].sz : 0;
    }

    void pull(int u)
    {
        // u 是非空节点编号；由儿子与本键重数重算子树大小。
        t[u].sz = size(t[u].ch[0]) + size(t[u].ch[1]) + t[u].cnt;
    }

    void rotate(int u)
    {
        // u 是非根节点；把 u 向父亲方向旋转一层。
        int p = t[u].fa, g = t[p].fa;
        int d = t[p].ch[1] == u, v = t[u].ch[d ^ 1];
        if (g)
        {
            t[g].ch[t[g].ch[1] == p] = u;
        }
        t[u].fa = g;
        t[u].ch[d ^ 1] = p;
        t[p].fa = u;
        t[p].ch[d] = v;
        if (v)
        {
            t[v].fa = p;
        }
        pull(p);
        pull(u);
    }

    void splay(int u, int g = 0)
    {
        // u 是待旋节点，g 是停止位置；把 u 旋成 g 的儿子，g=0 时旋到根。
        while (t[u].fa != g)
        {
            int p = t[u].fa, q = t[p].fa;
            if (q != g)
            {
                if ((t[p].ch[1] == u) == (t[q].ch[1] == p))
                {
                    rotate(p);
                }
                else
                {
                    rotate(u);
                }
            }
            rotate(u);
        }
        if (!g)
        {
            root = u;
        }
    }

    int find(const T &x)
    {
        // x 是查询键；返回对应节点，不存在时返回 0，并伸展最后访问点。
        int u = root, p = 0;
        while (u && t[u].x != x)
        {
            p = u;
            u = t[u].ch[t[u].x < x];
        }
        if (u)
        {
            splay(u);
        }
        else if (p)
        {
            splay(p);
        }
        return u;
    }

    void add(const T &x)
    {
        // x 是待插入键；重复键只增加重数。
        if (!root)
        {
            root = t.size();
            t.push_back({x, {}, 0, 1, 1});
            return;
        }
        int u = root, p = 0, d = 0;
        while (u)
        {
            if (t[u].x == x)
            {
                t[u].cnt++;
                pull(u);
                splay(u);
                return;
            }
            p = u;
            d = t[u].x < x;
            u = t[u].ch[d];
        }
        u = t.size();
        t.push_back({x, {}, p, 1, 1});
        t[p].ch[d] = u;
        splay(u);
    }

    bool erase(const T &x)
    {
        // x 是待删除键；删除一份并返回删除前是否存在。
        int u = find(x);
        if (!u)
        {
            return false;
        }
        if (t[u].cnt > 1)
        {
            t[u].cnt--;
            pull(u);
            return true;
        }
        int l = t[u].ch[0], r = t[u].ch[1];
        if (!l)
        {
            root = r;
            if (r)
            {
                t[r].fa = 0;
            }
            return true;
        }
        t[l].fa = 0;
        root = l;
        int p = l;
        while (t[p].ch[1])
        {
            p = t[p].ch[1];
        }
        splay(p);
        t[p].ch[1] = r;
        if (r)
        {
            t[r].fa = p;
        }
        pull(p);
        return true;
    }

    int rank(const T &x)
    {
        // x 是查询键；返回严格小于 x 的元素数。
        int u = root, p = 0, ans = 0;
        while (u)
        {
            p = u;
            if (x <= t[u].x)
            {
                u = t[u].ch[0];
            }
            else
            {
                ans += size(t[u].ch[0]) + t[u].cnt;
                u = t[u].ch[1];
            }
        }
        if (p)
        {
            splay(p);
        }
        return ans;
    }

    T kth(int k)
    {
        // k 是 0 基排名；返回第 k 小并把对应节点伸展到根。
        assert(0 <= k && k < size(root)); // 调试检查，可删
        int u = root;
        while (true)
        {
            int z = size(t[u].ch[0]);
            if (k < z)
            {
                u = t[u].ch[0];
            }
            else if (k < z + t[u].cnt)
            {
                splay(u);
                return t[u].x;
            }
            else
            {
                k -= z + t[u].cnt;
                u = t[u].ch[1];
            }
        }
    }

    optional<T> prev(const T &x)
    {
        // x 是查询键；返回严格前驱，不存在时返回空。
        int k = rank(x);
        return k ? optional<T>(kth(k - 1)) : nullopt;
    }

    optional<T> next(const T &x)
    {
        // x 是查询键；返回严格后继，不存在时返回空。
        int u = root, p = 0, best = 0;
        while (u)
        {
            p = u;
            if (x < t[u].x)
            {
                best = u;
                u = t[u].ch[0];
            }
            else
            {
                u = t[u].ch[1];
            }
        }
        if (best)
        {
            splay(best);
            return t[best].x;
        }
        if (p)
        {
            splay(p);
        }
        return nullopt;
    }
};

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q; Splay<int> tr;
    while (n--) { int x; cin >> x; tr.add(x); }
    while (q--)
    {
        int t, x; cin >> t >> x;
        if (t == 0) { if (tr.rank(x + 1) == tr.rank(x)) tr.add(x); }
        else if (t == 1) tr.erase(x);
        else if (t == 2) cout << (x > tr.size(tr.root) ? -1 : tr.kth(x - 1)) << '\n';
        else if (t == 3) cout << tr.rank(x + 1) << '\n';
        else if (t == 4) { int k = tr.rank(x + 1); cout << (k ? tr.kth(k - 1) : -1) << '\n'; }
        else { int k = tr.rank(x); cout << (k < tr.size(tr.root) ? tr.kth(k) : -1) << '\n'; }
    }
}
