#pragma once

#include "14_1_001.hpp"

struct Sudoku
{
    // a 保存 9x9 棋盘，0 表示空格，1..9 表示已填数字。
    array<array<int, 9>, 9> a{};
    // row[i] 的第 v 位表示第 i 行是否已使用数字 v。
    array<int, 9> row{};
    // col[j] 的第 v 位表示第 j 列是否已使用数字 v。
    array<int, 9> col{};
    // box[b] 的第 v 位表示第 b 个 3x3 宫是否已使用数字 v。
    array<int, 9> box{};

    bool dfs(int z = 0)
    {
        // z 是按行优先扫描时下一个待检查格编号；成功返回 true。
        while (z < 81 && a[z / 9][z % 9])
        {
            z++;
        }
        if (z == 81)
        {
            return true;
        }
        int x = z / 9;
        int y = z % 9;
        int b = x / 3 * 3 + y / 3;
        int m = ~(row[x] | col[y] | box[b]) & 1022;
        while (m)
        {
            int v = countr_zero((unsigned)m);
            m &= m - 1;
            a[x][y] = v;
            row[x] |= 1 << v;
            col[y] |= 1 << v;
            box[b] |= 1 << v;
            if (dfs(z + 1))
            {
                return true;
            }
            row[x] ^= 1 << v;
            col[y] ^= 1 << v;
            box[b] ^= 1 << v;
            a[x][y] = 0;
        }
        return false;
    }

    bool solve()
    {
        // 无参数；验证初盘后求字典序最小完整数独，成功返回 true。
        row.fill(0);
        col.fill(0);
        box.fill(0);
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // 调试检查，可删。
                assert(0 <= a[i][j] && a[i][j] <= 9);
                if (a[i][j])
                {
                    int b = i / 3 * 3 + j / 3;
                    int q = 1 << a[i][j];
                    if ((row[i] | col[j] | box[b]) & q)
                    {
                        return false;
                    }
                    row[i] |= q;
                    col[j] |= q;
                    box[b] |= q;
                }
            }
        }
        return dfs();
    }
};
