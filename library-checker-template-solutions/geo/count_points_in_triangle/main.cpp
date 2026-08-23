



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

struct TrianglePointCounter
{
    vector<Point<long long>> a;
    vector<int32_t> pointL, pointM;
    vector<vector<int32_t>> edgeL, edgeM;

    TrianglePointCounter(const vector<Point<long long>> &vertices,
                         const vector<Point<long long>> &points) : a(vertices)
    {
        int n = a.size();
        pointL.assign(n, 0);
        pointM.assign(n, 0);
        edgeL.assign(n, vector<int32_t>(n));
        edgeM.assign(n, vector<int32_t>(n));
        for (int u = 0; u < n; u++)
        {
            for (auto p : points)
            {
                if (p.y == a[u].y)
                {
                    pointL[u] += p.x < a[u].x;
                    pointM[u] += p.x == a[u].x;
                }
            }
        }
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                if (a[u].y >= a[v].y)
                {
                    continue;
                }
                for (auto p : points)
                {
                    if (a[u].y < p.y && p.y < a[v].y)
                    {
                        long long det = cross(a[u] - a[v], p - a[v]);
                        edgeL[u][v] += det < 0;
                        edgeM[u][v] += det == 0;
                    }
                }
            }
        }
    }

    bool lessYX(int u, int v) const
    {
        return pair(a[u].y, a[u].x) < pair(a[v].y, a[v].x);
    }

    int count(int x, int y, int z) const
    {
        int n = a.size();
        assert(0 <= x && x < n && 0 <= y && y < n && 0 <= z && z < n);
        if (lessYX(y, x))
        {
            swap(x, y);
        }
        if (lessYX(z, y))
        {
            swap(y, z);
        }
        if (lessYX(y, x))
        {
            swap(x, y);
        }
        long long det = cross(a[x] - a[z], a[y] - a[z]);
        if (det == 0)
        {
            return 0;
        }
        int ans = 0;
        if (a[x].y == a[y].y)
        {
            ans = edgeL[y][z] - edgeL[x][z] - edgeM[x][z];
        }
        else if (a[y].y == a[z].y)
        {
            ans = edgeL[x][z] - edgeL[x][y] - edgeM[x][y];
        }
        else if (det < 0)
        {
            ans = edgeL[x][z] - edgeL[y][z] - edgeM[y][z] -
                  edgeL[x][y] - edgeM[x][y] - pointL[y] - pointM[y];
        }
        else
        {
            ans = edgeL[x][y] + edgeL[y][z] + pointL[y] -
                  edgeL[x][z] - edgeM[x][z];
        }
        return ans;
    }
};

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<Point<long long>>a(n);for(auto&x:a)cin>>x.x>>x.y;int m;cin>>m;vector<Point<long long>>b(m);for(auto&x:b)cin>>x.x>>x.y;TrianglePointCounter c(a,b);int q;cin>>q;while(q--){int x,y,z;cin>>x>>y>>z;cout<<c.count(x,y,z)<<'\n';}}
