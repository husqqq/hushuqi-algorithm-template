#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Euclid
{
    __int128 x = 0;  // 已拼接路径的横步总数
    __int128 y = 0;  // 已拼接路径的纵步总数
    __int128 xy = 0; // 每个横步之前的纵步数之和

    Euclid operator+(const Euclid &other) const
    {
        // 把 other 路径接在当前路径后，返回合并结果。
        return {x + other.x, y + other.y,
                xy + other.xy + y * other.x};
    }
};

inline Euclid monoidPower(Euclid value, long long exponent)
{
    // 返回 value 在幺半群 + 下的 exponent 次幂。
    assert(exponent >= 0); // 调试检查，可删
    Euclid res;
    while (exponent)
    {
        if (exponent & 1)
        {
            res = res + value;
        }
        value = value + value;
        exponent >>= 1;
    }
    return res;
}

inline Euclid floorPath(long long p, long long q, long long r,
                        long long length, Euclid up, Euclid right)
{
    // p、q、r 定义取整直线，length 是横坐标上界；up、right 是纵步与横步元素，返回整段聚合。
    assert(p >= 0 && q > 0 && 0 <= r && r < q && length >= 0); // 调试检查，可删
    if (!length)
    {
        return {};
    }
    if (p >= q)
    {
        return floorPath(p % q, q, r, length, up,
                         monoidPower(up, p / q) + right);
    }
    long long height = (long long)(((__int128)length * p + r) / q);
    if (!height)
    {
        return monoidPower(right, length);
    }
    long long first = (long long)(((__int128)q - r - 1) / p);
    long long suffix = (long long)((__int128)length -
                                   ((__int128)q * height - r - 1) / p);
    return monoidPower(right, first) + up +
           floorPath(q, p, (q - r - 1) % p, height - 1,
                     right, up) +
           monoidPower(right, suffix);
}

constexpr int floorMomentMod = 1000000007;
constexpr int floorMomentDeg = 10;
using FloorTable = array<array<int, floorMomentDeg + 1>, floorMomentDeg + 1>;

namespace fm
{
inline int power(int a, int b)
{
    // a、b 是模幂底数和非负指数；返回 a^b mod floorMomentMod。
    int ans = 1;
    for (a %= floorMomentMod; b; b >>= 1, a = a * a % floorMomentMod)
        if (b & 1) ans = ans * a % floorMomentMod;
    return ans;
}

struct Pre
{
    // bin 是组合数，sum[p] 是 0 到 x 的 p 次幂和多项式系数。
    int bin[floorMomentDeg + 2][floorMomentDeg + 2]{};
    array<vector<int>, floorMomentDeg + 1> sum;

    Pre()
    {
        // 无参数；预处理组合数与幂和的 Newton 插值多项式。
        for (int n = 0; n <= floorMomentDeg + 1; n++)
        {
            bin[n][0] = bin[n][n] = 1;
            for (int k = 1; k < n; k++)
                bin[n][k] = (bin[n - 1][k - 1] + bin[n - 1][k]) % floorMomentMod;
        }
        for (int p = 0; p <= floorMomentDeg; p++)
        {
            int d = p + 1, now = 0;
            vector<int> dif(d + 1), base{1}, coef(d + 1);
            for (int x = 0; x <= d; x++)
                now = (now + power(x, p)) % floorMomentMod, dif[x] = now;
            for (int k = 0; k <= d; k++)
            {
                for (int i = 0; i < (int)base.size(); i++)
                    coef[i] = (coef[i] + dif[0] * base[i]) % floorMomentMod;
                for (int i = 0; i + 1 < (int)dif.size(); i++)
                    dif[i] = (dif[i + 1] - dif[i] + floorMomentMod) % floorMomentMod;
                dif.pop_back();
                if (k == d) break;
                vector<int> next(base.size() + 1);
                for (int i = 0; i < (int)base.size(); i++)
                    next[i] = (next[i] - base[i] * k) % floorMomentMod,
                    next[i + 1] = (next[i + 1] + base[i]) % floorMomentMod;
                int inv = power(k + 1, floorMomentMod - 2);
                for (int &x : next) x = (x * inv % floorMomentMod + floorMomentMod) % floorMomentMod;
                base.swap(next);
            }
            sum[p] = move(coef);
        }
    }

