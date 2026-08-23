#pragma once
#include "10_1_013.hpp"

template <class T> T hafnian(const vector<vector<T>> &a)
{
    int N = a.size();
    // 调试检查，可删。
    assert(N % 2 == 0);
    int n = N / 2;
    // 调试检查，可删。
    assert(n <= 20);
    for (int i = 0; i < N; i++)
    {
        // 调试检查，可删。
        assert((int)a[i].size() == N);
        for (int j = 0; j < N; j++)
        {
            // 调试检查，可删。
            assert(a[i][j] == a[j][i]);
        }
    }
    vector<T> cyc(1 << n);
    for (int i = 0; i < n; i++)
    {
        int u = 2 * i;
        int v = u + 1;
        int w = 2 * i;
        cyc[1 << i] += a[u][v];
        vector<T> dp(w * (1 << i));
        for (int j = 0; j < i; j++)
        {
            int x = 2 * j;
            int y = x + 1;
            dp[w * (1 << j) + x] += a[u][y];
            dp[w * (1 << j) + y] += a[u][x];
        }
        for (int mask = 0; mask < (1 << i); mask++)
        {
            for (int j = 0; j < i; j++)
            {
                int x = 2 * j;
                int y = x + 1;
                cyc[mask | (1 << i)] += dp[w * mask + x] * a[v][x];
                cyc[mask | (1 << i)] += dp[w * mask + y] * a[v][y];
                int rest = ((1 << i) - 1) ^ mask;
                while (rest)
                {
                    int k = countr_zero((unsigned)rest);
                    rest &= rest - 1;
                    int p = 2 * k;
                    int q = p + 1;
                    int to = mask | (1 << k);
                    dp[w * to + p] += dp[w * mask + x] * a[x][q];
                    dp[w * to + p] += dp[w * mask + y] * a[y][q];
                    dp[w * to + q] += dp[w * mask + x] * a[x][p];
                    dp[w * to + q] += dp[w * mask + y] * a[y][p];
                }
            }
        }
    }
    return setExp(cyc).back();
}
