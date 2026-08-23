#include <bits/stdc++.h>
using namespace std;
#define int long long

unsigned long long kthRoot(unsigned long long n, int k)
{
    // n 是被开方的非负整数，k 是正整数次数；返回 floor(n^(1/k))。
    assert(k > 0); // 调试检查，可删
    if (k == 1 || n <= 1)
    {
        return n;
    }
    if (k >= 64)
    {
        return 1;
    }
    auto ok = [&](unsigned long long x)
    {
        unsigned __int128 y = 1;
        for (int i = 0; i < k; i++)
        {
            y *= x;
            if (y > n)
            {
                return false;
            }
        }
        return true;
    };
    unsigned long long l = 1;
    unsigned long long r = min(n, 1ULL << (64 / k + 1));
    while (l < r)
    {
        unsigned long long m = l + (r - l + 1) / 2;
        if (ok(m))
        {
            l = m;
        }
        else
        {
            r = m - 1;
        }
    }
    return l;
}

struct LehmerPi
{
    static constexpr int N = 5000000; // 固定预筛上界
    vector<int> p; // 不超过 N 的素数表
    vector<int> pi; // pi[x] 是不超过 x 的素数个数
    unordered_map<unsigned long long, long long> memPhi; // Legendre phi 缓存
    unordered_map<long long, long long> memPi; // 大参数 pi 缓存

    LehmerPi() : pi(N + 1)
    {
        // 线性筛出不超过 N 的素数及其计数前缀。
        vector<bool> c(N + 1);
        for (int i = 2; i <= N; i++)
        {
            if (!c[i])
            {
                p.push_back(i);
            }
            for (int q : p)
            {
                if (i > N / q)
                {
                    break;
                }
                c[i * q] = true;
                if (i % q == 0)
                {
                    break;
                }
            }
            pi[i] = p.size();
        }
    }

    long long phi(long long x, int s)
    {
        // 返回 [1,x] 中不被前 s 个素数整除的整数个数。
        assert(x >= 0 && 0 <= s && s <= (int)p.size()); // 调试检查，可删
        if (!x)
        {
            return 0;
        }
        if (!s)
        {
            return x;
        }
        if (s == 1)
        {
            return x - x / 2;
        }
        if (x <= N && 1LL * p[s - 1] * p[s - 1] > x)
        {
            return max(1LL, pi[x] - s + 1);
        }
        if (s < 100 && x < 100000)
        {
            unsigned long long k = (unsigned long long)x << 7 | s;
            if (auto it = memPhi.find(k); it != memPhi.end())
            {
                return it->second;
            }
            return memPhi[k] = phi(x, s - 1) - phi(x / p[s - 1], s - 1);
        }
        return phi(x, s - 1) - phi(x / p[s - 1], s - 1);
    }

    long long operator()(long long x)
    {
        // 返回 pi(x)，固定表版本要求 0<=x<=N^2。
        assert(0 <= x && x <= (long long)N * N); // 调试检查，可删
        if (x <= N)
        {
            return pi[x];
        }
        if (auto it = memPi.find(x); it != memPi.end())
        {
            return it->second;
        }
        long long a = (*this)(kthRoot((unsigned long long)x, 4));
        long long b = (*this)(kthRoot((unsigned long long)x, 2));
        long long c = (*this)(kthRoot((unsigned long long)x, 3));
        long long ans = phi(x, a) + (b + a - 2) * (b - a + 1) / 2;
        for (long long i = a; i < b; i++)
        {
            long long w = x / p[i];
            ans -= (*this)(w);
            if (i < c)
            {
                long long z = (*this)(kthRoot((unsigned long long)w, 2));
                for (long long j = i; j < z; j++)
                {
                    ans -= (*this)(w / p[j]) - j;
                }
            }
        }
        return memPi[x] = ans;
    }
};

signed main()
{
    long long n; cin >> n;
    LehmerPi pi;
    cout << pi(n) << '\n';
}
