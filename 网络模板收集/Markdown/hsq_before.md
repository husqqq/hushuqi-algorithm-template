树状数组
```c++
template<typename T> struct BIT
{
    int n;
    std::vector<T> a;
    std::vector<T> b;
    BIT(int n_ = 0)
    {
        init(n_);
    }
    void init(int n_)
    {
        n = n_;
        a.assign(n, T{});
        b.assign(n, T{});
    }
    void add(int x, const T &v)
    {
        b[x] += v;
        for (int i = x + 1; i <= n; i += i & -i)
        {
            a[i - 1] = a[i - 1] + v;
        }
    }
    void modify(int x, const T &v)
    {
        add(x, v - b[x]);
    }
    T sum(int x)
    {
        T ans{};
        for (int i = x; i > 0; i -= i & -i)
        {
            ans = ans + a[i - 1];
        }
        return ans;
    }
    T rangeSum(int l, int r)
    {
        return sum(r) - sum(l);
    }
    int select(const T &k)
    {
        int x = 0;
        T cur{};
        for (int i = 1 << std::__lg(n); i; i /= 2)
        {
            if (x + i <= n && cur + a[x + i - 1] <= k)
            {
                x += i;
                cur = cur + a[x - 1];
            }
        }
        return x;
    }
};
```

```c++
template<class T> auto CartesianTree(const vector<T> &as, int gr = 0)
{
    int n = as.size();
    vector<int> ls(n, -1), rs(n, -1), stk;
    for (int i = 0; i < n; ++i)
    {
        while (!stk.empty() && ((as[i] < as[stk.back()]) ^ gr))
        {
            ls[i] = stk.back();
            stk.pop_back();
        }
        if (!stk.empty()) rs[stk.back()] = i;
        stk.push_back(i);
    }
    return make_tuple(stk[0], ls, rs);
}
```

```c++
struct Comb {
    int n;
    std::vector<Z> _fac;
    std::vector<Z> _invfac;
    std::vector<Z> _inv;
    
    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }
    
    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);
        
        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _invfac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i;
            _inv[i] = _invfac[i] * _fac[i - 1];
        }
        n = m;
    }
    
    Z fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    Z invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    Z inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    Z binom(int n, int m) {
        if (n < m || m < 0) return 0;
        return fac(n) * invfac(m) * invfac(n - m);
    }
} comb;
```

```c++
struct DSU
{
    int n;
    vector<int> f;
    vector<int> siz;
    DSU(int n)
    {
        this->n = n;
        f.assign(n + 1, 0);
        siz.assign(n + 1, 1);
        iota(f.begin(),f.end(), 0);
    }
    int find(int x)
    {
        return f[x] == x ? x : f[x] = find(f[x]);
    }
    void merge(int x,int y)
    {
        x = find(x), y = find(y);
        if (x != y)
        {
            if (siz[x] > siz[y]) swap(x, y);
            f[x] = y;
            siz[y] += siz[x];
        }
    }
};
```

```c++
std::set<std::pair<int, int>> E;

struct EBCC {
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> stk;
    std::vector<int> dfn, low, bel;
    int cur, cnt;
    
    EBCC() {}
    EBCC(int n) {
        init(n);
    }
    
    void init(int n) {
        this->n = n;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.resize(n);
        bel.assign(n, -1);
        stk.clear();
        cur = cnt = 0;
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void dfs(int x, int p) {
        dfn[x] = low[x] = cur++;
        stk.push_back(x);
        
        for (auto y : adj[x]) {
            if (y == p) {
                continue;
            }
            if (dfn[y] == -1) {
                E.emplace(x, y);
                dfs(y, x);
                low[x] = std::min(low[x], low[y]);
            } else if (bel[y] == -1 && dfn[y] < dfn[x]) {
                E.emplace(x, y);
                low[x] = std::min(low[x], dfn[y]);
            }
        }
        
        if (dfn[x] == low[x]) {
            int y;
            do {
                y = stk.back();
                bel[y] = cnt;
                stk.pop_back();
            } while (y != x);
            cnt++;
        }
    }
    
    std::vector<int> work() {
        dfs(0, -1);
        return bel;
    }
    
    struct Graph {
        int n;
        std::vector<std::pair<int, int>> edges;
        std::vector<int> siz;
        std::vector<int> cnte;
    };
    Graph compress() {
        Graph g;
        g.n = cnt;
        g.siz.resize(cnt);
        g.cnte.resize(cnt);
        for (int i = 0; i < n; i++) {
            g.siz[bel[i]]++;
            for (auto j : adj[i]) {
                if (bel[i] < bel[j]) {
                    g.edges.emplace_back(bel[i], bel[j]);
                } else if (i < j) {
                    g.cnte[bel[i]]++;
                }
            }
        }
        return g;
    }
};
```

