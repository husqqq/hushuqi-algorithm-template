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

template <class T> struct RectAddSum
{
    struct R
    {
        // l、d、r、u 依次是加权矩形的左、下、右、上边界。
        long long l, d, r, u;
        // w 是矩形内每个位置增加的权值。
        T w;
    };

    struct Q
    {
        // l、d、r、u 依次是询问矩形的左、下、右、上边界。
        long long l, d, r, u;
    };

    // a 保存全部矩形加操作，q 保存全部矩形和询问。
    vector<R> a;
    vector<Q> q;

    void add(long long l, long long d, long long r, long long u, T w)
    {
        // 给半开矩形 [l,r) x [d,u) 内每个位置增加 w；退化矩形无操作。
        if (l < r && d < u)
        {
            a.push_back({l, d, r, u, w});
        }
    }

    int query(long long l, long long d, long long r, long long u)
    {
        // 登记半开矩形 [l,r) x [d,u) 的权值和询问并返回询问编号。
        q.push_back({l, d, r, u});
        return q.size() - 1;
    }

    vector<T> work() const
    {
        // 统一处理全部操作；返回值按 query 的登记编号排列。
        struct E
        {
            // x、y 是二维差分事件坐标，w 是该点的带符号权值。
            long long x, y;
            T w;
        };
        struct P
        {
            // x、y 是前缀询问上界；id 是原询问编号，s 是容斥符号。
            long long x, y;
            int id, s;
        };

        vector<E> e;
        vector<P> p;
        vector<long long> ys;
        auto addE = [&](long long x, long long y, T w)
        {
            e.push_back({x, y, w});
            ys.push_back(y);
        };
        for (auto [l, d, r, u, w] : a)
        {
            addE(l, d, w);
            addE(l, u, -w);
            addE(r, d, -w);
            addE(r, u, w);
        }
        auto addP = [&](long long x, long long y, int id, int s)
        {
            p.push_back({x, y, id, s});
            ys.push_back(y);
        };
        for (int i = 0; i < (int)q.size(); i++)
        {
            auto [l, d, r, u] = q[i];
            addP(r, u, i, 1);
            addP(l, u, i, -1);
            addP(r, d, i, -1);
            addP(l, d, i, 1);
        }
        sort(e.begin(),
             e.end(),
             [](const E &x, const E &y)
             {
                 return x.x < y.x;
             });
        sort(p.begin(),
             p.end(),
             [](const P &x, const P &y)
             {
                 return x.x < y.x;
             });
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        struct B
        {
            // a 保存一棵扫描线纵轴 BIT 的 lowbit 块和。
            vector<T> a;

            B(int n) : a(n + 1)
            {
                // n 是离散纵坐标数；初值全为零。
            }
            void add(int x, T v)
            {
                // x 是离散纵坐标，v 是增量；更新该点。
                for (++x; x < (int)a.size(); x += x & -x)
                {
                    a[x] += v;
                }
            }
            T sum(int x) const
            {
                // x 是前缀长度；返回前 x 个离散位置的和。
                T s{};
                for (; x; x -= x & -x)
                {
                    s += a[x];
                }
                return s;
            }
        };

        array<B, 4> b{B(ys.size()), B(ys.size()), B(ys.size()), B(ys.size())};
        vector<T> ans(q.size());
        int k = 0;
        for (auto z : p)
        {
            while (k < (int)e.size() && e[k].x < z.x)
            {
                auto [x, y, w] = e[k++];
                int v = lower_bound(ys.begin(), ys.end(), y) - ys.begin();
                b[0].add(v, w);
                b[1].add(v, w * T(y));
                b[2].add(v, w * T(x));
                b[3].add(v, w * T(x) * T(y));
            }
            int y = lower_bound(ys.begin(), ys.end(), z.y) - ys.begin();
            T v = T(z.x) * T(z.y) * b[0].sum(y) - T(z.x) * b[1].sum(y) - T(z.y) * b[2].sum(y) + b[3].sum(y);
            ans[z.id] += T(z.s) * v;
        }
        return ans;
    }
};
