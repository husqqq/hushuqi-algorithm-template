#pragma once

#include "support/base.hpp"

namespace treeFreqDtl
{
template <int MOD, int ROOT> struct NTT
{
    static int power(int a, int n)
    {
        int r = 1;
        while (n)
        {
            if (n & 1) r = (unsigned long long)r * a % MOD;
            a = (unsigned long long)a * a % MOD;
            n >>= 1;
        }
        return r;
    }
    static void transform(vector<int> &a, bool inverse)
    {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int bit = n >> 1;
            while (j & bit) j ^= bit, bit >>= 1;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1)
        {
            int wlen = power(ROOT, (MOD - 1) / len);
            if (inverse) wlen = power(wlen, MOD - 2);
            for (int l = 0; l < n; l += len)
            {
                int w = 1;
                for (int i = 0; i < len / 2; i++)
                {
                    int u = a[l + i];
                    int v = (unsigned long long)a[l + i + len / 2] * w % MOD;
                    a[l + i] = u + v < MOD ? u + v : u + v - MOD;
                    a[l + i + len / 2] = u >= v ? u - v : u + MOD - v;
                    w = (unsigned long long)w * wlen % MOD;
                }
            }
        }
        if (inverse)
        {
            int inv = power(n, MOD - 2);
            for (auto &x : a) x = (unsigned long long)x * inv % MOD;
        }
    }
    static vector<int> square(const vector<int> &a)
    {
        if (a.empty()) return {};
        if (a.size() <= 48)
        {
            vector<int> c(2 * a.size() - 1);
            for (int i = 0; i < (int)a.size(); i++)
                for (int j = 0; j < (int)a.size(); j++)
                    c[i + j] = (c[i + j] + (unsigned long long)a[i] * a[j]) % MOD;
            return c;
        }
        int need = 2 * a.size() - 1;
        int n = bit_ceil((unsigned long long)need);
        vector<int> f(n);
        for (int i = 0; i < (int)a.size(); i++) f[i] = a[i] % MOD;
        transform(f, false);
        for (auto &x : f) x = (unsigned long long)x * x % MOD;
        transform(f, true);
        f.resize(need);
        return f;
    }
};

template <int MOD, int ROOT>
vector<int> solveMod(const vector<vector<int>> &g)
{
    int n = g.size();
    vector<char> blocked(n);
    vector<int> parent(n), size(n), ans(n), work{0};
    while (!work.empty())
    {
        int start = work.back();
        work.pop_back();
        if (blocked[start]) continue;
        vector<int> order{start};
        parent[start] = -1;
        for (int i = 0; i < (int)order.size(); i++)
        {
            int u = order[i];
            for (int v : g[u]) if (v != parent[u] && !blocked[v])
            {
                parent[v] = u;
                order.push_back(v);
            }
        }
        for (int i = order.size() - 1; i >= 0; i--)
        {
            int u = order[i];
            size[u] = 1;
            for (int v : g[u]) if (!blocked[v] && parent[v] == u) size[u] += size[v];
        }
        int sz = order.size(), cen = start;
        for (int u : order)
        {
            int largest = sz - size[u];
            for (int v : g[u]) if (!blocked[v] && parent[v] == u) largest = max(largest, size[v]);
            if (largest * 2 <= sz)
            {
                cen = u;
                break;
            }
        }

        vector<int> total(1, 1);
        for (int first : g[cen]) if (!blocked[first])
        {
            vector<int> histogram(2);
            vector<array<int, 3>> stack{{first, cen, 1}};
            while (!stack.empty())
            {
                auto [u, p, d] = stack.back();
                stack.pop_back();
                if ((int)histogram.size() <= d) histogram.resize(d + 1);
                histogram[d]++;
                for (int v : g[u]) if (v != p && !blocked[v]) stack.push_back({v, u, d + 1});
            }
            auto subtract = NTT<MOD, ROOT>::square(histogram);
            for (int d = 0; d < (int)subtract.size() && d < n; d++)
            {
                ans[d] -= subtract[d];
                if (ans[d] < 0) ans[d] += MOD;
            }
            if (total.size() < histogram.size()) total.resize(histogram.size());
            for (int d = 0; d < (int)histogram.size(); d++) total[d] += histogram[d];
        }
        auto add = NTT<MOD, ROOT>::square(total);
        for (int d = 0; d < (int)add.size() && d < n; d++)
        {
            ans[d] += add[d];
            if (ans[d] >= MOD) ans[d] -= MOD;
        }
        blocked[cen] = true;
        for (int v : g[cen]) if (!blocked[v]) work.push_back(v);
    }
    vector<int> res(n);
    for (int i = 0; i < n; i++) res[i] = ans[i];
    return res;
}
}

vector<long long> treeDistFreq(const vector<vector<int>> &g)
{
    // g 是非空无权树；返回 ans[d-1]=距离恰为 d 的无序点对数（1<=d<n）。
    int n = g.size();
    if (n <= 1) return {};
    constexpr int M0 = 998244353, M1 = 924844033;
    auto a = treeFreqDtl::solveMod<M0, 3>(g);
    auto b = treeFreqDtl::solveMod<M1, 5>(g);
    long long inverse = treeFreqDtl::NTT<M1, 5>::power(M0 % M1, M1 - 2);
    vector<long long> ans(n - 1);
    for (int d = 1; d < n; d++)
    {
        long long delta = (b[d] + M1 - a[d] % M1) % M1;
        long long ordered = a[d] + (long long)M0 * (delta * inverse % M1);
        ans[d - 1] = ordered / 2;
    }
    return ans;
}