```c++
using Real = long double;
constexpr Real eps = 1e-16;
constexpr Real inf = 1e16;
template<class T> struct Point
{
    T x;
    T y;
    Point(const T &x_ = 0, const T &y_ = 0) : x(x_), y(y_)
    {
    }
    template<class U> operator Point<U>()
    {
        return Point<U>(U(x), U(y));
    }
    Point &operator+=(const Point &p) &
    {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point &operator-=(const Point &p) &
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point &operator*=(const T &v) &
    {
        x *= v;
        y *= v;
        return *this;
    }
    Point &operator/=(const T &v) &
    {
        x /= v;
        y /= v;
        return *this;
    }
    Point operator-() const
    {
        return Point(-x, -y);
    }
    friend Point operator+(Point a, const Point &b)
    {
        return a += b;
    }
    friend Point operator-(Point a, const Point &b)
    {
        return a -= b;
    }
    friend Point operator*(Point a, const T &b)
    {
        return a *= b;
    }
    friend Point operator/(Point a, const T &b)
    {
        return a /= b;
    }
    friend Point operator*(const T &a, Point b)
    {
        return b *= a;
    }
    friend bool operator<(const Point &a, const Point &b)
    {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
    friend bool operator==(const Point &a, const Point &b)
    {
        return a.x == b.x && a.y == b.y;
    }
    friend std::istream &operator>>(std::istream &is, Point &p)
    {
        return is >> p.x >> p.y;
    }
    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};
template<class T> struct Line
{
    Point<T> a;
    Point<T> b;
    Line(const Point<T> &a_ = Point<T>(), const Point<T> &b_ = Point<T>()) : a(a_), b(b_)
    {
    }
};
// 点乘
template<class T> T dot(const Point<T> &a, const Point<T> &b)
{
    return a.x * b.x + a.y * b.y;
}
// 叉乘
template<class T> T cross(const Point<T> &a, const Point<T> &b)
{
    return a.x * b.y - a.y * b.x;
}
// 向量模长平方
template<class T> T square(const Point<T> &p)
{
    return dot(p, p);
}
// 向量模长
template<class T> double length(const Point<T> &p)
{
    return std::sqrt(square(p));
}
// 线段长度
template<class T> double length(const Line<T> &l)
{
    return length(l.a - l.b);
}
// 获得单位向量
template<class T> Point<T> normalize(const Point<T> &p)
{
    return p / length(p);
}
// 两直线是否平行
template<class T> bool parallel(const Line<T> &l1, const Line<T> &l2)
{
    return cross(l1.b - l1.a, l2.b - l2.a) == 0;
}
// 两点之间距离
template<class T> double distance(const Point<T> &a, const Point<T> &b)
{
    return length(a - b);
}
// 点到直线距离
template<class T> double distancePL(const Point<T> &p, const Line<T> &l)
{
    return std::abs(cross(l.a - l.b, l.a - p)) / length(l);
}
// 点到线段距离
template<class T> double distancePS(const Point<T> &p, const Line<T> &l)
{
    if (dot(p - l.a, l.b - l.a) < 0)
    {
        return distance(p, l.a);
    }
    if (dot(p - l.b, l.a - l.b) < 0)
    {
        return distance(p, l.b);
    }
    return distancePL(p, l);
}
// 旋转90度
template<class T> Point<T> rotate(const Point<T> &a)
{
    return Point(-a.y, a.x);
}
// 判断是否在上半平面
template<class T> int sgn(const Point<T> &a)
{
    return a.y > 0 || (a.y == 0 && a.x > 0) ? 1 : -1;
}
// 判断点是否在直线左侧
template<class T> bool pointOnLineLeft(const Point<T> &p, const Line<T> &l)
{
    return cross(l.b - l.a, p - l.a) > 0;
}
// 两直线交点
template<class T> Point<T> lineIntersection(const Line<T> &l1, const Line<T> &l2)
{
    return l1.a + (l1.b - l1.a) * (cross(l2.b - l2.a, l1.a - l2.a) / cross(l2.b - l2.a, l1.a - l1.b));
}
// 点是否在线段上
template<class T> bool pointOnSegment(const Point<T> &p, const Line<T> &l)
{
    return cross(p - l.a, l.b - l.a) == 0 && std::min(l.a.x, l.b.x) <= p.x && p.x <= std::max(l.a.x, l.b.x)
           && std::min(l.a.y, l.b.y) <= p.y && p.y <= std::max(l.a.y, l.b.y);
}
// 点在直线上
template<class T> bool pointOnLine(const Point<T> &p, const Line<T> &l)
{
    return cross(p - l.a, l.b - l.a) == 0;
}
// 射线法判 点是否在多边形内O(n)
template<class T> bool pointInPolygon(const Point<T> &a, const std::vector<Point<T> > &p)
{
    int n = p.size();
    for (int i = 0; i < n; i++)
    {
        if (pointOnSegment(a, Line(p[i], p[(i + 1) % n])))
        {
            return true;
        }
    }

    int t = 0;
    for (int i = 0; i < n; i++)
    {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        if (u.x < a.x && v.x >= a.x && pointOnLineLeft(a, Line(v, u)))
        {
            t ^= 1;
        }
        if (u.x >= a.x && v.x < a.x && pointOnLineLeft(a, Line(u, v)))
        {
            t ^= 1;
        }
    }

    return t == 1;
}
// 两个线段相交 (状态，左端点，右端点)
// 0 : 不相交
// 1 : 规范相交
// 2 : 共线且部分重合
// 3 : 端点相交
template<class T> std::tuple<int, Point<T>, Point<T> > segmentIntersection(const Line<T> &l1, const Line<T> &l2)
{
    if (std::max(l1.a.x, l1.b.x) < std::min(l2.a.x, l2.b.x))
    {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::min(l1.a.x, l1.b.x) > std::max(l2.a.x, l2.b.x))
    {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::max(l1.a.y, l1.b.y) < std::min(l2.a.y, l2.b.y))
    {
        return {0, Point<T>(), Point<T>()};
    }
    if (std::min(l1.a.y, l1.b.y) > std::max(l2.a.y, l2.b.y))
    {
        return {0, Point<T>(), Point<T>()};
    }
    if (cross(l1.b - l1.a, l2.b - l2.a) == 0)
    {
        if (cross(l1.b - l1.a, l2.a - l1.a) != 0)
        {
            return {0, Point<T>(), Point<T>()};
        }
        else
        {
            auto maxx1 = std::max(l1.a.x, l1.b.x);
            auto minx1 = std::min(l1.a.x, l1.b.x);
            auto maxy1 = std::max(l1.a.y, l1.b.y);
            auto miny1 = std::min(l1.a.y, l1.b.y);
            auto maxx2 = std::max(l2.a.x, l2.b.x);
            auto minx2 = std::min(l2.a.x, l2.b.x);
            auto maxy2 = std::max(l2.a.y, l2.b.y);
            auto miny2 = std::min(l2.a.y, l2.b.y);
            Point<T> p1(std::max(minx1, minx2), std::max(miny1, miny2));
            Point<T> p2(std::min(maxx1, maxx2), std::min(maxy1, maxy2));
            if (!pointOnSegment(p1, l1))
            {
                std::swap(p1.y, p2.y);
            }
            if (p1 == p2)
            {
                return {3, p1, p2};
            }
            else
            {
                return {2, p1, p2};
            }
        }
    }
    auto cp1 = cross(l2.a - l1.a, l2.b - l1.a);
    auto cp2 = cross(l2.a - l1.b, l2.b - l1.b);
    auto cp3 = cross(l1.a - l2.a, l1.b - l2.a);
    auto cp4 = cross(l1.a - l2.b, l1.b - l2.b);

    if ((cp1 > 0 && cp2 > 0) || (cp1 < 0 && cp2 < 0) || (cp3 > 0 && cp4 > 0) || (cp3 < 0 && cp4 < 0))
    {
        return {0, Point<T>(), Point<T>()};
    }

    Point p = lineIntersection(l1, l2);
    if (cp1 != 0 && cp2 != 0 && cp3 != 0 && cp4 != 0)
    {
        return {1, p, p};
    }
    else
    {
        return {3, p, p};
    }
}
// 两条线段最短距离
template<class T> double distanceSS(const Line<T> &l1, const Line<T> &l2)
{
    if (std::get<0>(segmentIntersection(l1, l2)) != 0)
    {
        return 0.0;
    }
    return std::min({distancePS(l1.a, l2), distancePS(l1.b, l2), distancePS(l2.a, l1), distancePS(l2.b, l1)});
}
//线段是否在多边形内(不需要是凸包可以有优角但要逆时针给出点)
template<class T> bool segmentInPolygon(const Line<T> &l, const std::vector<Point<T> > &p)
{
    int n = p.size();
    if (!pointInPolygon(l.a, p))
    {
        return false;
    }
    if (!pointInPolygon(l.b, p))
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        auto w = p[(i + 2) % n];
        auto [t, p1, p2] = segmentIntersection(l, Line(u, v));

        if (t == 1)
        {
            return false;
        }
        if (t == 0)
        {
            continue;
        }
        if (t == 2)
        {
            if (pointOnSegment(v, l) && v != l.a && v != l.b)
            {
                if (cross(v - u, w - v) > 0)
                {
                    return false;
                }
            }
        }
        else
        {
            if (p1 != u && p1 != v)
            {
                if (pointOnLineLeft(l.a, Line(v, u))
                    || pointOnLineLeft(l.b, Line(v, u)))
                {
                    return false;
                }
            }
            else if (p1 == v)
            {
                if (l.a == v)
                {
                    if (pointOnLineLeft(u, l))
                    {
                        if (pointOnLineLeft(w, l)
                            && pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (pointOnLineLeft(w, l)
                            || pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                }
                else if (l.b == v)
                {
                    if (pointOnLineLeft(u, Line(l.b, l.a)))
                    {
                        if (pointOnLineLeft(w, Line(l.b, l.a))
                            && pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (pointOnLineLeft(w, Line(l.b, l.a))
                            || pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if (pointOnLineLeft(u, l))
                    {
                        if (pointOnLineLeft(w, Line(l.b, l.a))
                            || pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (pointOnLineLeft(w, l)
                            || pointOnLineLeft(w, Line(u, v)))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
// 半平面交
template<class T> std::vector<Point<T> > hp(std::vector<Line<T> > lines)
{
    std::sort(lines.begin(), lines.end(), [&](auto l1, auto l2)
    {
        auto d1 = l1.b - l1.a;
        auto d2 = l2.b - l2.a;
        if (sgn(d1) != sgn(d2))
        {
            return sgn(d1) == 1;
        }
        return cross(d1, d2) > 0;
    });

    std::deque<Line<T> > ls;
    std::deque<Point<T> > ps;
    for (auto l: lines)
    {
        if (ls.empty())
        {
            ls.push_back(l);
            continue;
        }
        while (!ps.empty() && !pointOnLineLeft(ps.back(), l))
        {
            ps.pop_back();
            ls.pop_back();
        }
        while (!ps.empty() && !pointOnLineLeft(ps[0], l))
        {
            ps.pop_front();
            ls.pop_front();
        }
        if (cross(l.b - l.a, ls.back().b - ls.back().a) == 0)
        {
            if (dot(l.b - l.a, ls.back().b - ls.back().a) > 0)
            {
                if (!pointOnLineLeft(ls.back().a, l))
                {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {};
        }

        ps.push_back(lineIntersection(ls.back(), l));
        ls.push_back(l);
    }

    while (!ps.empty() && !pointOnLineLeft(ps.back(), ls[0]))
    {
        ps.pop_back();
        ls.pop_back();
    }
    if (ls.size() <= 2)
    {
        return {};
    }
    ps.push_back(lineIntersection(ls[0], ls.back()));

    return std::vector(ps.begin(), ps.end());
}
// 静态凸包
template<class T> std::vector<Point<T> > getHull(std::vector<Point<T> > p)
{
    int n = p.size();
    if (n <= 2) return p;
    std::sort(p.begin(), p.end(), [&](auto a, auto b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    std::vector<Point<T> > res;
    res.reserve(n + 1);
    for (int i = 0; i < n; i++)
    {
        while (res.size() > 1 && cross(res[res.size() - 1] - res[res.size() - 2], p[i] - res[res.size() - 1]) <= 0)
        {
            res.pop_back();
        }
        res.push_back(p[i]);
    }
    int sz = res.size();
    for (int i = n - 2; i >= 0; --i)
    {
        while (res.size() > sz && cross(res[res.size() - 1] - res[res.size() - 2], p[i] - res[res.size() - 1]) <= 0)
        {
            res.pop_back();
        }
        res.push_back(p[i]);
    }
    res.pop_back();
    return res;
}
// 把凸包左下的点放到第一个
template<class T> std::vector<Point<T> > changeHull(std::vector<Point<T> > hull)
{
    int idx = 0;
    for (int i = 1; i < hull.size(); i++)
    {
        if (hull[i].x < hull[idx].x || (hull[i].x == hull[idx].x && hull[i].y < hull[idx].y))
        {
            idx = i;
        }
    }
    rotate(hull.begin(), hull.begin() + idx, hull.end());
    return hull;
}
template<class T> bool PointInPolygon(const Point<T> &a, const std::vector<Point<T> > &p)
{
    const int N = p.size();
    if (N < 3) return false;
    if (pointOnLine(a, Line(p[0], p.back())) || pointOnLine(a, Line(p[0], p[1])))
    {
        return false;
    }
    if (pointOnLineLeft(a, Line(p[0], p.back())) || !pointOnLineLeft(a, Line(p[0], p[1])))
    {
        return false;
    }
    int l = 1, r = N - 1, res = 0;
    auto check = [&](int mid)-> bool
    {
        return pointOnLineLeft(a, Line(p[0], p[mid]));
    };
    while (l <= r)
    {
        int mid = (l + r) >> 1;
        if (check(mid))
        {
            res = mid;
            l = mid + 1;
        }
        else r = mid - 1;
    }
    if (pointOnLine(a, {p[res], p[res + 1]})) return false;
    return pointOnLineLeft(a, {p[res], p[res + 1]});
}
// 获得点到凸包的切点(点不能在凸包上，凸包必须严格凸)
template<class T> array<Point<T>, 2> getTangent(const Point<T> p, const std::vector<Point<T> > hull)
{
    int n = hull.size();
    if (n == 0) return {Point<T>(), Point<T>()};
    if (n == 1) return {hull[0], hull[0]};
    if (pointOnSegment(p, Line<T>(hull[0], hull.back())))
    {
        return {hull.back(), hull[0]};
    }
    auto find_max = [&](auto cmp) -> int
    {
        int l = 0, r = n - 1;
        if (cmp(hull[0], hull[n - 1]))
        {
            while (l + 1 < r)
            {
                int mid = (l + r) / 2;
                if (cmp(hull[mid], hull[l]) && cmp(hull[mid], hull[mid - 1])) l = mid;
                else r = mid;
            }
            return l;
        }
        while (l + 1 < r)
        {
            int mid = (l + r) / 2;
            if (cmp(hull[mid], hull[r]) && cmp(hull[mid], hull[mid + 1])) r = mid;
            else l = mid;
        }
        return r;
    };
    auto fl = [&](const Point<T> &x, const Point<T> &y)
    {
        return pointOnLineLeft(x, Line<T>(p, y));
    };
    auto fr = [&](const Point<T> &x, const Point<T> &y)
    {
        return pointOnLineLeft(y, Line<T>(p, x));
    };

    int l_idx = find_max(fl);
    int r_idx = find_max(fr);

    return {hull[l_idx], hull[r_idx]};
}
```
```c++
struct LCA
{
    int n, idx{};
    vector<vector<int> > ver;
    vector<int> siz, dep;
    vector<int> top, son, parent;
    vector<int> tin, tout;
    int timer = 0;
    explicit LCA(int n)
    {
        this->n = n;
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        ver.resize(n + 1);
        siz.resize(n + 1);
        dep.resize(n + 1);
        top.resize(n + 1);
        son.resize(n + 1);
        parent.resize(n + 1);
        timer = 0;
    }
    void add(int x, int y)
    {
        // 建立双向边
        ver[x].push_back(y);
        ver[y].push_back(x);
    }
    void dfs1(int x)
    {
        siz[x] = 1;
        dep[x] = dep[parent[x]] + 1;
        tin[x] = ++timer;
        for (auto y: ver[x])
        {
            if (y == parent[x]) continue;
            parent[y] = x;
            dfs1(y);
            siz[x] += siz[y];
            if (siz[y] > siz[son[x]])
            {
                son[x] = y;
            }
        }
        tout[x] = timer;
    }
    [[nodiscard]] bool is_anc(const int u, const int v) const
    {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }
    void dfs2(const int x, const int up)
    {
        top[x] = up;
        if (son[x]) dfs2(son[x], up);
        for (auto y: ver[x])
        {
            if (y == parent[x] || y == son[x]) continue;
            dfs2(y, y);
        }
    }
    [[nodiscard]] int lca(int x, int y) const
    {
        while (top[x] != top[y])
        {
            if (dep[top[x]] > dep[top[y]])
            {
                x = parent[top[x]];
            }
            else
            {
                y = parent[top[y]];
            }
        }
        return dep[x] < dep[y] ? x : y;
    }
    [[nodiscard]] int clac(int x, int y) const
    {
        // 查询两点间距离
        return dep[x] + dep[y] - 2 * dep[lca(x, y)];
    }
    void work(const int root = 1)
    {
        // 在此初始化
        dfs1(root);
        dfs2(root, root);
    }
    [[nodiscard]] bool judge(int a,int b,int t) const
    {
        // 判断t在不在a b之间
        return clac(a, b) == clac(a, t) + clac(t, b);
    }
};
```

