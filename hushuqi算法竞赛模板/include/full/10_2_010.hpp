#pragma once

#include "10_2_007.hpp"

Z fastFact(int n)
{
    // n 满足 0<=n<Z::mod()；返回 n!，使用分块多项式乘积与多点求值降低单次复杂度。
    assert(0 <= n && n < Z::mod()); // 调试检查，可删。
    if (n <= 1)
    {
        return 1;
    }
    if (n > (Z::mod() - 1) / 2)
    {
        int m = Z::mod() - 1 - n;
        Z s = m & 1 ? Z(1) : Z(-1);
        return s / fastFact(m);
    }
    int b = max<int>(1, sqrtl(n));
    while ((b + 1) * (b + 1) <= n)
    {
        b++;
    }
    while (b * b > n)
    {
        b--;
    }
    int q = n / b;
    vector<Z> roots(b);
    vector<Z> xs(q);
    for (int i = 0; i < b; i++)
    {
        roots[i] = -Z(i + 1);
    }
    for (int i = 0; i < q; i++)
    {
        xs[i] = Z(i * b);
    }
    PolyTree prod(roots);
    PolyTree eval(xs);
    auto val = eval.eval(prod.t[1]);
    Z ans = 1;
    for (auto x : val)
    {
        ans *= x;
    }
    for (int x = q * b + 1; x <= n; x++)
    {
        ans *= x;
    }
    return ans;
}

vector<Z> factBatch(const vector<int> &q)
{
    // q 是所有待查询的非负阶乘下标；返回与 q 同序的 n!。
    // 先用多点求值预处理整块阶乘，再用一棵乘积树批量求全部块内前缀积。
    if (q.empty())
    {
        return {};
    }
    constexpr int LOG_BLOCK = 15;
    constexpr int BLOCK = 1 << LOG_BLOCK;
    static const vector<Z> blockFac = []
    {
        int cnt = (mod - 1) / BLOCK;
        vector<Z> roots(BLOCK), xs(cnt);
        for (int i = 0; i < BLOCK; i++) roots[i] = -Z(i + 1);
        for (int i = 0; i < cnt; i++) xs[i] = Z(i * BLOCK);
        Poly f;
        {
            PolyTree prod(roots);
            f = prod.t[1];
        }
        vector<Z> val;
        {
            PolyTree eval(xs);
            val = eval.eval(f);
        }
        vector<Z> fac(cnt + 1, 1);
        for (int i = 0; i < cnt; i++) fac[i + 1] = fac[i] * val[i];
        return fac;
    }();

    vector<Z> ans(q.size());
    vector<vector<pair<int, int>>> queries(BLOCK);
    int leaves = BLOCK - 1;
    for (int i = 0; i < (int)q.size(); i++)
    {
        int n = q[i];
        assert(0 <= n && n < mod);
        int base = n / BLOCK * BLOCK;
        int r = n % BLOCK;
        ans[i] = blockFac[n / BLOCK];
        if (r)
        {
            queries[r].push_back({base, i});
            leaves++;
        }
    }

    int z = bit_ceil((unsigned long long)max<int>(1, leaves));
    vector<int> queryLeaf(q.size(), -1);
    vector<Poly> factors(2 * z, Poly{1}), points(2 * z, Poly{1});
    int at = 0;
    for (int r = 1; r < BLOCK; r++)
    {
        factors[z + at++] = {Z(r), Z(1)};
        for (auto [x, id] : queries[r])
        {
            queryLeaf[id] = z + at;
            points[z + at++] = {-Z(x), Z(1)};
        }
    }
    for (int p = z - 1; p > 1; p--)
    {
        factors[p] = factors[p << 1] * factors[p << 1 | 1];
        points[p] = points[p << 1] * points[p << 1 | 1];
    }
    points[1] = {Z(1)};
    for (int p = 1; p < z; p++)
    {
        Poly prefix = points[p];
        points[p << 1] = polyDivmod(prefix, points[p << 1]).second;
        points[p << 1 | 1] = polyDivmod(prefix * factors[p << 1],
                                         points[p << 1 | 1]).second;
    }
    for (int i = 0; i < (int)q.size(); i++)
    {
        if (queryLeaf[i] != -1)
        {
            const Poly &r = points[queryLeaf[i]];
            ans[i] *= r.empty() ? Z(0) : r[0];
        }
    }
    return ans;
}
