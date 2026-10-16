#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

long double binomialPmf(int n, int k, long double p, bool logp = false)
{
    // n>=0 是试验数，k 是成功数，p 是 [0,1] 内有限成功率。
    // logp=false 返回 P(X=k)，为 true 返回其自然对数；零概率的对数为负无穷。
    assert(n >= 0 && 0 <= p && p <= 1); // 调试检查，可删。
    long double zero = logp ? -numeric_limits<long double>::infinity() : 0;
    if (k < 0 || k > n)
    {
        return zero;
    }
    if (p == 0)
    {
        return k == 0 ? (logp ? 0 : 1) : zero;
    }
    if (p == 1)
    {
        return k == n ? (logp ? 0 : 1) : zero;
    }
    long double z;
    if (k == 0) z = n * log1pl(-p);
    else if (k == n) z = n * logl(p);
    else
    {
        const long double logtau = logl(2 * acosl(-1.0L));
        auto err = [&](long double x)
        {
            // x 是正整数；返回 log(x!)-(x+1/2)*log(x)+x-log(2*pi)/2。
            if (x < 16) return lgammal(x + 1) - (x + 0.5L) * logl(x) + x - logtau / 2;
            // Stirling 余项的前八项；只在 x>=16 时使用，避免发散渐近级数的小参数区。
            const array<long double, 8> c = {1.0L / 12, -1.0L / 360, 1.0L / 1260,
                -1.0L / 1680, 1.0L / 1188, -691.0L / 360360, 1.0L / 156, -3617.0L / 122400};
            long double q = 1 / (x * x), ans = 0;
            for (auto it = c.rbegin(); it != c.rend(); ++it)
            {
                ans = ans * q + *it;
            }
            return ans / x;
        };
        auto dev = [](long double x, long double m, long double d)
        {
            // x 是正计数，m 是正期望计数，d 是补偿后的 x-m；返回偏差项 D(x,m)。
            if (fabsl(d) < m / 8)
            {
                long double u = d / m, term = u * u, ans = 0;
                for (int j = 2; ; j++)
                {
                    long double next = ans + term / (j * (j - 1));
                    if (next == ans) return m * ans;
                    ans = next;
                    term *= -u;
                }
            }
            // 分开取对数，避免 x/m 在极小成功率下先溢出。
            return x * (logl(x) - logl(m)) - d;
        };
        long double a = k, b = n - k, t = n, small = min(k, n - k);
        // 从较小概率一侧求中心差，避免接近 1 的概率乘积抵消；两侧差值互为相反数。
        long double d = p <= 0.5L ? fmal(-t, p, a) : -fmal(-t, 1 - p, b);
        long double ma = t * p, mb = t * (1 - p);
        z = err(t) - err(a) - err(b) - dev(a, ma, d) - dev(b, mb, -d);
        z -= (logtau + logl(small) + log1pl(-small / t)) / 2;
    }
    if (logp) return z;
    // 先求概率平方根再平方，推迟指数函数的下溢，最后乘法按当前格式舍入。
    long double ans = expl(z / 2);
    return ans * ans;
}

array<long double, 2> geomMoments(long double p)
{
    // p 是每次成功概率；返回几何分布（首次成功试验数）的 {期望, 方差}。
    assert(0 < p && p <= 1); // 调试检查，可删。
    return {1 / p, (1 - p) / (p * p)};
}
