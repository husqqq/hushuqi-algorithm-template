#pragma once

#include <bits/stdc++.h>
#include "support/base.hpp"

struct Cover2Seg
{
    vector<int> ys;   // ys 保存离散化后的纵坐标，叶子 i 表示 [ys[i], ys[i + 1])。
    vector<int> cnt;  // cnt[p] 是节点 p 对应区间被整段覆盖的次数。
    vector<int> len1; // len1[p] 是节点 p 内至少覆盖一次的实际长度。
    vector<int> len2; // len2[p] 是节点 p 内至少覆盖两次的实际长度。

    Cover2Seg(vector<int> y = {}) : ys(move(y))
    {
        // 参数 y 必须严格递增；构造后所有覆盖次数和长度均为零，无返回值。
        int n = max<int>(1, (int)ys.size() - 1);
        cnt.assign(4 * n, 0);
        len1.assign(4 * n, 0);
        len2.assign(4 * n, 0);
    }

    void pull(int p, int l, int r)
    {
        // 参数 p、l、r 表示当前节点及其半开下标区间；函数重算两种覆盖长度，无返回值。
        int full = ys[r] - ys[l];
        if (cnt[p] >= 2)
        {
            len1[p] = len2[p] = full;
            return;
        }
        int one = 0;
        int two = 0;
        if (r - l > 1)
        {
            one = len1[2 * p] + len1[2 * p + 1];
            two = len2[2 * p] + len2[2 * p + 1];
        }
        if (cnt[p] == 1)
        {
            len1[p] = full;
            len2[p] = one;
        }
        else
        {
            len1[p] = one;
            len2[p] = two;
        }
    }

    void add(int p, int l, int r, int ql, int qr, int v)
    {
        // 参数 p、l、r 是当前节点，ql、qr 是修改区间，v 为覆盖次数增量；函数原地修改，无返回值。
        if (qr <= l || r <= ql)
        {
            return;
        }
        if (ql <= l && r <= qr)
        {
            cnt[p] += v;
            pull(p, l, r);
            return;
        }
        auto m = (l + r) / 2;
        add(2 * p, l, m, ql, qr, v);
        add(2 * p + 1, m, r, ql, qr, v);
        pull(p, l, r);
    }

    void add(int l, int r, int v)
    {
        // 参数 l、r 是纵坐标离散下标半开区间，v 为 +1 或 -1；函数原地修改，无返回值。
        if (l < r)
        {
            add(1, 0, (int)ys.size() - 1, l, r, v);
        }
    }

    int twice() const
    {
        // 无参数；返回当前纵轴上至少被覆盖两次的总长度。
        return len2[1];
    }
};

// Rect[0..3] 依次是左 x、下 y、右 x、上 y，表示半开矩形 [x1,x2) x [y1,y2)。
using Rect2 = array<int, 4>;

