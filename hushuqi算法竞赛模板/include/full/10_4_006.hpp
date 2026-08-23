#pragma once

#include "10_2_002.hpp"
#include "10_4_005.hpp"

Poly invTail(Poly q, unsigned long long k)
{
    // q 的次数 d>0 且首尾系数非零；返回 [x^k..x^(k+d-1)]1/q(x)。
    q.norm();
    assert(q.size() > 1 && q[0] != Z(0) && q.back() != Z(0)); // 调试检查，可删。
    int d = q.size() - 1;
    assert(d <= NTT_LIM); // 调试检查，可删。
    if (k <= (unsigned long long)d)
    {
        assert(k <= (unsigned long long)(NTT_LIM - d)); // 调试检查，可删。
    }
    else
    {
        assert(convFit(d + 1, 2 * d - 1)); // 调试检查，可删。
    }
    vector<pair<Poly, int>> st;
    st.reserve(64);
    while (k > (unsigned long long)d)
    {
        auto [neg, nq] = bmStep(move(q));
        int t = (k - (unsigned long long)d) & 1ULL;
        st.push_back({move(neg), t});
        q = move(nq);
        k = (k - (unsigned long long)d + t) / 2;
    }

    int s = k;
    Poly a = fpsInvFast(q, s + d);
    a.erase(a.begin(), a.begin() + s);
    while (!st.empty())
    {
        Poly neg = move(st.back().first);
        int t = st.back().second;
        st.pop_back();
        Poly b(2 * d - 1);
        for (int i = 0; i < d; i++)
        {
            b[2 * i] = a[i];
        }
        Poly f = conv(move(neg), move(b));
        int l = d - t;
        a = Poly(f.begin() + l, f.begin() + l + d);
    }
    return a;
}

Poly ratBlock(Poly p, Poly q, unsigned long long k, int m)
{
    // p/q 是有理 FPS，q[0] 非零；返回从第 k 项起的 m 项。
    // 设 L=log(1+k/d)，时间 O(M(|p|)+M(d)L+M(d+m))，空间 O(|p|+dL+d+m)。
    // 所有实际卷积长度须不超过公共 NTT 的 2^23 上限。
    assert(m >= 0); // 调试检查，可删。
    q.norm();
    assert(!q.empty() && q[0] != Z(0)); // 调试检查，可删。
    if (!m)
    {
        return {};
    }
    assert(k <= numeric_limits<unsigned long long>::max() -
                    (unsigned long long)(m - 1)); // 调试检查，可删。
    p.norm();
    Z iv = q[0].inv();
    p *= iv;
    q *= iv;

    if (q.size() == 1)
    {
        Poly ans(m);
        if (k < (unsigned long long)p.size())
        {
            int n = min<unsigned long long>(m, (unsigned long long)p.size() - k);
            copy_n(p.begin() + (size_t)k, n, ans.begin());
        }
        return ans;
    }

    auto [a, r] = polyDivmod(move(p), q);
    Poly ans(m);
    if (k < (unsigned long long)a.size())
    {
        int n = min<unsigned long long>(m, (unsigned long long)a.size() - k);
        for (int i = 0; i < n; i++)
        {
            ans[i] = a[(size_t)k + i];
        }
    }

    int d = q.size() - 1;
    if (r.empty())
    {
        return ans;
    }
    assert(m <= NTT_LIM && convFit(d + 1, d)); // 调试检查，可删。
    Poly t = invTail(q, k);
    Poly u = conv(q, move(t));
    u.resize(d);
    Poly v = conv(move(u), move(r));
    Poly s = polyDivmod(move(v), q).second;
    assert(convFit(s.size(), m)); // 调试检查，可删。
    Poly iq = fpsInvFast(q, m);
    Poly b = conv(move(s), move(iq));
    b.resize(m);
    ans += b;
    return ans;
}
