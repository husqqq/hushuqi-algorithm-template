#pragma once

#include "11_2_001.hpp"

struct Fraction128
{
    __int128 num; // 分子。
    __int128 den; // 始终为正的分母。

    Fraction128(__int128 num = 0, __int128 den = 1) : num(num), den(den)
    {
        // num、den 是分子和非零分母；构造符号规范化的有理数。
        assert(den != 0); // 调试检查，可删。
        if (den < 0)
        {
            this->num = -num;
            this->den = -den;
        }
    }

    friend bool operator<(Fraction128 a, Fraction128 b)
    {
        // a、b 是有理数；返回 a 是否严格小于 b，交叉乘积必须能由 __int128 表示。
        return a.num * b.den < b.num * a.den;
    }

    friend bool operator==(Fraction128 a, Fraction128 b)
    {
        // a、b 是有理数；返回二者是否严格相等，交叉乘积必须能由 __int128 表示。
        return a.num * b.den == b.num * a.den;
    }
};

__int128 covCross(Point<long long> a, Point<long long> b, Point<long long> c)
{
    // a、b 确定有向直线，c 是查询点；返回使用宽整数计算的叉积。
    __int128 x1 = (__int128)b.x - a.x;
    __int128 y1 = (__int128)b.y - a.y;
    __int128 x2 = (__int128)c.x - a.x;
    __int128 y2 = (__int128)c.y - a.y;
    return x1 * y2 - y1 * x2;
}

int covSide(Point<long long> a, Point<long long> b, Point<long long> p)
{
    // a、b 确定整数有向直线，p 是查询点；返回 p 所在侧的精确符号。
    return sgn(covCross(a, b, p));
}

int covSide(P a, P b, P p)
{
    // a、b 确定浮点有向直线，p 是查询点；按统一误差返回 p 所在侧的符号。
    return sgn(cross(a, b, p));
}

int covDir(const vector<Point<long long>> &p)
{
    // p 是整数多边形顶点；返回其有向双倍面积的精确符号。
    __int128 sum = 0;
    for (int i = 0; i < (int)p.size(); i++)
    {
        int j = (i + 1) % p.size();
        sum += (__int128)p[i].x * p[j].y - (__int128)p[i].y * p[j].x;
    }
    return sgn(sum);
}

int covDir(const vector<P> &p)
{
    // p 是浮点多边形顶点；按统一误差返回其有向双倍面积的符号。
    return sgn(area2(p));
}

Fraction128 covParam(Point<long long> a, Point<long long> b, Point<long long> p)
{
    // a、b 确定整数参数直线，p 是线上点；返回满足 p=a+t*(b-a) 的精确参数 t。
    __int128 x = (__int128)b.x - a.x;
    __int128 y = (__int128)b.y - a.y;
    __int128 u = (__int128)p.x - a.x;
    __int128 v = (__int128)p.y - a.y;
    return {u * x + v * y, x * x + y * y};
}

Real covParam(P a, P b, P p)
{
    // a、b 确定浮点参数直线，p 是线上点；返回满足 p=a+t*(b-a) 的参数 t。
    P v = b - a;
    return dot(p - a, v) / norm(v);
}

Fraction128 covMeet(Point<long long> a, Point<long long> b, Point<long long> c, Point<long long> d)
{
    // a、b 与 c、d 确定两条不平行整数直线；返回前一条直线交点的精确参数 t。
    __int128 ux = (__int128)b.x - a.x;
    __int128 uy = (__int128)b.y - a.y;
    __int128 vx = (__int128)d.x - c.x;
    __int128 vy = (__int128)d.y - c.y;
    __int128 wx = (__int128)c.x - a.x;
    __int128 wy = (__int128)c.y - a.y;
    return {wx * vy - wy * vx, ux * vy - uy * vx};
}

Real covMeet(P a, P b, P c, P d)
{
    // a、b 与 c、d 确定两条不平行浮点直线；返回前一条直线交点的参数 t。
    P u = b - a;
    P v = d - c;
    return cross(c - a, v) / cross(u, v);
}

Real covVal(Fraction128 x)
{
    // x 是精确有理参数；返回其 long double 近似值。
    return (Real)x.num / (Real)x.den;
}

Real covVal(Real x)
{
    // x 是浮点参数；返回其原值。
    return x;
}