int rectCover2(const vector<Rect2> &a)
{
    // 参数 a 是半开矩形集合；返回至少被两个矩形覆盖的并区域面积。
    // Event[0..3] 依次是横坐标、下纵坐标、上纵坐标、覆盖增量。
    using Event = array<int, 4>;
    vector<int> ys;
    vector<Event> ev;
    for (auto [x1, y1, x2, y2] : a)
    {
        if (x1 >= x2 || y1 >= y2)
        {
            continue;
        }
        ys.push_back(y1);
        ys.push_back(y2);
        ev.push_back({x1, y1, y2, 1});
        ev.push_back({x2, y1, y2, -1});
    }
    if (ev.empty())
    {
        return 0;
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(ev.begin(), ev.end());
    Cover2Seg seg(ys);
    int ans = 0;
    int pre = ev[0][0];
    for (int i = 0; i < (int)ev.size();)
    {
        auto x = ev[i][0];
        ans += (x - pre) * seg.twice();
        while (i < (int)ev.size() && ev[i][0] == x)
        {
            int l = (int)(lower_bound(ys.begin(), ys.end(), ev[i][1]) - ys.begin());
            int r = (int)(lower_bound(ys.begin(), ys.end(), ev[i][2]) - ys.begin());
            seg.add(l, r, ev[i][3]);
            i++;
        }
        pre = x;
    }
    return ans;
}

struct PerBitTrie
{
    // Node[0..2] 依次是 0 儿子编号、1 儿子编号、经过该节点的版本区间元素数。
    using Node = array<int, 3>;
    int bit;         // bit 是处理的最高二进制位，默认 62 覆盖非负 long long。
    vector<Node> tr; // tr 保存所有版本共享的节点，0 号节点为空节点。
    vector<int> rt;  // rt[i] 是前 i 个数构成的 Trie 根，rt[0] 为空版本。

    PerBitTrie(int bit = 62) : bit(bit), tr(1, Node{}), rt(1, 0)
    {
        // bit 是最高处理位；构造只含空版本的持久化 Trie。
        assert(0 <= bit && bit <= 62); // 调试检查，可删
    }

    int clone(int p)
    {
        // 参数 p 是旧节点编号；复制该节点并返回新节点编号。
        tr.push_back(tr[p]);
        return tr.size() - 1;
    }

    void append(int x)
    {
        // x 是能由 bit+1 位表示的非负整数；追加后生成新前缀版本。
        assert(0 <= x && (bit == 62 || (unsigned long long)x < (1ULL << (bit + 1)))); // 调试检查，可删
        auto old = rt.back();
        auto cur = clone(old);
        rt.push_back(cur);
        tr[cur][2]++;
        for (int b = bit; b >= 0; b--)
        {
            auto c = x >> b & 1;
            auto nxt = clone(tr[old][c]);
            tr[cur][c] = nxt;
            old = tr[old][c];
            cur = nxt;
            tr[cur][2]++;
        }
    }

    int count(int l, int r, int c) const
    {
        // 参数 l、r 是两个版本节点，c 是儿子方向；返回两版本之差中该儿子的元素数。
        return tr[tr[r][c]][2] - tr[tr[l][c]][2];
    }

    int maxXor(int l, int r, int x) const
    {
        // [l,r) 是下标区间，x 是询问值；返回最大异或值，空区间返回 -1。
        assert(0 <= l && l <= r && r < (int)rt.size()); // 调试检查，可删
        assert(0 <= x && (bit == 62 || (unsigned long long)x < (1ULL << (bit + 1)))); // 调试检查，可删
        if (l == r)
        {
            return -1;
        }
        auto u = rt[l];
        auto v = rt[r];
        int ans = 0;
        for (int b = bit; b >= 0; b--)
        {
            auto c = x >> b & 1;
            auto go = c ^ 1;
            if (count(u, v, go) == 0)
            {
                go ^= 1;
            }
            if (go != c)
            {
                ans |= 1LL << b;
            }
            u = tr[u][go];
            v = tr[v][go];
        }
        return ans;
    }
};

struct StaticMex
{
    // Node[0..2] 依次是左儿子、右儿子、该值域区间内最小的最后出现位置。
    using Node = array<int, 3>;
    int n;           // n 是原序列长度，值域只需保留 [0,n]。
    vector<Node> tr; // tr 保存所有前缀版本的动态线段树节点，0 号节点表示全为 -1。
    vector<int> rt;  // rt[i] 表示扫描完前 i 个元素后的版本根。

    StaticMex(const vector<int> &a) : n(a.size()), tr(1, Node{0, 0, -1}), rt(n + 1)
    {
        // 参数 a 是原序列；构造所有前缀版本供区间 MEX 查询，无返回值。
        for (int i = 0; i < n; i++)
        {
            rt[i + 1] = rt[i];
            if (0 <= a[i] && a[i] <= n)
            {
                rt[i + 1] = set(rt[i], 0, n + 1, a[i], i);
            }
        }
    }

    int copy(int p)
    {
        // 参数 p 是旧节点编号；复制节点并返回新编号。
        tr.push_back(tr[p]);
        return tr.size() - 1;
    }

    int val(int p) const
    {
        // 参数 p 是节点编号；返回该节点维护的最小最后出现位置，空节点返回 -1。
        return p ? tr[p][2] : -1;
    }

    int set(int p, int l, int r, int x, int v)
    {
        // 参数 p、l、r 是旧节点和值域，x 是被赋值的位置，v 是最后出现位置；返回新根编号。
        auto q = copy(p);
        if (r - l == 1)
        {
            tr[q][2] = v;
            return q;
        }
        auto m = (l + r) / 2;
        if (x < m)
        {
            tr[q][0] = set(tr[q][0], l, m, x, v);
        }
        else
        {
            tr[q][1] = set(tr[q][1], m, r, x, v);
        }
        tr[q][2] = min(val(tr[q][0]), val(tr[q][1]));
        return q;
    }

    int find(int p, int l, int r, int lim) const
    {
        // 参数 p、l、r 是查询节点和值域，lim 是左端点；返回首个最后出现位置小于 lim 的值。
        if (r - l == 1)
        {
            return l;
        }
        auto m = (l + r) / 2;
        if (val(tr[p][0]) < lim)
        {
            return find(tr[p][0], l, m, lim);
        }
        return find(tr[p][1], m, r, lim);
    }

    int query(int l, int r) const
    {
        // 参数 l、r 是原序列半开区间 [l,r)；返回该区间没有出现的最小非负整数。
        assert(0 <= l && l <= r && r <= n);
        return find(rt[r], 0, n + 1, l);
    }
};

struct ISAP
{
    // Edge[0..2] 依次是终点、反向边下标、剩余容量。
    using Edge = array<int, 3>;
    int n;                  // n 是点数，点编号为 [0,n)。
    vector<vector<Edge>> e; // e[x] 保存从 x 出发的残量边。
    vector<int> h;          // h[x] 是 x 到汇点的当前距离标号。
    vector<int> gap;        // gap[d] 是距离标号等于 d 的点数，用于 GAP 优化。
    vector<int> cur;        // cur[x] 是 x 的当前弧下标。
    int src = 0;            // src 是当前最大流调用的源点，供 GAP 优化提前终止。

    ISAP(int n = 0) : n(n), e(n), h(n), gap(n + 1), cur(n)
    {
        // 参数 n 是点数；构造空残量网络，无返回值。
    }

    void add(int x, int y, int c)
    {
        // 参数 x、y、c 是有向边起点、终点和非负容量；加入正反两条残量边，无返回值。
        auto a = (int)e[x].size();
        auto b = (int)e[y].size();
        e[x].push_back({y, b, c});
        e[y].push_back({x, a, 0});
    }

    void bfs(int t)
    {
        // 参数 t 是汇点；在反向残量意义下初始化距离标号和 gap 计数，无返回值。
        fill(h.begin(), h.end(), n);
        fill(gap.begin(), gap.end(), 0);
        queue<int> q;
        h[t] = 0;
        q.push(t);
        while (!q.empty())
        {
            auto x = q.front();
            q.pop();
            for (auto [y, rev, cap] : e[x])
            {
                if (h[y] != n || e[y][rev][2] == 0)
                {
                    continue;
                }
                h[y] = h[x] + 1;
                q.push(y);
            }
        }
        for (auto d : h)
        {
            gap[d]++;
        }
    }

    int dfs(int x, int t, int lim)
    {
        // 参数 x、t、lim 是当前点、汇点和可增广上限；返回本次实际增广量并修改残量网络。
        if (x == t)
        {
            return lim;
        }
        int used = 0;
        for (auto &i = cur[x]; i < (int)e[x].size(); i++)
        {
            auto &a = e[x][i];
            if (a[2] == 0 || h[x] != h[a[0]] + 1)
            {
                continue;
            }
            auto d = dfs(a[0], t, min(lim - used, a[2]));
            a[2] -= d;
            e[a[0]][a[1]][2] += d;
            used += d;
            if (used == lim || h[x] == n)
            {
                return used;
            }
        }
        if (--gap[h[x]] == 0)
        {
            h[src] = n;
        }
        auto nh = n - 1;
        for (auto a : e[x])
        {
            if (a[2])
            {
                nh = min(nh, h[a[0]]);
            }
        }
        h[x] = nh + 1;
        gap[h[x]]++;
        cur[x] = 0;
        return used;
    }

    int flow(int s, int t)
    {
        // 参数 s、t 是源点和汇点；返回最大流并把 e 改成最终残量网络。
        src = s;
        bfs(t);
        int ans = 0;
        while (h[s] < n)
        {
            fill(cur.begin(), cur.end(), 0);
            ans += dfs(s, t, INF);
        }
        return ans;
    }
};

struct TreeOrder
{
    vector<int> par; // par[x] 是 x 的父亲，根的父亲为 -1。
    vector<int> ord; // ord 按 DFS 前序保存全部点。
    vector<int> pos; // pos[x] 是 x 在 ord 中的下标。
    vector<int> siz; // siz[x] 是以 x 为根的子树大小，因此子树对应 ord 的连续区间。
};

TreeOrder preorderTab(const vector<vector<int>> &g, int root = 0)
{
    // 参数 g 是无向树邻接表，root 是根；返回父亲、前序、前序位置和子树大小四张表。
    auto n = (int)g.size();
    if (!n)
    {
        return {};
    }
    assert(0 <= root && root < n); // 调试检查，可删
    TreeOrder z{vector<int>(n, -1), {}, vector<int>(n), vector<int>(n, 1)};
    z.ord.reserve(n);
    auto dfs = [&](auto &&self, int x, int p) -> void
    {
        z.par[x] = p;
        z.pos[x] = z.ord.size();
        z.ord.push_back(x);
        for (auto y : g[x])
        {
            if (y != p)
            {
                self(self, y, x);
                z.siz[x] += z.siz[y];
            }
        }
    };
    dfs(dfs, root, -1);
    return z;
}

int minModInt(int m, int d, int l, int r)
{
    // 参数满足 m>0 且 0<=d,l<=r<m；返回使 l<=d*x mod m<=r 的最小非负 x，无解返回 -1。
    assert(m > 0 && 0 <= d && d < m && 0 <= l && l <= r && r < m);
    if (l == 0)
    {
        return 0;
    }
    if (d == 0)
    {
        return -1;
    }
    if (2 * d > m)
    {
        return minModInt(m, m - d, m - r, m - l);
    }
    if ((l - 1) / d < r / d)
    {
        return (l - 1) / d + 1;
    }
    auto k = minModInt(d, (-m % d + d) % d, l % d, r % d);
    if (k == -1)
    {
        return -1;
    }
    return (k * m + l - 1) / d + 1;
}

struct ValueGcd
{
    int n;                     // n 是允许输入的最大值。
    int b;                     // b=floor(sqrt(n))+1，是小因子表和 GCD 表边长。
    vector<char> prime;        // prime[x] 表示 x 是否为质数。
    vector<array<int, 3>> fac; // fac[x][0..2] 是 x 分成的三个因子，每个因子不超过 b 或本身为质数。
    vector<vector<int>> tab;   // tab[x][y] 预存 0<=x,y<=b 的最大公约数。

    ValueGcd(int n) : n(n), b(sqrtl(n) + 1), prime(n + 1, true), fac(n + 1), tab(b + 1, vector<int>(b + 1))
    {
        // 参数 n 是闭值域上界；预处理分解和小值 GCD 表，无返回值。
        prime[0] = false;
        if (n)
        {
            prime[1] = false;
        }
        for (int i = 0; i <= b; i++)
        {
            tab[i][0] = tab[0][i] = i;
        }
        for (int i = 1; i <= b; i++)
        {
            for (int j = i; j <= b; j++)
            {
                tab[i][j] = tab[j][i] = tab[i][j % i];
            }
        }
        vector<int> ps;
        fac[1] = {1, 1, 1};
        for (int i = 2; i <= n; i++)
        {
            if (prime[i])
            {
                ps.push_back(i);
                fac[i] = {1, 1, i};
            }
            for (auto p : ps)
            {
                if (i * p > n)
                {
                    break;
                }
                prime[i * p] = false;
                fac[i * p] = fac[i];
                auto at = min_element(fac[i * p].begin(), fac[i * p].end());
                *at *= p;
                if (i % p == 0)
                {
                    break;
                }
            }
        }
    }

    int small(int x, int y) const
    {
        // 参数 x 是不超过 b 或为质数的因子，y 在 [0,n]；返回 gcd(x,y)。
        if (x == 1)
        {
            return 1;
        }
        auto t = y % x;
        if (prime[x])
        {
            return t ? 1 : x;
        }
        return tab[t][x];
    }

    int get(int x, int y) const
    {
        // 参数 x、y 均在 [1,n]；返回二者最大公约数，查询阶段为常数次查表。
        assert(1 <= x && x <= n && 1 <= y && y <= n);
        auto a = small(fac[x][0], y);
        y /= a;
        auto c = small(fac[x][1], y);
        y /= c;
        auto d = small(fac[x][2], y);
        return a * c * d;
    }
};

vector<char> primeTable(int n)
{
    // 参数 n 是闭上界；返回 [0,n] 的素数标记表。
    vector<char> is(n + 1, true);
    if (n >= 0)
    {
        is[0] = false;
    }
    if (n >= 1)
    {
        is[1] = false;
    }
    for (int i = 2; i * i <= n; i++)
    {
        if (!is[i])
        {
            continue;
        }
        for (int j = i * i; j <= n; j += i)
        {
            is[j] = false;
        }
    }
    return is;
}

pair<int, int> goldbach(int n)
{
    // 参数 n 是不小于 4 的偶数；返回一组和为 n 的素数，未找到时返回 {-1,-1}。
    if (n < 4 || n % 2)
    {
        return {-1, -1};
    }
    auto is = primeTable(n);
    for (int p = 2; p <= n; p++)
    {
        if (is[p] && is[n - p])
        {
            return {p, n - p};
        }
    }
    return {-1, -1};
}

int frobenius2(int a, int b)
{
    // 参数 a、b 是正整数；互质时返回最大不可表示数 ab-a-b，否则返回 -1。
    assert(a > 0 && b > 0);
    if (gcd(a, b) != 1)
    {
        return -1;
    }
    return a * b - a - b;
}

struct GeoPoint
{
    long double x = 0; // x 是横坐标。
    long double y = 0; // y 是纵坐标。

    GeoPoint operator+(GeoPoint p) const
    {
        // 参数 p 是另一个点或向量；返回逐坐标相加结果。
        return {x + p.x, y + p.y};
    }

    GeoPoint operator-(GeoPoint p) const
    {
        // 参数 p 是另一个点或向量；返回从 p 指向当前点的向量。
        return {x - p.x, y - p.y};
    }

    GeoPoint operator*(long double k) const
    {
        // 参数 k 是缩放倍数；返回缩放后的向量。
        return {x * k, y * k};
    }

    GeoPoint operator/(long double k) const
    {
        // 参数 k 是非零除数；返回缩放后的向量。
        return {x / k, y / k};
    }
};

long double dot(GeoPoint a, GeoPoint b)
{
    // 参数 a、b 是二维向量；返回点积。
    return a.x * b.x + a.y * b.y;
}

long double cross(GeoPoint a, GeoPoint b)
{
    // 参数 a、b 是二维向量；返回有向叉积。
    return a.x * b.y - a.y * b.x;
}

long double norm(GeoPoint a)
{
    // 参数 a 是二维向量；返回欧氏长度。
    return sqrtl(dot(a, a));
}

array<GeoPoint, 2> perpBisector(GeoPoint a, GeoPoint b)
{
    // 参数 a、b 是两个不同点；返回中垂线上的一点和方向向量。
    auto m = (a + b) / 2;
    auto d = b - a;
    return {m, GeoPoint{-d.y, d.x}};
}

int rectKind(const array<GeoPoint, 4> &p)
{
    // 参数 p 是四个无序点；返回 0 表示非矩形，1 表示矩形，2 表示正方形。
    vector<long double> d;
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            d.push_back(dot(p[i] - p[j], p[i] - p[j]));
        }
    }
    sort(d.begin(), d.end());
    auto eq = [](long double a, long double b)
    {
        return fabsl(a - b) <= 1E-12L * max<long double>(1, max(fabsl(a), fabsl(b)));
    };
    if (d[0] <= 1E-24L || !eq(d[0], d[1]) || !eq(d[2], d[3]) || !eq(d[4], d[5]) || !eq(d[0] + d[2], d[4]))
    {
        return 0;
    }
    return eq(d[0], d[2]) ? 2 : 1;
}

