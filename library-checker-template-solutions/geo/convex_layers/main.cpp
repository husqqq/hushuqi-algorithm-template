



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

struct UpperHull
{
    struct Node
    {
        int l = -1, r = -1, bl = -1, br = -1;
    };

    int n, sz;
    vector<Point<long long>> p;
    vector<Node> tr;
    vector<int> orig, pos;

    UpperHull(vector<Point<long long>> a) : n(a.size()), p(move(a))
    {
        orig.resize(n);
        iota(orig.begin(), orig.end(), 0);
        sort(orig.begin(), orig.end(), [&](int x, int y) { return p[x] < p[y]; });
        vector<Point<long long>> q(n);
        pos.resize(n);
        for (int i = 0; i < n; i++)
        {
            q[i] = p[orig[i]];
            pos[orig[i]] = i;
        }
        p.swap(q);
        sz = 1;
        while (sz < n)
        {
            sz <<= 1;
        }
        tr.resize(2 * sz);
        for (int i = 0; i < n; i++)
        {
            tr[sz + i] = {i, i + 1, i, i};
        }
        for (int i = sz - 1; i; i--)
        {
            pull(i);
        }
    }

    bool has(int x) const
    {
        return tr[x].r != -1;
    }

    void pull(int x)
    {
        auto cross128 = [](Point<long long> a, Point<long long> b)
        {
            return (__int128)a.x * b.y - (__int128)a.y * b.x;
        };
        if (!has(2 * x) && !has(2 * x + 1))
        {
            tr[x] = {};
            return;
        }
        if (!has(2 * x))
        {
            tr[x] = tr[2 * x + 1];
            return;
        }
        if (!has(2 * x + 1))
        {
            tr[x] = tr[2 * x];
            return;
        }
        int u = 2 * x, v = 2 * x + 1;
        long long split = p[tr[v].l].x;
        while (u < sz || v < sz)
        {
            if (u < sz && !has(2 * u))
            {
                u = 2 * u + 1;
                continue;
            }
            if (u < sz && !has(2 * u + 1))
            {
                u = 2 * u;
                continue;
            }
            if (v < sz && !has(2 * v))
            {
                v = 2 * v + 1;
                continue;
            }
            if (v < sz && !has(2 * v + 1))
            {
                v = 2 * v;
                continue;
            }
            int a = tr[u].bl, b = tr[u].br, c = tr[v].bl, d = tr[v].br;
            if (a != b && cross128(p[b] - p[a], p[c] - p[a]) > 0)
            {
                u = 2 * u;
            }
            else if (c != d && cross128(p[c] - p[b], p[d] - p[b]) > 0)
            {
                v = 2 * v + 1;
            }
            else if (a == b)
            {
                v = 2 * v;
            }
            else if (c == d)
            {
                u = 2 * u + 1;
            }
            else
            {
                __int128 x1 = cross128(p[b] - p[a], p[c] - p[a]);
                __int128 x2 = cross128(p[a] - p[b], p[d] - p[b]);
                if (x1 + x2 == 0 || x1 * p[d].x + x2 * p[c].x < (__int128)split * (x1 + x2))
                {
                    u = 2 * u + 1;
                }
                else
                {
                    v = 2 * v;
                }
            }
        }
        tr[x].l = tr[2 * x].l;
        tr[x].r = tr[2 * x + 1].r;
        tr[x].bl = tr[u].l;
        tr[x].br = tr[v].l;
    }

    void erase(int id)
    {
        int x = sz + pos[id];
        tr[x] = {};
        for (x >>= 1; x; x >>= 1)
        {
            pull(x);
        }
    }

    vector<int> get() const
    {
        vector<int> ans;
        auto dfs = [&](auto &&self, int x, int l, int r) -> void
        {
            if (!has(x) || l >= r)
            {
                return;
            }
            if (x >= sz)
            {
                ans.push_back(tr[x].l);
                return;
            }
            if (!has(2 * x))
            {
                self(self, 2 * x + 1, l, r);
                return;
            }
            if (!has(2 * x + 1))
            {
                self(self, 2 * x, l, r);
                return;
            }
            if (r <= tr[x].bl)
            {
                self(self, 2 * x, l, r);
                return;
            }
            if (tr[x].br <= l)
            {
                self(self, 2 * x + 1, l, r);
                return;
            }
            self(self, 2 * x, l, tr[x].bl + 1);
            self(self, 2 * x + 1, tr[x].br, r);
        };
        if (n)
        {
            dfs(dfs, 1, 0, n);
        }
        for (auto &x : ans)
        {
            x = orig[x];
        }
        return ans;
    }
};

vector<vector<int>> convexLayers(const vector<Point<long long>> &p)
{
    if (p.empty())
    {
        return {};
    }
    vector<Point<long long>> q;
    q.reserve(p.size());
    for (auto [x, y] : p)
    {
        assert(llabs(x) <= 1000000000 && llabs(y) <= 1000000000);
        q.push_back({-x, -y});
    }
    UpperHull up(p), down(q);
    vector<vector<int>> ans;
    int done = 0;
    while (done < (int)p.size())
    {
        auto a = down.get();
        auto b = up.get();
        if (a.size() == p.size() - done && b.size() == a.size())
        {
            // 剩余点全共线时两条壳相同，只保留其中一条。
        }
        else if (a.size() >= 2)
        {
            a.pop_back();
            b.pop_back();
            a.insert(a.end(), b.begin(), b.end());
            reverse(a.begin(), a.end());
        }
        ans.push_back(a);
        for (int x : a)
        {
            up.erase(x);
            down.erase(x);
            done++;
        }
    }
    return ans;
}

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<Point<long long>>p(n);for(auto&x:p)cin>>x.x>>x.y;auto layer=convexLayers(p);vector<int>ans(n);for(int i=0;i<(int)layer.size();i++)for(int x:layer[i])ans[x]=i+1;for(int x:ans)cout<<x<<'\n';}
