#pragma once

#include "10_1_003.hpp"

Z lagrange(const vector<Z> &x, const vector<Z> &y, Z t)
{
    // x 是互异横坐标，y 是对应点值，t 是查询点；返回插值多项式在 t 处的值。
    assert(x.size() == y.size() && !x.empty()); // 调试检查，可删。
    int n = x.size();
    for (int i = 0; i < n; i++)
    {
        if (x[i] == t)
        {
            return y[i];
        }
    }
    Z ans = 0;
    for (int i = 0; i < n; i++)
    {
        Z c = y[i];
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                c *= (t - x[j]) / (x[i] - x[j]);
            }
        }
        ans += c;
    }
    return ans;
}