array<GeoPoint, 6> triCenters(GeoPoint a, GeoPoint b, GeoPoint c)
{
    // 参数 a、b、c 是不共线三角形顶点；依次返回内心、外心、垂心及 a、b、c 对面的三个旁心。
    auto x = norm(b - c);
    auto y = norm(c - a);
    auto z = norm(a - b);
    auto in = (a * x + b * y + c * z) / (x + y + z);
    auto d = 2 * cross(b - a, c - a);
    auto bb = dot(b - a, b - a);
    auto cc = dot(c - a, c - a);
    GeoPoint out{
        a.x + ((c.y - a.y) * bb - (b.y - a.y) * cc) / d,
        a.y + ((b.x - a.x) * cc - (c.x - a.x) * bb) / d,
    };
    auto orth = a + b + c - out * 2;
    auto exa = (a * -x + b * y + c * z) / (-x + y + z);
    auto exb = (a * x + b * -y + c * z) / (x - y + z);
    auto exc = (a * x + b * y + c * -z) / (x + y - z);
    return {in, out, orth, exa, exb, exc};
}

bool boundSubset(const vector<int> &cnt, const vector<int> &w, int sum)
{
    // 参数 cnt、w 分别是每类物品数量和正权值，sum 是目标和；返回目标是否可达。
    assert(cnt.size() == w.size() && sum >= 0);
    vector<int> rem(sum + 1, -1);
    rem[0] = 0;
    for (int i = 0; i < (int)w.size(); i++)
    {
        for (int s = 0; s <= sum; s++)
        {
            if (rem[s] >= 0)
            {
                rem[s] = cnt[i];
            }
            else if (s < w[i] || rem[s - w[i]] <= 0)
            {
                rem[s] = -1;
            }
            else
            {
                rem[s] = rem[s - w[i]] - 1;
            }
        }
    }
    return rem[sum] >= 0;
}

