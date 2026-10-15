#pragma once

#include "13_1_001.hpp"

vector<long double> derivative(const vector<long double> &a)
{
    // a[i] 是 x^i 系数；返回导函数系数，常数项导数为 {0}。
    if (a.size() <= 1)
    {
        return {0};
    }
    vector<long double> b(a.size() - 1);
    for (int i = 1; i < (int)a.size(); i++)
    {
        b[i - 1] = i * a[i];
    }
    return b;
}

vector<long double> integral(const vector<long double> &a, long double c = 0)
{
    // a[i] 是 x^i 系数，c 是积分常数；返回一个原函数的系数。
    vector<long double> b(a.size() + 1);
    b[0] = c;
    for (int i = 0; i < (int)a.size(); i++)
    {
        b[i + 1] = a[i] / (i + 1);
    }
    return b;
}
