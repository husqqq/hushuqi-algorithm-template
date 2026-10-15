#pragma once

#include "10_1_003.hpp"

vector<bool> wildcard(const string &s, string t, char any = '*')
{
    // s、t 只含 a-z 与 any，any 在两侧都表示通配符；返回每个合法起点是否匹配。
    int n = s.size(), m = t.size();
    if (!m)
    {
        return vector<bool>(n + 1, true);
    }
    if (m > n)
    {
        return {};
    }
    reverse(t.begin(), t.end());
    vector<Z> sx(n), sx2(n), sm(n);
    vector<Z> ty(m), ty2(m), tm(m);
    for (int i = 0; i < n; i++)
    {
        if (s[i] != any)
        {
            int x = s[i] - 'a' + 1;
            assert(1 <= x && x <= 26); // 调试检查，可删。
            sx[i] = x;
            sx2[i] = x * x;
            sm[i] = 1;
        }
    }
    for (int i = 0; i < m; i++)
    {
        if (t[i] != any)
        {
            int y = t[i] - 'a' + 1;
            assert(1 <= y && y <= 26); // 调试检查，可删。
            ty[i] = y;
            ty2[i] = y * y;
            tm[i] = 1;
        }
    }
    auto c0 = conv(move(sx2), move(tm));
    auto c1 = conv(move(sx), move(ty));
    auto c2 = conv(move(sm), move(ty2));
    vector<bool> ans(n - m + 1);
    for (int i = 0; i + m <= n; i++)
    {
        int k = i + m - 1;
        ans[i] = c0[k] - c1[k] * 2 + c2[k] == Z(0);
    }
    return ans;
}
