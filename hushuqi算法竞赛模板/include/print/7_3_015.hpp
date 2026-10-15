#pragma once

#include "7_1_014.hpp"
#include "7_2_001.hpp"
#include "7_3_001.hpp" // 正文依赖 7.3.002

vector<int> powerfulNums(int n)
{
    // n 是非负上界；返回不超过 n 的全部 Powerful 数，按升序排列；n>=1 时包含 1。
    assert(n >= 0);
    int lim = kthRoot(n, 3);
    auto mu = mobiusTable(lim);
    vector<int> ans;
    for (int b = 1; b <= lim; b++)
    {
        if (mu[b] == 0) continue;
        int base = (int)((__int128)b * b * b);
        int end = kthRoot(n / base, 2);
        for (int a = 1; a <= end; a++)
        {
            ans.push_back((int)((__int128)a * a * base));
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

template <class T, class H, class G> T powerfulSum(int n, H hp, G gs)
{
    // n 是非负上界；hp(p,e) 返回积性函数 h 的素数幂值，要求 h(1)=1、h(p)=0；
    // gs(m) 返回 sum_{i=1..m} g(i)；返回 sum_{i=1..n} (h*g)(i)，T 承载求和结果。
    assert(n >= 0);
    if (n == 0) return T(0);
    auto p = Sieve(kthRoot(n, 2)).primes;
    auto dfs = [&](auto &&self, int x, int pos, T val) -> T
    {
        // x 是已选素数幂之积，pos 是下一素数下标，val=h(x)；返回该分支贡献。
        T ans = val * gs(n / x);
        for (int i = pos; i < (int)p.size() && p[i] <= (n / x) / p[i]; i++)
        {
            int q = p[i] * p[i];
            for (int e = 2; q <= n / x; e++)
            {
                ans += self(self, x * q, i + 1, val * hp(p[i], e));
                if (q > (n / x) / p[i]) break;
                q *= p[i];
            }
        }
        return ans;
    };
    return dfs(dfs, 1, 0, T(1));
}
