#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

// MoQ[0..3] 依次是左端点、右端点、答案编号与修改时间。
using MoQ = array<int, 4>;

template <class Ans, class AddL, class AddR, class DelL, class DelR, class Get>
vector<Ans> mo(vector<MoQ> q, AddL addL, AddR addR, DelL delL, DelR delR, Get get)
{
    // q 的每项为 {l,r,id,time}；四个回调移动区间端点，get 返回当前答案。
    // 本接口忽略 time，返回值按 id 排列。
    int n = 0;
    for (auto x : q)
    {
        chmax(n, x[1]);
    }
    int z = max<int>(1, (int)(sqrt(max<int>(1, n))));
    sort(q.begin(),
         q.end(),
         [&](const MoQ &a, const MoQ &b)
         {
             int x = a[0] / z, y = b[0] / z;
             if (x != y)
             {
                 return x < y;
             }
             return x & 1 ? a[1] > b[1] : a[1] < b[1];
         });
    vector<Ans> ans(q.size());
    int l = 0, r = 0;
    for (auto x : q)
    {
        while (l > x[0])
        {
            addL(--l);
        }
        while (r < x[1])
        {
            addR(r++);
        }
        while (l < x[0])
        {
            delL(l++);
        }
        while (r > x[1])
        {
            delR(--r);
        }
        ans[x[2]] = get();
    }
    return ans;
}

unsigned long long hilbertOrder(int x, int y, int power, int rotate = 0)
{
    // x、y 是 [0,2^power) 内的坐标，power 不超过 31；返回 Hilbert 序键。
    if (!power)
    {
        return 0;
    }
    int h = 1LL << (power - 1);
    int seg = x < h ? (y < h ? 0 : 3) : (y < h ? 1 : 2);
    seg = (seg + rotate) & 3;
    static constexpr int delta[4] = {3, 0, 0, 1};
    int nextRot = (rotate + delta[seg]) & 3;
    unsigned long long z = 1ULL << (2 * power - 2);
    unsigned long long sub = hilbertOrder(x & (h - 1), y & (h - 1), power - 1, nextRot);
    return (unsigned long long)seg * z + (seg == 1 || seg == 2 ? sub : z - sub - 1);
}

template <class Ans, class Add, class Del, class Get>
vector<Ans> moHilbert(vector<MoQ> q, Add add, Del del, Get get)
{
    // q 的每项为 {l,r,id,time}，区间为半开 [l,r)；add/del 接收被加入或删除的下标，get 返回答案。
    int n = 0;
    for (auto x : q)
    {
        chmax(n, x[1]);
    }
    int power = 0;
    while ((1LL << power) < max<int>(1, n))
    {
        power++;
    }
    assert(power <= 31);
    sort(q.begin(), q.end(), [&](const MoQ &a, const MoQ &b)
    {
        int ax = min<int>(a[0], max<int>(0, n - 1));
        int ay = min<int>(max<int>(0, a[1] - 1), max<int>(0, n - 1));
        int bx = min<int>(b[0], max<int>(0, n - 1));
        int by = min<int>(max<int>(0, b[1] - 1), max<int>(0, n - 1));
        return hilbertOrder(ax, ay, power) < hilbertOrder(bx, by, power);
    });
    vector<Ans> ans(q.size());
    int l = 0, r = 0;
    for (auto x : q)
    {
        while (l > x[0])
        {
            add(--l);
        }
        while (r < x[1])
        {
            add(r++);
        }
        while (l < x[0])
        {
            del(l++);
        }
        while (r > x[1])
        {
            del(--r);
        }
        ans[x[2]] = get();
    }
    return ans;
}

// change(k, forward, l, r)：执行/撤销第 k 次修改，并自行维护落在 [l,r) 内的贡献。
template <class Ans, class AddL, class AddR, class DelL, class DelR, class Change, class Get>
vector<Ans> moModify(vector<MoQ> q, AddL addL, AddR addR, DelL delL, DelR delR, Change change, Get get)
{
    // q 的每项为 {l,r,id,time}；端点回调维护区间，change 执行或撤销修改。
    // get 返回当前答案，结果按 id 排列。
    int n = 0;
    for (auto x : q)
    {
        chmax(n, x[1]);
    }
    int z = max<int>(1, (int)(powl(max<int>(1, n), 2.0L / 3)));
    sort(q.begin(),
         q.end(),
         [&](const MoQ &a, const MoQ &b)
         {
             int al = a[0] / z, bl = b[0] / z;
             if (al != bl)
             {
                 return al < bl;
             }
             int ar = a[1] / z, br = b[1] / z;
             if (ar != br)
             {
                 return al & 1 ? ar > br : ar < br;
             }
             return ar & 1 ? a[3] > b[3] : a[3] < b[3];
         });
    vector<Ans> ans(q.size());
    int l = 0, r = 0, t = 0;
    for (auto x : q)
    {
        while (t < x[3])
        {
            change(t++, true, l, r);
        }
        while (t > x[3])
        {
            change(--t, false, l, r);
        }
        while (l > x[0])
        {
            addL(--l);
        }
        while (r < x[1])
        {
            addR(r++);
        }
        while (l < x[0])
        {
            delL(l++);
        }
        while (r > x[1])
        {
            delR(--r);
        }
        ans[x[2]] = get();
    }
    return ans;
}
