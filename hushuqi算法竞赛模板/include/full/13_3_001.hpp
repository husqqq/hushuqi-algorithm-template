#pragma once

#include "13_1_001.hpp"

vector<long double> derivative(const vector<long double> &a)
{
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
    vector<long double> b(a.size() + 1);
    b[0] = c;
    for (int i = 0; i < (int)a.size(); i++)
    {
        b[i + 1] = a[i] / (i + 1);
    }
    return b;
}
