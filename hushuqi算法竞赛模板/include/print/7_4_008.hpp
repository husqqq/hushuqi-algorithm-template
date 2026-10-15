#pragma once
#include "7_1_001.hpp"

struct DLogHash
{
    // tab 是开放寻址表，非空槽高 32 位存域元素、低 32 位存指数，0 表示空槽（域元素非零）。
    vector<unsigned long long> tab;

    DLogHash(int n) : tab(max(3LL, n * 10 / 7 + 3))
    {
        // n 是预计插入的键数；表长取约 1.43n，装载率不超过 0.7。
    }

    unsigned long long hash(unsigned long long x) const
    {
        // x 是键；返回 splitmix64 混合后的散列值。
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void insert(int key, int val)
    {
        // key 是 [1,p) 内的域元素，val 是正指数；同一键只保留最小的指数。
        int p = hash(key) % tab.size();
        while (tab[p] && (int)(tab[p] >> 32) != key)
        {
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        if (!tab[p] || val < (int)(tab[p] & 0xffffffffULL))
        {
            tab[p] = ((unsigned long long)key << 32) | val;
        }
    }

    int find(int key) const
    {
        // 返回 key 对应的最小指数，不存在时返回 -1。
        int p = hash(key) % tab.size();
        while (tab[p])
        {
            if ((int)(tab[p] >> 32) == key)
            {
                return (int)(tab[p] & 0xffffffffULL);
            }
            if (++p == (int)tab.size())
            {
                p = 0;
            }
        }
        return -1;
    }
};

vector<int> batchDLog(int p, int g, const vector<int> &a)
{
    // p 是不超过 1e9 的素数，g 是模 p 的原根，a[i] 在 [1,p) 内；
    // 返回 ans[i] 为满足 g^x = a[i] (mod p) 的最小 x，范围 [0,p-2]。
    assert(p >= 2 && 1 <= g && g < p); // 调试检查，可删。
    int qn = a.size();
    vector<int> ans(qn, -1);
    if (qn == 0)
    {
        return ans;
    }
    if (p == 2)
    {
        fill(ans.begin(), ans.end(), 0);
        return ans;
    }
    // 巨步表存 g^{Bx}（1<=x<=lim），小步对所有查询共享 g^y（0<=y<B）。
    int B = sqrtl((long double)(p - 1) / qn) + 1;
    int lim = (p - 2 + B - 1) / B;
    DLogHash tab(lim);
    int step = powerMod(g, B, p), cur = step;
    for (int x = 1; x <= lim; x++)
    {
        tab.insert(cur, x);
        cur = cur * step % p;
    }
    int gy = 1; // gy 是 g^y。
    for (int y = 0; y < B; y++)
    {
        for (int i = 0; i < qn; i++)
        {
            assert(1 <= a[i] && a[i] < p); // 调试检查，可删。
            if (a[i] == 1)
            {
                ans[i] = 0;
                continue;
            }
            // a[i]*g^y = g^{Bx} 说明 a[i] = g^{Bx-y}。
            int x = tab.find(a[i] * gy % p);
            if (x != -1)
            {
                int e = ((x * B - y) % (p - 1) + p - 1) % (p - 1);
                if (ans[i] == -1 || e < ans[i])
                {
                    ans[i] = e;
                }
            }
        }
        gy = gy * g % p;
    }
    return ans;
}
