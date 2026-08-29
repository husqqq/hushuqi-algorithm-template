// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 83 旋转卡壳






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


template <class T> vector<Point<T>> hull(vector<Point<T>> a)
{
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    if (a.size() <= 1)
    {
        return a;
    }
    vector<Point<T>> h;
    for (int t = 0; t < 2; t++)
    {
        int z = (int)h.size();
        for (Point<T> p : a)
        {
            while ((int)h.size() >= z + 2
                   && sgn(cross(h.back() - h[h.size() - 2], p - h.back())) <= 0)
            {
                h.pop_back();
            }
            h.push_back(p);
        }
        h.pop_back();
        reverse(a.begin(), a.end());
    }
    return h;
}


template <class T> T diameter2(const vector<Point<T>> &a)
{
    int n = (int)a.size();
    if (n <= 1)
    {
        return 0;
    }
    if (n == 2)
    {
        return norm(a[0] - a[1]);
    }
    T ans = 0;
    int j = 1;
    for (int i = 0; i < n; i++)
    {
        while (sgn(cross(a[(i + 1) % n] - a[i],
                         a[(j + 1) % n] - a[j])) > 0)
        {
            j = (j + 1) % n;
        }
        ans = max(ans, norm(a[i] - a[j]));
        ans = max(ans, norm(a[(i + 1) % n] - a[j]));
    }
    return ans;
}

template <class T> T weakConvexDiameter2(const vector<Point<T>> &a)
{
    // a 是按边界顺序给出的弱凸多边形；允许相邻边共线，返回最远点对距离平方。
    int n = a.size();
    if (n <= 2)
    {
        return n < 2 ? 0 : norm(a[0] - a[1]);
    }
    vector<Point<T>> h;
    for (int i = 0; i < n; i++)
    {
        if (cross(a[i] - a[(i + n - 1) % n], a[(i + 1) % n] - a[i]) != 0)
        {
            h.push_back(a[i]);
        }
    }
    if (h.empty())
    {
        auto [l, r] = minmax_element(a.begin(), a.end());
        return norm(*l - *r);
    }
    if (h.size() <= 2)
    {
        return h.size() < 2 ? 0 : norm(h[0] - h[1]);
    }
    T ans = 0;
    int m = h.size(), j = 1;
    for (int i = 0; i < m; i++)
    {
        auto area = [&](int k)
        {
            T x = cross(h[(i + 1) % m] - h[i], h[k] - h[i]);
            return x < 0 ? -x : x;
        };
        while (area((j + 1) % m) > area(j))
        {
            j = (j + 1) % m;
        }
        ans = max(ans, norm(h[i] - h[j]));
        ans = max(ans, norm(h[(i + 1) % m] - h[j]));
        int k = (j + 1) % m;
        if (area(k) == area(j))
        {
            ans = max(ans, norm(h[i] - h[k]));
            ans = max(ans, norm(h[(i + 1) % m] - h[k]));
        }
    }
    return ans;
}

template <class T> pair<int, int> farthestPair(const vector<Point<T>> &p)
{
    // p 是至少含两个点的点集；返回一对欧氏距离最大的原下标。
    assert(p.size() >= 2); // 调试检查，可删。
    map<Point<T>, int> id;
    for (int i = 0; i < (int)p.size(); i++)
    {
        id.try_emplace(p[i], i);
    }
    auto h = hull(p);
    if (h.size() == 1)
    {
        return {0, 1};
    }
    if (h.size() == 2)
    {
        return {id[h[0]], id[h[1]]};
    }
    int m = h.size(), j = 1;
    T best = -1;
    pair<int, int> ans{0, 1};
    auto update = [&](int x, int y)
    {
        T d = norm(h[x] - h[y]);
        if (best < d)
        {
            best = d;
            ans = {id[h[x]], id[h[y]]};
        }
    };
    for (int i = 0; i < m; i++)
    {
        while (cross(h[(i + 1) % m] - h[i],
                     h[(j + 1) % m] - h[j]) > 0)
        {
            j = (j + 1) % m;
        }
        update(i, j);
        update((i + 1) % m, j);
    }
    return ans;
}
