#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace exp_var
{
array<long double, 2> moments(const vector<long double> &x, const vector<long double> &p)
{
    // x 是取值，p 是有限非负概率且总和约为 1；零概率项忽略，其余取值须有限。
    // 返回按实际概率和归一化后的 {期望, 方差}，方差超出浮点范围时可为正无穷。
    assert(!x.empty() && x.size() == p.size());
    auto add = [](long double v, long double &s, long double &c)
    {
        // v 是新增项，s 是主和，c 是舍入补偿；更新二者，无返回值。
        long double t = s + v;
        c += fabsl(s) >= fabsl(v) ? (s - t) + v : (v - t) + s;
        s = t;
    };
    long double sum = 0, pcorr = 0; // 概率总和及其舍入补偿。
    long double lo = numeric_limits<long double>::infinity(), hi = -lo;
    int at = -1; // 最大概率项作为中心，避免由极罕见的离群值决定平移量。
    for (int i = 0; i < (int)x.size(); i++)
    {
        assert(isfinite(p[i]) && p[i] >= 0);
        add(p[i], sum, pcorr);
        if (p[i] == 0) continue;
        assert(isfinite(x[i]));
        if (at < 0 || p[i] > p[at]) at = i;
        lo = min(lo, x[i]);
        hi = max(hi, x[i]);
    }
    sum += pcorr;
    assert(fabsl(sum - 1) <= 1E-12L);
    assert(at >= 0);
    long double scale = max(fabsl(lo), fabsl(hi)) > numeric_limits<long double>::max() / 4 ? 0.25L : 1;
    long double base = x[at] * scale; // 极大输入先缩小四倍，保证中心差不会溢出。
    long double mean = 0, mcorr = 0; // 原取值加权和及补偿，用于单独返回期望。
    long double delta = 0, dcorr = 0; // 中心差的加权和及补偿，用于保留小方差。
    for (int i = 0; i < (int)x.size(); i++)
    {
        if (p[i] == 0) continue;
        long double y = x[i] * scale, d = y - base;
        long double a = p[i] * y;
        add(a, mean, mcorr);
        if (lo < 0 && hi > 0) add(fmal(p[i], y, -a), mean, mcorr); // 补回异号抵消前的乘积舍入。
        add((p[i] / sum) * d, delta, dcorr);
    }
    delta += dcorr; // 只保存相对中心的均值，计算方差时不加回大偏移。
    long double var = 0;
    for (int i = 0; i < (int)x.size(); i++)
    {
        if (p[i] == 0) continue;
        long double d = (x[i] * scale - base) - delta;
        var += ((p[i] / sum) * d) * d; // 先乘概率，避免先平方造成中间溢出。
    }
    // 期望位于正概率取值范围内，限制舍入越界后再还原尺度。
    mean = clamp((mean + mcorr) / sum, lo * scale, hi * scale);
    return {mean / scale, var / scale / scale};
}
}
