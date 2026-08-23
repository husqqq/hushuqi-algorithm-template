#pragma once

#include "10_2_001.hpp"

vector<pair<int, Z>> spNorm(vector<pair<int, Z>> f)
{
    // f 是任意次序的稀疏项；返回按次数递增、合并同次且删除零系数后的稀疏项。
    auto cmp = [](const auto &a, const auto &b)
    {
        return a.first < b.first;
    };
    if (!is_sorted(f.begin(), f.end(), cmp))
    {
        sort(f.begin(), f.end(), cmp);
    }
    vector<pair<int, Z>> g;
    for (auto [i, x] : f)
    {
        assert(i >= 0); // 调试检查，可删。
        if (!g.empty() && g.back().first == i)
        {
            g.back().second += x;
        }
        else
        {
            g.push_back({i, x});
        }
    }
    g.erase(remove_if(g.begin(), g.end(), [](const auto &x)
    {
        return x.second == Z(0);
    }), g.end());
    return g;
}

Poly sparseDiv(const Poly &a, vector<pair<int, Z>> b, int n)
{
    // a 是稠密分子，b 保存非零的 {次数,系数}，n 是商的截断长度；返回 a/b mod x^n。
    assert(n >= 0); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    b = spNorm(move(b));
    b.erase(lower_bound(b.begin(), b.end(), n, [](const auto &x, int y)
    {
        return x.first < y;
    }), b.end());
    assert(!b.empty() && b[0].first == 0 && b[0].second != Z(0)); // 调试检查，可删。
    Z iv = b[0].second.inv();
    Poly q(n);
    for (int i = 0; i < n; i++)
    {
        q[i] = i < (int)a.size() ? a[i] : Z(0);
        for (int j = 1; j < (int)b.size() && b[j].first <= i; j++)
        {
            q[i] -= b[j].second * q[i - b[j].first];
        }
        q[i] *= iv;
    }
    return q;
}
