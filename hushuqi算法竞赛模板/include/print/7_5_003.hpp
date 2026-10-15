#pragma once
#include "1_1_008.hpp"

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

using Z7 = MInt<1000000007>; // 矩表固定模 1e9+7，换模数只改这里。
constexpr int floorDeg = 10;  // floorDeg 是 p+q 的上界。
// FloorTable[p][q] 存 sum i^p floor(...)^q，只有 p+q<=d 的项有效。
using FloorTable = array<array<Z7, floorDeg + 1>, floorDeg + 1>;

struct FloorPre
{
    // C[n][k] 是组合数；S[p] 是 S_p(x)=sum_{i=0}^{x} i^p 的系数（低次在前，0^0 记为 1）。
    array<array<Z7, floorDeg + 2>, floorDeg + 2> C;
    array<vector<Z7>, floorDeg + 1> S;

    FloorPre()
    {
        // 无参数；由 sum_k C(p+1,k) S_k(x) = (x+1)^{p+1} 逐个求出幂和多项式。
        for (int n = 0; n <= floorDeg + 1; n++)
        {
            C[n][0] = 1;
            for (int k = 1; k <= n; k++)
            {
                C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
            }
        }
        for (int p = 0; p <= floorDeg; p++)
        {
            S[p].assign(p + 2, 0);
            for (int j = 0; j <= p + 1; j++)
            {
                S[p][j] = C[p + 1][j];
            }
            for (int k = 0; k < p; k++)
            {
                for (int j = 0; j < (int)S[k].size(); j++)
                {
                    S[p][j] -= C[p + 1][k] * S[k][j];
                }
            }
            Z7 iv = Z7(p + 1).inv();
            for (auto &x : S[p])
            {
                x *= iv;
            }
        }
    }

    Z7 eval(int p, int x) const
    {
        // 返回 S_p(x)，x>=0。
        Z7 ans = 0;
        for (int i = (int)S[p].size() - 1; i >= 0; i--)
        {
            ans = ans * x + S[p][i];
        }
        return ans;
    }
};

FloorTable floorMoment(int n, int a, int b, int c, int d)
{
    // n>=0，a,b>=0，c>0，0<=d<=floorDeg；记 f(i)=floor((a*i+b)/c)，
    // 返回表 T，T[p][q] = sum_{i=0}^{n} i^p f(i)^q（p+q<=d），均模 1e9+7。
    assert(n >= 0 && a >= 0 && b >= 0 && c > 0 && 0 <= d && d <= floorDeg); // 调试检查，可删。
    static const FloorPre pre;
    FloorTable ans{};
    if (!a)
    {
        // f 恒为 b/c。
        Z7 v = b / c;
        for (int p = 0; p <= d; p++)
        {
            for (int q = 0; p + q <= d; q++)
            {
                ans[p][q] = pre.eval(p, n) * v.pow(q);
            }
        }
        return ans;
    }
    int da = a / c, db = b / c;
    if (da || db)
    {
        // f(i) = da*i + db + g(i)，g 用 (a%c, b%c)；按三项式展开 f^q。
        auto sub = floorMoment(n, a % c, b % c, c, d);
        vector<Z7> pa(d + 1, 1), pb(d + 1, 1);
        for (int i = 1; i <= d; i++)
        {
            pa[i] = pa[i - 1] * da;
            pb[i] = pb[i - 1] * db;
        }
        for (int p = 0; p <= d; p++)
        {
            for (int q = 0; p + q <= d; q++)
            {
                for (int j = 0; j <= q; j++)
                {
                    for (int t = 0; t + j <= q; t++)
                    {
                        // g^j (da*i)^t db^{q-j-t} 的系数是 C(q,j) C(q-j,t)。
                        ans[p][q] += pre.C[q][j] * pre.C[q - j][t] * pa[t] * pb[q - j - t] * sub[p + t][j];
                    }
                }
            }
        }
        return ans;
    }
    // a<c 且 b<c：交换坐标轴。h=f(n)，f(i)>j 当且仅当 i>g(j)，g(j)=floor((c*j+c-b-1)/a)。
    int h = (int)(((__int128)a * n + b) / c);
    for (int p = 0; p <= d; p++)
    {
        ans[p][0] = pre.eval(p, n);
    }
    if (!h)
    {
        return ans;
    }
    // f(i)^q = sum_{j<f(i)} ((j+1)^q - j^q)，交换求和后 i 取 (g(j), n]。
    auto sub = floorMoment(h - 1, c, c - b - 1, a, d);
    for (int p = 0; p <= d; p++)
    {
        for (int q = 1; p + q <= d; q++)
        {
            Z7 w = ans[p][0] * Z7(h).pow(q);
            for (int u = 0; u < q; u++)
            {
                for (int r = 0; r <= p + 1; r++)
                {
                    w -= pre.C[q][u] * pre.S[p][r] * sub[u][r];
                }
            }
            ans[p][q] = w;
        }
    }
    return ans;
}

vector<int> floorMoments(const vector<array<int, 6>> &qs)
{
    // qs[i] 的六维依次是 n、a、b、c、p、q（p+q<=floorDeg）；
    // 返回各询问的 sum_{i=0}^{n} i^p floor((a*i+b)/c)^q 模 1e9+7。
    vector<int> ans;
    for (auto [n, a, b, c, p, q] : qs)
    {
        assert(p >= 0 && q >= 0 && p + q <= floorDeg); // 调试检查，可删。
        ans.push_back(floorMoment(n, a, b, c, p + q)[p][q].val());
    }
    return ans;
}
