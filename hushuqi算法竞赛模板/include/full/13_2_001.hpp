#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

// 知识点 13.2.001：条件概率、Bayes 与全概率公式。
namespace prob_formula
{
long double condProb(long double pab, long double pb)
{
    // pab 是 P(A∩B)，pb 是 P(B)>0；返回 P(A|B)。
    assert(0 <= pab && pab <= pb && pb > 0 && pb <= 1); // 调试检查，可删。
    return pab / pb;
}

vector<long double> bayesLog(const vector<long double> &prior, const vector<long double> &loglike)
{
    // prior 是和为 1 的非负先验；loglike 是自然对数似然，负无穷表示零似然。
    // 要求至少一个正先验对应有限对数似然；返回按原假设顺序排列的后验分布。
    assert(prior.size() == loglike.size()); // 调试检查，可删。
    assert(!prior.empty()); // 调试检查，可删。
    const long double neg = -numeric_limits<long double>::infinity();
    long double sum = 0, shift = neg; // 先减最大有效对数似然，保留巨大公共偏移下的先验差异。
    for (int i = 0; i < (int)prior.size(); i++)
    {
        assert(isfinite(prior[i]) && prior[i] >= 0); // 调试检查，可删。
        assert(isfinite(loglike[i]) || loglike[i] == neg); // 调试检查，可删。
        sum += prior[i];
        if (prior[i] > 0) shift = max(shift, loglike[i]);
    }
    assert(fabsl(sum - 1) <= 1E-12L); // 调试检查，可删。
    assert(isfinite(shift)); // 调试检查，可删：排除证据权重全为零。
    vector<long double> ans(prior.size(), neg); // 先存移位后的对数权重，随后原地改为后验。
    long double top = neg;
    for (int i = 0; i < (int)prior.size(); i++)
    {
        if (prior[i] > 0 && loglike[i] != neg)
        {
            ans[i] = logl(prior[i]) + (loglike[i] - shift);
            top = max(top, ans[i]);
        }
    }
    long double z = 0;
    for (auto &x : ans)
    {
        x = expl(x - top);
        z += x;
    }
    for (auto &x : ans)
    {
        x /= z;
    }
    return ans;
}

vector<long double> bayes(const vector<long double> &prior, const vector<long double> &like)
{
    // prior 是先验分布，like 是有限非负似然（不必归一）；返回原假设顺序的后验分布。
    // 至少一项 prior[i]、like[i] 均为正；若似然已下溢为零，应改用 bayesLog。
    assert(!prior.empty() && prior.size() == like.size()); // 调试检查，可删。
    vector<long double> ans(prior.size(), 0);
    vector<int> power(prior.size(), 0); // 保存每项乘积的二进制指数，避免先算出下溢的乘积。
    int top = numeric_limits<int>::min();
    long double sum = 0;
    for (int i = 0; i < (int)prior.size(); i++)
    {
        assert(isfinite(prior[i]) && prior[i] >= 0); // 调试检查，可删。
        assert(isfinite(like[i]) && like[i] >= 0); // 调试检查，可删。
        sum += prior[i];
        if (prior[i] == 0 || like[i] == 0) continue;
        signed ep, ew; // frexpl 的指数输出接口要求原生 int，不受整数宏影响。
        ans[i] = frexpl(prior[i], &ep) * frexpl(like[i], &ew);
        power[i] = ep + ew;
        top = max(top, power[i]);
    }
    assert(fabsl(sum - 1) <= 1E-12L); // 调试检查，可删。
    assert(top != numeric_limits<int>::min()); // 调试检查，可删：至少一项权重为正。
    long double z = 0;
    for (int i = 0; i < (int)ans.size(); i++)
    {
        if (ans[i] != 0) ans[i] = ldexpl(ans[i], power[i] - top);
        z += ans[i];
    }
    for (auto &x : ans)
    {
        x /= z;
    }
    return ans;
}
}