```c++
struct LCA
{
    int n;
    vector<vector<int> > adj;
    vector<vector<int> > up;
    vector<int> depth;
    vector<int> siz;
    explicit LCA(int n)
    {
        this->n = n;
        adj.assign(n + 1, {});
        up.assign(LOG, vector<int>(n + 1));
        depth.assign(n + 1, 0);
        siz.assign(n + 1, 1);
    }
    void add(int u,int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void dfs(int u,int fa)
    {
        for (auto v: adj[u])
        {
            if (v == fa) continue;
            depth[v] = depth[u] + 1;
            up[0][v] = u;
            dfs(v, u);
            siz[u] += siz[v];
        }
    }
    void init(int root = 0)
    {
        dfs(root, 0);
        for (int k = 1; k < LOG; k++)
        {
            for (int i = 0; i <= n; i++)
            {
                up[k][i] = up[k - 1][up[k - 1][i]];
            }
        }
    }
    [[nodiscard]] int lca(int u,int v) const
    {
        if (depth[u] < depth[v]) swap(u, v);
        if (depth[u] > depth[v])
        {
            for (int k = LOG - 1; k >= 0; k--)
            {
                if (depth[up[k][u]] >= depth[v])
                {
                    u = up[k][u];
                }
            }
        }
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--)
        {
            if (up[k][u] != up[k][v])
            {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }
    int dist(int u,int v)
    {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
    bool check(int u,int v,int k)
    {
        return dist(u, v) == (dist(u, k) + dist(k, v));
    }
    int getsiz(int u,int v)
    {
        if (depth[u] >= depth[v]) return siz[u];
        for (int k = LOG - 1; k >= 0; k--)
        {
            if ((depth[v] - depth[u] - 1) >> k & 1)
            {
                v = up[k][v];
            }
        }
        if (up[0][v] == u)
        {
            return n + 1 - siz[v];
        }
        return siz[u];
    }
};
```

