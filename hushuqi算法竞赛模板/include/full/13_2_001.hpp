#pragma once

#include "13_1_001.hpp"

// 知识点 13.2.001：条件概率、Bayes 与全概率公式。
namespace prob_formula
{
long double condProb(long double pab, long double pb)
{
    // pab 是 P(A∩B)，pb 是 P(B)>0；返回 P(A|B)。
    assert(0 <= pab && pab <= pb && pb > 0 && pb <= 1); // 调试检查，可删。
    return pab / pb;
}

vector<long double> bayes(const vector<long double> &prior, const vector<long double> &like)
{
    // prior 是先验概率，like 是各假设下证据似然；返回观察证据后的后验分布。
    assert(prior.size() == like.size()); // 调试检查，可删。
    assert(!prior.empty()); // 调试检查，可删。
    long double z = 0;
    long double sum = 0;
    for (int i = 0; i < (int)prior.size(); i++)
    {
        assert(prior[i] >= 0 && like[i] >= 0); // 调试检查，可删。
        sum += prior[i];
        z += prior[i] * like[i];
    }
    assert(fabsl(sum - 1) <= 1E-12L); // 调试检查，可删。
    assert(z > 0); // 调试检查，可删。
    vector<long double> ans(prior.size());
    for (int i = 0; i < (int)prior.size(); i++)
    {
        ans[i] = prior[i] * like[i] / z;
    }
    return ans;
}
}
