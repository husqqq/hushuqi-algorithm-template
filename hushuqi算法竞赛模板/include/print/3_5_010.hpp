#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct TopK
{
    // k 是目标个数，sum 是 hi 中元素和。
    int k;
    T sum{};
    // hi 保存当前最大的 min(k,n) 个元素，lo 保存其余元素。
    multiset<T> hi, lo;

    TopK(int k) : k(k)
    {
        // k 是要取的最大元素个数，允许 k=0。
        assert(k >= 0); // 调试检查，可删
    }

    void fix()
    {
        // 调整两组大小，使 hi 恰含最大的 min(k,n) 个元素。
        while ((int)hi.size() > k)
        {
            auto it = hi.begin();
            sum -= *it;
            lo.insert(*it);
            hi.erase(it);
        }
        while ((int)hi.size() < k && !lo.empty())
        {
            auto it = prev(lo.end());
            sum += *it;
            hi.insert(*it);
            lo.erase(it);
        }
    }

    void insert(T x)
    {
        // x 是新元素；插入一份并恢复分组。
        if (!hi.empty() && x >= *hi.begin())
        {
            hi.insert(x);
            sum += x;
        }
        else
        {
            lo.insert(x);
        }
        fix();
    }

    bool erase(T x)
    {
        // x 是待删元素；删除一份并返回删除前是否存在。
        auto it = hi.find(x);
        if (it != hi.end())
        {
            sum -= x;
            hi.erase(it);
            fix();
            return true;
        }
        it = lo.find(x);
        if (it == lo.end())
        {
            return false;
        }
        lo.erase(it);
        return true;
    }

    optional<T> query() const
    {
        // 返回最大 k 个元素之和；总数不足 k 时返回空，k=0 时返回零。
        if ((int)hi.size() < k)
        {
            return nullopt;
        }
        return sum;
    }
};
