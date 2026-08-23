#pragma once

#include "11_1_003.hpp"

template <class T> tuple<T, int, int, int> maxTriangle(const vector<Point<T>> &a)
{
    int n = (int)a.size();
    tuple<T, int, int, int> ans{0, -1, -1, -1};
    if (n < 3)
    {
        return ans;
    }
    auto area = [&](int i, int j, int k)
    {
        T s = cross(a[i % n], a[j % n], a[k % n]);
        return s < 0 ? -s : s;
    };
    for (int i = 0; i < n; i++)
    {
        int k = i + 2;
        for (int j = i + 1; j < n; j++)
        {
            if (k <= j)
            {
                k = j + 1;
            }
            while (k + 1 < i + n && sgn(area(i, j, k + 1) - area(i, j, k)) > 0)
            {
                k++;
            }
            T s = area(i, j, k);
            if (s > get<0>(ans))
            {
                ans = {s, i, j % n, k % n};
            }
        }
    }
    return ans;
}

template <class T> T maxQuad(const vector<Point<T>> &a)
{
    int n = (int)a.size();
    T ans = 0;
    auto area = [&](int i, int j, int k)
    {
        T s = cross(a[i], a[j], a[k]);
        return s < 0 ? -s : s;
    };
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            T x = 0;
            T y = 0;
            for (int k = i + 1; k < j; k++)
            {
                x = max(x, area(i, j, k));
            }
            for (int k = j + 1; k < n; k++)
            {
                y = max(y, area(i, j, k));
            }
            for (int k = 0; k < i; k++)
            {
                y = max(y, area(i, j, k));
            }
            ans = max(ans, x + y);
        }
    }
    return ans;
}