string trimNum(string s)
{
    // 参数 s 是非空十进制数字串；返回删除多余前导零后的规范表示。
    auto p = s.find_first_not_of('0');
    return p == string::npos ? "0" : s.substr(p);
}

int cmpNum(const string &a, const string &b)
{
    // 参数 a、b 是规范非负十进制数；返回 -1、0、1 表示 a 与 b 的大小关系。
    if (a.size() != b.size())
    {
        return a.size() < b.size() ? -1 : 1;
    }
    if (a == b)
    {
        return 0;
    }
    return a < b ? -1 : 1;
}

string mulSmall(const string &a, int b)
{
    // 参数 a 是规范非负十进制数，b 在 [0,100]；返回精确乘积。
    if (b == 0 || a == "0")
    {
        return "0";
    }
    string c;
    int carry = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        auto v = (a[i] - '0') * b + carry;
        c.push_back(char('0' + v % 10));
        carry = v / 10;
    }
    while (carry)
    {
        c.push_back(char('0' + carry % 10));
        carry /= 10;
    }
    reverse(c.begin(), c.end());
    return c;
}

string addSmall(string a, int b)
{
    // 参数 a 是规范非负十进制数，b 在 [0,99]；返回精确和。
    for (int i = (int)a.size() - 1; i >= 0 && b; i--)
    {
        auto v = a[i] - '0' + b;
        a[i] = char('0' + v % 10);
        b = v / 10;
    }
    while (b)
    {
        a.insert(a.begin(), char('0' + b % 10));
        b /= 10;
    }
    return trimNum(a);
}

