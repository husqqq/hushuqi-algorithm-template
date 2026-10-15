#pragma once

#include "11_4_001.hpp"

Real circleUnion(const vector<C> &a)
{
    // a 是非负半径圆集；返回至少被一个圆覆盖的面积。
    const Real pi = acosl(-1);
    Real ans = 0;
    for (int i = 0; i < (int)a.size(); i++)
    {
        assert(sgn(a[i].r) >= 0); // 调试检查，可删。
        vector<pair<Real, int>> e;
        bool cover = false;
        for (int j = 0; j < (int)a.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            Real d = abs(a[j].o - a[i].o);
            if (sgn(d + a[i].r - a[j].r) <= 0 && (sgn(a[i].r - a[j].r) < 0 || i > j))
            {
                cover = true;
                break;
            }
            if (sgn(d - a[i].r - a[j].r) >= 0 || sgn(d - fabsl(a[i].r - a[j].r)) <= 0)
            {
                continue;
            }
            Real x = atan2l(a[j].o.y - a[i].o.y, a[j].o.x - a[i].o.x);
            Real y = acosl(clamp((d * d + a[i].r * a[i].r - a[j].r * a[j].r) / (2 * d * a[i].r), Real(-1), Real(1)));
            Real l = x - y;
            Real r = x + y;
            while (l < 0)
            {
                l += 2 * pi;
                r += 2 * pi;
            }
            while (l >= 2 * pi)
            {
                l -= 2 * pi;
                r -= 2 * pi;
            }
            e.push_back({l, 1});
            e.push_back({min(r, 2 * pi), -1});
            if (r > 2 * pi)
            {
                e.push_back({0, 1});
                e.push_back({r - 2 * pi, -1});
            }
        }
        if (cover)
        {
            continue;
        }
        e.push_back({0, 0});
        e.push_back({2 * pi, 0});
        sort(e.begin(),
             e.end(),
             [](auto x, auto y)
             {
                 return x.first != y.first ? x.first < y.first : x.second > y.second;
             });
        int cnt = 0;
        Real last = 0;
        for (auto [x, v] : e)
        {
            if (!cnt)
            {
                P p = a[i].o + P{cosl(last), sinl(last)} * a[i].r;
                P q = a[i].o + P{cosl(x), sinl(x)} * a[i].r;
                Real t = x - last;
                ans += cross(p, q) / 2 + a[i].r * a[i].r * (t - sinl(t)) / 2;
            }
            cnt += v;
            last = x;
        }
    }
    return ans;
}

vector<Real> circleCov(const vector<C> &a)
{
    // a 是非负半径圆集；返回各覆盖层面积，ans[k-1] 表示至少被 k 个圆覆盖的面积。
    int n = (int)a.size();
    vector<Real> ans(n);
    const Real pi = acosl(-1);
    P shift = n ? a[0].o : P{};
    for (int i = 0; i < n; i++)
    {
        assert(sgn(a[i].r) >= 0); // 调试检查，可删。
        if (sgn(a[i].r) == 0)
        {
            continue;
        }
        vector<pair<Real, int>> e;
        int cnt = 0;
        for (int j = 0; j < n; j++)
        {
            assert(sgn(a[j].r) >= 0); // 调试检查，可删。
            if (i == j || sgn(a[j].r) == 0)
            {
                continue;
            }
            Real d = abs(a[j].o - a[i].o);
            if (eq(a[i].o, a[j].o) && eq(a[i].r, a[j].r))
            {
                cnt += j < i;
                continue;
            }
            if (sgn(d + a[i].r - a[j].r) <= 0)
            {
                cnt++;
                continue;
            }
            if (sgn(d - a[i].r - a[j].r) >= 0 || sgn(d - fabsl(a[i].r - a[j].r)) <= 0)
            {
                continue;
            }
            Real mid = atan2l(a[j].o.y - a[i].o.y, a[j].o.x - a[i].o.x);
            if (mid < 0)
            {
                mid += 2 * pi;
            }
            Real ang = acosl(clamp((d * d + a[i].r * a[i].r - a[j].r * a[j].r) / (2 * d * a[i].r), Real(-1), Real(1)));
            Real l = mid - ang;
            Real r = mid + ang;
            while (l < 0)
            {
                l += 2 * pi;
            }
            while (r >= 2 * pi)
            {
                r -= 2 * pi;
            }
            if (l < r)
            {
                e.push_back({l, 1});
                e.push_back({r, -1});
            }
            else
            {
                cnt++;
                e.push_back({r, -1});
                e.push_back({l, 1});
            }
        }
        sort(e.begin(),
             e.end(),
             [](auto x, auto y)
             {
                 return x.first != y.first ? x.first < y.first : x.second < y.second;
             });
        auto add = [&](Real l, Real r)
        {
            // l、r 是当前圆弧的起止角；把该弧的格林积分加入当前覆盖层。
            if (l == r)
            {
                return;
            }
            assert(0 <= cnt && cnt < n); // 调试检查，可删。
            P o = a[i].o - shift;
            Real sum =
                a[i].r * a[i].r * (r - l) + o.x * a[i].r * (sinl(r) - sinl(l)) - o.y * a[i].r * (cosl(r) - cosl(l));
            ans[cnt] += sum / 2;
        };
        Real last = 0;
        for (int j = 0; j < (int)e.size();)
        {
            int k = j;
            int delta = 0;
            Real x = e[j].first;
            add(last, x);
            while (k < (int)e.size() && e[k].first == x)
            {
                delta += e[k].second;
                k++;
            }
            cnt += delta;
            last = x;
            j = k;
        }
        add(last, 2 * pi);
    }
    for (Real &x : ans)
    {
        if (sgn(x) == 0)
        {
            x = 0;
        }
    }
    return ans;
}
