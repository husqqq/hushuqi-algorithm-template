#pragma once

#include "14_1_001.hpp"

vector<int> nQueens(int n)
{
    // n 是棋盘边长；返回每行皇后的 0 下标列号，n=2 或 3 时返回空表。
    // 调试检查，可删。
    assert(n >= 1);
    if (n == 1)
    {
        return {0};
    }
    if (n < 4)
    {
        return {};
    }
    vector<int> p(n);
    auto gen = [&](auto &&self, int m, vector<int> &q) -> void
    {
        if (m & 1)
        {
            vector<int> t(m - 1);
            self(self, m - 1, t);
            copy(t.begin(), t.end(), q.begin());
            q[m - 1] = m - 1;
        }
        else if (m % 6 != 2)
        {
            for (int i = 1; i <= m / 2; i++)
            {
                q[i - 1] = 2 * i - 1;
            }
            for (int i = m / 2 + 1; i <= m; i++)
            {
                q[i - 1] = 2 * i - m - 2;
            }
        }
        else
        {
            for (int i = 1; i <= m / 2; i++)
            {
                q[i - 1] = (2 * i + m / 2 - 3) % m;
            }
            for (int i = m / 2 + 1; i <= m; i++)
            {
                q[i - 1] = m - 1 - (2 * (m - i + 1) + m / 2 - 3) % m;
            }
        }
    };
    gen(gen, n, p);
    return p;
}
