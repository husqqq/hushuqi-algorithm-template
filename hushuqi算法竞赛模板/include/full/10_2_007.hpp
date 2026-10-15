#pragma once

#include "10_2_002.hpp"
#include "10_2_003.hpp"

vector<Z> multiEval(const Poly &f, const vector<Z> &x)
{
    // f 是待求值多项式，x 是求值点；用频域乘积树返回与 x 同序的点值。
    int s = x.size();
    if (s == 0)
    {
        return {};
    }
    if (f.empty())
    {
        return vector<Z>(s);
    }
    if (s == 1 || f.size() <= 64 || (__int128)f.size() * s <= 4096)
    {
        vector<Z> ans(s);
        for (int i = 0; i < s; i++)
        {
            for (auto it = f.rbegin(); it != f.rend(); ++it)
            {
                ans[i] = ans[i] * x[i] + *it;
            }
        }
        return ans;
    }

    int n = bit_ceil((unsigned long long)s);
    assert(n <= NTT_LIM && convFit(f.size(), f.size())); // 调试检查，可删。
    vector<Poly> tr(2 * n);
    for (int i = 0; i < n; i++)
    {
        Z y = i < s ? -x[i] : Z(0);
        tr[n + i] = {y + 1, y - 1};
    }

    auto twice = [&](Poly &a)
    {
        // a 是长度 k 的频域值；原地扩展成同一多项式的长度 2k 频域值。
        int k = a.size();
        Z w = 31;
        for (int i = 23; i > bit_width((unsigned long long)k); i--)
        {
            w *= w;
        }
        Z v = 1;
        Poly b = a;
        idft(b);
        for (int i = 0; i < k; i++)
        {
            b[i] *= v;
            v *= w;
        }
        dft(b);
        a.insert(a.end(), b.begin(), b.end());
    };

    for (int p = n - 1; p; p--)
    {
        auto &a = tr[p << 1];
        auto &b = tr[p << 1 | 1];
        int k = a.size();
        tr[p].resize(k);
        for (int i = 0; i < k; i++)
        {
            tr[p][i] = a[i] * b[i] - 1;
        }
        if (p == 1)
        {
            continue;
        }
        twice(tr[p]);
        for (int i = 0; i < 2 * k; i++)
        {
            tr[p][i] += i < k ? Z(1) : Z(-1);
        }
    }

    Poly a = tr[1];
    idft(a);
    a.push_back(1);
    reverse(a.begin(), a.end());
    a.resize(f.size());
    a = fpsInvFast(a, f.size());
    reverse(a.begin(), a.end());
    a *= f;
    a.erase(a.begin(), a.begin() + f.size() - 1);
    a.resize(n);

    vector<Z> ans(n);
    auto dfs = [&](auto &&self, int p, Poly cur) -> void
    {
        if (p >= n)
        {
            ans[p - n] = cur[0];
            return;
        }
        int k = cur.size();
        dft(cur);
        for (int q = p << 1; q <= (p << 1 | 1); q++)
        {
            Poly b = tr[q ^ 1];
            for (int i = 0; i < k; i++)
            {
                b[i] *= cur[i];
            }
            idft(b);
            self(self, q, Poly(b.begin() + k / 2, b.end()));
        }
    };
    dfs(dfs, 1, move(a));
    ans.resize(s);
    return ans;
}

struct PolyTree
{
    static constexpr int S = 128; // 子树点数不超过 S 时直接逐点 Horner 求值。
    // n 保存求值点个数，z 保存补齐到的 2 的幂叶子数。
    int n;
    int z;
    // x 保存各求值点，t 保存乘积树多项式。
    vector<Z> x;
    vector<Poly> t;

    PolyTree(const vector<Z> &points)
        : n(points.size()), z(bit_ceil((unsigned long long)max<int>(1, n))), x(points)
    {
        // points 是全部求值点；建立乘积树。
        assert(z <= NTT_LIM); // 调试检查，可删。
        t.assign(2 * z, Poly{1});
        for (int i = 0; i < n; i++)
        {
            t[z + i] = {-x[i], 1};
        }
        for (int i = z - 1; i; i--)
        {
            t[i] = t[i << 1] * t[i << 1 | 1];
        }
    }

    vector<Z> eval(const Poly &a) const
    {
        // a 是待求值多项式；返回 a(x[i])，顺序与构造参数 x 相同。
        if (n == 0)
        {
            return {};
        }
        int m = a.size();
        while (m && a[m - 1] == Z(0))
        {
            m--;
        }
        int b = t[1].size();
        if (m >= b && m > 256 && b > 256)
        {
            int q = m - b + 1;
            assert(convFit(q, q) && convFit(b, q)); // 调试检查，可删。
        }
        vector<Z> ans(n);
        auto dfs = [&](auto &&self, int p, int l, int w, Poly r) -> void
        {
            int cnt = max<int>(0, min(n, l + w) - l);
            if (cnt <= S)
            {
                for (int i = l; i < l + cnt; i++)
                {
                    Z v = 0;
                    for (auto it = r.rbegin(); it != r.rend(); ++it)
                    {
                        v = v * x[i] + *it;
                    }
                    ans[i] = v;
                }
                return;
            }
            if (l + w / 2 >= n)
            {
                self(self, p << 1, l, w / 2,
                     polyDivmod(move(r), t[p << 1]).second);
                return;
            }
            Poly left = polyDivmod(r, t[p << 1]).second;
            Poly right = polyDivmod(move(r), t[p << 1 | 1]).second;
            self(self, p << 1, l, w / 2, move(left));
            self(self, p << 1 | 1, l + w / 2, w / 2, move(right));
        };
        dfs(dfs, 1, 0, z, polyDivmod(a, t[1]).second);
        return ans;
    }
};
