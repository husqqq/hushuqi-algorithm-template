#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

// MoQuery[0..2] 依次表示左端点、右端点、答案编号。
using MoQuery = array<int, 3>;

template <class Ans, class AddL, class AddR, class DelL, class DelR, class Get>
vector<Ans> mo(int n, vector<MoQuery> qs, AddL addL, AddR addR, DelL delL, DelR delR, Get get)
{
    // n 是数组长度，qs 是询问；四个端点回调维护状态。
    auto z = max<int>(1, (int)(n / sqrtl(max<int>(1, qs.size()))));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto x = a[0] / z, y = b[0] / z;
             if (x != y)
             {
                 return x < y;
             }
             return x & 1 ? a[1] > b[1] : a[1] < b[1];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0;
    for (auto q : qs)
    {
        while (l > q[0])
        {
            addL(--l);
        }
        while (r < q[1])
        {
            addR(r++);
        }
        while (l < q[0])
        {
            delL(l++);
        }
        while (r > q[1])
        {
            delR(--r);
        }
        ans[q[2]] = get();
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
vector<Ans> moHilbert(int n, vector<MoQuery> qs, Add add, Del del, Get get)
{
    // n 是数组长度，qs 是半开区间询问；add/del 接收下标，get 返回答案。
    int power = 0;
    while ((1LL << power) < max<int>(1, n))
    {
        power++;
    }
    assert(power <= 31);
    sort(qs.begin(), qs.end(), [&](const MoQuery &a, const MoQuery &b)
    {
        int ax = min<int>(a[0], max<int>(0, n - 1));
        int ay = min<int>(max<int>(0, a[1] - 1), max<int>(0, n - 1));
        int bx = min<int>(b[0], max<int>(0, n - 1));
        int by = min<int>(max<int>(0, b[1] - 1), max<int>(0, n - 1));
        return hilbertOrder(ax, ay, power) < hilbertOrder(bx, by, power);
    });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0;
    for (auto q : qs)
    {
        while (l > q[0])
        {
            add(--l);
        }
        while (r < q[1])
        {
            add(r++);
        }
        while (l < q[0])
        {
            del(l++);
        }
        while (r > q[1])
        {
            del(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}

// ModifyQuery[0..3] 依次表示左端点、右端点、答案编号、修改时间。
using ModifyQuery = array<int, 4>;

template <class Ans, class Add, class Del, class Change, class Get>
vector<Ans> moModify(int n, vector<ModifyQuery> qs, Add add, Del del, Change change, Get get)
{
    // n 是数组长度，qs 含修改时间；add/del 维护端点，change(p,forward,l,r) 切换第 p 次修改。
    auto z = max<int>(1, (int)powl(max<int>(1, n), 2.0L / 3));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto al = a[0] / z, bl = b[0] / z;
             if (al != bl)
             {
                 return al < bl;
             }
             auto ar = a[1] / z, br = b[1] / z;
             if (ar != br)
             {
                 return al & 1 ? ar > br : ar < br;
             }
             return ar & 1 ? a[3] > b[3] : a[3] < b[3];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0, t = 0;
    for (auto q : qs)
    {
        while (t < q[3])
        {
            change(t++, true, l, r);
        }
        while (t > q[3])
        {
            change(--t, false, l, r);
        }
        while (l > q[0])
        {
            add(--l);
        }
        while (r < q[1])
        {
            add(r++);
        }
        while (l < q[0])
        {
            del(l++);
        }
        while (r > q[1])
        {
            del(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}
