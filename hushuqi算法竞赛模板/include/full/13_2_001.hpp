#pragma once

#include "13_1_001.hpp"

namespace prob_formula
{
long double condProb(long double pab, long double pb)
{
    assert(0 <= pab && pab <= pb && pb > 0 && pb <= 1);
    return pab / pb;
}

vector<long double> bayes(const vector<long double> &prior, const vector<long double> &like)
{
    assert(prior.size() == like.size());
    assert(!prior.empty());
    long double z = 0;
    long double sum = 0;
    for (int i = 0; i < (int)prior.size(); i++)
    {
        assert(prior[i] >= 0 && like[i] >= 0);
        sum += prior[i];
        z += prior[i] * like[i];
    }
    assert(fabsl(sum - 1) <= 1E-12L);
    assert(z > 0);
    vector<long double> ans(prior.size());
    for (int i = 0; i < (int)prior.size(); i++)
    {
        ans[i] = prior[i] * like[i] / z;
    }
    return ans;
}
}
