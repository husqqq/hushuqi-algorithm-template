#pragma once

#include "10_4_001.hpp"

template <class T> vector<T> shortRel(const vector<T> &s)
{
    // s 是序列前缀；返回最短关系多项式 q，满足 sum(q[i]*s[n-i])=0 且 q[0]=1。
    auto rec = BM(s);
    vector<T> q(rec.size() + 1);
    q[0] = 1;
    for (int i = 0; i < (int)rec.size(); i++)
    {
        q[i + 1] = -rec[i];
    }
    return q;
}
