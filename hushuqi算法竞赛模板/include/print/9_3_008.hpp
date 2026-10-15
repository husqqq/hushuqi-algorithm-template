#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

optional<vector<vector<int>>> lll(vector<vector<int>> b, long double delta = 0.75L)
{
    // b 是 n 个长度为 m 的整数向量，须线性无关；delta 取 (1/4,1)，越接近 1 约简越强。
    // 返回同一格的 LLL 约简基（第一个向量长度不超过最短向量的 (4/(4delta-1))^{(n-1)/2} 倍）；
    // 线性相关时返回空。Gram-Schmidt 用 long double 计算，系数很大时可能有精度误差。
    int n = b.size();
    if (!n)
    {
        return b;
    }
    int m = b[0].size();
    assert(0.25L < delta && delta < 1); // 调试检查，可删。
    // st[i] 是 b[i] 的 Gram-Schmidt 正交化向量，nm[i]=|st[i]|^2，mu[i][j]=<b[i],st[j]>/nm[j]。
    vector<vector<long double>> st(n, vector<long double>(m)), mu(n, vector<long double>(n));
    vector<long double> nm(n);
    auto gs = [&]() -> bool
    {
        // 重新做 Gram-Schmidt；出现零向量（线性相关）时返回 false。
        for (int i = 0; i < n; i++)
        {
            for (int t = 0; t < m; t++)
            {
                st[i][t] = b[i][t];
            }
            for (int j = 0; j < i; j++)
            {
                long double dot = 0;
                for (int t = 0; t < m; t++)
                {
                    dot += b[i][t] * st[j][t];
                }
                mu[i][j] = dot / nm[j];
                for (int t = 0; t < m; t++)
                {
                    st[i][t] -= mu[i][j] * st[j][t];
                }
            }
            nm[i] = 0;
            for (auto x : st[i])
            {
                nm[i] += x * x;
            }
            if (nm[i] < 1e-9L)
            {
                return false;
            }
        }
        return true;
    };
    if (!gs())
    {
        return nullopt;
    }
    int k = 1;
    while (k < n)
    {
        // 尺寸约简：让 |mu[k][j]| <= 1/2。
        for (int j = k - 1; j >= 0; j--)
        {
            int q = llroundl(mu[k][j]);
            if (q)
            {
                for (int t = 0; t < m; t++)
                {
                    b[k][t] -= q * b[j][t];
                }
                // 正交向量不变，只需同步 mu[k][*]。
                for (int i = 0; i < j; i++)
                {
                    mu[k][i] -= q * mu[j][i];
                }
                mu[k][j] -= q;
            }
        }
        // Lovász 条件成立则前进，否则交换相邻两向量并后退。
        if (nm[k] >= (delta - mu[k][k - 1] * mu[k][k - 1]) * nm[k - 1])
        {
            k++;
        }
        else
        {
            swap(b[k], b[k - 1]);
            gs();
            k = max(1LL, k - 1);
        }
    }
    return b;
}
