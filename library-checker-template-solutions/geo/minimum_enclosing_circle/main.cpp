





#include <bits/stdc++.h>
using namespace std;
#define int long long

using Real = long double;
constexpr Real eps = 1E-12L;

template <class T> int sgn(T x)
{
    return (x > 0) - (x < 0);
}

int sgn(Real x)
{
    // x 是待判断的浮点数；返回 -1、0、1 表示负、近似零、正。
    Real t = eps * max<Real>(1, fabsl(x));
    return (x > t) - (x < -t);
}

template <class T> bool eq(T x, T y)
{
    return x == y;
}

bool eq(Real x, Real y)
{
    // x、y 是两个浮点数；使用相对误差判断是否近似相等。
    return fabsl(x - y) <= eps * max<Real>(1, max(fabsl(x), fabsl(y)));
}

template <class T> struct Point
{
    T x;
    T y;

    Point(const T &x = 0, const T &y = 0) : x(x), y(y)
    {
    }

    template <class U> explicit operator Point<U>() const
    {
        return Point<U>((U)x, (U)y);
    }

    Point &operator+=(const Point &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point &operator-=(const Point &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point &operator*=(const T &k)
    {
        x *= k;
        y *= k;
        return *this;
    }

    Point &operator/=(const T &k)
    {
        assert(k != T(0)); // 调试检查，可删。
        x /= k;
        y /= k;
        return *this;
    }

    Point operator-() const
    {
        return {-x, -y};
    }

    friend Point operator+(Point a, const Point &b)
    {
        return a += b;
    }

    friend Point operator-(Point a, const Point &b)
    {
        return a -= b;
    }

    friend Point operator*(Point a, const T &k)
    {
        return a *= k;
    }

    friend Point operator*(const T &k, Point a)
    {
        return a *= k;
    }

    friend Point operator/(Point a, const T &k)
    {
        return a /= k;
    }

    friend bool operator<(const Point &a, const Point &b)
    {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    }

    friend bool operator==(const Point &a, const Point &b)
    {
        return a.x == b.x && a.y == b.y;
    }
};

using P = Point<Real>;

template <class T> bool eq(Point<T> a, Point<T> b)
{
    // a、b 是两个点；整数精确比较，浮点按统一误差比较。
    return eq(a.x, b.x) && eq(a.y, b.y);
}

template <class T> T dot(const Point<T> &a, const Point<T> &b)
{
    return a.x * b.x + a.y * b.y;
}

template <class T> T cross(const Point<T> &a, const Point<T> &b)
{
    return a.x * b.y - a.y * b.x;
}

template <class T> T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c)
{
    return cross(b - a, c - a);
}

template <class T> T square(const Point<T> &a)
{
    return dot(a, a);
}

template <class T> Real abs(const Point<T> &a)
{
    return sqrtl((Real)square(a));
}

template <class T> T norm(const Point<T> &a)
{
    return square(a);
}

template <class T> Real angle(const Point<T> &a, const Point<T> &b)
{
    assert(a.x != T{} || a.y != T{}); // 调试检查，可删。
    assert(b.x != T{} || b.y != T{}); // 调试检查，可删。
    return atan2l(cross(a, b), dot(a, b));
}

template <class T> Point<Real> rotate(const Point<T> &a, Real t)
{
    return {a.x * cosl(t) - a.y * sinl(t), a.x * sinl(t) + a.y * cosl(t)};
}

template <class T> Point<T> rot(const Point<T> &a)
{
    return {-a.y, a.x};
}

struct C
{
    P o;
    Real r;
};

bool in(C c, P p)
{
    assert(sgn(c.r) >= 0); // 调试检查，可删。
    return sgn(abs(p - c.o) - c.r) <= 0;
}

int circleRel(C a, C b)
{
    assert(sgn(a.r) >= 0 && sgn(b.r) >= 0); // 调试检查，可删。
    Real d = abs(a.o - b.o);
    if (sgn(d) == 0 && sgn(a.r - b.r) == 0)
    {
        return 5;
    }
    int x = sgn(d - a.r - b.r);
    int y = sgn(d - fabsl(a.r - b.r));
    if (x > 0)
    {
        return 0;
    }
    if (x == 0)
    {
        return 1;
    }
    if (y > 0)
    {
        return 2;
    }
    if (y == 0)
    {
        return 3;
    }
    return 4;
}

C circle(P a, P b)
{
    // a、b 是直径端点；返回对应的圆。
    P o = (a + b) / 2;
    return {o, abs(a - o)};
}

C circle(P a, P b, P c)
{
    // a、b、c 是三个点；共线时返回覆盖三点的最小圆。
    P u = b - a;
    P v = c - a;
    Real d = 2 * cross(u, v);
    if (sgn(d) == 0)
    {
        if (norm(a - b) >= norm(a - c) && norm(a - b) >= norm(b - c))
        {
            return circle(a, b);
        }
        if (norm(a - c) >= norm(b - c))
        {
            return circle(a, c);
        }
        return circle(b, c);
    }
    P o = a + P{norm(u) * v.y - norm(v) * u.y, u.x * norm(v) - v.x * norm(u)} / d;
    return {o, abs(a - o)};
}

optional<C> incircle(P a, P b, P c)
{
    // a、b、c 是三角形顶点；返回内切圆，三点共线或有重复点时返回空。
    Real s = fabsl(cross(b - a, c - a));
    if (sgn(s) == 0)
    {
        return nullopt;
    }
    Real x = abs(b - c);
    Real y = abs(c - a);
    Real z = abs(a - b);
    Real sum = x + y + z;
    P o = (a * x + b * y + c * z) / sum;
    return C{o, s / sum};
}

C minCircle(vector<P> a, uint32_t seed = 0)
{
    mt19937 rng(seed);
    shuffle(a.begin(), a.end(), rng);
    C c{{0, 0}, 0};
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (!in(c, a[i]))
        {
            c = {a[i], 0};
            for (int j = 0; j < i; j++)
            {
                if (!in(c, a[j]))
                {
                    c = circle(a[i], a[j]);
                    for (int k = 0; k < j; k++)
                    {
                        if (!in(c, a[k]))
                        {
                            c = circle(a[i], a[j], a[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}

string minimumEnclosingCircleBoundary(const vector<Point<long long>> &p,
                                      uint64_t seed = 0x9E3779B97F4A7C15ULL)
{
    // p 是整数点集，seed 是确定性洗牌种子；返回各点是否位于最小包围圆圆周的 0/1 串。
    struct Support
    {
        int size = 0; // 当前支撑点个数。
        array<Point<long long>, 3> p; // p[0..size) 是定义当前圆的支撑点。
    };
    auto cross128 = [](Point<long long> a, Point<long long> b)
    {
        return (__int128)a.x * b.y - (__int128)a.y * b.x;
    };
    auto norm128 = [](Point<long long> a)
    {
        return (__int128)a.x * a.x + (__int128)a.y * a.y;
    };
    auto diameter = [&](Point<long long> a, Point<long long> b, Point<long long> x)
    {
        Point<long long> u{2 * x.x - a.x - b.x, 2 * x.y - a.y - b.y};
        __int128 lhs = norm128(u), rhs = norm128(a - b);
        return lhs < rhs ? 1 : lhs == rhs ? 0 : -1;
    };
    auto contain = [&](const Support &s, Point<long long> x)
    {
        if (s.size == 0) return -1;
        if (s.size == 1) return x == s.p[0] ? 0 : -1;
        if (s.size == 2) return diameter(s.p[0], s.p[1], x);
        auto a = s.p[0] - x, b = s.p[1] - x, c = s.p[2] - x;
        __int128 det = norm128(a) * cross128(b, c) +
                       norm128(b) * cross128(c, a) +
                       norm128(c) * cross128(a, b);
        __int128 ori = cross128(s.p[1] - s.p[0], s.p[2] - s.p[0]);
        det = ori > 0 ? det : -det;
        return det > 0 ? 1 : det == 0 ? 0 : -1;
    };
    auto reduce = [&](Support &s)
    {
        if (s.size != 3 || cross128(s.p[1] - s.p[0], s.p[2] - s.p[0]) != 0)
        {
            return;
        }
        __int128 d01 = norm128(s.p[0] - s.p[1]);
        __int128 d12 = norm128(s.p[1] - s.p[2]);
        __int128 d02 = norm128(s.p[0] - s.p[2]);
        if (d12 >= d01 && d12 >= d02)
        {
            s.p[0] = s.p[1];
            s.p[1] = s.p[2];
        }
        else if (d02 >= d01)
        {
            s.p[1] = s.p[2];
        }
        s.size = 2;
    };
    vector<Point<long long>> q = p;
    mt19937_64 rng(seed);
    shuffle(q.begin(), q.end(), rng);
    Support s;
    for (int i = 0; i < (int)q.size(); i++)
    {
        if (contain(s, q[i]) >= 0) continue;
        s = {1, {q[i], {}, {}}};
        for (int j = 0; j < i; j++)
        {
            if (contain(s, q[j]) >= 0) continue;
            s = {2, {q[i], q[j], {}}};
            for (int k = 0; k < j; k++)
            {
                if (contain(s, q[k]) >= 0) continue;
                s = {3, {q[i], q[j], q[k]}};
                reduce(s);
            }
        }
    }
    string ans(p.size(), '0');
    for (int i = 0; i < (int)p.size(); i++)
    {
        ans[i] = contain(s, p[i]) == 0 ? '1' : '0';
    }
    return ans;
}

signed main(){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;cout<<minimumEnclosingCircleBoundary(p)<<'\n';}
