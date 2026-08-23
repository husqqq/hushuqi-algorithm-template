#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace young_tab
{
template <class T = long long>
T youngTab(const vector<int> &row)
{
    // row 是非增的行长分拆；返回标准 Young 表数量。
    static_assert(numeric_limits<T>::is_integer && numeric_limits<T>::is_signed &&
                  sizeof(T) >= sizeof(long long));
    size_t cells = 0;
    for (int i = 0; i < (int)row.size(); i++)
    {
        assert(row[i] >= 0); // 调试检查，可删。
        if (i)
        {
            assert(row[i - 1] >= row[i]); // 调试检查，可删。
        }
        cells += row[i];
    }
    int n = (int)cells;
    vector<int> spf(n + 1), ps;
    for (int i = 2; i <= n; i++)
    {
        if (!spf[i])
        {
            spf[i] = i;
            ps.push_back(i);
        }
        for (int p : ps)
        {
            if (p > spf[i] || i > n / p)
            {
                break;
            }
            spf[i * p] = p;
        }
    }
    vector<int> e(n + 1);
    for (int p : ps)
    {
        for (int x = n / p; x; x /= p)
        {
            e[p] += x;
        }
    }
    vector<int> col(row.empty() ? 0 : row[0]);
    for (int x : row)
    {
        for (int j = 0; j < x; j++)
        {
            col[j]++;
        }
    }
    for (int i = 0; i < (int)row.size(); i++)
    {
        for (int j = 0; j < row[i]; j++)
        {
            int x = row[i] - j + col[j] - i - 1;
            while (x > 1)
            {
                int p = spf[x];
                e[p]--;
                x /= p;
            }
        }
    }
    T ans = 1;
    for (int p : ps)
    {
        assert(e[p] >= 0); // 调试检查，可删。
        for (int i = 0; i < e[p]; i++)
        {
            ans *= p;
        }
    }
    return ans;
}
}
