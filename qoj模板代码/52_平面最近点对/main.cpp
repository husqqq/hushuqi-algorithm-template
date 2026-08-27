



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

template <class T> struct ClosestPair
{
    struct Q
    {
        Point<T> p;
        int id;
    };

    vector<Q> a;
    vector<Q> b;
    T ans;
    pair<int, int> ids;

    void upd(Q x, Q y)
    {
        T d = norm(x.p - y.p);
        if (d < ans)
        {
            ans = d;
            ids = {x.id, y.id};
        }
    }

    void solve(int l, int r)
    {
        if (r - l <= 3)
        {
            for (int i = l; i < r; i++)
            {
                for (int j = l; j < i; j++)
                {
                    upd(a[i], a[j]);
                }
            }
            sort(a.begin() + l, a.begin() + r, [](Q x, Q y)
                 {
                     return x.p.y < y.p.y;
                 });
            return;
        }
        int m = (l + r) / 2;
        T x = a[m].p.x;
        solve(l, m);
        solve(m, r);
        merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r,
              b.begin() + l, [](Q x, Q y)
              {
                  return x.p.y < y.p.y;
              });
        copy(b.begin() + l, b.begin() + r, a.begin() + l);
        vector<Q> s;
        for (int i = l; i < r; i++)
        {
            if ((a[i].p.x - x) * (a[i].p.x - x) > ans)
            {
                continue;
            }
            for (int j = (int)s.size() - 1; j >= 0; j--)
            {
                if ((a[i].p.y - s[j].p.y) * (a[i].p.y - s[j].p.y) > ans)
                {
                    break;
                }
                upd(a[i], s[j]);
            }
            s.push_back(a[i]);
        }
    }

    optional<pair<T, pair<int, int>>> work(const vector<Point<T>> &p)
    {
        if (p.size() < 2)
        {
            return nullopt;
        }
        a.resize(p.size());
        b.resize(p.size());
        for (int i = 0; i < (int)p.size(); i++)
        {
            a[i] = {p[i], i};
        }
        sort(a.begin(), a.end(), [](Q x, Q y)
             {
                 return x.p.x != y.p.x ? x.p.x < y.p.x : x.p.y < y.p.y;
             });
        for (int i = 1; i < (int)a.size(); i++)
        {
            if (a[i - 1].p == a[i].p)
            {
                return pair<T, pair<int, int>>{0, {a[i - 1].id, a[i].id}};
            }
        }
        ans = norm(a[0].p - a[1].p);
        ids = {a[0].id, a[1].id};
        solve(0, (int)a.size());
        return pair<T, pair<int, int>>{ans, ids};
    }
};

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int t;cin>>t;while(t--){int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto r=ClosestPair<long long>().work(p);cout<<r->second.first<<' '<<r->second.second<<'\n';}}