```c++
template<class Info, class Tag> struct LazySegmentTree
{
    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;
    LazySegmentTree() : n(0)
    {
    }
    LazySegmentTree(int n_, Info v_ = Info())
    {
        init(n_, v_);
    }
    template<class T> LazySegmentTree(std::vector<T> init_)
    {
        init(init_);
    }
    void init(int n_, Info v_ = Info())
    {
        init(std::vector(n_, v_));
    }
    template<class T> void init(std::vector<T> init_)
    {
        n = init_.size();
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r)
        {
            if (r - l == 1)
            {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p)
    {
        info[p] = info[2 * p] + info[2 * p + 1];
    }
    void apply(int p, const Tag &v)
    {
        info[p].apply(v);
        tag[p].apply(v);
    }
    void push(int p)
    {
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag();
    }
    void modify(int p, int l, int r, int x, const Info &v)
    {
        if (r - l == 1)
        {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        if (x < m)
        {
            modify(2 * p, l, m, x, v);
        }
        else
        {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v)
    {
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y)
    {
        if (l >= y || r <= x)
        {
            return Info();
        }
        if (l >= x && r <= y)
        {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p);
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r)
    {
        return rangeQuery(1, 0, n, l, r);
    }
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v)
    {
        if (l >= y || r <= x)
        {
            return;
        }
        if (l >= x && r <= y)
        {
            apply(p, v);
            return;
        }
        int m = (l + r) / 2;
        push(p);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p);
    }
    void rangeApply(int l, int r, const Tag &v)
    {
        return rangeApply(1, 0, n, l, r, v);
    }
    template<class F> int findFirst(int p, int l, int r, int x, int y, F &&pred)
    {
        if (l >= y || r <= x)
        {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1)
        {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F> int findFirst(int l, int r, F &&pred)
    {
        return findFirst(1, 0, n, l, r, pred);
    }
    template<class F> int findLast(int p, int l, int r, int x, int y, F &&pred)
    {
        if (l >= y || r <= x)
        {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p]))
        {
            return -1;
        }
        if (r - l == 1)
        {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1)
        {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F> int findLast(int l, int r, F &&pred)
    {
        return findLast(1, 0, n, l, r, pred);
    }
};
struct Tag
{
    int x = 0;
    void apply(const Tag &t) &
    {
        x = max(x, t.x);
    }
};
struct Info
{
    int x = 0;
    void apply(const Tag &t) &
    {
        x = max(x, t.x);
    }
};
Info operator+(const Info &a, const Info &b)
{
    return {max(a.x, b.x)};
}
```

```c++
template<typename T = int, class Compare = std::less<T> > class LiChaoTree
{
    private:
        struct Line
        {
            T k, b;
            Line(T _k = 0, T _b = 0) : k(_k), b(_b)
            {
            }
            T operator()(T x) const
            {
                return k * x + b;
            }
        };
        struct Node
        {
            int ls, rs;
            Line f;
            bool has_line;
            Node() : ls(0), rs(0), f(Line(0, 0)), has_line(false)
            {
            }
        };
        std::vector<Node> tr;
        int root;
        T treel, treer;
        T worst_val;
        Compare comp;
        int newNode()
        {
            tr.emplace_back();
            return tr.size() - 1;
        }
        void insert(int p, T l, T r, Line f)
        {
            if (!tr[p].has_line)
            {
                tr[p].f = f;
                tr[p].has_line = true;
                return;
            }
            T mid = l + ((r - l) >> 1);
            if (comp(f(mid), tr[p].f(mid)))
            {
                std::swap(tr[p].f, f);
            }
            if (l == r) return;

            if (comp(f(l), tr[p].f(l)))
            {
                if (!tr[p].ls)
                {
                    int u = newNode();
                    tr[p].ls = u;
                }
                insert(tr[p].ls, l, mid, f);
            }
            else if (comp(f(r), tr[p].f(r)))
            {
                if (!tr[p].rs)
                {
                    int u = newNode();
                    tr[p].rs = u;
                }
                insert(tr[p].rs, mid + 1, r, f);
            }
        }
        void add(int p, T l, T r, T ql, T qr, Line f)
        {
            if (ql <= l && r <= qr)
            {
                insert(p, l, r, f);
                return;
            }
            T mid = l + ((r - l) >> 1);
            if (ql <= mid)
            {
                if (!tr[p].ls)
                {
                    int u = newNode();
                    tr[p].ls = u;
                }
                add(tr[p].ls, l, mid, ql, qr, f);
            }
            if (mid < qr)
            {
                if (!tr[p].rs)
                {
                    int u = newNode();
                    tr[p].rs = u;
                }
                add(tr[p].rs, mid + 1, r, ql, qr, f);
            }
        }
        T query(int p, T l, T r, T x)
        {
            if (!p || !tr[p].has_line) return worst_val;
            T res = tr[p].f(x);
            if (l == r) return res;
            T mid = l + ((r - l) >> 1);
            if (x <= mid)
            {
                T child_res = query(tr[p].ls, l, mid, x);
                if (comp(child_res, res)) res = child_res;
            }
            else
            {
                T child_res = query(tr[p].rs, mid + 1, r, x);
                if (comp(child_res, res)) res = child_res;
            }
            return res;
        }
    public:
        LiChaoTree(T _worst_val = std::numeric_limits<T>::max() / 2) : root(0), treel(0), treer(0),
                                                                       worst_val(_worst_val)
        {
            tr.emplace_back();
        }
        void Init(T _treel, T _treer, T _worst_val)
        {
            treel = _treel;
            treer = _treer;
            worst_val = _worst_val;
            clear();
        }
        void Init(T _treel, T _treer)
        {
            treel = _treel;
            treer = _treer;
            clear();
        }
        void clear()
        {
            tr.clear();
            tr.emplace_back();
            root = 0;
        }
        using Function = Line;
        void operator()(T l, T r, Function f)
        {
            if (!root) root = newNode();
            add(root, treel, treer, l, r, f);
        }
        void operator()(Function f)
        {
            if (!root) root = newNode();
            insert(root, treel, treer, f);
        }
        T operator[](T x)
        {
            return query(root, treel, treer, x);
        }
};
```

```c++
template<class T>
struct MinCostFlow {
    struct _Edge {
        int to;
        T cap;
        T cost;
        _Edge(int to_, T cap_, T cost_) : to(to_), cap(cap_), cost(cost_) {}
    };
    int n;
    std::vector<_Edge> e;
    std::vector<std::vector<int>> g;
    std::vector<T> h, dis;
    std::vector<int> pre;
    bool dijkstra(int s, int t) {
        dis.assign(n, std::numeric_limits<T>::max());
        pre.assign(n, -1);
        std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> que;
        dis[s] = 0;
        que.emplace(0, s);
        while (!que.empty()) {
            T d = que.top().first;
            int u = que.top().second;
            que.pop();
            if (dis[u] != d) {
                continue;
            }
            for (int i : g[u]) {
                int v = e[i].to;
                T cap = e[i].cap;
                T cost = e[i].cost;
                if (cap > 0 && dis[v] > d + h[u] - h[v] + cost) {
                    dis[v] = d + h[u] - h[v] + cost;
                    pre[v] = i;
                    que.emplace(dis[v], v);
                }
            }
        }
        return dis[t] != std::numeric_limits<T>::max();
    }
    MinCostFlow() {}
    MinCostFlow(int n_) {
        init(n_);
    }
    void init(int n_) {
        n = n_;
        e.clear();
        g.assign(n, {});
    }
    void addEdge(int u, int v, T cap, T cost) {
        g[u].push_back(e.size());
        e.emplace_back(v, cap, cost);
        g[v].push_back(e.size());
        e.emplace_back(u, 0, -cost);
    }
    std::pair<T, T> flow(int s, int t) {
        T flow = 0;
        T cost = 0;
        h.assign(n, 0);
        while (dijkstra(s, t)) {
            for (int i = 0; i < n; ++i) {
                h[i] += dis[i];
            }
            T aug = std::numeric_limits<int>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                aug = std::min(aug, e[pre[i]].cap);
            }
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                e[pre[i]].cap -= aug;
                e[pre[i] ^ 1].cap += aug;
            }
            flow += aug;
            cost += aug * h[t];
        }
        return std::make_pair(flow, cost);
    }
    struct Edge {
        int from;
        int to;
        T cap;
        T cost;
        T flow;
    };
    std::vector<Edge> edges() {
        std::vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.cost = e[i].cost;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }
};
```

```c++
using i64 = long long;
vector<vector<i64> > operator*(const vector<vector<i64> > &a, const vector<vector<i64> > &b)
{
    int n = a.size();
    vector c(n, vector(n, 0ll));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                c[i][k] += a[i][j] * b[j][k];
            }
        }
    }
    return c;
}
vector<vector<int> > Power(const vector<vector<i64> > &A, long long K)
{
    int n = A.size();
    auto C(A);
    vector ans(n, vector(n, 0ll));
    for (int i = 0; i < n; i++)
    {
        ans[i][i] = 1;
    }
    for (; K; K /= 2, C = C * C)
    {
        if (K % 2)
        {
            ans = ans * C;
        }
    }
    return ans;
}
```

