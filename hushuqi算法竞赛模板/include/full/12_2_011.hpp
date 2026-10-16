#pragma once

#include "12_1_001.hpp"

// 插头 DP（括号表示法）：统计用一条闭合回路覆盖棋盘全部空格的方案数（URAL1519 型）。
// 轮廓线上有 m+1 个插头，每个取值 {0 无, 1 左括号'(', 2 右括号')'}，2 bit 编码。
template <class Acc = long long>
struct PlugDP
{
    int n; // n 是棋盘行数。
    int m; // m 是棋盘列数。
    vector<vector<int>> a; // a[i][j] 为 1 表示障碍，0 表示必须覆盖的空格。

    PlugDP(vector<vector<int>> g) : n(g.size()), m(0), a(move(g))
    {
        // g 是矩形棋盘，1 表示障碍，0 表示必须覆盖的空格；初始化棋盘。
        assert(n > 0 && !a[0].empty()); // 调试检查，可删。
        m = a[0].size();
        assert(2 * (m + 1) <= 64); // 调试检查，可删。
        for (int i = 0; i < n; i++)
        {
            assert((int)a[i].size() == m); // 调试检查，可删。
        }
    }

    // 状态需精确容纳 64 位位布局，不能随 int 宏宽度改变。
    int get(unsigned long long s, int p) const
    {
        // s 是插头状态，p 是插头位置；返回该位置的 0/1/2 编码。
        return (s >> (2 * p)) & 3;
    }

    unsigned long long set(unsigned long long s, int p, int v) const
    {
        // s 是插头状态，p 是插头位置，v 是新的 0/1/2 编码；返回修改后的状态。
        return (s & ~(3ULL << (2 * p))) | ((unsigned long long)v << (2 * p));
    }

    Acc solve()
    {
        // 无参数；返回覆盖全部空格且只有一条闭合回路的方案数。
        int ex = -1;
        int ey = -1;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (!a[i][j])
                {
                    ex = i;
                    ey = j;
                }
            }
        }
        if (ex < 0)
        {
            return 0;
        }

        map<unsigned long long, Acc> cur;
        cur[0] = 1;
        Acc ans = 0;
        for (int i = 0; i < n; i++)
        {
            map<unsigned long long, Acc> nxt;
            for (auto [s, c] : cur)
            {
                nxt[s << 2] += c;
            }
            cur = nxt;
            for (int j = 0; j < m; j++)
            {
                map<unsigned long long, Acc> nx;
                for (auto [s, c] : cur)
                {
                    int L = get(s, j), U = get(s, j + 1);
                    if (a[i][j])                         // 障碍：左、上插头必须都空
                    {
                        if (!L && !U)
                        {
                            nx[s] += c;
                        }
                        continue;
                    }
                    if (!L && !U)                        // ┌：新建一对插头
                    {
                        if (i + 1 < n && j + 1 < m)
                        {
                            nx[set(set(s, j, 1), j + 1, 2)] += c;
                        }
                    }
                    else if (!L || !U)                   // 直行/拐弯：单插头可向右或向下
                    {
                        int t = L ? L : U;
                        if (j + 1 < m)
                        {
                            nx[set(set(s, j, 0), j + 1, t)] += c;
                        }
                        if (i + 1 < n)
                        {
                            nx[set(set(s, j, t), j + 1, 0)] += c;
                        }
                    }
                    else if (L == 1 && U == 1)           // ( ( ：找 U 的配对 ) 改成 (
                    {
                        unsigned long long t = set(set(s, j, 0), j + 1, 0);
                        int cnt = 1;
                        for (int k = j + 2; k <= m; k++)
                        {
                            int v = get(t, k);
                            if (v == 1)
                            {
                                cnt++;
                            }
                            else if (v == 2)
                            {
                                cnt--;
                                if (cnt == 0)
                                {
                                    t = set(t, k, 1);
                                    break;
                                }
                            }
                        }
                        nx[t] += c;
                    }
                    else if (L == 2 && U == 2)           // ) ) ：找 L 的配对 ( 改成 )
                    {
                        unsigned long long t = set(set(s, j, 0), j + 1, 0);
                        int cnt = 1;
                        for (int k = j - 1; k >= 0; k--)
                        {
                            int v = get(t, k);
                            if (v == 2)
                            {
                                cnt++;
                            }
                            else if (v == 1)
                            {
                                cnt--;
                                if (cnt == 0)
                                {
                                    t = set(t, k, 2);
                                    break;
                                }
                            }
                        }
                        nx[t] += c;
                    }
                    else if (L == 2 && U == 1)           // ) ( ：两条链合并，两插头消去
                    {
                        nx[set(set(s, j, 0), j + 1, 0)] += c;
                    }
                    else                                 // ( ) ：相邻配对，闭合回路
                    {
                        // 仅当此为末空格、且闭合后轮廓再无其它插头（整条回路收尾）才计入
                        if (i == ex && j == ey && set(set(s, j, 0), j + 1, 0) == 0)
                        {
                            ans += c;
                        }
                    }
                }
                cur = nx;
            }
            map<unsigned long long, Acc> f;
            for (auto [s, c] : cur)
            {
                if (!get(s, m))
                {
                    f[s] += c;
                }
            }
            cur = f;
        }
        return ans;
    }
};