    int eval(const vector<int> &f, int x) const
    {
        // f 是低次到高次的系数，x 是代入值；返回多项式模值。
        int ans = 0;
        for (int i = f.size() - 1; i >= 0; i--) ans = (ans * (x % floorMomentMod) + f[i]) % floorMomentMod;
        return ans;
    }
};

inline const Pre &pre()
{
    // 无参数；返回只初始化一次的矩表预处理。
    static const Pre p;
    return p;
}
}

inline FloorTable floorMoment(int n, int a, int b, int c, int d)
{
    // n、a、b、c 定义 0<=i<=n 时 floor((a*i+b)/c)，d 是总次数上界；返回 sum i^p floor(...)^q。
    assert(n >= 0 && a >= 0 && b >= 0 && c > 0 && 0 <= d && d <= floorMomentDeg); // 调试检查，可删
    const auto &pc = fm::pre();
    FloorTable ans{};
    if (!a)
    {
        int v = b / c;
        for (int p = 0; p <= d; p++)
        {
            int s = pc.eval(pc.sum[p], n);
            for (int q = 0; p + q <= d; q++) ans[p][q] = s * fm::power(v, q) % floorMomentMod;
        }
        return ans;
    }
    int da = a / c, db = b / c;
    if (da || db)
    {
        auto sub = floorMoment(n, a % c, b % c, c, d);
        vector<int> pa(d + 1, 1), pb(d + 1, 1);
        for (int i = 1; i <= d; i++)
            pa[i] = pa[i - 1] * (da % floorMomentMod) % floorMomentMod,
            pb[i] = pb[i - 1] * (db % floorMomentMod) % floorMomentMod;
        for (int p = 0; p <= d; p++) for (int q = 0; p + q <= d; q++)
            for (int j = 0; j <= q; j++) for (int t = 0; t + j <= q; t++)
            {
                int w = pc.bin[q][j] * pc.bin[q - j][t] % floorMomentMod;
                w = w * pa[t] % floorMomentMod * pb[q - j - t] % floorMomentMod;
                ans[p][q] = (ans[p][q] + w * sub[p + t][j]) % floorMomentMod;
            }
        return ans;
    }
    int h = (long long)(((__int128)a * n + b) / c);
    for (int p = 0; p <= d; p++) ans[p][0] = pc.eval(pc.sum[p], n);
    if (!h) return ans;
    auto sub = floorMoment(h - 1, c, c - b - 1, a, d);
    for (int p = 0; p <= d; p++) for (int q = 1; p + q <= d; q++)
    {
        int w = ans[p][0] * fm::power(h, q) % floorMomentMod;
        for (int u = 0; u < q; u++) for (int r = 0; r <= p + 1; r++)
            w = (w - pc.bin[q][u] * pc.sum[p][r] % floorMomentMod * sub[u][r]) % floorMomentMod;
        ans[p][q] = (w + floorMomentMod) % floorMomentMod;
    }
    return ans;
}

inline vector<int> floorMoments(const vector<array<int, 6>> &qs)
{
    // qs 每项是 n,a,b,c,p,q；返回 sum_{i=0}^n i^p floor((ai+b)/c)^q 模 1e9+7。
    vector<int> ans;
    ans.reserve(qs.size());
    for (auto x : qs)
    {
        assert(x[4] >= 0 && x[5] >= 0 && x[4] + x[5] <= floorMomentDeg); // 调试检查，可删
        auto t = floorMoment(x[0], x[1], x[2], x[3], x[4] + x[5]);
        ans.push_back(t[x[4]][x[5]]);
    }
    return ans;
}
