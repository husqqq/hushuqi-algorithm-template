#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace exp_var
{
struct OnlineMom
{
    long double w = 0; // 总权重；零权重表示空集，成员只由接口更新。
    long double base = 0; // 一条已加入观测的取值，作为平移中心。
    long double mean = 0; // 相对于 base 的带权均值，避免反复加回大偏移。
    long double m2 = 0; // 未归一化的中心二阶矩，即 sum(weight*(x-均值)^2)。

    void merge(OnlineMom b)
    {
        // b 是待合并分组的副本；合并到当前对象，不修改来源，无返回值。
        // 空集为单位元；自合并相当于把全部观测重复一次，不去重。
        if (b.w == 0) return;
        if (w == 0)
        {
            *this = b;
            return;
        }
        if (w < b.w) swap(*this, b); // 保留较重分组的中心。
        long double next = w + b.w;
        long double d = (b.base - base) + (b.mean - mean); // 两组均值之差。
        // 指数分解避免先计算 b.w/next 下溢，再乘很大的 d。
        signed ed, ew, en; // frexpl 的指数输出要求原生 int，不受整数宏影响。
        long double part = frexpl(d, &ed) * frexpl(b.w, &ew) / frexpl(next, &en);
        mean += ldexpl(part, ed + ew - en);
        m2 += b.m2 + ((b.w * d) * (w / next)) * d;
        w = next;
    }

    void add(long double x, long double weight = 1)
    {
        // x 是观测，weight 是有限非负权重；加入当前分组，无返回值。
        // 零权重忽略（x 可非有限）；正权重要求 x 有限。
        assert(isfinite(weight) && weight >= 0);
        if (weight == 0) return;
        assert(isfinite(x));
        OnlineMom b;
        b.w = weight;
        b.base = x;
        merge(b);
    }

    optional<array<long double, 2>> get() const
    {
        // 无参数；返回 {带权均值, 总体方差}，空集返回 nullopt，不修改状态。
        if (w == 0) return nullopt;
        return array<long double, 2>{base + mean, m2 / w};
    }
};
}
