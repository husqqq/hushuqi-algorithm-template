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

template <class T = long long> struct KDBlocks
{
    struct KDPoint
    {
        // x、y 是点坐标，w 是可加权值。
        int x, y;
        T w;
    };

    struct Block
    {
        struct Node
        {
            // p 是本点，l、r 是儿子，四个边界与 sum 描述整棵子树。
            KDPoint p;
            int l = -1, r = -1;
            int xl, xr, yl, yr;
            T sum{};
        };

        // a 是本块的全部点，t 是静态 K-D Tree 节点池，root 是根。
        vector<KDPoint> a;
        vector<Node> t;
        int root = -1;

        Block(vector<KDPoint> a = {}) : a(move(a))
        {
            // a 是本块点集；一次性建立静态 K-D Tree。
            t.reserve(this->a.size());
            root = build(0, this->a.size(), 0);
        }

        int build(int l, int r, int d)
        {
            // [l,r) 是待建点段，d=0/1 表示按横/纵坐标切分；返回根编号。
            if (l == r)
            {
                return -1;
            }
            int m = midpoint(l, r);
            nth_element(a.begin() + l,
                        a.begin() + m,
                        a.begin() + r,
                        [&](const KDPoint &p, const KDPoint &q)
                        {
                            return d ? p.y < q.y : p.x < q.x;
                        });
            int u = t.size();
            t.push_back({a[m], -1, -1, a[m].x, a[m].x, a[m].y, a[m].y, a[m].w});
            t[u].l = build(l, m, d ^ 1);
            t[u].r = build(m + 1, r, d ^ 1);
            pull(u, t[u].l);
            pull(u, t[u].r);
            return u;
        }

        void pull(int u, int v)
        {
            // u 是父节点，v 是儿子；把非空儿子的包围盒与权值和合入 u。
            if (v < 0)
            {
                return;
            }
            t[u].xl = min(t[u].xl, t[v].xl);
            t[u].xr = max(t[u].xr, t[v].xr);
            t[u].yl = min(t[u].yl, t[v].yl);
            t[u].yr = max(t[u].yr, t[v].yr);
            t[u].sum += t[v].sum;
        }

        T query(int u, int xl, int xr, int yl, int yr) const
        {
            // u 是当前节点；返回闭矩形 [xl,xr] x [yl,yr] 内的点权和。
            if (u < 0 || t[u].xr < xl || xr < t[u].xl || t[u].yr < yl || yr < t[u].yl)
            {
                return {};
            }
            if (xl <= t[u].xl && t[u].xr <= xr && yl <= t[u].yl && t[u].yr <= yr)
            {
                return t[u].sum;
            }
            T ans{};
            if (xl <= t[u].p.x && t[u].p.x <= xr && yl <= t[u].p.y && t[u].p.y <= yr)
            {
                ans += t[u].p.w;
            }
            return ans + query(t[u].l, xl, xr, yl, yr) + query(t[u].r, xl, xr, yl, yr);
        }

        T query(int xl, int xr, int yl, int yr) const
        {
            // 四个参数是有序的闭矩形边界；返回本块内点权和。
            assert(xl <= xr && yl <= yr); // 调试检查，可删
            return query(root, xl, xr, yl, yr);
        }
    };

    // b[k] 保存大小为 2^k 的静态块，n 是当前点数。
    vector<optional<Block>> b;
    int n = 0;

    void add(int x, int y, T w)
    {
        // x、y 是新点坐标，w 是权值；插入后按二进制进位重建块。
        vector<KDPoint> a{{x, y, w}};
        int k = 0;
        while (k < (int)b.size() && b[k])
        {
            a.insert(a.end(), b[k]->a.begin(), b[k]->a.end());
            b[k].reset();
            k++;
        }
        if (k == (int)b.size())
        {
            b.emplace_back();
        }
        b[k].emplace(move(a));
        n++;
    }

    T query(int xl, int xr, int yl, int yr) const
    {
        // 四个参数是有序的闭矩形边界；返回所有块内的点权和。
        assert(xl <= xr && yl <= yr); // 调试检查，可删
        T ans{};
        for (auto &x : b)
        {
            if (x)
            {
                ans += x->query(xl, xr, yl, yr);
            }
        }
        return ans;
    }
};
