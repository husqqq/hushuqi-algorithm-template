#pragma once

#include "10_2_005.hpp"
#include "10_2_006.hpp"

vector<Z> sparseInv(int n, vector<pair<int, Z>> f)
{
    // n 是截断长度，f 是常数项非零的稀疏 FPS；返回 1/f 的前 n 项。
    assert(0 <= n && n <= Z::mod()); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    f = spNorm(move(f));
    assert(!f.empty() && f[0].first == 0 && f[0].second != Z(0)); // 调试检查，可删。
    vector<Z> g(n);
    g[0] = f[0].second.inv();
    for (int i = 1; i < n; i++)
    {
        Z sum = 0;
        for (int j = 1; j < (int)f.size() && f[j].first <= i; j++)
        {
            sum += f[j].second * g[i - f[j].first];
        }
        g[i] = -sum * g[0];
    }
    return g;
}

vector<Z> sparseExp(int n, vector<pair<int, Z>> f)
{
    // n 是截断长度，f 保存常数项为 0 的稀疏 FPS；规范化后返回 exp(f) 的前 n 项。
    assert(0 <= n && n <= Z::mod()); // 调试检查，可删。
    f = spNorm(move(f));
    assert(f.empty() || f[0].first > 0 || f[0].second == Z(0)); // 调试检查，可删。
    vector<Z> g(n);
    if (!n)
    {
        return g;
    }
    const auto &iv = invTable<mod>(n - 1);
    g[0] = 1;
    for (int i = 1; i < n; i++)
    {
        for (auto [j, x] : f)
        {
            if (!j)
            {
                continue;
            }
            if (j > i)
            {
                break;
            }
            g[i] += Z(j) * x * g[i - j];
        }
        g[i] *= iv[i];
    }
    return g;
}

vector<Z> sparseLog(int n, vector<pair<int, Z>> f)
{
    // n 是截断长度，f 保存常数项为 1 的稀疏 FPS；规范化后返回 ln(f) 的前 n 项。
    assert(0 <= n && n <= Z::mod()); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    f = spNorm(move(f));
    assert(!f.empty() && f[0].first == 0 && f[0].second == Z(1)); // 调试检查，可删。
    vector<Z> q(max<int>(0, n - 1));
    vector<Z> g(n);
    const auto &iv = invTable<mod>(n - 1);
    for (int i = 0; i + 1 < n; i++)
    {
        for (auto [j, x] : f)
        {
            if (j == i + 1)
            {
                q[i] += Z(j) * x;
            }
            if (!j)
            {
                continue;
            }
            if (j > i)
            {
                break;
            }
            q[i] -= x * q[i - j];
        }
        g[i + 1] = q[i] * iv[i + 1];
    }
    return g;
}

vector<Z> sparsePow(int n, vector<pair<int, Z>> f, long long k)
{
    // n 是截断长度，f 是可乱序且允许重复次数的稀疏 FPS，k 是非负指数；规范化后返回 f^k 的前 n 项。
    assert(0 <= n && n <= Z::mod() && k >= 0); // 调试检查，可删。
    vector<Z> g(n);
    if (!n)
    {
        return g;
    }
    if (k == 0)
    {
        g[0] = 1;
        return g;
    }
    auto h = spNorm(move(f));
    if (h.empty())
    {
        return g;
    }
    int p = h[0].first;
    if ((__int128)p * k >= n)
    {
        return g;
    }
    int sh = p * k;
    int m = n - sh;
    Z c = h[0].second;
    vector<pair<int, Z>> q;
    for (auto [i, x] : h)
    {
        if (i - p >= m)
        {
            break;
        }
        q.push_back({i - p, x / c});
    }
    const auto &iv = invTable<mod>(m - 1);
    vector<Z> r(m);
    r[0] = 1;
    long long km = k % mod;
    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j < (int)q.size() && q[j].first <= i; j++)
        {
            int d = q[j].first;
            r[i] += Z((km + 1) * d - i) * q[j].second * r[i - d];
        }
        r[i] *= iv[i];
    }
    Z scale = c.pow(k);
    for (int i = 0; i < m; i++)
    {
        g[sh + i] = scale * r[i];
    }
    return g;
}

optional<vector<Z>> spSqrt(int n, vector<pair<int, Z>> f, Z root)
{
    // n 是截断长度，f 已按次数规范化，root 是首项的一组平方根；返回对应平方根，无解时返回空。
    assert(0 <= n && n <= Z::mod()); // 调试检查，可删。
    vector<Z> ans(n);
    if (n == 0)
    {
        return ans;
    }
    if (f.empty())
    {
        return ans;
    }
    int p = f[0].first;
    if (p >= n)
    {
        return ans;
    }
    if (p & 1)
    {
        return nullopt;
    }
    if (root * root != f[0].second)
    {
        return nullopt;
    }
    int sh = p / 2;
    int m = n - sh;
    Z c = f[0].second;
    vector<pair<int, Z>> q;
    for (auto [i, x] : f)
    {
        if (i - p >= m)
        {
            break;
        }
        q.push_back({i - p, x / c});
    }
    const auto &iv = invTable<mod>(m - 1);
    Z half = Z(2).inv();
    vector<Z> r(m);
    r[0] = 1;
    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j < (int)q.size() && q[j].first <= i; j++)
        {
            int d = q[j].first;
            r[i] += (Z(3 * d) * half - Z(i)) * q[j].second * r[i - d];
        }
        r[i] *= iv[i];
    }
    for (int i = 0; i < m; i++)
    {
        ans[sh + i] = root * r[i];
    }
    return ans;
}

optional<vector<Z>> sparseSqrt(int n, vector<pair<int, Z>> f, Z root)
{
    // n 是截断长度，f 是可乱序且允许重复次数的稀疏 FPS，root 是首项平方根；返回对应平方根，无解时返回空。
    return spSqrt(n, spNorm(move(f)), root);
}

optional<vector<Z>> sparseSqrt(int n, vector<pair<int, Z>> f)
{
    // n 是截断长度，f 是稀疏 FPS；自动选择首个非零系数的一组平方根，返回对应平方根的前 n 项，无平方根时返回空。
    assert(0 <= n && n <= Z::mod()); // 调试检查，可删。
    auto g = spNorm(move(f));
    if (n == 0 || g.empty() || g[0].first >= n)
    {
        return vector<Z>(n);
    }
    if (g[0].first & 1)
    {
        return nullopt;
    }
    auto root = sqrtMod(g[0].second.val(), mod);
    if (!root)
    {
        return nullopt;
    }
    return spSqrt(n, move(g), Z(*root));
}