```c++
char buf[1 << 21], *p1 = buf, *p2 = buf;
inline char getc() {
    return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 21, stdin), p1 == p2) ? 0 : *p1++;
}
template<typename T> void Cin(T &a) {
    T ans = 0;
    bool f = 0;
    char c = getc();
    for (; c < '0' || c > '9'; c = getc()) {
        if (c == '-') f = -1;
    }
    for (; c >= '0' && c <= '9'; c = getc()) {
        ans = ans * 10 + c - '0';
    }
    a = f ? -ans : ans;
}
template<typename T, typename... Args> void Cin(T &a, Args &...args) {
    Cin(a), Cin(args...);
}
template<typename T> void Cout(T x) { // 注意，这里输出不带换行
    if (x < 0) putchar('-'), x = -x;
    if (x > 9) Cout(x / 10);
    putchar(x % 10 + '0');
}
```

```c++
template<class T,
    class Cmp = std::less<T> > struct RMQ//默认最小值
{
    const Cmp cmp = Cmp();
    static constexpr unsigned B = 64;
    using u64 = unsigned long long;
    int n;
    std::vector<std::vector<T> > a;
    std::vector<T> pre, suf, ini;
    std::vector<u64> stk;
    RMQ()
    {
    }
    RMQ(const std::vector<T> &v)
    {
        init(v);
    }
    void init(const std::vector<T> &v)
    {
        n = v.size();
        pre = suf = ini = v;
        stk.resize(n);
        if (!n)
        {
            return;
        }
        const int M = (n - 1) / B + 1;
        const int lg = std::__lg(M);
        a.assign(lg + 1, std::vector<T>(M));
        for (int i = 0; i < M; i++)
        {
            a[0][i] = v[i * B];
            for (int j = 1; j < B && i * B + j < n; j++)
            {
                a[0][i] = std::min(a[0][i], v[i * B + j], cmp);
            }
        }
        for (int i = 1; i < n; i++)
        {
            if (i % B)
            {
                pre[i] = std::min(pre[i], pre[i - 1], cmp);
            }
        }
        for (int i = n - 2; i >= 0; i--)
        {
            if (i % B != B - 1)
            {
                suf[i] = std::min(suf[i], suf[i + 1], cmp);
            }
        }
        for (int j = 0; j < lg; j++)
        {
            for (int i = 0; i + (2 << j) <= M; i++)
            {
                a[j + 1][i] = std::min(a[j][i], a[j][i + (1 << j)], cmp);
            }
        }
        for (int i = 0; i < M; i++)
        {
            const int l = i * B;
            const int r = std::min(1U * n, l + B);
            u64 s = 0;
            for (int j = l; j < r; j++)
            {
                while (s && cmp(v[j], v[std::__lg(s) + l]))
                {
                    s ^= 1ULL << std::__lg(s);
                }
                s |= 1ULL << (j - l);
                stk[j] = s;
            }
        }
    }
    T operator()(int l, int r)
    {
        if (l / B != (r - 1) / B)
        {
            T ans = std::min(suf[l], pre[r - 1], cmp);
            l = l / B + 1;
            r = r / B;
            if (l < r)
            {
                int k = std::__lg(r - l);
                ans = std::min({ans, a[k][l], a[k][r - (1 << k)]}, cmp);
            }
            return ans;
        }
        else
        {
            int x = B * (l / B);
            return ini[__builtin_ctzll(stk[r - 1] >> (l - x)) + l];
        }
    }
};
```

```c++
mt19937_64 rg(random_device{}());
int rnd(int l, int r)
{
    return l + rg() % (r - l + 1);
}
```

```c++

struct SCC
{
    int n;
    std::vector<std::vector<int> > adj;
    std::vector<int> stk;
    std::vector<int> dfn, low, bel;
    int cur, cnt;
    SCC()
    {
    }
    SCC(int n)
    {
        init(n);
    }
    void init(int n)
    {
        this->n = n;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.resize(n);
        bel.assign(n, -1);
        stk.clear();
        cur = cnt = 0;
    }
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
    }
    void dfs(int x)
    {
        dfn[x] = low[x] = cur++;
        stk.push_back(x);

        for (auto y: adj[x])
        {
            if (dfn[y] == -1)
            {
                dfs(y);
                low[x] = std::min(low[x], low[y]);
            }
            else if (bel[y] == -1)
            {
                low[x] = std::min(low[x], dfn[y]);
            }
        }

        if (dfn[x] == low[x])
        {
            int y;
            do
            {
                y = stk.back();
                bel[y] = cnt;
                stk.pop_back();
            } while (y != x);
            cnt++;
        }
    }
    std::vector<int> work()
    {
        for (int i = 0; i < n; i++)
        {
            if (dfn[i] == -1)
            {
                dfs(i);
            }
        }
        return bel;
    }
};
```

```c++
template<class Info,
    class Merge = std::plus<Info> > struct SegmentTree
{
    const int n;
    const Merge merge;
    std::vector<Info> info;
    SegmentTree(int n) : n(n), merge(Merge()), info(4 << std::__lg(n))
    {
    }
    SegmentTree(std::vector<Info> init) : SegmentTree(init.size())
    {
        std::function<void(int, int, int)> build = [&](int p, int l, int r)
        {
            if (r - l == 1)
            {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p)
    {
        info[p] = merge(info[2 * p], info[2 * p + 1]);
    }
    void modify(int p, int l, int r, int x, const Info &v)
    {
        if (r - l == 1)
        {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (x < m)
        {
            modify(2 * p, l, m, x, v);
        }
        else
        {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v)
    {
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y)
    {
        if (l >= y || r <= x)
        {
            return Info();
        }
        if (l >= x && r <= y)
        {
            return info[p];
        }
        int m = (l + r) / 2;
        return merge(rangeQuery(2 * p, l, m, x, y), rangeQuery(2 * p + 1, m, r, x, y));
    }
    Info rangeQuery(int l, int r)
    {
        return rangeQuery(1, 0, n, l, r);
    }
};
struct Info
{
};
Info operator+(const Info &a, const Info &b)
{
}
```

```c++
struct TopKSum
{
    std::multiset<int> topK;
    std::multiset<int> rest;
    int sumK;
    int K;
    TopKSum(int k) : K(k), sumK(0)
    {
    }
    void insert(int x)
    {
        topK.insert(x);
        sumK += x;
        if (topK.size() > K)
        {
            int min_val = *topK.begin();
            topK.erase(topK.begin());
            sumK -= min_val;
            rest.insert(min_val);
        }
    }
    void remove(int x)
    {
        if (!topK.empty() && x >= *topK.begin())
        {
            auto it = topK.find(x);
            if (it != topK.end())
            {
                topK.erase(it);
                sumK -= x;
                if (!rest.empty())
                {
                    auto last_it = std::prev(rest.end());
                    int max_rest = *last_it;
                    rest.erase(last_it);
                    topK.insert(max_rest);
                    sumK += max_rest;
                }
            }
        }
        else
        {
            auto it = rest.find(x);
            if (it != rest.end())
            {
                rest.erase(it);
            }
        }
    }
    int query() const
    {
        if (topK.size() < K)
        {
            return -1;
        }
        return sumK;
    }
};
```
```c++
template<int ALPHA = 26, char BASE = 'a'> struct Trie
{
    vector<array<int, ALPHA> > ch;
    vector<int> cnt, end;
    Trie()
    {
        newNode();
    }
    int newNode()
    {
        ch.push_back({});
        cnt.push_back(0);
        end.push_back(0);
        return ch.size() - 1;
    }
    int insert(const string &s)
    {
        int p = 0;
        cnt[p]++;

        for (char c: s)
        {
            int x = c - BASE;
            assert(0 <= x && x < ALPHA);

            if (!ch[p][x])
            {
                ch[p][x] = newNode();
            }

            p = ch[p][x];
            cnt[p]++;
        }

        end[p]++;
        return p;
    }
    // 返回 s 与 Trie 中任意字符串的最长公共前缀长度
    int lcp(const string &s) const
    {
        int p = 0;
        int len = 0;

        for (char c: s)
        {
            int x = c - BASE;
            if (x < 0 || x >= ALPHA || !ch[p][x])
            {
                break;
            }

            p = ch[p][x];
            len++;
        }

        return len;
    }
    // 返回字符串 s 对应的节点，不存在则返回 -1
    int find(const string &s) const
    {
        int p = 0;

        for (char c: s)
        {
            int x = c - BASE;
            if (x < 0 || x >= ALPHA || !ch[p][x])
            {
                return -1;
            }
            p = ch[p][x];
        }

        return p;
    }
    // 查询有多少个插入字符串以 s 为前缀
    int prefixCount(const string &s) const
    {
        int p = find(s);
        return p == -1 ? 0 : cnt[p];
    }
    // 查询字符串 s 被完整插入了多少次
    int stringCount(const string &s) const
    {
        int p = find(s);
        return p == -1 ? 0 : end[p];
    }
    int size() const
    {
        return ch.size();
    }
};
```

