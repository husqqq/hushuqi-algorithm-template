#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

namespace gf2
{
using Row = vector<unsigned long long>; // Row 是压位的 0/1 行，第 i 位存在第 i/64 个字的第 i%64 位。

bool bit(const Row &r, int i)
{
    // 返回 r 的第 i 位。
    return r[i / 64] >> (i % 64) & 1;
}

void flip(Row &r, int i)
{
    // 翻转 r 的第 i 位。
    r[i / 64] ^= 1ULL << (i % 64);
}

struct Result
{
    bool consistent; // consistent 表示方程组有解。
    int rank, free;  // rank 是系数矩阵的秩，free = n - rank 是自由元个数。
    Row solution;    // solution 是一组特解（自由元取 0），无解时无意义。
    vector<Row> basis; // basis 是齐次方程组解空间的一组基，共 free 个。
};

Result solve(vector<Row> a, int n)
{
    // a 是 GF(2) 上的增广矩阵，每行第 0..n-1 位是系数，第 n 位是右端，行长须为 (n+64)/64 个字；
    // 返回可解性、秩、特解与齐次解基。复杂度 O(m n^2 / 64)。
    int rank = 0, m = a.size();
    vector<int> where(n, -1); // where[c] 是以第 c 列为主元的行号，自由元为 -1。
    for (int c = 0; c < n; c++)
    {
        int p = rank;
        while (p < m && !bit(a[p], c))
        {
            p++;
        }
        if (p == m)
        {
            continue;
        }
        swap(a[p], a[rank]);
        where[c] = rank;
        for (int i = 0; i < m; i++)
        {
            if (i != rank && bit(a[i], c))
            {
                for (int j = 0; j < (int)a[i].size(); j++)
                {
                    a[i][j] ^= a[rank][j];
                }
            }
        }
        rank++;
    }
    Result res{true, rank, n - rank, Row((n + 63) / 64), {}};
    for (int i = rank; i < m; i++)
    {
        if (bit(a[i], n))
        {
            res.consistent = false;
            return res;
        }
    }
    // 消成简化阶梯形后，主元变量 = 右端 xor (该行上自由元的取值)。
    for (int c = 0; c < n; c++)
    {
        if (where[c] >= 0 && bit(a[where[c]], n))
        {
            flip(res.solution, c);
        }
    }
    for (int f = 0; f < n; f++)
    {
        if (where[f] < 0)
        {
            // 令自由元 f 为 1、其余自由元为 0，得到一个齐次解。
            Row v((n + 63) / 64);
            flip(v, f);
            for (int c = 0; c < n; c++)
            {
                if (where[c] >= 0 && bit(a[where[c]], f))
                {
                    flip(v, c);
                }
            }
            res.basis.push_back(move(v));
        }
    }
    return res;
}
}
