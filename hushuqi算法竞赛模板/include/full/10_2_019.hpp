#pragma once

#include "10_2_003.hpp"

vector<Z> monoToNewton(const Poly &f, const vector<Z> &p)
{
    // f 是低次在前的单项式基系数，p 是 Newton 基的节点；两者等长，返回各级前缀乘积的系数。
    int n = f.size();
    assert((int)p.size() == n); // 调试检查，可删。
    if (n == 0)
    {
        return {};
    }
    int z = bit_ceil((unsigned long long)n);
    assert(z <= NTT_LIM / 2); // 调试检查，可删。
    vector<Poly> prod(2 * z, Poly{1});
    for (int i = 0; i < z; i++)
    {
        Z x = i < n ? p[i] : Z(0);
        prod[z + i] = {-x, 1};
    }
    for (int i = z - 1; i; i--)
    {
        prod[i] = prod[i << 1] * prod[i << 1 | 1];
    }

    Poly a = f.cut(z);
    reverse(a.begin(), a.end());
    Poly root = prod[1];
    reverse(root.begin(), root.end());
    Poly h = (a * fpsInvFast(root, z)).cut(z);
    reverse(h.begin(), h.end());

    vector<Z> ans(n);
    auto dfs = [&](auto &&self, int u, Poly cur) -> void
    {
        if (u >= z)
        {
            int i = u - z;
            if (i < n)
            {
                ans[i] = cur[0];
            }
            return;
        }
        int m = prod[u << 1 | 1].size() - 1;
        Poly q = cur * prod[u << 1 | 1];
        Poly left(q.begin() + m, q.begin() + 2 * m);
        Poly right(cur.begin() + m, cur.end());
        self(self, u << 1, move(left));
        self(self, u << 1 | 1, move(right));
    };
    dfs(dfs, 1, move(h));
    return ans;
}