```c++
using u64 = unsigned long long;
template<int LOG = 30> struct BinaryTrie
{
    static_assert(1 <= LOG && LOG <= 64);
    vector<array<int, 2> > ch;
    vector<int> cnt;
    BinaryTrie(int n = 0)
    {
        if (n)
        {
            ch.reserve(1LL * n * (LOG + 1));
            cnt.reserve(1LL * n * (LOG + 1));
        }
        newNode();
    }
    int newNode()
    {
        ch.push_back({});
        cnt.push_back(0);
        return ch.size() - 1;
    }
    bool valid(u64 x) const
    {
        if constexpr (LOG == 64)
        {
            return true;
        }
        else
        {
            return x < (u64(1) << LOG);
        }
    }
    int size() const
    {
        return cnt[0];
    }
    bool empty() const
    {
        return size() == 0;
    }
    void clear()
    {
        ch.clear();
        cnt.clear();
        newNode();
    }
    // 查询数 x 出现了多少次
    int count(u64 x) const
    {
        assert(valid(x));

        int p = 0;
        for (int i = LOG - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            if (!ch[p][b])
            {
                return 0;
            }
            p = ch[p][b];
        }
        return cnt[p];
    }
    // v = 1 表示插入，v = -1 表示删除
    // 也可以一次插入或删除多个相同的数
    void add(u64 x, int v = 1)
    {
        assert(valid(x));
        assert(v != 0);

        if (v < 0)
        {
            assert(count(x) >= -v);
        }

        int p = 0;
        cnt[p] += v;

        for (int i = LOG - 1; i >= 0; i--)
        {
            int b = x >> i & 1;

            if (!ch[p][b])
            {
                assert(v > 0);
                int q = newNode();
                ch[p][b] = q;
            }

            p = ch[p][b];
            cnt[p] += v;
        }
    }
    void insert(u64 x)
    {
        add(x, 1);
    }
    void erase(u64 x)
    {
        add(x, -1);
    }
    // 返回 max(x xor y)
    u64 maxXor(u64 x) const
    {
        assert(valid(x));
        assert(!empty());

        int p = 0;
        u64 ans = 0;

        for (int i = LOG - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int to = b ^ 1;

            if (!ch[p][to] || cnt[ch[p][to]] == 0)
            {
                to ^= 1;
            }

            if (b != to)
            {
                ans |= u64(1) << i;
            }

            p = ch[p][to];
        }

        return ans;
    }
    // 返回使 x xor y 最大的 y
    u64 maxElement(u64 x) const
    {
        return x ^ maxXor(x);
    }
    // 返回 min(x xor y)
    u64 minXor(u64 x) const
    {
        assert(valid(x));
        assert(!empty());

        int p = 0;
        u64 ans = 0;

        for (int i = LOG - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int to = b;

            if (!ch[p][to] || cnt[ch[p][to]] == 0)
            {
                to ^= 1;
            }

            if (b != to)
            {
                ans |= u64(1) << i;
            }

            p = ch[p][to];
        }

        return ans;
    }
    // 返回使 x xor y 最小的 y
    u64 minElement(u64 x) const
    {
        return x ^ minXor(x);
    }
    // 把所有 y 按照 x xor y 从小到大排序
    // 返回第 k 小的 x xor y，k 从 0 开始
    u64 kthXor(u64 x, int k) const
    {
        assert(valid(x));
        assert(0 <= k && k < size());

        int p = 0;
        u64 ans = 0;

        for (int i = LOG - 1; i >= 0; i--)
        {
            int b = x >> i & 1;
            int q = ch[p][b];
            int c = q ? cnt[q] : 0;

            if (k < c)
            {
                p = q;
            }
            else
            {
                k -= c;
                p = ch[p][b ^ 1];
                ans |= u64(1) << i;
            }
        }

        return ans;
    }
    // 返回使 x xor y 第 k 小的 y，k 从 0 开始
    u64 kthElement(u64 x, int k) const
    {
        return x ^ kthXor(x, k);
    }
    // 查询有多少个 y 满足 (x xor y) < k
    int countXorLess(u64 x, u64 k) const
    {
        assert(valid(x));

        if constexpr (LOG < 64)
        {
            if (k >= (u64(1) << LOG))
            {
                return size();
            }
        }

        int p = 0;
        int ans = 0;

        for (int i = LOG - 1; i >= 0 && p; i--)
        {
            int a = x >> i & 1;
            int b = k >> i & 1;

            if (b)
            {
                int q = ch[p][a];
                if (q)
                {
                    ans += cnt[q];
                }
                p = ch[p][a ^ 1];
            }
            else
            {
                p = ch[p][a];
            }
        }

        return ans;
    }
};
```

```c++
struct TwoSat {
    int n;
    std::vector<std::vector<int>> e;
    std::vector<bool> ans;
    TwoSat(int n) : n(n), e(2 * n), ans(n) {}
    void addClause(int u, bool f, int v, bool g) {
        e[2 * u + !f].push_back(2 * v + g);
        e[2 * v + !g].push_back(2 * u + f);
    }
    bool satisfiable() {
        std::vector<int> id(2 * n, -1), dfn(2 * n, -1), low(2 * n, -1);
        std::vector<int> stk;
        int now = 0, cnt = 0;
        std::function<void(int)> tarjan = [&](int u) {
            stk.push_back(u);
            dfn[u] = low[u] = now++;
            for (auto v : e[u]) {
                if (dfn[v] == -1) {
                    tarjan(v);
                    low[u] = std::min(low[u], low[v]);
                } else if (id[v] == -1) {
                    low[u] = std::min(low[u], dfn[v]);
                }
            }
            if (dfn[u] == low[u]) {
                int v;
                do {
                    v = stk.back();
                    stk.pop_back();
                    id[v] = cnt;
                } while (v != u);
                ++cnt;
            }
        };
        for (int i = 0; i < 2 * n; ++i) if (dfn[i] == -1) tarjan(i);
        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
    std::vector<bool> answer() { return ans; }
};
```