string subNum(string a, const string &b)
{
    // 参数 a、b 是规范非负十进制数且 a>=b；返回精确差。
    assert(cmpNum(a, b) >= 0);
    int borrow = 0;
    for (int i = (int)a.size() - 1, j = (int)b.size() - 1; i >= 0; i--, j--)
    {
        auto v = a[i] - '0' - borrow - (j >= 0 ? b[j] - '0' : 0);
        borrow = v < 0;
        if (borrow)
        {
            v += 10;
        }
        a[i] = char('0' + v);
    }
    return trimNum(a);
}

string bigSqrt(string s)
{
    // 参数 s 是无前导符号的非负十进制整数；返回 floor(sqrt(s)) 的十进制表示。
    assert(!s.empty());
    for (auto c : s)
    {
        assert('0' <= c && c <= '9');
    }
    s = trimNum(s);
    if (s == "0")
    {
        return "0";
    }
    if (s.size() & 1)
    {
        s = "0" + s;
    }
    string root = "0";
    string rem = "0";
    for (int i = 0; i < (int)s.size(); i += 2)
    {
        rem = trimNum(rem + s.substr(i, 2));
        auto base = mulSmall(root, 20);
        int d = 9;
        while (cmpNum(mulSmall(addSmall(base, d), d), rem) > 0)
        {
            d--;
        }
        rem = subNum(rem, mulSmall(addSmall(base, d), d));
        root = addSmall(mulSmall(root, 10), d);
    }
    return root;
}

