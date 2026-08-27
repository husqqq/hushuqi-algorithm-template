// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 85 线段到线段距离









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


template <class T> struct Line
{
    Point<T> a;
    Point<T> b;

    Line(Point<T> a = {}, Point<T> b = {}) : a(a), b(b)
    {
    }
};

struct DirLine
{
    P p;
    P v;

    P at(Real t) const
    {
        return p + v * t;
    }
};

using L = DirLine;

template <class T> DirLine toDir(const Line<T> &l)
{
    auto v = l.b - l.a;
    assert(l.a != l.b); // 调试检查，可删。
    return {(P)l.a, (P)v};
}


template <class T> bool parallel(const Line<T> &a, const Line<T> &b)
{
    return sgn(cross(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> bool perp(const Line<T> &a, const Line<T> &b)
{
    return sgn(dot(a.b - a.a, b.b - b.a)) == 0;
}

template <class T> int side(const Line<T> &l, const Point<T> &p)
{
    return sgn(cross(l.b - l.a, p - l.a));
}

bool parallel(L a, L b)
{
    return sgn(cross(a.v, b.v)) == 0;
}


template <class T> Point<Real> lineMeet(const Line<T> &a, const Line<T> &b)
{
    auto u = (P)a.a;
    auto v = (P)(a.b - a.a);
    auto p = (P)b.a;
    auto q = (P)(b.b - b.a);
    assert(sgn(cross(v, q)) != 0); // 调试检查，可删。
    return u + v * (cross(p - u, q) / cross(v, q));
}

template <class T> Point<Real> projection(const Line<T> &l, const Point<T> &p)
{
    auto a = (P)l.a;
    auto v = (P)(l.b - l.a);
    auto q = (P)p;
    assert(sgn(square(v)) != 0); // 调试检查，可删。
    return a + v * (dot(q - a, v) / square(v));
}

template <class T> Point<Real> reflection(const Line<T> &l, const Point<T> &p)
{
    return projection(l, p) * 2 - (P)p;
}

P isect(L a, L b)
{
    assert(sgn(cross(a.v, b.v)) != 0); // 调试检查，可删。
    return a.at(cross(b.p - a.p, b.v) / cross(a.v, b.v));
}

P proj(L l, P p)
{
    assert(sgn(square(l.v)) != 0); // 调试检查，可删。
    return l.at(dot(p - l.p, l.v) / square(l.v));
}

P refl(L l, P p)
{
    return proj(l, p) * 2 - p;
}

Real dis(L l, P p)
{
    assert(sgn(square(l.v)) != 0); // 调试检查，可删。
    return fabsl(cross(l.v, p - l.p)) / abs(l.v);
}


template <class T> Real distancePL(const Point<T> &p, const Line<T> &l)
{
    assert(l.a != l.b); // 调试检查，可删。
    return fabsl((Real)cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

template <class T> Real distancePS(const Point<T> &p, const Line<T> &l)
{
    if (l.a == l.b)
    {
        return abs(p - l.a);
    }
    if (dot(p - l.a, l.b - l.a) <= 0)
    {
        return abs(p - l.a);
    }
    if (dot(p - l.b, l.a - l.b) <= 0)
    {
        return abs(p - l.b);
    }
    return distancePL(p, l);
}


template <class T> bool onSegment(const Point<T> &p, const Line<T> &l)
{
    return sgn(cross(p - l.a, l.b - l.a)) == 0 && sgn(dot(p - l.a, p - l.b)) <= 0;
}

template <class T> bool segMeet(const Line<T> &a, const Line<T> &b)
{
    int c1 = sgn(cross(a.b - a.a, b.a - a.a));
    int c2 = sgn(cross(a.b - a.a, b.b - a.a));
    int c3 = sgn(cross(b.b - b.a, a.a - b.a));
    int c4 = sgn(cross(b.b - b.a, a.b - b.a));
    if (c1 == 0 && onSegment(b.a, a))
    {
        return true;
    }
    if (c2 == 0 && onSegment(b.b, a))
    {
        return true;
    }
    if (c3 == 0 && onSegment(a.a, b))
    {
        return true;
    }
    if (c4 == 0 && onSegment(a.b, b))
    {
        return true;
    }
    return ((c1 > 0) != (c2 > 0)) && ((c3 > 0) != (c4 > 0));
}

template <class T> Real distanceSS(const Line<T> &a, const Line<T> &b)
{
    if (segMeet(a, b))
    {
        return 0;
    }
    return min({distancePS(a.a, b), distancePS(a.b, b),
                distancePS(b.a, a), distancePS(b.b, a)});
}

template <class T> bool onSeg(Point<T> p, Point<T> a, Point<T> b)
{
    return onSegment(p, Line<T>{a, b});
}

template <class T> bool segIsect(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    return segMeet(Line<T>{a, b}, Line<T>{c, d});
}

template <class T> Real segDis(Point<T> a, Point<T> b, Point<T> c, Point<T> d)
{
    return distanceSS(Line<T>{a, b}, Line<T>{c, d});
}