template <class T, class Key> vector<Real> covAreaImpl(vector<vector<Point<T>>> poly)
{
    // poly 是无洞简单逆时针多边形集；返回各覆盖层面积，Key 是事件参数类型。
    int n = (int)poly.size();
    map<vector<Point<T>>, int> weight;
    for (auto p : poly)
    {
        if (p.size() > 1 && p.front() == p.back())
        {
            p.pop_back();
        }
        assert(p.size() >= 3 && covDir(p) > 0); // 调试检查，可删。
        for (int i = 0; i < (int)p.size(); i++)
        {
            assert(!(p[i] == p[(i + 1) % p.size()])); // 调试检查，可删。
        }
        int pos = min_element(p.begin(), p.end()) - p.begin();
        rotate(p.begin(), p.begin() + pos, p.end());
        weight[p]++;
    }
    vector<vector<Point<T>>> q;
    vector<int> w;
    for (auto &[p, cnt] : weight)
    {
        q.push_back(p);
        w.push_back(cnt);
    }
    for (int i = 0; i < (int)q.size(); i++)
    {
        for (int j = i + 1; j < (int)q.size(); j++)
        {
            for (int x = 0; x < (int)q[i].size(); x++)
            {
                Point<T> a = q[i][x];
                Point<T> b = q[i][(x + 1) % q[i].size()];
                for (int y = 0; y < (int)q[j].size(); y++)
                {
                    Point<T> c = q[j][y];
                    Point<T> d = q[j][(y + 1) % q[j].size()];
                    if (covSide(a, b, c) != 0 || covSide(a, b, d) != 0)
                    {
                        continue;
                    }
                    T al, ar, bl, br;
                    if (a.x != b.x)
                    {
                        al = min(a.x, b.x);
                        ar = max(a.x, b.x);
                        bl = min(c.x, d.x);
                        br = max(c.x, d.x);
                    }
                    else
                    {
                        al = min(a.y, b.y);
                        ar = max(a.y, b.y);
                        bl = min(c.y, d.y);
                        br = max(c.y, d.y);
                    }
                    assert(!(max(al, bl) < min(ar, br))); // 不支持不同代表多边形正长度共边。
                }
            }
        }
    }
    vector<Real> diff(n + 1);
    for (int i = 0; i < (int)q.size(); i++)
    {
        for (int x = 0; x < (int)q[i].size(); x++)
        {
            Point<T> a = q[i][x];
            Point<T> b = q[i][(x + 1) % q[i].size()];
            Key zero = covParam(a, b, a);
            Key one = covParam(a, b, b);
            vector<pair<Key, int>> e{{zero, 0}, {one, 0}};
            for (int j = 0; j < (int)q.size(); j++)
            {
                if (i == j)
                {
                    continue;
                }
                for (int y = 0; y < (int)q[j].size(); y++)
                {
                    Point<T> c = q[j][y];
                    Point<T> d = q[j][(y + 1) % q[j].size()];
                    int sc = covSide(a, b, c);
                    int sd = covSide(a, b, d);
                    if (sc == 0 && sd == 0)
                    {
                        e.push_back({covParam(a, b, c), 0});
                        e.push_back({covParam(a, b, d), 0});
                        continue;
                    }
                    int delta = 0;
                    if (sc < 0 && sd >= 0)
                    {
                        delta = -w[j];
                    }
                    if (sc >= 0 && sd < 0)
                    {
                        delta = w[j];
                    }
                    if (!delta)
                    {
                        continue;
                    }
                    Key t;
                    if (sc == 0)
                    {
                        t = covParam(a, b, c);
                    }
                    else if (sd == 0)
                    {
                        t = covParam(a, b, d);
                    }
                    else
                    {
                        t = covMeet(a, b, c, d);
                    }
                    e.push_back({t, delta});
                }
            }
            sort(e.begin(),
                 e.end(),
                 [](const auto &p, const auto &r)
                 {
                     if (p.first < r.first)
                     {
                         return true;
                     }
                     if (r.first < p.first)
                     {
                         return false;
                     }
                     return p.second < r.second;
                 });
            int cnt = 0;
            Key last = e.front().first;
            for (int j = 0; j < (int)e.size();)
            {
                Key t = e[j].first;
                Key l = last < zero ? zero : last;
                Key r = one < t ? one : t;
                if (l < r)
                {
                    assert(0 <= cnt && cnt + w[i] <= n); // 调试检查，可删。
                    P pa = (P)a;
                    P u = (P)(b - a);
                    P s = pa + u * covVal(l);
                    P z = pa + u * covVal(r);
                    Real val = cross(s, z) / 2;
                    diff[cnt] += val;
                    diff[cnt + w[i]] -= val;
                }
                int k = j;
                int delta = 0;
                while (k < (int)e.size() && e[k].first == t)
                {
                    delta += e[k].second;
                    k++;
                }
                cnt += delta;
                last = t;
                j = k;
            }
        }
    }
    vector<Real> ans(n);
    Real sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += diff[i];
        ans[i] = sgn(sum) == 0 ? 0 : sum;
    }
    return ans;
}

vector<Real> covArea(vector<vector<Point<long long>>> poly)
{
    // poly 是整数简单逆时针多边形集；事件拓扑精确、面积近似，坐标绝对值须不超过 1E9。
    // 完全重复多边形须仅差循环移位；其余不同多边形不能有正长度共边。
    return covAreaImpl<long long, Fraction128>(move(poly));
}

vector<Real> covArea(vector<vector<P>> poly)
{
    // poly 是浮点简单逆时针多边形集；返回近似覆盖面积，重复须仅差循环移位且不同代表不能正长度共边。
    return covAreaImpl<Real, Real>(move(poly));
}