vector<long double> quadRoots(long double a, long double b, long double c)
{
    // 参数 a、b、c 是方程 ax^2+bx+c=0 的系数；返回升序实根，重根只返回一次，无实根返回空表。
    constexpr long double eps = 1E-18L;
    if (fabsl(a) <= eps)
    {
        if (fabsl(b) <= eps)
        {
            return {};
        }
        return {-c / b};
    }
    auto d = b * b - 4 * a * c;
    if (d < -eps)
    {
        return {};
    }
    if (d <= eps)
    {
        return {-b / (2 * a)};
    }
    auto q = -0.5L * (b + copysignl(sqrtl(d), b));
    vector<long double> ans{q / a, c / q};
    sort(ans.begin(), ans.end());
    return ans;
}

vector<int> nQueens(int n)
{
    // 参数 n 是棋盘边长；返回每行皇后的 0 下标列号，n=2 或 3 时返回空表。
    if (n == 1)
    {
        return {0};
    }
    if (n < 4)
    {
        return {};
    }
    vector<int> p(n);
    auto gen = [&](auto &&self, int m, vector<int> &q) -> void
    {
        if (m & 1)
        {
            vector<int> t(m - 1);
            self(self, m - 1, t);
            copy(t.begin(), t.end(), q.begin());
            q[m - 1] = m - 1;
        }
        else if (m % 6 != 2)
        {
            for (int i = 1; i <= m / 2; i++)
            {
                q[i - 1] = 2 * i - 1;
            }
            for (int i = m / 2 + 1; i <= m; i++)
            {
                q[i - 1] = 2 * i - m - 2;
            }
        }
        else
        {
            for (int i = 1; i <= m / 2; i++)
            {
                q[i - 1] = (2 * i + m / 2 - 3) % m;
            }
            for (int i = m / 2 + 1; i <= m; i++)
            {
                q[i - 1] = m - 1 - (2 * (m - i + 1) + m / 2 - 3) % m;
            }
        }
    };
    gen(gen, n, p);
    return p;
}
