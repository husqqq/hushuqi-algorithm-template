#include <bits/stdc++.h>
using namespace std;
#define int long long

class LineMinSeg
{
    struct P
    {
        // x 是数组下标，y 是该位置的当前值。
        long long x = 0, y = 0;

        bool operator==(const P &) const = default;
    };

    // L[0]、L[1] 分别是一次函数的斜率和截距；B[0]、B[1] 是桥的左右端点。
    using L = array<long long, 2>;
    using B = array<P, 2>;

    // n 是原数组长度，z 是补成二次幂后的叶子数，h 是树高。
    int n, z = 1, h = 0;
    // pt 是叶子点；lz 是整段待加的一次函数；br 是两棵儿子下凸包的公切桥。
    vector<P> pt;
    vector<L> lz;
    vector<B> br;

    static L merge(L a, L b)
    {
        // a、b 是先后累加的一次函数；返回逐项相加后的函数。
        a[0] += b[0];
        a[1] += b[1];
        return a;
    }

    static P apply(P p, L f)
    {
        // p 是平面点，f 是待加的一次函数；返回纵坐标增加 f(p.x) 后的点。
        p.y = (long long)((__int128)p.y + (__int128)p.x * f[0] + f[1]);
        return p;
    }

    static __int128 cross(P a, P b, P c)
    {
        // a、b、c 是三个点；返回向量 ab 与 ac 的叉积。
        return ((__int128)b.x - a.x) * ((__int128)c.y - a.y)
             - ((__int128)b.y - a.y) * ((__int128)c.x - a.x);
    }

    static __int128 cross(P a, P b, P c, P d)
    {
        // a、b、c、d 是四个点；返回向量 ab 与 cd 的叉积。
        return ((__int128)b.x - a.x) * ((__int128)d.y - c.y)
             - ((__int128)b.y - a.y) * ((__int128)d.x - c.x);
    }

    void pull(int p)
    {
        // p 是内部节点；重新寻找左右儿子下凸包之间的公切桥。
        if (p >= z)
        {
            return;
        }
        L off{0, 0};
        for (int x = p; x; x >>= 1)
        {
            off = merge(off, lz[x]);
        }
        int l = 2 * p, r = 2 * p + 1;
        int mid = r;
        while (mid < z)
        {
            mid *= 2;
        }
        mid -= z;
        L fl = lz[l], fr = lz[r];
        while (l < z || r < z)
        {
            P a = apply(apply(br[l][0], off), fl);
            P b = apply(apply(br[l][1], off), fl);
            P c = apply(apply(br[r][0], off), fr);
            P d = apply(apply(br[r][1], off), fr);
            if (a != b && cross(a, b, c) < 0)
            {
                l *= 2;
                fl = merge(fl, lz[l]);
            }
            else if (c != d && cross(b, c, d) < 0)
            {
                r = 2 * r + 1;
                fr = merge(fr, lz[r]);
            }
            else if (a == b)
            {
                r *= 2;
                fr = merge(fr, lz[r]);
            }
            else if (c == d)
            {
                l = 2 * l + 1;
                fl = merge(fl, lz[l]);
            }
            else
            {
                __int128 c1 = cross(a, b, c, d);
                __int128 c2 = cross(a, b, c, b);
                bool left;
                if (!c1 && !c2)
                {
                    left = c.x < mid;
                }
                else
                {
                    left = (__int128)c.x * c1 + ((__int128)d.x - c.x) * c2
                         < c1 * mid;
                }
                if (left)
                {
                    l = 2 * l + 1;
                    fl = merge(fl, lz[l]);
                }
                else
                {
                    r *= 2;
                    fr = merge(fr, lz[r]);
                }
            }
        }
        br[p] = {apply(pt[l], fl), apply(pt[r], fr)};
    }

    long long subMin(int p) const
    {
        // p 是完整覆盖的线段树节点；返回其区间最小值。
        L f{0, 0};
        for (int x = p; x; x >>= 1)
        {
            f = merge(f, lz[x]);
        }
        while (p < z)
        {
            P a = apply(br[p][0], f), b = apply(br[p][1], f);
            p = a.y < b.y ? 2 * p : 2 * p + 1;
            f = merge(f, lz[p]);
        }
        return apply(pt[p], f).y;
    }

  public:
    LineMinSeg(const vector<long long> &a) : n(a.size())
    {
        // a 是非空初始数组；建立每个节点的下凸包桥。
        assert(n > 0); // 调试检查，可删
        while (z < n)
        {
            z *= 2;
            h++;
        }
        pt.resize(2 * z);
        lz.assign(2 * z, L{0, 0});
        br.resize(2 * z);
        for (int i = 0; i < z; i++)
        {
            pt[z + i] = {i, i < n ? a[i] : 4'000'000'000'000'000'000LL};
            br[z + i] = {pt[z + i], pt[z + i]};
        }
        for (int p = z - 1; p; p--)
        {
            pull(p);
        }
    }

    void add(int l, int r, long long b, long long c)
    {
        // [l,r) 是修改区间，b、c 定义一次函数 b*i+c；把它加到每个 a[i]。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return;
        }
        int x = l + z, y = r + z;
        while (x < y)
        {
            if (x & 1)
            {
                lz[x] = merge(lz[x], L{b, c});
                x++;
            }
            if (y & 1)
            {
                --y;
                lz[y] = merge(lz[y], L{b, c});
            }
            x >>= 1;
            y >>= 1;
        }
        x = l + z;
        y = r + z;
        for (int d = 1; d <= h; d++)
        {
            if ((x >> d << d) != x)
            {
                pull(x >> d);
            }
            if ((y >> d << d) != y)
            {
                pull((y - 1) >> d);
            }
        }
    }

    long long min(int l, int r) const
    {
        // [l,r) 是非空查询区间；返回其中最小值。
        assert(0 <= l && l < r && r <= n); // 调试检查，可删
        long long ans = 4'000'000'000'000'000'000LL;
        for (l += z, r += z; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                long long v = subMin(l++);
                ans = ans < v ? ans : v;
            }
            if (r & 1)
            {
                long long v = subMin(--r);
                ans = ans < v ? ans : v;
            }
        }
        return ans;
    }
};