```c++
using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;
using i128 = __int128;
template<class T> constexpr T power(T a, u64 b, T res = 1)
{
    for (; b != 0; b /= 2, a *= a)
    {
        if (b & 1)
        {
            res *= a;
        }
    }
    return res;
}
template<u32 P> constexpr u32 mulMod(u32 a, u32 b)
{
    return u64(a) * b % P;
}
template<u64 P> constexpr u64 mulMod(u64 a, u64 b)
{
    u64 res = a * b - u64(1.L * a * b / P - 0.5L) * P;
    res %= P;
    return res;
}
constexpr i64 safeMod(i64 x, i64 m)
{
    x %= m;
    if (x < 0)
    {
        x += m;
    }
    return x;
}
constexpr std::pair<i64, i64> invGcd(i64 a, i64 b)
{
    a = safeMod(a, b);
    if (a == 0)
    {
        return {b, 0};
    }

    i64 s = b, t = a;
    i64 m0 = 0, m1 = 1;

    while (t)
    {
        i64 u = s / t;
        s -= t * u;
        m0 -= m1 * u;

        std::swap(s, t);
        std::swap(m0, m1);
    }

    if (m0 < 0)
    {
        m0 += b / s;
    }

    return {s, m0};
}
template<std::unsigned_integral U, U P> struct ModIntBase
{
    public:
        constexpr ModIntBase() : x(0)
        {
        }
        template<std::unsigned_integral T> constexpr ModIntBase(T x_) : x(x_ % mod())
        {
        }
        template<std::signed_integral T> constexpr ModIntBase(T x_)
        {
            using S = std::make_signed_t<U>;
            S v = x_ % S(mod());
            if (v < 0)
            {
                v += mod();
            }
            x = v;
        }
        constexpr static U mod()
        {
            return P;
        }
        constexpr U val() const
        {
            return x;
        }
        constexpr ModIntBase operator-() const
        {
            ModIntBase res;
            res.x = (x == 0 ? 0 : mod() - x);
            return res;
        }
        constexpr ModIntBase inv() const
        {
            auto v = invGcd(x, mod());
            assert(v.first == 1);
            return v.second;
        }
        constexpr ModIntBase &operator*=(const ModIntBase &rhs) &
        {
            x = mulMod<mod()>(x, rhs.val());
            return *this;
        }
        constexpr ModIntBase &operator+=(const ModIntBase &rhs) &
        {
            x += rhs.val();
            if (x >= mod())
            {
                x -= mod();
            }
            return *this;
        }
        constexpr ModIntBase &operator-=(const ModIntBase &rhs) &
        {
            x -= rhs.val();
            if (x >= mod())
            {
                x += mod();
            }
            return *this;
        }
        constexpr ModIntBase &operator/=(const ModIntBase &rhs) &
        {
            return *this *= rhs.inv();
        }
        friend constexpr ModIntBase operator*(ModIntBase lhs, const ModIntBase &rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        friend constexpr ModIntBase operator+(ModIntBase lhs, const ModIntBase &rhs)
        {
            lhs += rhs;
            return lhs;
        }
        friend constexpr ModIntBase operator-(ModIntBase lhs, const ModIntBase &rhs)
        {
            lhs -= rhs;
            return lhs;
        }
        friend constexpr ModIntBase operator/(ModIntBase lhs, const ModIntBase &rhs)
        {
            lhs /= rhs;
            return lhs;
        }
        friend constexpr std::istream &operator>>(std::istream &is, ModIntBase &a)
        {
            i64 i;
            is >> i;
            a = i;
            return is;
        }
        friend constexpr std::ostream &operator<<(std::ostream &os, const ModIntBase &a)
        {
            return os << a.val();
        }
        friend constexpr bool operator==(const ModIntBase &lhs, const ModIntBase &rhs)
        {
            return lhs.val() == rhs.val();
        }
        friend constexpr std::strong_ordering operator<=>(const ModIntBase &lhs, const ModIntBase &rhs)
        {
            return lhs.val() <=> rhs.val();
        }
    private:
        U x;
};
template<u32 P> using ModInt = ModIntBase<u32, P>;
template<u64 P> using ModInt64 = ModIntBase<u64, P>;
struct Barrett
{
    public:
        Barrett(u32 m_) : m(m_), im((u64) (-1) / m_ + 1)
        {
        }
        constexpr u32 mod() const
        {
            return m;
        }
        constexpr u32 mul(u32 a, u32 b) const
        {
            u64 z = a;
            z *= b;

            u64 x = u64((u128(z) * im) >> 64);

            u32 v = u32(z - x * m);
            if (m <= v)
            {
                v += m;
            }
            return v;
        }
    private:
        u32 m;
        u64 im;
};
template<u32 Id> struct DynModInt
{
    public:
        constexpr DynModInt() : x(0)
        {
        }
        template<std::unsigned_integral T> constexpr DynModInt(T x_) : x(x_ % mod())
        {
        }
        template<std::signed_integral T> constexpr DynModInt(T x_)
        {
            int v = x_ % int(mod());
            if (v < 0)
            {
                v += mod();
            }
            x = v;
        }
        constexpr static void setMod(u32 m)
        {
            bt = m;
        }
        static u32 mod()
        {
            return bt.mod();
        }
        constexpr u32 val() const
        {
            return x;
        }
        constexpr DynModInt operator-() const
        {
            DynModInt res;
            res.x = (x == 0 ? 0 : mod() - x);
            return res;
        }
        constexpr DynModInt inv() const
        {
            auto v = invGcd(x, mod());
            assert(v.first == 1);
            return v.second;
        }
        constexpr DynModInt &operator*=(const DynModInt &rhs) &
        {
            x = bt.mul(x, rhs.val());
            return *this;
        }
        constexpr DynModInt &operator+=(const DynModInt &rhs) &
        {
            x += rhs.val();
            if (x >= mod())
            {
                x -= mod();
            }
            return *this;
        }
        constexpr DynModInt &operator-=(const DynModInt &rhs) &
        {
            x -= rhs.val();
            if (x >= mod())
            {
                x += mod();
            }
            return *this;
        }
        constexpr DynModInt &operator/=(const DynModInt &rhs) &
        {
            return *this *= rhs.inv();
        }
        friend constexpr DynModInt operator*(DynModInt lhs, const DynModInt &rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        friend constexpr DynModInt operator+(DynModInt lhs, const DynModInt &rhs)
        {
            lhs += rhs;
            return lhs;
        }
        friend constexpr DynModInt operator-(DynModInt lhs, const DynModInt &rhs)
        {
            lhs -= rhs;
            return lhs;
        }
        friend constexpr DynModInt operator/(DynModInt lhs, const DynModInt &rhs)
        {
            lhs /= rhs;
            return lhs;
        }
        friend constexpr std::istream &operator>>(std::istream &is, DynModInt &a)
        {
            i64 i;
            is >> i;
            a = i;
            return is;
        }
        friend constexpr std::ostream &operator<<(std::ostream &os, const DynModInt &a)
        {
            return os << a.val();
        }
        friend constexpr bool operator==(const DynModInt &lhs, const DynModInt &rhs)
        {
            return lhs.val() == rhs.val();
        }
        friend constexpr std::strong_ordering operator<=>(const DynModInt &lhs, const DynModInt &rhs)
        {
            return lhs.val() <=> rhs.val();
        }
    private:
        u32 x;
        static Barrett bt;
};
template<u32 Id> Barrett DynModInt<Id>::bt = 998244353;
using Z = ModInt<998244353>;
// using Z = ModInt<1000000007>;
```

