#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DuSieve
{
    int n; // 预筛上界
    vector<int> p; // 预筛范围内的素数
    vector<int> mu; // 莫比乌斯函数值
    vector<int> phi; // 欧拉函数值
    vector<bool> vis; // 合数标记
    vector<long long> smu; // 莫比乌斯函数前缀和
    vector<__int128> sphi; // 欧拉函数前缀和
    unordered_map<long long, long long> cmu; // 大参数 Mertens 值
    unordered_map<long long, __int128> cphi; // 大参数欧拉前缀和

    DuSieve(long long m)
    {
        // m 是最大查询上界；预筛约 m^(2/3) 内的 mu、phi 及前缀和。
        assert(m >= 0); // 调试检查，可删
        n = max(1LL, (long long)(powl(m, 2.0L / 3) + 2));
        mu.resize(n + 1);
        phi.resize(n + 1);
        vis.resize(n + 1);
        smu.resize(n + 1);
        sphi.resize(n + 1);
        mu[1] = 1;
        phi[1] = 1;
        for (int i = 2; i <= n; i++)
        {
            if (!vis[i])
            {
                p.push_back(i);
                mu[i] = -1;
                phi[i] = i - 1;
            }
            for (int q : p)
            {
                if (i > n / q)
                {
                    break;
                }
                vis[i * q] = true;
                if (i % q == 0)
                {
                    phi[i * q] = phi[i] * q;
                    break;
                }
                mu[i * q] = -mu[i];
                phi[i * q] = phi[i] * (q - 1);
            }
        }
        for (int i = 1; i <= n; i++)
        {
            smu[i] = smu[i - 1] + mu[i];
            sphi[i] = sphi[i - 1] + phi[i];
        }
    }

    long long sumMu(long long x)
    {
        // 返回 sum_{i=1}^x mu(i)，超出预筛范围时分块递归并记忆化。
        assert(x >= 0); // 调试检查，可删
        if (x <= n)
        {
            return smu[x];
        }
        if (cmu.contains(x))
        {
            return cmu[x];
        }
        __int128 ans = 1;
        long long l = 2;
        while (l <= x)
        {
            long long r = x / (x / l);
            ans -= (__int128)(r - l + 1) * sumMu(x / l);
            if (r == x)
            {
                break;
            }
            l = r + 1;
        }
        return cmu[x] = (long long)ans;
    }

    __int128 sumPhi(long long x)
    {
        // 返回 sum_{i=1}^x phi(i)，超出预筛范围时分块递归并记忆化。
        assert(x >= 0); // 调试检查，可删
        if (x <= n)
        {
            return sphi[x];
        }
        if (cphi.contains(x))
        {
            return cphi[x];
        }
        __int128 ans = (__int128)x * ((__int128)x + 1) / 2;
        long long l = 2;
        while (l <= x)
        {
            long long r = x / (x / l);
            ans -= (__int128)(r - l + 1) * sumPhi(x / l);
            if (r == x)
            {
                break;
            }
            l = r + 1;
        }
        return cphi[x] = ans;
    }
};