```c++
using u32 = unsigned; using i64 = long long; using u64 = unsigned long long;

template <std::uint32_t P> struct MontgomeryModInt32 {
public:
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;

private:
    u32 v;

    static constexpr u32 get_r() {
        u32 iv = P;

        for (u32 i = 0; i != 4; ++i)
            iv *= 2U - P * iv;

        return -iv;
    }

    static constexpr u32 r = get_r(), r2 = -u64(P) % P;

    static_assert((P & 1) == 1);
    static_assert(-r * P == 1);
    static_assert(P < (1 << 30));

public:
    static constexpr u32 pow_mod(u32 x, u64 y) {
        if ((y %= P - 1) < 0)
            y += P - 1;

        u32 res = 1;

        for (; y != 0; y >>= 1, x = u64(x) * x % P)
            if (y & 1)
                res = u64(res) * x % P;

        return res;
    }

    static constexpr u32 get_pr() {
        u32 tmp[32] = {}, cnt = 0;
        const u64 phi = P - 1;
        u64 m = phi;

        for (u64 i = 2; i * i <= m; ++i) {
            if (m % i == 0) {
                tmp[cnt++] = i;

                while (m % i == 0)
                    m /= i;
            }
        }

        if (m > 1)
            tmp[cnt++] = m;

        for (u64 res = 2; res <= phi; ++res) {
            bool flag = true;

            for (u32 i = 0; i != cnt && flag; ++i)
                flag &= pow_mod(res, phi / tmp[i]) != 1;

            if (flag)
                return res;
        }

        return 0;
    }

    MontgomeryModInt32() = default;
    ~MontgomeryModInt32() = default;
    constexpr MontgomeryModInt32(u32 v) : v(reduce(u64(v) * r2)) {}
    constexpr MontgomeryModInt32(const MontgomeryModInt32 &rhs) : v(rhs.v) {}
    static constexpr u32 reduce(u64 x) {
        return x + (u64(u32(x) * r) * P) >> 32;
    }
    static constexpr u32 norm(u32 x) {
        return x - (P & -(x >= P));
    }
    constexpr u32 get() const {
        u32 res = reduce(v) - P;
        return res + (P & -(res >> 31));
    }
    explicit constexpr operator u32() const {
        return get();
    }
    explicit constexpr operator i32() const {
        return i32(get());
    }
    constexpr MontgomeryModInt32 &operator=(const MontgomeryModInt32 &rhs) {
        return v = rhs.v, *this;
    }
    constexpr MontgomeryModInt32 operator-() const {
        MontgomeryModInt32 res;
        return res.v = (P << 1 & -(v != 0)) - v, res;
    }
    constexpr MontgomeryModInt32 inv() const {
        return pow(-1);
    }
    constexpr MontgomeryModInt32 &operator+=(const MontgomeryModInt32 &rhs) {
        return v += rhs.v - (P << 1), v += P << 1 & -(v >> 31), *this;
    }
    constexpr MontgomeryModInt32 &operator-=(const MontgomeryModInt32 &rhs) {
        return v -= rhs.v, v += P << 1 & -(v >> 31), *this;
    }
    constexpr MontgomeryModInt32 &operator*=(const MontgomeryModInt32 &rhs) {
        return v = reduce(u64(v) * rhs.v), *this;
    }
    constexpr MontgomeryModInt32 &operator/=(const MontgomeryModInt32 &rhs) {
        return this->operator*=(rhs.inv());
    }
    friend MontgomeryModInt32 operator+(const MontgomeryModInt32 &lhs,
                                        const MontgomeryModInt32 &rhs) {
        return MontgomeryModInt32(lhs) += rhs;
    }
    friend MontgomeryModInt32 operator-(const MontgomeryModInt32 &lhs,
                                        const MontgomeryModInt32 &rhs) {
        return MontgomeryModInt32(lhs) -= rhs;
    }
    friend MontgomeryModInt32 operator*(const MontgomeryModInt32 &lhs,
                                        const MontgomeryModInt32 &rhs) {
        return MontgomeryModInt32(lhs) *= rhs;
    }
    friend MontgomeryModInt32 operator/(const MontgomeryModInt32 &lhs,
                                        const MontgomeryModInt32 &rhs) {
        return MontgomeryModInt32(lhs) /= rhs;
    }
    friend bool operator==(const MontgomeryModInt32 &lhs, const MontgomeryModInt32 &rhs) {
        return norm(lhs.v) == norm(rhs.v);
    }
    friend bool operator!=(const MontgomeryModInt32 &lhs, const MontgomeryModInt32 &rhs) {
        return norm(lhs.v) != norm(rhs.v);
    }
    friend std::istream &operator>>(std::istream &is, MontgomeryModInt32 &rhs) {
        return is >> rhs.v, rhs.v = reduce(u64(rhs.v) * r2), is;
    }
    friend std::ostream &operator<<(std::ostream &os, const MontgomeryModInt32 &rhs) {
        return os << rhs.get();
    }
    constexpr MontgomeryModInt32 pow(i64 y) const {
        if ((y %= P - 1) < 0)
            y += P - 1; // phi(P) = P - 1, assume P is a prime number

        MontgomeryModInt32 res(1), x(*this);

        for (; y != 0; y >>= 1, x *= x)
            if (y & 1)
                res *= x;

        return res;
    }
};

template <std::uint32_t P> MontgomeryModInt32<P> sqrt(const MontgomeryModInt32<P> &x) {
    using value_type = MontgomeryModInt32<P>;
    static constexpr value_type negtive_one(P - 1), ZERO(0);

    if (x == ZERO || x.pow(P - 1 >> 1) == negtive_one)
        return ZERO;

    if ((P & 3) == 3)
        return x.pow(P + 1 >> 2);

    static value_type w2, ax;
    ax = x;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<std::uint32_t> dis(1, P - 1);
    const value_type four(value_type(4) * x);
    static value_type t;

    do
        t = value_type(dis(gen)), w2 = t * t - four;

    while (w2.pow(P - 1 >> 1) != negtive_one);

    struct Field_P2 { // (A + Bx)(C+Dx)=(AC-BDa)+(AD+BC+BDt)x
    public:
        value_type a, b;
        Field_P2(const value_type &a, const value_type &b) : a(a), b(b) {}
        ~Field_P2() = default;
        Field_P2 &operator*=(const Field_P2 &rhs) {
            value_type tmp1(b * rhs.b), tmp2(a * rhs.a - tmp1 * ax),
                       tmp3(a * rhs.b + b * rhs.a + tmp1 * t);
            return a = tmp2, b = tmp3, *this;
        }
        Field_P2 pow(std::uint64_t y) const {
            Field_P2 res(value_type(1), ZERO), x(*this);

            for (; y != 0; y >>= 1, x *= x)
                if (y & 1)
                    res *= x;

            return res;
        }
    } res(ZERO, value_type(1));
    return res.pow(P + 1 >> 1).a;
}

std::uint64_t get_len(std::uint64_t n) { // if n=0, boom
    return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8, n |= n >> 16, n |= n >> 32, ++n;
}

using Z = MontgomeryModInt32<1000000007>;
```

```c++
int exgcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
```

```c++
template<class T> struct MaxFlow
{
    struct _Edge
    {
        int to;
        T cap;
        _Edge(int to, T cap) : to(to), cap(cap)
        {
        }
    };
    int n;
    std::vector<_Edge> e;
    std::vector<std::vector<int> > g;
    std::vector<int> cur, h;
    MaxFlow()
    {
    }
    MaxFlow(int n)
    {
        init(n);
    }
    void init(int n)
    {
        this->n = n;
        e.clear();
        g.assign(n, {});
        cur.resize(n);
        h.resize(n);
    }
    bool bfs(int s, int t)
    {
        h.assign(n, -1);
        std::queue<int> que;
        h[s] = 0;
        que.push(s);
        while (!que.empty())
        {
            const int u = que.front();
            que.pop();
            for (int i: g[u])
            {
                auto [v, c] = e[i];
                if (c > 0 && h[v] == -1)
                {
                    h[v] = h[u] + 1;
                    if (v == t)
                    {
                        return true;
                    }
                    que.push(v);
                }
            }
        }
        return false;
    }
    T dfs(int u, int t, T f)
    {
        if (u == t)
        {
            return f;
        }
        auto r = f;
        for (int &i = cur[u]; i < signed(g[u].size()); ++i)
        {
            const int j = g[u][i];
            auto [v, c] = e[j];
            if (c > 0 && h[v] == h[u] + 1)
            {
                auto a = dfs(v, t, std::min(r, c));
                e[j].cap -= a;
                e[j ^ 1].cap += a;
                r -= a;
                if (r == 0)
                {
                    return f;
                }
            }
        }
        return f - r;
    }
    void addEdge(int u, int v, T c)
    {
        g[u].push_back(e.size());
        e.emplace_back(v, c);
        g[v].push_back(e.size());
        e.emplace_back(u, 0);
    }
    T flow(int s, int t)
    {
        T ans = 0;
        while (bfs(s, t))
        {
            cur.assign(n, 0);
            ans += dfs(s, t, std::numeric_limits<T>::max());
        }
        return ans;
    }
    std::vector<char> minCut()
    {
        std::vector<char> c(n);
        for (int i = 0; i < n; i++)
        {
            c[i] = (h[i] != -1);
        }
        return c;
    }
    struct Edge
    {
        int from;
        int to;
        T cap;
        T flow;
    };
    std::vector<Edge> edges()
    {
        std::vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2)
        {
            Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }
};
```

```c++
int ksm(int a, int b, int p) {
    int res = 1;
    for (; b; b /= 2, a = 1LL * a * a % p) {
        if (b % 2) {
            res = 1LL * res * a % p;
        }
    }
    return res;
}
```

```c++
vector<int> minp, primes;
void sieve(int n)
{
    minp.assign(n + 1, 0);
    primes.clear();
    for (int i = 2; i <= n; i++)
    {
        if (minp[i] == 0)
        {
            minp[i] = i;
            primes.push_back(i);
        }
        for (auto p: primes)
        {
            if (i * p > n) break;
            minp[i * p] = p;
            if (p == minp[i]) break;
        }
    }
}
map<int, vector<pair<int,int> > > rem;
vector<pair<int,int> > get(int x)
{
    if (rem.contains(x)) return rem[x];
    int orig = x;
    vector<pair<int,int> > res;
    while (x > 1)
    {
        int p = minp[x], e = 0;
        while (x % p == 0) x /= p, ++e;
        res.emplace_back(p, e);
    }
    return rem[orig] = res;
}
```

